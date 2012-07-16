// arpparse.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "define_struct_head.h"

int _tmain(int argc, _TCHAR* argv[])
{
	pcap_if_t*		alldevs;						//网络设备结构
	pcap_if_t*		d;								
	pcap_t*			adhandle;						//网卡描述符

	char			errbuf[PCAP_ERRBUF_SIZE];		//错误信息
	u_int			netmask;						//子网掩码
	char			packet_filter[] = "ether proto \\arp";    //过滤ARP帧
	struct			bpf_program		fcode;
	struct			pcap_pkthdr		*header;
	const			u_char			*pkt_data;

	//获取网络设备列表
	if( pcap_findalldevs( &alldevs, errbuf) == -1)
	{
		cout << "error in pcap_findalldevs:" <<errbuf;
		return -1;
	}
	//选择一个Ethernet卡
	for( d = alldevs; d; d = d->next)
	{ 
		//以混杂模式打开网卡,以接收所有的帧
		if( (adhandle = pcap_open_live(d->name,1000,1,300,errbuf) ) == NULL)
		{
			cout<<"\n Unable to open the adapter.";
			pcap_freealldevs(alldevs); 
			return -2;
		}
		//检查数据链路层是否为Ethernet
		if( pcap_datalink(adhandle) == DLT_EN10MB && d->addresses != NULL)
			break;
	}

	if( d == NULL )
	{
		cout << "\n No interfaces found! Make sure Winpcap is installed ! \n";
		return -3;
	}
	//获得子网掩码
	netmask = (( sockaddr_in * ) ( d->addresses->netmask) )->sin_addr.S_un.S_addr;
	
	//编译过滤器
	if( pcap_compile( adhandle, &fcode, packet_filter, 1, netmask) < 0 )
	{
		cout << "\n Cann't to compile the packet filter, check the syntax.\n";
		pcap_freealldevs(alldevs);
		return -4;
	}
	//设置过滤器
	if( pcap_setfilter(adhandle, &fcode) < 0 ) 
	{
		cout <<"\n Error setting the filter.\n";
		pcap_freealldevs(alldevs);
		return -5;
	}
	//输出包信息
	cout<<"Listing on "<< d->description<< "...."<<endl<<endl;

	cout << "sour IP " << "   " <<"sour  MAC address"<< "   "
		 <<"Des IP"<< "   "<<"Des MAC address" << "   "
		 <<" op " <<endl;

	//释放设备列表
	pcap_freealldevs(alldevs);

	//开始捕获MAC帧
	int result;
	while(( result = pcap_next_ex(adhandle, &header, &pkt_data ) ) >= 0 )
	{
		if( result == 0 )
			continue;
		packet_handler( header,pkt_data, cout);
	}

	return 0;
}

// pcap_pkthdr结构是从WINPCAP中引用的数据包的头(记录包的长度与发送的时间)
// pkt_data是表示MAC帧的起始位置
// out是输出流

void packet_handler(const pcap_pkthdr *header, const u_char *pkt_data, ostream& out)
{
	//忽略源和目的MAC各6个字节+标志符2个字节
	//从帧截
	arppkt * arph = (arppkt * ) (pkt_data + 14);   
	//输出源IP
	for(int i=0; i<3;i++)
		out<<int(arph->sip[i])<<'.';
	out.setf(ios::left);
	out<<setw(3)<<int (arph->sip[3]) <<" ";
	out.setf(ios::left);
	//输出源MAC
	char	oldfillchar = out.fill('0');
	out.setf(ios::uppercase);
	for(int i=0; i<5;i++)
		out<<hex<<setw(2)<<int (arph->smac[i])<<"-";
	out<<hex<<setw(2)<<int (arph->smac[5])<<" ";
	out.fill(oldfillchar);
	//输出目的IP
	out.unsetf(ios::hex | ios::uppercase);
	for(int i=0;i<3;i++)
		out<<int (arph->dip[i])<<".";
	out<<int (arph->dip[3])<<" ";
	out.unsetf(ios::left);
	//输出目的MAC
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