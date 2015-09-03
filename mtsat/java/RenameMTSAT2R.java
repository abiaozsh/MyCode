import java.io.File;

public class RenameMTSAT2R
{
	public static void main(String[] args) throws Exception
	{

		aa("f:\\mtsat2\\");


	}
	
	
	private static void aa(String sdir)
	{
		File dir = new File(sdir);

		File[] fs = dir.listFiles();

		for (File f : fs)
		{
			if (f.getName().length() == "MTSAT1R200611010030IR1.jpg".length())
			{
				String newFile = f.getName().substring(7, 17) + f.getName().substring(19);

				System.out.println(newFile);
				f.renameTo(new File(sdir + newFile));

			}

		}
		
	}
}
