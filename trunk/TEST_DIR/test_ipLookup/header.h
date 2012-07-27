#ifndef _rs_header_
#define _rs_header_

//Э��ͷ
typedef struct HeadTag
{
	DWORD dwFlag;//��� ��Ϊ0x11223344
	DWORD dwCommand;//����� 1����IP��,2�·�IP��,3�ϴ��ѳɹ�������IP��,4�ϴ�����ʧ�ܵ�IP��
};

//IP���·�
typedef struct IPSectionTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
	DWORD dwStartIP;//��ʼIPֵ
	DWORD dwEndIP;//����IPֵ
}IPSection;

//�����ϴ�
typedef struct DomainInfoTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
	DWORD dwIP;//IPֵ
	DWORD dwBytes;//����������ռ�ֽ���
	//������Ϣ ���Ը���ʵ�������ĸ����������ȶ���˻�����,������Ϣ�Ĵ�Ÿ�ʽΪ ��������:���� 
	char buff[32];
}DomainInfo;

typedef struct NoDomainInfoTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
	DWORD dwIPCount;//IP����
	char buff[32];  //IPֵ
}NoDomainInfo;

//IP��ȷ��
typedef struct IPConfirmTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
}IPConfirm;

#endif