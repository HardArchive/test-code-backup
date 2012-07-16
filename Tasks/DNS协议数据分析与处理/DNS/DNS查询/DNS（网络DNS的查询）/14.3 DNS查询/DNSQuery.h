// DNSQuery1.h: interface for the CDNSQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DNSQUERY1_H__64970EE1_BDC0_40AF_9A7F_7B674079FD36__INCLUDED_)
#define AFX_DNSQUERY1_H__64970EE1_BDC0_40AF_9A7F_7B674079FD36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//��ѯ�ͻ�Ӧ������λ�ö���
#define T_A		    1		// ������ַ
#define T_NS		2		// ��֤������
#define T_CNAME		5		// ����
#define T_SOA		6		// ��֤������ʼ
#define T_PTR		12		// ����ָ��
#define T_MX		15		// �ʼ�·����Ϣ

//DNS ͷ�ṹ
struct DNS_HEADER1
{
	unsigned	short id;		    // ��ʶ

	///////////////////////////�����ײ��ı�־�ֶ�16λ/////////////////////////////////
	unsigned	char rd     :1;		// �ݹ��ѯģʽ 
	unsigned	char tc     :1;		// ���������Ϣ��Ϊ1ʱ��ʾ�ɽضϵģ�ʹ��UDPʱ����ʾ��Ӧ�𳤶ȳ���512�ֽ�ʱ��ֻ����ǰ512�ֽڣ�
	unsigned	char aa     :1;		// ��֤��Ӧ 1
	unsigned	char opcode :4;	    // ������{ͨ��ֵΪ0����׼��ѯ��������ֵΪ1�������ѯ����2��������״̬����}
	unsigned	char qr     :1;		// 0��ѯ/1��Ӧ��־
	
	unsigned	char rcode  :4;	    // ��Ӧ�� 0û�в��3���ֲ��
	unsigned	char cd     :1;	    // �����Ч
	unsigned	char ad     :1;	    // ��֤����
	unsigned	char z      :1;		// Ԥ��
	unsigned	char ra     :1;		// �ܷ���еݹ��ѯ
	///////////////////////////�����ײ��ı�־�ֶ�16λ/////////////////////////////////
	

	unsigned    short q_count;	    // ������Ŀ
	unsigned	short ans_count;	// �ش���Ŀ
	unsigned	short auth_count;	// Ȩ���ش���Ŀ
	unsigned	short add_count;	// ������Ŀ
};

//typedef struct _DNS_HEADER
//{
//	WORD    Xid;                    // ��ʶ
//	///////////////////////////�����ײ��ı�־�ֶ�16λ/////////////////////////////////
//	BYTE    RecursionDesired : 1;   // �ݹ��ѯģʽ 
//	BYTE    Truncation : 1;         // ���������Ϣ��Ϊ1ʱ��ʾ�ɽضϵģ�ʹ��UDPʱ����ʾ��Ӧ�𳤶ȳ���512�ֽ�ʱ��ֻ����ǰ512�ֽڣ�
//	BYTE    Authoritative : 1;      // ��֤��Ӧ 1
//	BYTE    Opcode : 4;             // ������{ͨ��ֵΪ0����׼��ѯ��������ֵΪ1�������ѯ����2��������״̬����}
//	BYTE    IsResponse : 1;         // 0��ѯ/1��Ӧ��־
//
//	BYTE    ResponseCode : 4;       // ��Ӧ�� 0û�в��3���ֲ��
//	BYTE    CheckingDisabled : 1;   // �����Ч
//	BYTE    AuthenticatedData : 1;  // ��֤����
//	BYTE    Reserved : 1;           // Ԥ��
//	BYTE    RecursionAvailable : 1; // �ܷ���еݹ��ѯ
//
//	WORD    QuestionCount;          // ������Ŀ
//	WORD    AnswerCount;            // �ش���Ŀ
//	WORD    NameServerCount;        // Ȩ���ش���Ŀ
//	WORD    AdditionalCount;        // ������Ŀ
//}
//DNS_HEADER, *PDNS_HEADER;

//��ѯ����ṹ
struct QUESTION
{
	unsigned short qtype;   // ��ѯ����Դ����
	unsigned short qclass;  // ��ѯ��������
};


//��ѯ��Ӧ�ṹ
#pragma pack(push, 1)
struct  R_DATA
{
	unsigned short type;    // ��ѯ��Ӧ����
	unsigned short _class;  // ��ѯ��Ӧ����������
	unsigned int   ttl;     // ��DNS�������Ĵ��ʱ��
	unsigned short data_len;// ���ݵĳ���
};
#pragma pack(pop)

//��Դ��¼����ָ��
struct RES_RECORD
{
	unsigned char  *name;      // ��Դ����
	struct R_DATA  *resource;  // ��Դ����
	unsigned char  *rdata;     // ��ѯ���
};

//��ѯ�ṹ
typedef struct
{
	unsigned char *name;       // ��Դ����
	QUESTION      *ques;       // ����
} QUERY;


class CDNSQuery  
{
public:
	CDNSQuery();
	virtual ~CDNSQuery();

	CString DNSQuery(unsigned char *host, char *dnsserver);
	unsigned char* ReadName(unsigned char* reader,unsigned char* buffer,int* count);
	void RetrieveDnsServersFromRegistry();
	void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host);

	int GetDNSServer(char* dnsserver)
	{
		int i = 0;
		for(i = 0; i < 10; i++)
		{
			if( m_DnsServers[i][0] == '\0' )
				break;
			strcpy(&dnsserver[i*100], m_DnsServers[i]);
		}

		return i;
	}
private:
	//List of DNS Servers registered on the system
	char m_DnsServers[10][100];

};

#endif // !defined(AFX_DNSQUERY1_H__64970EE1_BDC0_40AF_9A7F_7B674079FD36__INCLUDED_)
