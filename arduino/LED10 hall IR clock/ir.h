PROGMEM prog_uint32_t IRdata1[] = {
0x0FFF00F,//0
0x0FF12ED,//1
0x0FFD02F,//2
0x0FF50AF,//3
0x0FF1AE5,//4
0x0FFD827,//5
0x0FF58A7,//6
0x0FF3AC5,//7
0x0FF9867,//8
0x0FF7887//9
};
PROGMEM prog_uint32_t IRdata2[] = {
0x35A9425F,//0
0xE35A7437,//1
0xD538681B,//2
0x2A89195F,//3
0xA8E05FBB,//4
0x86B0E697,//5
0xDC0197DB,//6
0xB3D4B87F,//7
0x97483BFB,//8
0xF63C8657//9
};

int RECV_PIN = 15;//定义红外接收器的引脚为11

IRrecv irrecv(RECV_PIN);

decode_results results;

void InitIR()
{
  irrecv.enableIRIn(); // 初始化红外接收器
}

void loopir() {
  if (irrecv.decode(&results)) {
	for(int i=0;i<10;i++)
	{
		if(results.value == pgm_read_dword_near(IRdata1+i))
		{
			//Serial.print("aa ");
			//Serial.println(i);
		}
	}
 	for(int i=0;i<10;i++)
	{
		if(results.value == pgm_read_dword_near(IRdata2+i))
		{
			//Serial.print("bb ");
			//Serial.println(i);
		}
	}
	//Serial.print("ircase ");
	//Serial.println(ircase);

    irrecv.resume(); // 接收下一个值
  }
}

/*
1 FF12ED E35A7437
2 FFD02F D538681B
3 FF50AF 2A89195F
4 FF1AE5 A8E05FBB
5 FFD827 86B0E697
6 FF58A7 DC0197DB
7 FF3AC5 B3D4B87F
8 FF9867 97483BFB
9 FF7887 F63C8657
0 FFF00F 35A9425F
*/


