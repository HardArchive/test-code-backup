// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include "reec/reec.h"
#define __CPLUSCPLUS_USED__ 1     //这个是定义编译环境还C++
#include "reec.h"
#pragma comment(lib, "REEC.lib") 
 

int _tmain(int argc, _TCHAR* argv[])
{
	//[<(0-9)^5>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(0-9)^5>]","突壳开源，欢迎使用突壳开源产品，欢9.00迎来到突壳开源，突壳123345开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_1));
	//结果：12334

	//<(a-zA-Z)^6>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^6>","突TOKSmart Compiler壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果：TOKSma

	//<(突壳)^1><(0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳)^1><(0-9)^*>","突壳2008开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果：突壳2008

	//[<(突壳)^1><(0-9){3,6}>][<(0-9){3,6}>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(突壳)^1><(0-9){3,6}>][<(0-9){3,6}>]","突壳2008开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果：2008

	//[<(突壳开源)^1><(你好)^1><(0-9)^*>][<(你好)^1><(0-9)^*>][<(0-9)^*>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(突壳开源)^1><(你好)^1><(0-9)^*>][<(你好)^1><(0-9)^*>][<(0-9)^*>]","突壳开源你好2008，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果：2008

	//[<(突壳开源)^1>|<(开源突壳)^1><(0-9)^*>][<(突壳)^1>|<(开源)^1><(0-9)^*>][<(0-9)^*>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(突壳开源)^1>|<(开源突壳)^1><(0-9)^*>][<(突壳)^1>|<(你好)^1><(0-9)^*>][<(0-9)^*>]","突壳好2008，开源突壳2008欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果：2008

	//<(http://)^1><(www.)^1><(a-z)^*>|<(A-Z)^*>|<(0-9)^*><(.com)^1>|<(.net)^1>|<(.cc)^1>
	printf("%s\n",TOKRegularExpressionEngineRun("<(http://)^1><(www.)^1><(a-z)^*>|<(A-Z)^*>|<(0-9)^*><(.cc)^1>|<(.com)^1>|<(.net)^1>","突壳开源你好2008，欢http://www.gtkings.c迎使用突http://www.tok.cc壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果：http://www.tok.cc

	return 0;
}

