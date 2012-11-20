#include "stdafx.h"
#include "CVodStreamCache.h"
#include <dos.h>

int main()
{
	CVodStreamCache vsc;
	vsc.Create("", "www.9cpp.com");  //www.vijaymukhi.com/vmis/olec.txt

	int rcv_bytes, i=0;
	char buff[1025];
	/*while (1)
	{
		rcv_bytes = vsc.WriteBlock(i, 1024, buff);
		if (rcv_bytes <= 0)
		{
			break;
		}
		i += rcv_bytes;
		system("pause");
	}*/
	vsc.WriteBlock(1024, 1024, buff);
	vsc.WriteBlock(0, 1024, buff);
	vsc.WriteBlock(2048, 1024, buff);

	vsc.Destroy();


	/*CVodStreamCache vsc;
	int rcv_bytes, i=0;
	char buff[1025];
	while (1)
	{
		rcv_bytes = vsc.ReadBlock(i, 1024, buff);
		if (rcv_bytes <= 0)
		{
			break;
		}
		cout << rcv_bytes << endl;
		cout << buff << endl;
		i += rcv_bytes;
		system("pause");
	}*/

	cout << "Hello,world" << endl;
	return 0;
}