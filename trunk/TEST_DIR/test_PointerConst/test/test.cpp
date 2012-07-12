// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "vld.h"
//#include "../vld/vld.h"
char* g_pCSS = "<STYLE type=text/css>table { font-family: 'Courier New', Courier, mono;font-size: 12px;color: #336600;border-top-style: none;border-right-style: dashed; border-right-width: 2px; border-bottom-style: dashed;border-left-style: none;border-bottom-width: 2px; border-left-width: 2px;word-break: break-all;}td { border-top-style: none; border-right-style: none;   border-bottom-style: none;  border-left-style: dashed;  border-left-width: 2px;word-break: break-all;} th { font-family: 'Courier New', Courier, mono;  font-size: 14px;    font-weight: 400;   color: #FFFFFF; background-color: #339933;  border: none; border-left-style: dashed; border-left-width: 2px; }.ztitle {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ffd165; BORDER-BOTTOM: windowtext 1.5pt solid; mso-style-parent: style0}.z1 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ebebeb; BORDER-BOTTOM: windowtext 0.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}.z2 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ffd165; BORDER-BOTTOM: windowtext 1.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}.z3 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #c8c840; BORDER-BOTTOM: windowtext 1.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}</STYLE>";
int _tmain(int argc, _TCHAR* argv[])
{
	printf("CSS内容:%s", g_pCSS);
	return 0;
}

