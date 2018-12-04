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
2:变量<br/>
变量是编程中储存数据的容器。<br/>
变量可以接受数据，也可以参与计算。<br/>
下面程序里的//表示后面的内容是注释，不会被执行。<br/>
同样的，大家也可以修改下下面的程序，看看会有什么结果<br/>
程序：<textarea id="txtSrcCode" style="width:650px;height:180px;">
var i;//var 表示变量的定义，变量需要先定义，然后使用。
var j;//课外话：我们学习的语言是js，数据不分类型，在更强大的c或java语言中，
var k;//        数据是区分类型的，这里会写成"int i;"。
i = 1;// =表示赋值，把1这个值放到变量i中去，执行后i的内容就是1了
j = 2;// 2 放到 j 中
k = i + j;//变量参与计算：计算i和j相加，结果放到k中去。 
          //i的值是1 j的值是2 相加等于3 所以k等于3
printf(k);//打印k
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

