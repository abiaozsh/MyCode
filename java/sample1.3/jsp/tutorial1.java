import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;

public class tutorial1 extends JspPage {
  public boolean authorization(String username, String password){
  return true;
	 }

	public void doRequest() throws Exception{
out.println("﻿<html>");
out.println("<head>");
out.println("<script src=\"../file/jquery-1.12.3.js\"></script>");
out.println("<script>");
out.println("  function printf(val){");
out.println("    var str = $(\"#txtResult\").val();");
out.println("    str = str+val+\"\\r\\n\";");
out.println("    $(\"#txtResult\").val(str);");
out.println("  }");
out.println("</script>");
out.println("</head>");
out.println("<body>");
out.println("1:数值计算<br/>");
out.println("这个教程的目的是为非计算机专业的人士提供一个简单的编程工具，<br/>");
out.println("让计算机帮助解决一些问题。<br/>");
out.println("编程是与计算机交流的一种方式，计算机的基本用途就是“计算”，<br/>");
out.println("下面我们来看下如何用编程的方式进行计算。<br/>");
out.println("先来看看最简单的：大家可以把加号换成- * /，或者把1换成其他数字。<br/>");
out.println("这里的printf表示把（）内的内容打印到运行结果中去。<br/>");
out.println("分号表示一个语句的结束，这个程序只有一个语句。<br/>");
out.println("程序：<textarea id=\"txtSrcCode\" style=\"width:300px;height:100px;\">");
out.println("printf(1+1);");
out.println("</textarea><br/>");
out.println("<input type=\"button\" value=\"执行\" id=\"buttonExec\"></input><br/>");
out.println("运行结果：<textarea id=\"txtResult\" style=\"width:300px;height:100px;\"></textarea>");
out.println("<script>");
out.println("$(\"#buttonExec\").bind(\"click\",function(){");
out.println("  try{");
out.println("  eval(\"\\\"use strict\\\";\"+$(\"#txtSrcCode\").val());");
out.println("  }catch(e){");
out.println("  $(\"#txtResult\").val(e);");
out.println("  }");
out.println("});");
out.println("</script>");
out.println("</body>");
out.println("</html>");

	}
}
