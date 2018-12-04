import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;

public class tutorial2 extends JspPage {
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
out.println("2:变量<br/>");
out.println("变量是编程中储存数据的容器。<br/>");
out.println("变量可以接受数据，也可以参与计算。<br/>");
out.println("下面程序里的//表示后面的内容是注释，不会被执行。<br/>");
out.println("同样的，大家也可以修改下下面的程序，看看会有什么结果<br/>");
out.println("程序：<textarea id=\"txtSrcCode\" style=\"width:650px;height:180px;\">");
out.println("var i;//var 表示变量的定义，变量需要先定义，然后使用。");
out.println("var j;//课外话：我们学习的语言是js，数据不分类型，在更强大的c或java语言中，");
out.println("var k;//        数据是区分类型的，这里会写成\"int i;\"。");
out.println("i = 1;// =表示赋值，把1这个值放到变量i中去，执行后i的内容就是1了");
out.println("j = 2;// 2 放到 j 中");
out.println("k = i + j;//变量参与计算：计算i和j相加，结果放到k中去。 ");
out.println("          //i的值是1 j的值是2 相加等于3 所以k等于3");
out.println("printf(k);//打印k");
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
