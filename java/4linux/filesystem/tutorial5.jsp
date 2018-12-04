<html>
<head>
<script src="jquery-1.12.3.js"></script>
<script>
  function printf(val){
    var str = $("#txtResult").val();
    str = str+val+"\r\n";
    $("#txtResult").val(str);
  }
</script>
</head>
<body>
5:函数<br/>


程序：<textarea id="txtSrcCode" style="width:700px;height:200px;">
var a = 1;//这种写法相当于var a; 和 a = 3; 的组合。
var b = 2;
var i;
var j;
i = 5;
j = 2;
var k;
k = (i + j) * (a + b);//先计算 i + j 再计算 a + b ，然后将两个结果相乘，积放到k中去。
var l;
l = i + j * a + b;//如果不加括号，先计算j * a 再把结果和 i 和 b 相加，放到l中。
var m;
m = (i - (a + b)) * j;//先计算a + b，再被 i 减，再把结果和 b 相乘，放到m中。
printf("k:" + k);//这里的 + 表示字符串连接，字符串也是表达式的一种。以后会讲到。
printf("l:" + l);
printf("m:" + m);
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

