<html>
<head>
<script>
function on_load()
{
 setScroll(document.formMain.scroll.value);
}

function do_action(action,value,value1,value2,value3,value4,value5)
{
	document.formMain.scroll.value = getScroll();
	document.formMain.action.value = action;
	document.formMain.value.value = value;
	document.formMain.value1.value = value1;
	document.formMain.value2.value = value2;
	document.formMain.value3.value = value3;
	document.formMain.value4.value = value4;
	document.formMain.value5.value = value5;
	
	document.formMain.submit();
}
function getScroll(){
 var t;
 if (document.documentElement && document.documentElement.scrollTop) {
  t = document.documentElement.scrollTop;
 } else if (document.body) {
  t = document.body.scrollTop;
 }
 return t;
}
function setScroll(t){
 if (document.documentElement && document.documentElement.scrollTop) {
  document.documentElement.scrollTop = t;
 } else if (document.body) {
  document.body.scrollTop = t;
 }
}

</script>
</head>
<body onload = "on_load();">

<form name = 'formMain' action = '<%=server.currentConfig.configPage%>' method = 'post'>
<input type = hidden name = "action" id = "action">
<input type = hidden name = "value" id = "value">
<input type = hidden name = "value1" id = "value1">
<input type = hidden name = "value2" id = "value2">
<input type = hidden name = "value3" id = "value3">
<input type = hidden name = "value4" id = "value4">
<input type = hidden name = "value5" id = "value5">
<%
	Integer sc = (Integer)session.getPrivate("config_scroll_position");
	int scroll=0;
	if(sc!=null)
	{
		scroll=sc.intValue();
	}
%>
<input type = hidden name = "scroll" id = "scroll" value = "<%=scroll%>">
</form>

<table border="1">
 <tr>
  <td><a href = "javascript:do_action('reset');">Apply config and reset server.</a></td>
  <td><a href = "javascript:do_action('shutdown');">Shutdown server.</a></td>
  <td><a href = "javascript:do_action('makeXML');">Write config to XML.</a></td>
  <td><a href = "javascript:do_action('exit');">Exit.</a></td>
 </tr>
</table>

<table border="1">
 <tr>
  <td>port:</td>
  <td><input type = text id = 'port' value = '<%=server.currentConfig.port%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_port',port.value);"></td>
 </tr>

 <tr>
  <td>SSLactive:</td>
  <td><%=webServer.ConfigServlet.getPullDown("SSLactive",server.currentConfig.SSLactive)%></td>
  <td><input type = button value = 'set' onclick = "do_action('set_SSLactive',SSLactive.value);"></td>
 </tr>
<%if(server.currentConfig.SSLactive){%>
 <tr>
  <td>SSLport:</td>
  <td><input type = text id = 'SSLport' value = '<%=server.currentConfig.SSLport%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_SSLport',SSLport.value);"></td>
 </tr>

 <tr>
  <td>clientKeysFile:</td>
  <td><input type = text id = 'SSLclientKeysFile' value = '<%=server.currentConfig.SSLclientKeysFile%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_SSLclientKeysFile',SSLclientKeysFile.value);"></td>
 </tr>

 <tr>
  <td>SSLkeyStorePass:</td>
  <td><input type = password id = 'SSLkeyStorePass' value = '<%=server.currentConfig.SSLkeyStorePass%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_SSLkeyStorePass',SSLkeyStorePass.value);"></td>
 </tr>

 <tr>
  <td>SSLkeyPassword:</td>
  <td><input type = password id = 'SSLkeyPassword' value = '<%=server.currentConfig.SSLkeyPassword%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_SSLkeyPassword',SSLkeyPassword.value);"></td>
 </tr>
