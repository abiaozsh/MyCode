<html>
<head>
<script src="../file/jquery-1.12.3.js"></script>
<script>
  function printf(val){
    var str = $("#txtResult").val();
    str = str+val+"\r\n";
    $("#txtResult").val(str);
  }

</script>
</head>
<body>
1:数值计算<br/>
这个教程的目的是为非计算机专业的人士提供一个简单的编程工具，<br/>
让计算机帮助解决一些问题。<br/>
编程是与计算机交流的一种方式，计算机的基本用途就是“计算”，<br/>
下面我们来看下如何用编程的方式进行计算。<br/>
先来看看最简单的：大家可以把加号换成- * /，或者把1换成其他数字。<br/>
这里的printf表示把（）内的内容打印到运行结果中去。<br/>
分号表示一个语句的结束，这个程序只有一个语句。<br/>
程序：<textarea id="txtSrcCode" style="width:300px;height:100px;">
printf(1+1);
</textarea><br/>
<input type="button" value="执行" id="buttonExec"></input><br/>
运行结果：<textarea id="txtResult" style="width:300px;height:100px;"></textarea>
<script>
$("#buttonExec").bind("click",function(){
  try{
  eval("\"use strict\";"+$("#txtSrcCode").val());
  }catch(e){
  $("#txtResult").val(e);
  }
});
</script>
</body>
</html>

