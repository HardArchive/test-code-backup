<?xml version="1.0" encoding="GB2312"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">


<html>
 <head>
  <title><xsl:value-of select="LogFile/AppName" /> 日志文件</title>
 </head>
 <body>
  <h1><xsl:value-of select="LogFile/AppName" /> 运行日志文件</h1>

  <table border="0" cellspacing="1" cellpadding="0">
   <tr>
    <td bgcolor="#dddddd">
     <table border="0" cellpadding="3" cellspacing="1">
      <tr>
       <td bgcolor="#ffffff"> </td>
       <td width="140" valign="center" bgcolor="#ffffff"><b>日期－时间</b></td>
       <td bgcolor="#ffffff"><b>错误信息</b></td>
       <td bgcolor="#ffffff"><b>具体位置信息</b></td>
      </tr>

      <xsl:for-each select="LogFile/LogItem">
       <tr onMouseover="">
        <td bgcolor="#ffffff" valign="top"><xsl:number count="LogItem" />.</td>
        <td bgcolor="#ffffff" valign="top"><nobr><xsl:value-of select="Date" /></nobr><xsl:value-of select="Time" /></td>
        <td bgcolor="#ffffff" valign="top"><xsl:value-of select="comment" /></td>
        <td bgcolor="#ffffff" valign="top"><xsl:value-of select="Filename" /></td>
       </tr>
      </xsl:for-each>
     </table>
    </td>
   </tr>
  </table>

  <font style="font-size:5.0mm;"><br/>Log.xsl by force eagle &lt;<a href="mailto:force_eagle@163.net">force_eagle@163.net</a>&gt;</font>

 </body>
</html>

</xsl:template>
</xsl:stylesheet>
