#pragma once
/******************************************************************************* 
* 1�� �ļ����ƣ� struct_define
* 2�� �桡������ Version *.*
* 3�� ��    ���� �Զ�����������ͷ�ļ�
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-4-20 16:46:12
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 

#define PACKET_FILTER "ip and udp"      //"ether proto \\dns"
//������Žṹ
typedef struct _DNS_DOMAIN
{
	int iDomainID;             //����ID
	char szDomain[MAX_PATH];   //����
}DNS_NOMAIN, *PDNS_NOMAIN;

//IP��Žṹ
typedef struct _DNS_IP
{
	int iIPID;            //IP ID
	char szIP[16];        //IP��ַ
}DNS_IP, *PDNS_IP;

//����IP��Ӧ��ϵ
typedef struct _DNS_DOMAIN_VS_IP
{
	int szDomainID[16];          //����ID���� ����Ě�һԪ��Ϊ������ŽṹID
	int szIPID[16];              //IP ID���� ����Ě�һԪ��ΪIP��ŽṹID
	//�������������֤IP��
}DNS_DOMAIN_VS_IP, *PDNS_DOMAIN_VS_IP;


typedef struct DNS_VERIFY
{
	PDNS_DOMAIN_VS_IP pDNSDomainVsIP; //����IP��Ӧ��ϵ
	int iVerifyStatus;                //��֤״̬�� 0-δ��֤ ��1-��֤ͨ����2-��֤ʧ��
}DNSVERIFY, *PDNSVERIFY;


//��ѯ�ͻ�Ӧ������λ�ö���  ��¼����
#define T_A		    1		// ������ַ A��¼
#define T_NS		2		// ��֤������
#define T_CNAME		5		// ���� CNAME��¼
#define T_SOA		6		// ��֤������ʼ
#define T_PTR		12		// ����ָ��
#define T_MX		15		// �ʼ�·����Ϣ

//DNS ͷ�ṹ
typedef struct _DNS_HEADER
{
	WORD    Xid;                    // ��ʶ
	///////////////////////////�����ײ��ı�־�ֶ�16λ/////////////////////////////////
	BYTE    RecursionDesired : 1;   // �ݹ��ѯģʽ 
	BYTE    Truncation : 1;         // ���������Ϣ��Ϊ1ʱ��ʾ�ɽضϵģ�ʹ��UDPʱ����ʾ��Ӧ�𳤶ȳ���512�ֽ�ʱ��ֻ����ǰ512�ֽڣ�
	BYTE    Authoritative : 1;      // ��֤��Ӧ 1
	BYTE    Opcode : 4;             // ������{ͨ��ֵΪ0����׼��ѯ��������ֵΪ1�������ѯ����2��������״̬����}
	BYTE    IsResponse : 1;         // 0��ѯ/1��Ӧ��־

	BYTE    ResponseCode : 4;       // ��Ӧ�� 0û�в��3���ֲ��
	BYTE    CheckingDisabled : 1;   // �����Ч
	BYTE    AuthenticatedData : 1;  // ��֤����
	BYTE    Reserved : 1;           // Ԥ��
	BYTE    RecursionAvailable : 1; // �ܷ���еݹ��ѯ
	///////////////////////////�����ײ��ı�־�ֶ�16λ/////////////////////////////////

	WORD    QuestionCount;          // ������Ŀ
	WORD    AnswerCount;            // �ش���Ŀ
	WORD    NameServerCount;        // Ȩ���ش���Ŀ
	WORD    AdditionalCount;        // ������Ŀ
}DNS_HEADER, *PDNS_HEADER;

//��ѯ����ṹ �ĺ��沿�� ����������
typedef struct _DNS_WIRE_QUESTION
{
	WORD    QuestionType;  // ��ѯ����
	WORD    QuestionClass; // ��ѯ����
}DNS_WIRE_QUESTION, *PDNS_WIRE_QUESTION;

//��ѯ��Ӧ�ṹ-��Դ��¼��ʽ�ṹ�ģ����͡��ࡢ����ʱ�䡢��Դ���ݳ��ȣ�
#pragma pack(push, 1)
typedef struct _DNS_WIRE_RECORD
{
	WORD    RecordType;  // ����
	WORD    RecordClass; // ��
	DWORD   TimeToLive;  // ��DNS�������Ĵ��ʱ��
	WORD    DataLength;  // ���ݵĳ���
}DNS_WIRE_RECORD, *PDNS_WIRE_RECORD;
#pragma pack(pop)

//��ѯ��Ӧ�ṹ_��Դ��¼��ʽ�ṹ
typedef struct _RES_RECORD
{
	BYTE *ResName;                      // ��Դ���� ����
	PDNS_WIRE_RECORD ResRecord;         // ��Դ����
	BYTE *ResData;                      // ��ѯ��� ��Դ����
}RES_RECORD, *PRES_RECORD;

//��ѯ�ṹ
typedef struct _QUERY
{
	BYTE *QueryName;                        // ��Դ����
	PDNS_WIRE_QUESTION QueryQuesTion;       // ����
} QUERY, *PQUERY;

