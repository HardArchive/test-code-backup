#ifndef _COMM_PROTOCOL_h_
#define _COMM_PROTOCOL_h_
//ͨѶЭ��
//##################################################################################################################
//�ַ�Э��ͷ
typedef struct HeadTag
{
	//��� ��Ϊ0x11223344
	DWORD dwFlag;
	//�����  1����IP��,2�·�IP��,3�ϴ��ѳɹ�������IP��,4�ϴ�����ʧ�ܵ�IP�� 5.ȷ�ϻظ�
	DWORD dwCommand;
}Head;

//IP���·�
typedef struct IPSectionTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
	DWORD dwStartIP;//��ʼIPֵ
	DWORD dwEndIP;//����IPֵ
}IPSection;

//IP��ȷ��
typedef struct IPConfirmTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
}IPConfirm;

//##################################################################################################################

//�ϴ�Э��ͷ
typedef struct UpLoadHeadTag
{
	//��� ��Ϊ0x11223344
	DWORD dwFlag;
	//�����  1����IP��,2�·�IP��,3�ϴ��ѳɹ�������IP��,4�ϴ�����ʧ�ܵ�IP�� 5.ȷ�ϻظ�
	DWORD dwCommand;
	//���������ֽڳ���
	DWORD dwBodyLen;
}UpLoadHead;

//�����ϴ�
typedef struct DomainInfoTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
	DWORD dwIP;//IPֵ
	//������Ϣ ���Ը���ʵ�������ĸ����������ȶ���˻�����,������Ϣ�Ĵ�Ÿ�ʽΪ ��������:����
	//��������ռһ���ֽ�,�������������ȵ���������Ϣ Ȼ������һ���������Ⱥ�������Ϣ ���˸�ʽһֱ����ȥ
	char buff[32];
}DomainInfo;

typedef struct NoDomainInfoTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
	DWORD dwIPCount;//IP����
	//���IP��Ϣ �ĸ��ֽڴ�һ��IP��ַ IP�ĸ�����dwIPCountָ�� ���Ը���ʵ����Ҫ����buff����
	char buff[32];
}NoDomainInfo;
//#################################################################################################################


#endif