<%}%>

 <tr>
  <td>javaCompiler:</td>
  <td><input type = text id = 'javaCompiler' value = '<%=server.currentConfig.javaCompiler%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_javaCompiler',javaCompiler.value);"></td>
 </tr>

 <tr>
  <td>defaultEncoding:</td>
  <td><input type = text id = 'defaultEncoding' value = '<%=server.currentConfig.defaultEncoding%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_defaultEncoding',defaultEncoding.value);"></td>
 </tr>
 
 <tr>
  <td>sessionExpires(in ms):</td>
  <td><input type = text id = 'sessionExpires' value = '<%=server.currentConfig.sessionExpires%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_sessionExpires',sessionExpires.value);"></td>
 </tr>
 
 <tr>
  <td>configPageEnabled:</td>
  <td><%=webServer.ConfigServlet.getPullDown("configPageEnabled",server.currentConfig.configPageEnabled)%></td>
  <td><input type = button value = 'set' onclick = "do_action('set_configPageEnabled',configPageEnabled.value);"></td>
 </tr>
 
 <tr>
  <td>configPageURL:</td>
  <td><input type = text id = 'configPage' value = '<%=server.currentConfig.configPage%>'></td>
  <td><input type = button value = 'set' onclick = "do_action('set_configPage',configPage.value);"></td>
 </tr>

 <tr>
  <td colspan = 3>
  	config accessable IP.
		<table border="1" width = 100%>
		 <tr>
		  <td>IP/HOSTNAME</td>
		  <td align="right">operation</td>
		 </tr>
		<%
		 for(int i=0;i<server.currentConfig.allowConfigIPs.size(); i++){
		  String s = (String)server.currentConfig.allowConfigIPs.get(i);
		%>
		 <tr>
		  <td><%=s%></td>
		  <td align="right">
		   <input type = button value = 'del' onclick = "do_action('del_allowConfigIPs',<%=i%>);">
		  </td>
		 </tr>
		<%
		 }
		%>
		 <tr>
		  <td><input type = text id = 'allowConfigIPs' value = ''></td>
		  <td align="right"><input type = button value = 'add' onclick = "do_action('add_allowConfigIPs',allowConfigIPs.value);"></td>
		 </tr>
		</table>
  </td>
 </tr>

 <tr>
  <td>config admin old password:</td>
  <td><input type = password id = 'configPass1' value = ''></td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>config admin new password:</td>
  <td><input type = password id = 'configPass2' value = ''></td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>config admin new password confirm:</td>
  <td><input type = password id = 'configPass3' value = ''></td>
  <td><input type = button value = 'set' onclick = "do_action('set_configPass',configPass1.value,configPass2.value,configPass3.value);"></td>
 </tr>
</table>
File System Setting:
<table border="1">
 <tr>
  <td>url</td>
  <td>rootPath</td>
  <td>allowList</td>
  <td>allowUpLoad</td>
  <td>allowDel</td>
  <td>passWord</td>
  <td>operation</td>
 </tr>
<%
 for(int i=0;i<server.currentConfig.fileSystems.size(); i++){
  webServer.Config_FileSystem cfs = (webServer.Config_FileSystem) server.currentConfig.fileSystems.get(i);
%>
 <tr>
  <td><%=cfs.url%></td>
  <td><%=cfs.rootPath%></td>
  <td><%=cfs.allowList%></td>
  <td><%=cfs.allowUpLoad%></td>
  <td><%=cfs.allowDel%></td>
  <td><%=cfs.passWord==null?"&nbsp;":"******"%></td>
  <td>
   <input type = button value = 'edit' onclick = "do_action('edit_fileSystem',<%=i%>);">
   <input type = button value = 'del' onclick = "do_action('del_fileSystem',<%=i%>);">
  </td>
 </tr>
<%
 }
%>
<%
Integer n = (Integer)session.getPrivate("edit_fileSystem");
if(n!=null){
int l = n.intValue();
  webServer.Config_FileSystem cfs = (webServer.Config_FileSystem) server.currentConfig.fileSystems.get(l);
%>
 <tr>
  <td><input type = text id = 'url' value = '<%=cfs.url%>'></td>
  <td><input type = text id = 'rootPath' value = '<%=cfs.rootPath%>'></td>
  <td><%=webServer.ConfigServlet.getPullDown("allowList",cfs.allowList)%></td>
  <td><%=webServer.ConfigServlet.getPullDown("allowUpLoad",cfs.allowUpLoad)%></td>
  <td><%=webServer.ConfigServlet.getPullDown("allowDel",cfs.allowDel)%></td>
  <td><input type = password id = 'passWord' value = '<%=cfs.passWord==null?"":cfs.passWord%>'></td>
  <td><input type = button value = 'update' onclick = "do_action('upd_fileSystem',url.value,rootPath.value,allowList.value,allowUpLoad.value,allowDel.value,passWord.value);"></td>
 </tr>
<%}else{%>
 <tr>
  <td><input type = text id = 'url' value = ''></td>
  <td><input type = text id = 'rootPath' value = ''></td>
  <td><%=webServer.ConfigServlet.getPullDown("allowList",false)%></td>
  <td><%=webServer.ConfigServlet.getPullDown("allowUpLoad",false)%></td>
  <td><%=webServer.ConfigServlet.getPullDown("allowDel",false)%></td>
  <td><input type = password id = 'passWord' value = ''></td>
  <td><input type = button value = 'add' onclick = "do_action('add_fileSystem',url.value,rootPath.value,allowList.value,allowUpLoad.value,allowDel.value,passWord.value);"></td>
 </tr>
<%}%>
</table>

