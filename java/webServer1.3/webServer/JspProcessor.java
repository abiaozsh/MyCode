package webServer;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.HashMap;

final class JspProcessor
{

    private String url;

    private String rootPath;

    JspProcessor(String url, String rootPath)
    {
        this.url = url;
		if (rootPath.endsWith(Server.SEP))
		{
			this.rootPath = rootPath;
		}
		else
		{
			this.rootPath = rootPath + Server.SEP;
		}
    }

    String getUrl()
    {
        return url;
    }

    private HashMap<String, JspClassPack> loadedJspPage = new HashMap<String, JspClassPack>();

    public JspPage getNewJspPage(String path, Server server) throws Exception
    {

        String baseName = (rootPath + path.substring(url.length())).replace("/", Server.SEP);

        if (!baseName.endsWith(".jsp"))
        {
            return null;
        }

        baseName = baseName.substring(0, baseName.length() - 4);

        // check
        String jspFileName = baseName + ".jsp";

        File jspFile = new File(jspFileName);

        if (jspFile.exists())
        {
            if (jspFile.isDirectory())
            {
                return null;
            }
        }
        else
        {
            return null;
        }

        JspClassPack loaded = loadedJspPage.get(path);

        if (loaded == null || loaded.lastModified != jspFile.lastModified())
        {
            String javaFileName = baseName + ".java";

            String classFileName = baseName + ".class";

            // jsp compile
            Compile(jspFile, javaFileName, server);

            // java compile
            StringWriter sw = new StringWriter();

            int exit = 0;

            Object[] objs = new Object[2];

            objs[0] = new String[] { "-encoding", "utf-8", javaFileName };
            objs[1] = new PrintWriter(sw);

            if (server.javaCompiler == null)
            {
                throw new Exception("Failed to load JavaCompiler, JSP function will not work.");
            }
            Object o = server.javaCompiler.invoke(null, objs);
            exit = ((Integer) o).intValue();

            if (exit != 0)
            {
                String err = sw.getBuffer().toString();
                throw new Exception(err);
            }

            JspClassLoader loader = new JspClassLoader();

            // class load
            loaded = new JspClassPack(loader.loadClass(classFileName),jspFile.lastModified());

            loadedJspPage.put(path, loaded);
        }

        JspPage jspPage = (JspPage)loaded.clazz.newInstance();

        jspPage.url = this.url;

        return jspPage;
    }

