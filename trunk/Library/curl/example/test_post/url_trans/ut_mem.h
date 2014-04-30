/*
 * �ļ����� : ut_mem.h
 * �ļ����� : URL�����ڴ����ģ��ӿڶ���
 *
 * �ļ����� : ����
 * �������� : 2010-09-03
 *
 * �޸����� : ����
 * �޸����� : ��չ�ڴ����ģ��ӿ�
 * �޸����� : 2010-09-06
 *
 */
#ifndef _UT_MEM_H_
#define _UT_MEM_H_

/**
 * �ڴ�����ݽṹ
 */
typedef struct{
	unsigned char *memory; /* �ڴ������ */
	size_t size;  /* �ڴ�鳤�� */
}mem_block;

/**
 * ˵������ʼ���ڴ��(mem_block�ṹָ��)
 *
 * ������
 * block : �ڴ��ָ��
 *
 * ����ֵ��
 * ��
 *
 */
void mem_block_init(mem_block *block);

/**
 * ˵�����ͷ��ڴ��(mem_block�ṹָ��)
 *
 * ������
 * block : �ڴ��ָ��
 *
 * ����ֵ��
 * ��
 *
 */
void mem_block_free(mem_block *block);

/**
 * ˵�������·����ڴ���С
 *
 * ������
 * ptr  : ԭʼָ��
 * size : �µĿ��С
 *
 * ����ֵ��
 * void * : ����ɹ�
 * NULL   : ����ʧ��, ��ϸ����ɲ鿴errno
 */
void *mem_block_realloc(void *ptr, size_t size);

/**
 * ˵��������block��¡һ���µ�mem_block
 *
 * ������
 * block  : ԭʼ�ڴ��
 *
 * ����ֵ��
 * mem_block * : ��¡�ɹ�
 * NULL        : ��¡ʧ��, ��ϸ����ɲ鿴errno
 */
mem_block *mem_block_dup(const mem_block *block);

/**
 * ˵��������memory��length����ʼ��block
 *
 * ������
 * block  : ԭʼ�ڴ��
 * memory : ���ݿ� 
 * length : ���ݿ鳤��
 *
 * ����ֵ��
 * ��
 */
void mem_block_new(mem_block *block, char *memory, unsigned int length);

#endif