</table>
JSP Setting:
<table border="1">
 <tr>
  <td>url</td>
  <td>rootPath</td>
  <td>operation</td>
 </tr>
<%
 for(int i=0;i<server.currentConfig.jspProcessors.size(); i++){
  webServer.Config_JspProcessor cjp = (webServer.Config_JspProcessor) server.currentConfig.jspProcessors.get(i);
%>
 <tr>
  <td><%=cjp.url%></td>
  <td><%=cjp.rootPath%></td>
  <td>
   <input type = button value = 'edit' onclick = "do_action('edit_jspProcessors',<%=i%>);">
   <input type = button value = 'del' onclick = "do_action('del_jspProcessors',<%=i%>);">
  </td>
 </tr>
<%
 }
%>
<%
n = (Integer)session.getPrivate("edit_jspProcessors");
if(n!=null){
int l = n.intValue();
  webServer.Config_JspProcessor cjp = (webServer.Config_JspProcessor) server.currentConfig.jspProcessors.get(l);
%>
 <tr>
  <td><input type = text id = 'JSPurl' value = '<%=cjp.url%>'></td>
  <td><input type = text id = 'JSProotPath' value = '<%=cjp.rootPath%>'></td>
  <td><input type = button value = 'update' onclick = "do_action('upd_jspProcessors',JSPurl.value,JSProotPath.value);"></td>
 </tr>
<%}else{%>
 <tr>
  <td><input type = text id = 'JSPurl' value = ''></td>
  <td><input type = text id = 'JSProotPath' value = ''></td>
  <td><input type = button value = 'add' onclick = "do_action('add_jspProcessors',JSPurl.value,JSProotPath.value);"></td>
 </tr>
<%}%>
</table>

</table>
servlet Setting:
<table border="1">
 <tr>
  <td>url</td>
  <td>class</td>
  <td>operation</td>
 </tr>
<%
 for(int i=0;i<server.currentConfig.servlets.size(); i++){
  webServer.Config_Servlet cs = (webServer.Config_Servlet) server.currentConfig.servlets.get(i);
%>
 <tr>
  <td><%=cs.url%></td>
  <td><%=cs.classFileName%></td>
  <td>
   <input type = button value = 'edit' onclick = "do_action('edit_servlets',<%=i%>);">
   <input type = button value = 'del' onclick = "do_action('del_servlets',<%=i%>);">
  </td>
 </tr>
<%
 }
%>
<%
n = (Integer)session.getPrivate("edit_servlets");
if(n!=null){
int l = n.intValue();
  webServer.Config_Servlet cs = (webServer.Config_Servlet) server.currentConfig.servlets.get(l);
%>
 <tr>
  <td><input type = text id = 'ServletUrl' value = '<%=cs.url%>'></td>
  <td><input type = text id = 'class' value = '<%=cs.classFileName%>'></td>
  <td><input type = button value = 'update' onclick = "do_action('upd_servlets',ServletUrl.value,class.value);"></td>
 </tr>
<%}else{%>
 <tr>
  <td><input type = text id = 'ServletUrl' value = ''></td>
  <td><input type = text id = 'class' value = ''></td>
  <td><input type = button value = 'add' onclick = "do_action('add_servlets',ServletUrl.value,class.value);"></td>
 </tr>
<%}%>
</table>

</body>
</html>