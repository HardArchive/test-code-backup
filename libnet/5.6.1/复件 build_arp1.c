#include "win32/libnet.h"
/*
=======================================================================================================================
在windows平台下必须包含此文件，文件libnet.h是libnet开发包的头文件
=======================================================================================================================
 */
void main()
{
    int packet_size;
    /* 存放数据包长度的变量 */
    libnet_t *l;
    /* libnet句柄 */
    libnet_ptag_t protocol_tag;
    /* 协议块标记 */
	int num;
    char *device = NULL;
    /* 设备名字，此时为NULL */
    char error_information[LIBNET_ERRBUF_SIZE];
    /* 用来存放错误信息 */
    char *destination_ip_str = "192.168.1.1";
    /* 目的IP地址字符串变量，可以指定任意一个合法的IP地址 */
    char *source_ip_str = "192.168.1.3";
    /* 源IP地址字符串变量，可以指定任意一个合法的IP地址 */
    u_char hardware_source[6] =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06
    };
    /* 源MAC地址，可以是任意指定 */
    u_char hardware_destination[6] =
    {
        0x06, 0x05, 0x04, 0x03, 0x02, 0x01
    };
    /* 目的MAC地址，可以是任意指定 */
    u_long destination_ip;
    /* 目的IP地址 */
    u_long source_ip;
    /* 源IP地址 */
    destination_ip = libnet_name2addr4(l, destination_ip_str, LIBNET_RESOLVE);
    /* 把目的IP地址字符串形式转化成网络顺序字节形式的数据 */
    source_ip = libnet_name2addr4(l, source_ip_str, LIBNET_RESOLVE);
    /* 把源IP地址字符串形式转化成网络顺序字节形式的数据 */
    l = libnet_init(
    /* 初始化libnet */
    LIBNET_LINK_ADV,
    /* libnet类型 */
    device,
    /* 网络设备 */
    error_information); /* 错误信息
     * */
    protocol_tag = libnet_build_arp(
    /* 构造ARP协议块，函数的返回值是代表新生成的ARP协议块的一个协议块标记， */
    ARPHRD_ETHER,
    /* 硬件地址类型，在这里是以太网 */
    ETHERTYPE_IP,
    /* 协议地址类型，在这里是IP协议 */
    6,
    /* 硬件地址长度，MAC地址的长度为6 */
    4,
    /* 协议地址长度，IP地址的长度为4 */
    ARPOP_REPLY,
    /* 操作类型，在这里是ARP应答类型 */
    hardware_source,
    /* 源硬件地址 */
    (u_int8_t*) &source_ip,
    /* 源IP地址 */
    hardware_destination,
    /* 目标硬件地址 */
    (u_int8_t*) &destination_ip,
    /* 目标协议地址 */
    NULL,
    /* 负载，此时为NULL */
    0,
    /* 负载的长度，此时为0 */
    l,
    /* libnet句柄，此句柄由libnet_init()函数生成 */
    0
    /* 协议块标记，此时为0，表示构造一个新的ARP协议块，而不是修改已经存在的协议块
     * */
    );
    protocol_tag = libnet_autobuild_ethernet(
    /* 构造一个以太网协议块，返回一个指向此协议块的标记 */
    hardware_destination,
    /* 目的硬件地址 */
    ETHERTYPE_ARP,
    /* 以太网上层协议类型，此时为ARP类型 */
    l /* libnet句柄 */
    );
	for( num = 0; num < 10; num++)
	{
    packet_size = libnet_write(l);
    /*
     * 发送已经构造的ARP数据包，注意此数据包应该包括两部分，一部分是ARP协议块，另外一部分是以太网协议块
     */
    printf("发送一个%d字节长度的ARP应答数据包\n", packet_size);
    /* 输出发送的ARP数据包的字节数 */
    }
	libnet_destroy(l);
    /* 销毁libnet */
}
