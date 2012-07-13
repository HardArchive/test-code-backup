// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "pcap.h" 
#pragma comment (lib, "wpcap.lib")
#pragma comment (lib, "wsock32.lib")

//////////////////////////////通过回调的方式捕获数据包///////////////////////////////////////////////////////

///* packet handler 函数原型 */
//void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char 
//					*pkt_data); 
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	pcap_if_t *alldevs;
//	pcap_if_t *d;
//	int inum;
//	int i=0;
//	pcap_t *adhandle;
//	char errbuf[PCAP_ERRBUF_SIZE];
//
//	/* 获取本机设备列表 */
//	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
//	if (pcap_findalldevs(&alldevs, errbuf) == -1)
//	{
//		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
//		exit(1);
//	}
//
//	/* 打印列表 */
//	for(d=alldevs; d; d=d->next)
//	{
//		printf("%d. %s", ++i, d->name);
//		if (d->description)
//			printf(" (%s)\n", d->description);
//		else
//			printf(" (No description available)\n");
//	}
//
//	if(i==0)
//	{
//		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
//		return -1;
//	}
//
//	printf("Enter the interface number (1-%d):",i);
//	scanf("%d", &inum);
//
//	if(inum < 1 || inum > i)
//	{
//		printf("\nInterface number out of range.\n");
//		/* 释放设备列表 */
//		pcap_freealldevs(alldevs);
//		return -1;
//	}
//
//	/* 跳转到选中的适配器 */
//	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
//
//	/* 打开设备 */
//	//if ( (adhandle= pcap_open(d->name,          // 设备名
//	//	65536,            // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
//	//	PCAP_OPENFLAG_PROMISCUOUS,    // 混杂模式
//	//	1000,             // 读取超时时间
//	//	NULL,             // 远程机器验证
//	//	errbuf            // 错误缓冲池
//	//	) ) == NULL)
//
//	if ( (adhandle= pcap_open_live(d->name, // 设备名 
//								65536,      // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容. 
//								1,          // 混杂模式 
//								1000,       // 读取超时时间 
//								errbuf      // 错误缓冲池 
//								) ) == NULL) 
//	{
//		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
//		/* 释放设备列表 */
//		pcap_freealldevs(alldevs);
//		return -1;
//	}
//
//	printf("\nlistening on %s...\n", d->description);
//
//	/* 释放设备列表 */
//	pcap_freealldevs(alldevs);
//
//	/* 开始捕获 */
//	pcap_loop(adhandle, 0, packet_handler, NULL); //通过回调方法
//
//	return 0;
//}
//
//
///* 每次捕获到数据包时，libpcap都会自动调用这个回调函数 */
//void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
//{
//	struct tm *ltime;
//	char timestr[16];
//	time_t local_tv_sec;
//
//	/* 将时间戳转换成可识别的格式 */
//	local_tv_sec = header->ts.tv_sec;
//	ltime=localtime(&local_tv_sec);
//	strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
//
//	printf("时间：%s,毫秒：%.6d 长度len:%d\n", timestr, header->ts.tv_usec, header->len);
//
//}


//////////////////////////////通过非回调的方式捕获数据包///////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i=0;
	pcap_t *adhandle;
	int res;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct tm *ltime;
	char timestr[16];
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	time_t local_tv_sec;


	/* 获取本机设备列表 */
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* 打印列表 */
	for(d=alldevs; d; d=d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if(i==0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):",i);
	scanf("%d", &inum);

	if(inum < 1 || inum > i)
	{
		printf("\nInterface number out of range.\n");
		/* 释放设备列表 */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* 跳转到已选中的适配器 */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);

	/* 打开设备 */
	//if ( (adhandle= pcap_open(d->name,          // 设备名
	//	65536,            // 要捕捉的数据包的部分 
	//	// 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
	//	PCAP_OPENFLAG_PROMISCUOUS,    // 混杂模式
	//	1000,             // 读取超时时间
	//	NULL,             // 远程机器验证
	//	errbuf            // 错误缓冲池
	//	) ) == NULL)
	if ( (adhandle= pcap_open_live(d->name, // 设备名 
		65536,      // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容. 
		1,          // 混杂模式 
		1000,       // 读取超时时间 
		errbuf      
		) ) == NULL) 
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		/* 释放设列表 */
		pcap_freealldevs(alldevs);
		return -1;
	}

	printf("\nlistening on %s...\n", d->description);

	/* 释放设备列表 */
	pcap_freealldevs(alldevs);

	/* 获取数据包 */
	while((res = pcap_next_ex( adhandle, &header, &pkt_data)) >= 0){

		if(res == 0)
			/* 超时时间到 */
			continue;

		/* 将时间戳转换成可识别的格式 */
		local_tv_sec = header->ts.tv_sec;
		ltime=localtime(&local_tv_sec);
		strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

		printf("时间：%s,毫秒：%.6d 长度len:%d\n", timestr, header->ts.tv_usec, header->len);
	}

	if(res == -1){
		printf("Error reading the packets: %s\n", pcap_geterr(adhandle));
		return -1;
	}

	return 0;
}
