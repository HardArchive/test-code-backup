#include <stdio.h>
#include "Rigor.h"

typedef char CHARARR[256];

//¼ÆËã½×³Ë  
void Factorial(int iInput)
{
	itoa()
	int iTem = iInput;
	//if (iTem) bnReturn = 1;
	while(iTem)
	{
		//bnReturn = bnReturn*(iTem--);
		//bnReturn = Mul(bnReturn, iTem--);
	}
	return bnReturn;
}

int main()
{
	CHARARR num1;
	CHARARR num2;
	puts("Input two numbers,please:");
	while(scanf("%s%s",num1,num2)!=-1) {
		printf("The result of addition:\n%s\n",Result(num1,num2,WTAdd));
		printf("The result of subtration:\n%s\n",Result(num1,num2,WTSubtract));
		printf("The result of multiplication:\n%s\n",Result(num1,num2,WTMultiply));
		printf("The result of divide:\n%s\n",Result(num1,num2,WTDivide));
		puts("Input two numbers,please:");
	}
	return 0;
}
