// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "pcap.h" 
#pragma comment (lib, "wpcap.lib")
#pragma comment (lib, "wsock32.lib")

//////////////////////////////ͨ���ص��ķ�ʽ�������ݰ�///////////////////////////////////////////////////////

///* packet handler ����ԭ�� */
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
//	/* ��ȡ�����豸�б� */
//	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
//	if (pcap_findalldevs(&alldevs, errbuf) == -1)
//	{
//		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
//		exit(1);
//	}
//
//	/* ��ӡ�б� */
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
//		/* �ͷ��豸�б� */
//		pcap_freealldevs(alldevs);
//		return -1;
//	}
//
//	/* ��ת��ѡ�е������� */
//	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
//
//	/* ���豸 */
//	//if ( (adhandle= pcap_open(d->name,          // �豸��
//	//	65536,            // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
//	//	PCAP_OPENFLAG_PROMISCUOUS,    // ����ģʽ
//	//	1000,             // ��ȡ��ʱʱ��
//	//	NULL,             // Զ�̻�����֤
//	//	errbuf            // ���󻺳��
//	//	) ) == NULL)
//
//	if ( (adhandle= pcap_open_live(d->name, // �豸�� 
//								65536,      // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������. 
//								1,          // ����ģʽ 
//								1000,       // ��ȡ��ʱʱ�� 
//								errbuf      // ���󻺳�� 
//								) ) == NULL) 
//	{
//		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
//		/* �ͷ��豸�б� */
//		pcap_freealldevs(alldevs);
//		return -1;
//	}
//
//	printf("\nlistening on %s...\n", d->description);
//
//	/* �ͷ��豸�б� */
//	pcap_freealldevs(alldevs);
//
//	/* ��ʼ���� */
//	pcap_loop(adhandle, 0, packet_handler, NULL); //ͨ���ص�����
//
//	return 0;
//}
//
//
///* ÿ�β������ݰ�ʱ��libpcap�����Զ���������ص����� */
//void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
//{
//	struct tm *ltime;
//	char timestr[16];
//	time_t local_tv_sec;
//
//	/* ��ʱ���ת���ɿ�ʶ��ĸ�ʽ */
//	local_tv_sec = header->ts.tv_sec;
//	ltime=localtime(&local_tv_sec);
//	strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
//
//	printf("ʱ�䣺%s,���룺%.6d ����len:%d\n", timestr, header->ts.tv_usec, header->len);
//
//}


//////////////////////////////ͨ���ǻص��ķ�ʽ�������ݰ�///////////////////////////////////////////////////////
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


	/* ��ȡ�����豸�б� */
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* ��ӡ�б� */
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
		/* �ͷ��豸�б� */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* ��ת����ѡ�е������� */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);

	/* ���豸 */
	//if ( (adhandle= pcap_open(d->name,          // �豸��
	//	65536,            // Ҫ��׽�����ݰ��Ĳ��� 
	//	// 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
	//	PCAP_OPENFLAG_PROMISCUOUS,    // ����ģʽ
	//	1000,             // ��ȡ��ʱʱ��
	//	NULL,             // Զ�̻�����֤
	//	errbuf            // ���󻺳��
	//	) ) == NULL)
	if ( (adhandle= pcap_open_live(d->name, // �豸�� 
		65536,      // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������. 
		1,          // ����ģʽ 
		1000,       // ��ȡ��ʱʱ�� 
		errbuf      
		) ) == NULL) 
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		/* �ͷ����б� */
		pcap_freealldevs(alldevs);
		return -1;
	}

	printf("\nlistening on %s...\n", d->description);

	/* �ͷ��豸�б� */
	pcap_freealldevs(alldevs);

	/* ��ȡ���ݰ� */
	while((res = pcap_next_ex( adhandle, &header, &pkt_data)) >= 0){

		if(res == 0)
			/* ��ʱʱ�䵽 */
			continue;

		/* ��ʱ���ת���ɿ�ʶ��ĸ�ʽ */
		local_tv_sec = header->ts.tv_sec;
		ltime=localtime(&local_tv_sec);
		strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

		printf("ʱ�䣺%s,���룺%.6d ����len:%d\n", timestr, header->ts.tv_usec, header->len);
	}

	if(res == -1){
		printf("Error reading the packets: %s\n", pcap_geterr(adhandle));
		return -1;
	}

	return 0;
}
