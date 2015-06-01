<html>
<table border=1>
<%
int n1=0x59;
for(int i=0;i<15;i++)
{
%>
<tr>
<%
for(int j=0;j<15;j++)
{
int val = n1*0x0100+i*0x10+j;
String s = new String(new char[]{(char)val});
String v = Integer.toString(val,16);
if(val<0x10)
{
	v="000"+v;
}
else if(val<0x100)
{
	v="00"+v;
}
else if(val<0x1000)
{
	v="0"+v;
}
v=v.toUpperCase();
%>
<td>
<img src="http://www.decodeunicode.org/data/glyph/26x26/<%=v%>.gif"><%=s%>(<%=v%>)
</td>

<%}%>
</tr>
<%}%>
</table>
</html>