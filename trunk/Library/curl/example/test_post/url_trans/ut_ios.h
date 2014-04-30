/*
 * �ļ����� : ut_ios.h
 * �ļ����� : URL��������/���ģ��ӿڶ���
 *
 * �ļ����� : ����
 * �������� : 2010-09-03
 *
 * �޸����� : ����
 * �޸����� : ��չ����/���ģ��ӿ�
 * �޸����� : 2010-09-06
 *
 * �޸����� : ����
 * �޸����� : ���raw_send_recv�ӿ�
 * �޸����� : 2010-09-07
 * 
 * �޸����� : ����
 * �޸����� : ��Ӵ�����Ϣ�ӿ�
 * �޸����� : 2010-09-07
 *
 * �޸����� : ����
 * �޸����� : ����շ��ļ��ӿ�
 * �޸����� : 2010-09-08
 */
#ifndef _UT_IOS_H_
#define _UT_IOS_H_

#include "ut_mem.h"

/**
 * �ļ����������ͣ�
 * FD_SEND : ���Է������ݵ��ļ�������
 * FS_RECV : ���ڽ������ݵ��ļ�������
 */
typedef enum fd_type{
	FD_SEND, /* 0-���� */
	FD_RECV  /* 1-���� */
};

#define ONE_MINUTE 60000L
#define BLOCK_SIZE 1024
#define UT_ERROR_LEN 1024

typedef enum ut_status{
	UT_OK,    /* ���� */
	UT_ERROR  /* ���� */
};

/**
 * ��ȡ�����һ��������Ϣ��������������û�м�ʱ��ȡ������Ϣ����
 * ��Ϣ���ܱ���һ�����ܳ���ĵ��õĴ�����Ϣ����
 */
const char *get_last_error(void);

/**
 * ΪUI�ṩ�Ĵ�����Ȼص��ӿ�
 */
typedef void (*processing)(double current, double total);

/**
 * ˵����
 * ��url���ͳ���Ϊreq_len��req_buf��������������������res_buf, ����
 * Ϊres_len, res_buf��res_len���Ա�����˸�д��
 *
 * ������
 * url     : ��Ҫ��ȡ��URL(ͳһ��Դ����)
 * req_buf : �������ݻ�����
 * req_len : �������ݻ���������
 * res_buf : ��Ӧ���ݻ�����ָ��
 * res_len : ��Ӧ���ݻ���������ָ��
 * 
 * ����ֵ��
 * -1     : ����ʧ��
 * 0      : �����ɹ�
 */
int raw_send_recv(const char *url, void *req_buf, unsigned int req_len, \
				  void **res_buf, unsigned int *res_len);

/**
 * ˵����
 * �򿪳���Ϊfile_size���ļ�file_name,������urlָ����λ��, ͨ������
 * processing���½���
 *
 */
int raw_send_file(const char *url, const char *file_name, \
				  const unsigned int file_size, processing proc);

/**
 * ˵����
 * ��urlָ����λ�ö��ļ����������ļ�file_name, ͨ������processing���½���
 *
 */
int raw_recv_file(const char *url, const char *file_name, \
				  const unsigned int file_size, processing proc);

/**
 * ˵����
 * ��url�϶�ȡ����(n)���ַ��������ݴ���buffer, length��ʶʵ�ʶ����ĳ���.
 *
 * ������
 * url    : ��Ҫ��ȡ��URL(ͳһ��Դ����)
 * buffer : ���뻺����
 * length : ���뻺��������
 * 
 * ����ֵ��
 * -1     : ��ȡʧ��
 * length : ��ȡ�ɹ�
 */
int raw_read_n( const char *url, void *buffer, unsigned int length);

/**
 * ˵����
 * ��url�϶�ȡ�����������ݵ�buffer,���buffer��С����url_write_u��������µ�
 * �ڴ沢ͬʱ�޸�lengthֵ���ȣ��ڴ��ɵ����߸����ͷ�
 *
 * ������
 * url      : ��Ҫ��ȡ��URL(ͳһ��Դ����)
 * p_buffer : ���뻺������ָ��
 * p_length : ���뻺�������ȵ�ָ��
 * proc   : Ϊ����UI������Ļص�����
 *
 * ����ֵ��
 *  -1    : ��ȡʧ��
 * !-1    : ��ȡ�ɹ�
 */
int raw_read_u(const char *url, void **p_buffer, unsigned int *p_length, \
			   processing proc);

/**
 * ˵����
 * ��url��д�붨��(length)���ַ���д��������buffer��, length��ʶbuffer����.
 *
 * ������
 * url    : ��Ҫд���URL(ͳһ��Դ����)
 * buffer : д�뻺����
 * length : д�뻺��������
 * 
 * ����ֵ��
 * -1     : д��ʧ��
 * length : д��ɹ�
 */
int raw_write_n(const char *url, void *buffer, unsigned int length);

/**
 * ˵����
 * ��URLд�붨���ַ�����д������ݴ洢��buffer�У�lengthΪbuffer�ĳ���
 * raw_write_u��raw_write_n������Ϊ��raw_write_u���ڲ����ܽ�buffer��
 * ��Σ���鴫�䡣
 *
 * ������
 * url    : ��Ҫд���URL(ͳһ��Դ����)
 * buffer : д�뻺����
 * length : д�뻺��������
 * proc   : Ϊ����UI������Ļص�����
 * 
 * ����ֵ��
 * -1     : д��ʧ��
 * length : д��ɹ�
 */
int raw_write_u(const char *url, void *buffer, unsigned int length, \
				processing proc);

/**
 * ˵����
 * ��urlָ�����ļ������ڴ��block
 *
 * ������
 * url   : ��Ҫ��ȡ��URL(ͳһ��Դ����)
 * block : �ڴ��ָ��
 *
 * ����ֵ��
 * -1     : ��ȡʧ��
 * length : ��ȡ�ɹ�������Ϊblock��size
 * 
 */
int url_read_mem(const char *url, mem_block *block);

/**
 * ˵����
 * ��blockָ���ڴ��д��url
 *
 * ������
 * url   : ��Ҫд���URL(ͳһ��Դ����)
 * block : �ڴ��ָ��
 *
 * ����ֵ��
 * -1     : д��ʧ��
 * length : д��ɹ�������Ϊblock��size
 */
int url_write_mem(const char *url, const mem_block *block);

#endif