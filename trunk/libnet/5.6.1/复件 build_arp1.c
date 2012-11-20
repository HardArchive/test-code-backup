#include "win32/libnet.h"
/*
=======================================================================================================================
��windowsƽ̨�±���������ļ����ļ�libnet.h��libnet��������ͷ�ļ�
=======================================================================================================================
 */
void main()
{
    int packet_size;
    /* ������ݰ����ȵı��� */
    libnet_t *l;
    /* libnet��� */
    libnet_ptag_t protocol_tag;
    /* Э����� */
	int num;
    char *device = NULL;
    /* �豸���֣���ʱΪNULL */
    char error_information[LIBNET_ERRBUF_SIZE];
    /* ������Ŵ�����Ϣ */
    char *destination_ip_str = "192.168.1.1";
    /* Ŀ��IP��ַ�ַ�������������ָ������һ���Ϸ���IP��ַ */
    char *source_ip_str = "192.168.1.3";
    /* ԴIP��ַ�ַ�������������ָ������һ���Ϸ���IP��ַ */
    u_char hardware_source[6] =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06
    };
    /* ԴMAC��ַ������������ָ�� */
    u_char hardware_destination[6] =
    {
        0x06, 0x05, 0x04, 0x03, 0x02, 0x01
    };
    /* Ŀ��MAC��ַ������������ָ�� */
    u_long destination_ip;
    /* Ŀ��IP��ַ */
    u_long source_ip;
    /* ԴIP��ַ */
    destination_ip = libnet_name2addr4(l, destination_ip_str, LIBNET_RESOLVE);
    /* ��Ŀ��IP��ַ�ַ�����ʽת��������˳���ֽ���ʽ������ */
    source_ip = libnet_name2addr4(l, source_ip_str, LIBNET_RESOLVE);
    /* ��ԴIP��ַ�ַ�����ʽת��������˳���ֽ���ʽ������ */
    l = libnet_init(
    /* ��ʼ��libnet */
    LIBNET_LINK_ADV,
    /* libnet���� */
    device,
    /* �����豸 */
    error_information); /* ������Ϣ
     * */
    protocol_tag = libnet_build_arp(
    /* ����ARPЭ��飬�����ķ���ֵ�Ǵ��������ɵ�ARPЭ����һ��Э����ǣ� */
    ARPHRD_ETHER,
    /* Ӳ����ַ���ͣ�����������̫�� */
    ETHERTYPE_IP,
    /* Э���ַ���ͣ���������IPЭ�� */
    6,
    /* Ӳ����ַ���ȣ�MAC��ַ�ĳ���Ϊ6 */
    4,
    /* Э���ַ���ȣ�IP��ַ�ĳ���Ϊ4 */
    ARPOP_REPLY,
    /* �������ͣ���������ARPӦ������ */
    hardware_source,
    /* ԴӲ����ַ */
    (u_int8_t*) &source_ip,
    /* ԴIP��ַ */
    hardware_destination,
    /* Ŀ��Ӳ����ַ */
    (u_int8_t*) &destination_ip,
    /* Ŀ��Э���ַ */
    NULL,
    /* ���أ���ʱΪNULL */
    0,
    /* ���صĳ��ȣ���ʱΪ0 */
    l,
    /* libnet������˾����libnet_init()�������� */
    0
    /* Э����ǣ���ʱΪ0����ʾ����һ���µ�ARPЭ��飬�������޸��Ѿ����ڵ�Э���
     * */
    );
    protocol_tag = libnet_autobuild_ethernet(
    /* ����һ����̫��Э��飬����һ��ָ���Э���ı�� */
    hardware_destination,
    /* Ŀ��Ӳ����ַ */
    ETHERTYPE_ARP,
    /* ��̫���ϲ�Э�����ͣ���ʱΪARP���� */
    l /* libnet��� */
    );
	for( num = 0; num < 10; num++)
	{
    packet_size = libnet_write(l);
    /*
     * �����Ѿ������ARP���ݰ���ע������ݰ�Ӧ�ð��������֣�һ������ARPЭ��飬����һ��������̫��Э���
     */
    printf("����һ��%d�ֽڳ��ȵ�ARPӦ�����ݰ�\n", packet_size);
    /* ������͵�ARP���ݰ����ֽ��� */
    }
	libnet_destroy(l);
    /* ����libnet */
}
