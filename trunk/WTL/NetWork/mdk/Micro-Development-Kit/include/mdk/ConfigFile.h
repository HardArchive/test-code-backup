// ConfigFile.h: interface for the ConfigFile class.
//
//////////////////////////////////////////////////////////////////////
/*
	��ʶ����ļ���ʽ��
	ע������#����//��ͷ
	����һ��һ��
	���ø�ʽΪkey=value
	���Դ���������ո��Ʊ��
	key��value�ַ��������а����Ŀո����Ʊ���������������˵Ŀո����Ʊ���ᱻ����
	����:"  ip list = \t 192.168.0.1 \t 192.168.0.2 \t "
	�������������
		key ="ip list"
		value="192.168.0.1 \t 192.168.0.2"

	ʹ�÷���
	ConfigFile cfg( "./test.cfg" );
	string ip = cfg["ip"];
	cfg["ip"] = "127.0.0.1";
	cfg["port"] = 8080;//����ֱ��ʹ��char*��string��int��float�Ƚ��и�ֵ
	cfg.save();
*/
#ifndef MDK_CONFIGFILE_H
#define MDK_CONFIGFILE_H

#include "FixLengthInt.h"

#include <stdio.h>
#include <string>
#include <map>

namespace mdk
{

class CFGItem
{
	friend class ConfigFile;
public:
	bool IsNull();
	operator std::string();
	operator char();
	operator unsigned char();
	operator short();
	operator unsigned short();
	operator int();
	operator unsigned int();
	operator int64();
	operator uint64();
	operator float();
	operator double();
	CFGItem& operator = ( double value );
	CFGItem& operator = ( int value );
	CFGItem& operator = ( std::string value );
	virtual ~CFGItem();
private:
	CFGItem();
	std::string m_value;
	std::string m_description;
	int m_index;
	bool m_valid;
};
typedef std::map<std::string,CFGItem> ConfigMap;

class ConfigFile  
{
public:
	ConfigFile( const char *strName );
	virtual ~ConfigFile();
	
public:
	//����д����ֵ
	CFGItem& operator []( std::string key );
	bool Save();
private:
	//���ļ�
	bool Open( bool bRead );
	//�ر��ļ�
	void Close();
	bool ReadFile();

private:
	std::string m_strName;
	FILE *m_pFile;
	ConfigMap m_content;
	
};

}//namespace mdk

#endif // MDK_CONFIGFILE_H
