#include "DTString.h"
using namespace std;

int main()
{
	DTString str;
	str.Format("%d,%s",1,"Hello123456789");
	cout<<str<<endl;
	
	return 0;
}
