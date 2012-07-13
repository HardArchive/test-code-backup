
#include "stdio.h"
#include "reec.h"

#pragma comment(lib,"REEC.lib") 
int main(int argc , char** argv)
{
	//正则式：<(a-z)^3>、<(a-z)^*>、<(a-z){2,3}>、<(a-z){2,*}>
	//<(a-z)^3>
	//匹配3个字母，出现位置不必连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z)^3>","你好a我们是b突壳c开源，欢迎使用1.0我们的产品。",RS_LEVEL_0));
	//结果 abc

	//匹配3个字母，出现位置必须连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z)^3>","你好a我们是b突壳c开源，欢迎使用1.0我们的产品。",RS_LEVEL_1));
	//不连续，结果 null

	//<(a-z)^*>
	//匹配所有字母，出现位置不必连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z)^*>","你好a我们是b突壳c开源，欢迎使用1.0我们的产品。",RS_LEVEL_0));
	//结果 abc

	//匹配n个字母，出现位置必须连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z)^*>","你好a我们是b突壳c开源，欢迎使用1.0我们的产品。",RS_LEVEL_1));
	//结果 a

	//<(a-z){2,3}>
	//匹配至少2个字母，至多3个字母，出现位置不必连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,3}>","你好a我们是b突壳c开源，欢e迎d使f用1.0我们的f产品。",RS_LEVEL_0));
	//结果 abc

	//匹配至少2个字母，至多3个字母，出现位置不必连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,3}>","你好a我们是突壳开源，欢迎使用1.0我们的产品。",RS_LEVEL_0));
	//不足2个字母，结果 null

	//匹配至少2个字母，至多3个字母，出现位置不必连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,3}>","你好a我们是b突壳c开源，欢迎使用1.0我们的产品。",RS_LEVEL_1));
	//不连续，结果 null

	//<(a-z){2,*}>
	//匹配至少2个小写字母，至多n个小写字母，出现位置不必连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,*}>","你好a我们是b突A壳c开源，欢F迎G使G用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 abcdef

	//匹配至少2个小写字母，至多n个小写字母，出现位置不必连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,*}>","你好A我们是B突A壳C开源，欢F迎G使G用1.0我D们E的F产品。",RS_LEVEL_0));
	//结果 null

	//匹配至少2个字母，至多n个字母，出现位置必须连续
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,*}>","你好a我们是b突A壳c开源，欢F迎G使G用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 null

	//1连续，0不连续
	//以下是匹配大写字母，正则式：<(A-Z)^3>、<(A-Z)^*>、<(A-Z){2,3}>、<(A-Z){2,*}>
	//<(A-Z)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^3>","你好a我们是b突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 ABC

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^3>","你好a我们是b突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 null

	//<(A-Z)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^*>","你好a我们是b突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 ABC

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^*>","你好a我们是b突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 A

	//<(A-Z){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,3}>","你好a我们是b突A壳c开源，欢B迎C使D用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 ABC

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,3}>","你好a我们是b突A壳c开源，欢迎使用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,3}>","你好a我们是b突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 null

	//<(A-Z){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,*}>","你好a我们是b突A壳c开源，欢B迎C使用1.0我d们e的f产D品。",RS_LEVEL_0));
	//结果 ABCD

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,*}>","你好a我们是b突A壳c开源，欢迎使用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,*}>","你好a我们是b突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 null

	//以下是匹配大小写字母，正则式：<(a-zA-Z)^3>、<(a-zA-Z)^*>、<(a-zA-Z){2,3}>、<(a-zA-Z){2,*}>
	//<(a-zA-Z)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^3>","你好a我们是突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 aAc

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^3>","你好a我们是突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 null

	//<(a-zA-Z)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^*>","你好a我们是突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 aAcBCdef

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^*>","你好a我们是突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 a

	//<(a-zA-Z){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,3}>","你好a我们是突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 aAc

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,3}>","你好a我们是突壳开源，欢迎使用1.0我们的产品。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,3}>","你好a我们是突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 null

	//<(a-zA-Z){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,*}>","你好a我们是突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_0));
	//结果 aAcBCdef

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,*}>","你好a我们是突壳开源，欢迎使用1.0我们的产品。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,*}>","你好a我们是突A壳c开源，欢B迎C使用1.0我d们e的f产品。",RS_LEVEL_1));
	//结果 null

	//以下是匹配数字，正则式：<(0-9)^3>、<(0-9)^*>、<(0-9){2,3}>、<(0-9){2,*}>
	//<(0-9)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9)^3>","你好我们1是2突壳3开源，欢迎使用1.0我们的产品。",RS_LEVEL_0));
	//结果 123

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9)^3>","你好我们1是2突壳3开源，欢迎使用1.0我们的产品。",RS_LEVEL_1));
	//结果 null

	//<(0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9)^*>","你好我们1是2突壳3开源，欢迎使用1.0我们的产品。",RS_LEVEL_0));
	//结果 12310

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9)^*>","你好我们1是2突壳3开源，欢迎使用1.0我们的产品。",RS_LEVEL_1));
	//结果 1

	//<(0-9){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","你好我们1是2突壳3开源，欢迎使用1.0我们的产品。",RS_LEVEL_0));
	//结果 123

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","你好我们1是突壳开源，欢迎使用我们的产品。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","你好我们12是2突壳3开源，欢迎使用1.0我们的产品。",RS_LEVEL_1));
	//结果 12

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","你好我们1是突壳开源，欢迎使用我们的产品。",RS_LEVEL_1));
	//结果 null

	//<(0-9){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,*}>","你好我们1是2突壳3开源，欢迎使用1.0我们1的2产3品。",RS_LEVEL_0));
	//结果 12310123

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,*}>","你好我们1是突壳开源，欢迎使用我们的产品。",RS_LEVEL_1));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","你好我们12345是突壳开源，欢迎使用1.0我们1的2产3品。",RS_LEVEL_1));
	//结果 123

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","你好我们1是突壳开源，欢迎使用1.0我们1的2产3品。",RS_LEVEL_1));
	//结果 null

	//以下是匹配数字以及小写字母，正则式：<(a-z0-9)^3>、<(a-z0-9)^*>、<(a-z0-9){2,3}>、<(a-z0-9){2,*}>
	//<(a-z0-9)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9)^3>","你好我们12a3是突壳开源，欢迎使用1.0我们1的2产3品。",RS_LEVEL_0));
	//结果 12a

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9)^3>","你好我们12是a突壳开源，欢迎使用1.0我们1的2产3品。",RS_LEVEL_1));
	//结果 null

	//<(a-z0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9)^*>","你好我们12a3是突a壳开d源，欢迎b使f用1.0我c们1的2产3品。",RS_LEVEL_0));
	//结果 12a3adbf10c123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9)^*>","你好我们a3是突a壳开d源，欢迎b使f用1.0我c们1的2产3品。",RS_LEVEL_1));
	//结果 a3

	//<(a-z0-9){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","你好我们1a是突a壳开d源，欢迎b使f用1.0我c们1的2产3品。",RS_LEVEL_0));
	//结果 1aa

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","你好我们1是突壳开源，欢迎使用我们的产品。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","你好我们1a是突a壳开d源，欢迎b使f用1.0我c们1的2产3品。",RS_LEVEL_1));
	//结果 1a

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","你好我们1是a突a壳开d源，欢迎b使f用1.0我c们1的2产3品。",RS_LEVEL_1));
	//结果 null

	//<(a-z0-9){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,*}>","你好我们12a3是突a壳开d源，欢迎b使f用1.0我c们1的2产3品。",RS_LEVEL_0));
	//结果 12a3adbf10c123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,*}>","你好我们12A3是突B壳开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 12310123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,*}>","你好我们1a3是突B壳开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 1a3

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","你好我们12a3是突B壳开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 12a

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","你好我们1是a突B壳开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 null

	//以下是匹配数字以及大写字母，正则式：<(A-Z0-9)^3>、<(A-Z0-9)^*>、<(A-Z0-9){2,3}>、<(A-Z0-9){2,*}>
	//<(A-Z0-9)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9)^3>","你好我们12A3是突B壳开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 12A

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9)^3>","你好我们1A是突B壳开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 null

	//<(A-Z0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9)^*>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 12A3BCDE10F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9)^*>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 1

	//<(A-Z0-9){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 12A

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","你好我们1a23是e突壳f开源，欢迎使用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 123

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","你好我们1A23是e突壳f开源，欢迎使用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 1A2

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","你好我们1a23是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 23

	//<(A-Z0-9){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,*}>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 12A3BCDE10F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,*}>","你好我们1a23是e突壳f开源，欢迎使用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 12310F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,*}>","你好我们1aA3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 A3

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","你好我们1aA123是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 A12

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","你好我们1a23是e突壳f开源，欢迎使用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 23

	//以下是匹配数字以及大小写字母，正则式：<(a-zA-Z0-9)^3>、<(a-zA-Z0-9)^*>、<(a-zA-Z0-9){2,3}>、<(a-zA-Z0-9){2,*}>
	//<(a-zA-Z0-9)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9)^3>","你好我们1aA3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 1aA

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9)^3>","你好我们1a是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 null

	//<(a-zA-Z0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9)^*>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 1a2A3eBfCDE10F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9)^*>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 1a2A3

	//<(a-zA-Z0-9){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 1a2

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","你好我们是突壳开源，欢迎使用我们的产3品。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 1a2

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","你好我们1是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 null

	//<(a-zA-Z0-9){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,*}>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_0));
	//结果 1a2A3eBfCDE10F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,*}>","你好我们是突壳开源，欢迎使用我们的产3品。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,*}>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 1a2A3

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","你好我们1a2A3是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 1a2

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","你好我们1是e突B壳f开C源，欢迎D使E用1.0我F们1的2产3品。",RS_LEVEL_1));
	//结果 null

	//以下是匹配数字以及大小写字母，正则式：<(symbol)^3>、<(symbol)^*>、<(symbol){2,3}>、<(symbol){2,*}>
	//<(突壳开源)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源)^3>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳开源突壳开源突壳开源

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源)^3>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_1));
	//结果 null

	//<(突壳开源)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源)^*>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳开源突壳开源突壳开源突壳开源突壳开源

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源)^*>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_1));
	//结果 突壳开源

	//<(突壳开源){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,3}>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳开源突壳开源突壳开源

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,3}>","突a壳开源，欢迎使用突壳d开源产品，欢迎来到突d壳开源，突壳d开源致力于基础软件开发与研究，突壳d开源。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,3}>","突壳开源突壳开源突壳开源突壳开源，欢迎使用，欢迎来到，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果 突壳开源突壳开源突壳开源

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,3}>","突壳开源突壳开源突壳开源突壳开源，欢迎使用，欢迎来到，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果 突壳开源突壳开源突壳开源

	//<(突壳开源){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,*}>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳开源突壳开源突壳开源突壳开源突壳开源

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,*}>","突a壳开源，欢迎使用突壳d开源产品，欢迎来到突d壳开源，突壳d开源致力于基础软件开发与研究，突壳d开源。",RS_LEVEL_0));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,*}>","欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果 null

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,3}>","突壳开源突壳开源突壳开源突壳开源，欢迎使用，欢迎来到，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果 突壳开源突壳开源突壳开源

	printf("%s\n",TOKRegularExpressionEngineRun("<(突壳开源){2,3}>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_1));
	//结果 null

	//以下是匹配任意字符，正则式：<(*)^4>、<(*)^*>、<(*){2,4}>、<(*){2,*}>
	//<(*)^4>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*)^4>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳

	printf("%s\n",TOKRegularExpressionEngineRun("<(*)^4>","突",RS_LEVEL_1));
	//结果 null

	//<(*)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*)^*>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。

	printf("%s\n",TOKRegularExpressionEngineRun("<(*)^*>","突壳开源",RS_LEVEL_1));
	//结果 突壳开源

	//<(*){2,4}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*){2,4}>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳

	printf("%s\n",TOKRegularExpressionEngineRun("<(*){4,6}>","突",RS_LEVEL_0));
	//结果 null

	//<(*){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*){2,*}>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。

	printf("%s\n",TOKRegularExpressionEngineRun("<(*){4,*}>","突",RS_LEVEL_0));
	//结果 null

	//<(*){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*){2,*}>","突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果 突壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究，突壳开源。

	printf("%s\n",TOKRegularExpressionEngineRun("<(*){4,*}>","突",RS_LEVEL_0));
	//结果 null

	//综合测试：
	//<(A-Z)^2><(a-z)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^2><(a-z)^3>","突TOKSmart Compiler壳开源，欢迎使用突壳开源产品，欢迎来到突壳开源，突壳开源致力于基础软件开发与研究。",RS_LEVEL_1));
	//结果：KSmar

	//[<(0-9)^5>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(0-9)^5>]","突壳开源，欢迎使用突壳开源产品，欢9.00迎来到突壳开源，突壳123345开源致力于基础软件开发与研究，突壳开源。",RS_LEVEL_0));
	//结果：90012

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


	return 0 ;
}
