// arpparse.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "define_struct_head.h"

int _tmain(int argc, _TCHAR* argv[])
{
	pcap_if_t*		alldevs;						//�����豸�ṹ
	pcap_if_t*		d;								
	pcap_t*			adhandle;						//����������

	char			errbuf[PCAP_ERRBUF_SIZE];		//������Ϣ
	u_int			netmask;						//��������
	char			packet_filter[] = "ether proto \\arp";    //����ARP֡
	struct			bpf_program		fcode;
	struct			pcap_pkthdr		*header;
	const			u_char			*pkt_data;

	//��ȡ�����豸�б�
	if( pcap_findalldevs( &alldevs, errbuf) == -1)
	{
		cout << "error in pcap_findalldevs:" <<errbuf;
		return -1;
	}
	//ѡ��һ��Ethernet��
	for( d = alldevs; d; d = d->next)
	{ 
		//�Ի���ģʽ������,�Խ������е�֡
		if( (adhandle = pcap_open_live(d->name,1000,1,300,errbuf) ) == NULL)
		{
			cout<<"\n Unable to open the adapter.";
			pcap_freealldevs(alldevs); 
			return -2;
		}
		//���������·���Ƿ�ΪEthernet
		if( pcap_datalink(adhandle) == DLT_EN10MB && d->addresses != NULL)
			break;
	}

	if( d == NULL )
	{
		cout << "\n No interfaces found! Make sure Winpcap is installed ! \n";
		return -3;
	}
	//�����������
	netmask = (( sockaddr_in * ) ( d->addresses->netmask) )->sin_addr.S_un.S_addr;
	
	//���������
	if( pcap_compile( adhandle, &fcode, packet_filter, 1, netmask) < 0 )
	{
		cout << "\n Cann't to compile the packet filter, check the syntax.\n";
		pcap_freealldevs(alldevs);
		return -4;
	}
	//���ù�����
	if( pcap_setfilter(adhandle, &fcode) < 0 ) 
	{
		cout <<"\n Error setting the filter.\n";
		pcap_freealldevs(alldevs);
		return -5;
	}
	//�������Ϣ
	cout<<"Listing on "<< d->description<< "...."<<endl<<endl;

	cout << "sour IP " << "   " <<"sour  MAC address"<< "   "
		 <<"Des IP"<< "   "<<"Des MAC address" << "   "
		 <<" op " <<endl;

	//�ͷ��豸�б�
	pcap_freealldevs(alldevs);

	//��ʼ����MAC֡
	int result;
	while(( result = pcap_next_ex(adhandle, &header, &pkt_data ) ) >= 0 )
	{
		if( result == 0 )
			continue;
		packet_handler( header,pkt_data, cout);
	}

	return 0;
}

// pcap_pkthdr�ṹ�Ǵ�WINPCAP�����õ����ݰ���ͷ(��¼���ĳ����뷢�͵�ʱ��)
// pkt_data�Ǳ�ʾMAC֡����ʼλ��
// out�������

void packet_handler(const pcap_pkthdr *header, const u_char *pkt_data, ostream& out)
{
	//����Դ��Ŀ��MAC��6���ֽ�+��־��2���ֽ�
	//��֡��
	arppkt * arph = (arppkt * ) (pkt_data + 14);   
	//���ԴIP
	for(int i=0; i<3;i++)
		out<<int(arph->sip[i])<<'.';
	out.setf(ios::left);
	out<<setw(3)<<int (arph->sip[3]) <<" ";
	out.setf(ios::left);
	//���ԴMAC
	char	oldfillchar = out.fill('0');
	out.setf(ios::uppercase);
	for(int i=0; i<5;i++)
		out<<hex<<setw(2)<<int (arph->smac[i])<<"-";
	out<<hex<<setw(2)<<int (arph->smac[5])<<" ";
	out.fill(oldfillchar);
	//���Ŀ��IP
	out.unsetf(ios::hex | ios::uppercase);
	for(int i=0;i<3;i++)
		out<<int (arph->dip[i])<<".";
	out<<int (arph->dip[3])<<" ";
	out.unsetf(ios::left);
	//���Ŀ��MAC
	out.fill('0');
	out.setf(ios::uppercase);
	for(int i=0;i<5;i++)
		out<<hex<<setw(2)<<int(arph->dmac[i])<<"-";
	out<<hex<<setw(2)<<int(arph->dmac[5])<<" ";
	out.fill(oldfillchar);
	out.unsetf(ios::hex|ios::uppercase);
	out<<ntohs(arph->op)<<" ";

	struct  tm *ltime;
	time_t ttime = (time_t)header->ts.tv_sec;
	ltime  =  localtime(&ttime);
	out.fill('0');
	out<<ltime->tm_hour<<":"<<setw(2)<<ltime->tm_min<<':'<<setw(2)<<ltime->tm_sec;
	out.fill(oldfillchar);
	out<<endl;

}