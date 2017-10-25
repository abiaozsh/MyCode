using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using Lib;

namespace ConvNet
{

	class MyRnn
	{
		Graph G;

		Model model;

		Solver solver;

		// prediction params
		float sample_softmax_temperature = 1.0f; // how peaky model predictions should be
		int max_chars_gen = 100; // max length of generated sentences

		// various global var inits
		int epoch_size = -1;
		int input_size = -1;
		int output_size = -1;
		Dictionary<char, int> letterToIndex;//{};
		Dictionary<int, char> indexToLetter;//{};
		List<char> vocab;//[];
		List<string> data_sents = new List<string>();//[];

		int[] hidden_sizes;

		int letter_size;
		float regc;
		float learning_rate;
		float clipval;

		Mat logprobs;
		Mat probs;


		public void initVocab(string sents, int count_threshold)
		{
			// go over all characters and keep track of all unique ones seen
			var txt = sents;//sents.join(''); // concat all

			// count up all characters
			Dictionary<char, int> d = new Dictionary<char, int>();
			for (int i = 0, n = txt.Length; i < n; i++)
			{
				var txti = txt[i];
				if (d.ContainsKey(txti))
				{
					d[txti] += 1;
				}
				else
				{
					d[txti] = 1;
				}
			}

			// filter by count threshold and create pointers
			letterToIndex = new Dictionary<char, int>();
			indexToLetter = new Dictionary<int, char>();
			vocab = new List<char>();
			// NOTE: start at one because we will have START and END tokens!
			// that is, START token will be index 0 in model letter vectors
			// and END token will be index 0 in the next character softmax
			int q = 1;
			foreach (var ch in d)
			{
				//if(d.hasOwnProperty(ch)) {
				if (ch.Value >= count_threshold)
				{
					// add character to vocab
					letterToIndex[ch.Key] = q;
					indexToLetter[q] = ch.Key;
					vocab.Add(ch.Key);
					q++;
				}
				//}
			}

			// globals written: indexToLetter, letterToIndex, vocab (list), and:
			input_size = vocab.Count + 1;
			output_size = vocab.Count + 1;
			//$("#prepro_status").text('found ' + vocab.length + ' distinct characters: ' + vocab.join(''));
			string prepro_status = ("found " + vocab.Count + " distinct characters: " + vocab.ToString());
		}

		public Model initModel()
		{
			// letter embedding vectors
			Model model = new Model();
			model["Wil"] = RNN.RandMat(input_size, letter_size, 0, 0.08f, "Wil");

			//if(generator === 'rnn') {
			//Model rnn = RNN.initRNN(letter_size, hidden_sizes, output_size);
			//utilAddToModel(model, rnn);
			//} else {
			var lstm = RNN.initLSTM(letter_size, hidden_sizes, output_size);
			utilAddToModel(model, lstm);
			//}

			return model;
		}

		public void utilAddToModel(Model modelto, Model modelfrom)
		{
			foreach (var k in modelfrom)
			{
				//if(modelfrom.hasOwnProperty(k)) {
				// copy over the pointer but change the key to use the append
				modelto[k.Key] = modelfrom[k.Key];
				//}
			}
		}

		public void reinit()
		{
			// note: reinit writes global vars

			// eval options to set some globals
			// model parameters
			//generator = 'lstm'; // can be 'rnn' or 'lstm'
			hidden_sizes = new int[] { 40, 40, 40 }; // list of sizes of hidden layers
			letter_size = 10; // size of letter embeddings

			// optimization
			regc = 0.000001f; // L2 regularization strength
			learning_rate = 0.01f; // learning rate
			clipval = 5.0f; // clip gradients at this value


			//reinit_learning_rate_slider();

			solver = new Solver(); // reinit solver
			//pplGraph = new Rvis.Graph();

			//ppl_list = [];

			// process the input, filter out blanks
			var data_sents_raw = Data.data.Split('\r');

			data_sents = new List<string>();
			for (var i = 0; i < data_sents_raw.Length; i++)
			{
				var sent = data_sents_raw[i].Trim();
				if (sent.Length > 0)
				{
					data_sents.Add(sent);
				}
			}
			epoch_size = data_sents.Count;

			initVocab(Data.data, 1); // takes count threshold for characters
			model = initModel();
		}