    private void Compile(File jspFile, String javaFileName, Server server) throws Exception
    {
        File javaFile = new File(javaFileName);
        if (javaFile.exists())
        {
            javaFile.delete();
        }

        FileInputStream fis = new FileInputStream(jspFile);

        byte[] b = new byte[(int) jspFile.length()];

        fis.read(b);

        fis.close();

        String s = new String(b, server.currentConfig.defaultEncoding);

        char[] cs = s.toCharArray();

        ArrayList<StringBufferWraper> tempAl = new ArrayList<StringBufferWraper>();

        int status = 0;// 0:htm/1:java

        StringBufferWraper tempSb = new StringBufferWraper(StringBufferWraper.TYPE.HTML);
        //tempSb.append("H");

        for (int i = 0; i < cs.length; i++)
        {
            if (status == 0)
            {
                if (match(cs, i, "<%="))
                {
                    i += 2;
                    status = 1;
                    tempAl.add(tempSb);
                    tempSb = new StringBufferWraper(StringBufferWraper.TYPE.OUTPUT);
                    //tempSb.append("=");
                }
                else if (match(cs, i, "<%@"))
                {
                    i += 2;
                    status = 1;
                    tempAl.add(tempSb);
                    tempSb = new StringBufferWraper(StringBufferWraper.TYPE.PAGE);
                    //tempSb.append("@");
                }
                else if (match(cs, i, "<%!"))
                {
                    i += 2;
                    status = 1;
                    tempAl.add(tempSb);
                    tempSb = new StringBufferWraper(StringBufferWraper.TYPE.OUTER);
                    //tempSb.append("!");
                }
                else if (match(cs, i, "<%"))
                {
                    i++;
                    status = 1;
                    tempAl.add(tempSb);
                    tempSb = new StringBufferWraper(StringBufferWraper.TYPE.JAVA);
                    //tempSb.append("J");
                }
                else if (match(cs, i, "\r\n"))
                {
                    i++;
                    tempAl.add(tempSb);
                    tempSb = new StringBufferWraper(StringBufferWraper.TYPE.HTML);
                    //tempSb.append("H");
                }
                else if (cs[i] == '\r')
                {
                    tempAl.add(tempSb);
                    tempSb = new StringBufferWraper(StringBufferWraper.TYPE.HTML);
                    //tempSb.append("H");
                }
                else if (cs[i] == '\n')
                {
                    tempAl.add(tempSb);
                    tempSb = new StringBufferWraper(StringBufferWraper.TYPE.HTML);
                    //tempSb.append("H");
                }
                else
                {
                    tempSb.append(cs[i]);
                }
            }
            else
            {
                if (match(cs, i, "%>"))
                {
                    i++;
                    status = 0;
                    tempAl.add(tempSb);
                    tempSb = new StringBufferWraper(StringBufferWraper.TYPE.HTML);
                    //tempSb.append("H");
                }
                else
                {
                    tempSb.append(cs[i]);
                }
            }
        }

        tempAl.add(tempSb);

        StringBuffer sb_import = new StringBuffer();
        StringBuffer sb_inner = new StringBuffer();
        StringBuffer sb_outer = new StringBuffer();
        String authorization = null;
        String classname = jspFile.getName().substring(0, jspFile.getName().length() - 4);

        for (int i = 0; i < tempAl.size(); i++)
        {
            StringBufferWraper sLine = tempAl.get(i);

            if (sLine.type == StringBufferWraper.TYPE.HTML)
            {
                String sHtml;
                sHtml = sLine.toString().replace("\\", "\\\\");
                sHtml = sHtml.replace("\"", "\\\"");
                if (sHtml.length() > 0)
                {
                    if ((i + 1 < tempAl.size()) && (tempAl.get(i + 1).type == StringBufferWraper.TYPE.OUTPUT))
                    {
                        sb_inner.append("out.print(\"");
                    }
                    else
                    {
                        sb_inner.append("out.println(\"");
                    }
                    sb_inner.append(sHtml);
                    sb_inner.append("\");\r\n");
                }
            }
            else if (sLine.type == StringBufferWraper.TYPE.JAVA)
            {
                sb_inner.append(sLine.toString());
            }
            else if (sLine.type == StringBufferWraper.TYPE.PAGE)
            {
                String sLine2 = sLine.toString().trim();

                if (sLine2.startsWith("page import"))
                {
                    String v = sLine2.substring("page import".length() + 1);
                    v = v.replace('\"', ' ');
                    v = v.replace(" ", "");
                    String[] vv = v.split(",");
                    for (int j = 0; j < vv.length; j++)
                    {
                        sb_import.append("import " + vv[j] + ";\r\n");
                    }
                }
                if (sLine2.startsWith("include"))
                {
                    String v = sLine2.substring("include".length() + 1);
                    v = v.replace('\\', '/');
                    sb_inner.append("includeJSP(\"" + v + "\");\r\n");
                }
                if (sLine2.startsWith("authorization"))
                {
                    String v = sLine2.substring("authorization".length());
                    authorization = v;
                }
            }
            else if (sLine.type == StringBufferWraper.TYPE.OUTER)
            {
                sb_outer.append(sLine.toString() + "\r\n");
            }
            else if (sLine.type == StringBufferWraper.TYPE.OUTPUT)
            {
                sb_inner.append("out.print(" + sLine.toString() + ");\r\n");
            }
        }

        FileOutputStream fos = new FileOutputStream(javaFile);
        OutputStreamWriter osw = new OutputStreamWriter(fos, server.currentConfig.defaultEncoding);
        PrintWriter pw = new PrintWriter(osw);

        pw.println("import java.io.PrintStream;");
        pw.println("import webServer.JspPage;");
        pw.println("import webServer.Request;");
        pw.println("import webServer.Response;");
        pw.println("import webServer.Session;");
        pw.println("import webServer.Server;");
        pw.println(sb_import.toString());
        pw.println("public class " + classname + " extends JspPage {");
        pw.println("  public boolean authorization(String username, String password){");
        if (authorization == null)
        {
            pw.println("  return true;");
        }
        else
        {
            pw.println(authorization);
        }
        pw.println("	 }");
        pw.println(sb_outer.toString());
        pw.println("	public void doRequest() throws Exception{");
        pw.println(sb_inner.toString());
        pw.println("	}");
        pw.println("}");
        pw.flush();
        fos.close();
    }

    private boolean match(char[] c, int idx, String s)
    {
        char[] c2 = s.toCharArray();
        for (int i = 0; i < c2.length; i++)
        {
            if (i + idx < c.length)
            {
                if (c[i + idx] != c2[i])
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }
}

class StringBufferWraper {
    enum TYPE {
        HTML, // H
        JAVA, // J
        PAGE, // @
        OUTER, // !
        OUTPUT // =
    }

    StringBuffer sb = new StringBuffer();
    TYPE type;
    StringBufferWraper(TYPE type) {
        this.type = type;
    }

    @Override
	public String toString()
    {
        return sb.toString();
    }

    public void append(char c)
    {
        sb.append(c);
    }
}

class JspClassPack
{
    JspClassPack(Class<?> clazz, long lastModified)
    {
        this.clazz = clazz;
        this.lastModified = lastModified;
    }

    Class<?> clazz;

    long lastModified;
}

final class JspClassLoader extends ClassLoader
{
    @Override
    public synchronized Class<?> loadClass(String className) throws ClassNotFoundException
    {
        Class<?> newClass = null;

        // needed!!!!!!!!
        try
        {
            newClass = findSystemClass(className);
            return newClass;
        }
        catch (ClassNotFoundException e)
        {
            //ClassNotFoundException ok
        }

        byte[] classData;

        classData = getClassDataFILE(className);

        if (classData == null)
        {
            throw new ClassNotFoundException();
        }
        newClass = defineClass(null, classData, 0, classData.length);

        return newClass;
    }

    byte[] getClassDataFILE(String className)
    {
        byte[] data;
        int length;
        try
        {
            File file = new File(className);

            if (file.exists())
            {
                if (file.isDirectory())
                {
                    throw new Exception();
                }
            }
            else
            {
                throw new Exception();
            }

            length = (int) file.length();

            FileInputStream inputStream = new FileInputStream(file);
            data = new byte[length];
            inputStream.read(data);
            inputStream.close();
            return data;
        }
        catch (Exception e)
        {
            return null;
        }
    }
}