		public string predictSentence(Model model, bool samplei, float temperature)
		{

			var G = new Graph(false);
			var s = "";
			Lib.RNN.HCO prev = null;
			while (true)
			{

				// RNN tick
				var ix = s.Length == 0 ? 0 : letterToIndex[s[s.Length - 1]];
				var lh = forwardIndex(G, model, ix, prev);
				prev = lh;

				// sample predicted letter
				logprobs = lh.o;
				if (temperature != 1.0f && samplei)
				{
					// scale log probabilities by temperature and renormalize
					// if temperature is high, logprobs will go towards zero
					// and the softmax outputs will be more diffuse. if temperature is
					// very low, the softmax outputs will be more peaky
					for (int q = 0, nq = logprobs.w.size; q < nq; q++)
					{
						logprobs.w[q] /= temperature;
					}
				}

				probs = RNN.softmax(logprobs);
				if (samplei)
				{
					ix = RNN.samplei(probs.w);
				}
				else
				{
					ix = RNN.maxi(probs.w);
				}

				if (ix == 0) break; // END token predicted, break out
				if (s.Length > max_chars_gen) { break; } // something is wrong

				var letter = indexToLetter[ix];
				s += letter;
			}
			return s;
		}

		int idx = 0;
		Stopwatch stopwatch = new Stopwatch();
		public void tick()
		{

			// sample sentence fromd data
			var sentix = idx % data_sents.Count;//RNN.randi(0, data_sents.Count);
			idx++;
			var sent = data_sents[sentix];

			//var t0 = +new Date();  // log start timestamp

			// evaluate cost function on a sentence
			var cost = costfun(model, sent);

			// use built up graph to compute backprop (set .dw fields in mats)
			G.backward();
			// perform param update
			var solver_stats = solver.step(model, learning_rate, regc, clipval);
			//$("#gradclip").text('grad clipped ratio: ' + solver_stats.ratio_clipped)

			//var t1 = +new Date();
			//var tick_time = t1 - t0;

			//ppl_list.push(cost_struct.ppl); // keep track of perplexity

			// evaluate now and then
			if (idx % 50 == 0)
			{
				stopwatch.Stop();
				Console.WriteLine("time:" + stopwatch.Elapsed + "   epoch: " + (idx) + "   cost:" + cost);
				string samples = "";
				// draw samples
				for (var q = 0; q < 5; q++)
				{
					var pred = predictSentence(model, true, sample_softmax_temperature);
					//var pred_div = '<div class="apred">'+pred+'</div>'
					samples += pred;
				}
				Console.WriteLine(samples);

				samples = "epoch: " + (idx) + samples + "\r\n";
				FileStream fs = new FileStream("out.txt", FileMode.Append, FileAccess.Write);
				byte[] byteArray = System.Text.Encoding.Default.GetBytes(samples);
				fs.Write(byteArray, 0, byteArray.Length);
				fs.Flush();
				fs.Close();

				stopwatch.Reset();
				stopwatch.Start();
			}
			//if (idx % 10 == 0)
			//{
			//	// draw argmax prediction
			//	string argmax;
			//	var pred = predictSentence(model, false, 1.0f);
			//	argmax = pred;
			//	//Console.WriteLine(argmax);
			//
			//	// keep track of perplexity
			//	Console.WriteLine("epoch: " + (idx));
			//	//Console.WriteLine("perplexity: " + cost_struct.ppl.toFixed(2));
			//	//Console.WriteLine('forw/bwd time per example: ' + tick_time.toFixed(1) + 'ms');
			//
			//}
		}

		public Lib.RNN.HCO forwardIndex(Graph G, Model model, int ix, Lib.RNN.HCO prev)
		{
			var x = G.rowPluck(model["Wil"], ix);
			// forward prop the sequence learner
			//if(generator === 'rnn') {
			//var out_struct = RNN.forwardRNN(G, model, hidden_sizes, x, prev);
			//} else {
			var out_struct = RNN.forwardLSTM(G, model, hidden_sizes, x, prev);
			//}
			return out_struct;
		}

		public float costfun(Model model, string sent)
		{
			// takes a model and a sentence and
			// calculates the loss. Also returns the Graph
			// object which can be used to do backprop
			var n = sent.Length;
			G = new Graph(true);
			//var log2ppl = 0.0f;
			var cost = 0.0f;
			Lib.RNN.HCO prev = null;
			for (var i = -1; i < n; i++)
			{
				// start and end tokens are zeros
				var ix_source = i == -1 ? 0 : letterToIndex[sent[i]]; // first step: start with START token
				var ix_target = i == n - 1 ? 0 : letterToIndex[sent[i + 1]]; // last step: end with END token

				Lib.RNN.HCO lh = forwardIndex(G, model, ix_source, prev);
				prev = lh;

				// set gradients into logprobabilities
				logprobs = lh.o; // interpret output as logprobs
				probs = RNN.softmax(logprobs); // compute the softmax probabilities

				//log2ppl += -Math.Log2(probs.w[ix_target]); // accumulate base 2 log prob and do smoothing
				cost += (float)-Math.Log(probs.w[ix_target]);

				// write gradients into log probabilities
				logprobs.dw = probs.w;
				logprobs.dw[ix_target] -= 1;
			}
			//var ppl = Math.Pow(2, log2ppl / (n - 1));
			return cost;
		}


	}
}
