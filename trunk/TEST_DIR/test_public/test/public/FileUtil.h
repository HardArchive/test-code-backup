//file TraceUtil.h
/***************************************************************************************************
* 1�� Class      �� CFileUtil
* 2�� Version    �� *.*
* 3�� Description�� �ļ���д��
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-11-13 16:46:43
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __PATH_UTIL_H_
#define __PATH_UTIL_H_
#include <Windows.h>

namespace QNA
{
	class CFileUtil
	{
		CFileUtil()
		{

		}
		CFileUtil(int hFile)
		{

		}
		//nOpenFlags ����ͷ���ģʽ��ָ�������ļ�ʱ���еĶ��������Խ�����������OR(|)������������������Ӧ��һ������Ȩ�޺�һ������ѡ�modeCreate��modeNoInherit�ǿ�ѡ�ġ�ֵ������ʾ�� �� CFile::modeCreate ���ù��캯������һ�����ļ�������ļ��Ѵ��ڣ��򳤶ȱ��0��  
		//	�� CFile::modeNoTruncate ��ֵ��modeCreate���ʹ�á�������������ļ��Ѵ������䳤�Ȳ���Ϊ0��������ļ����򿪣�������Ϊһ�����ļ�������Ϊһ���Ѵ��ڵ��ļ����⽫�Ǻ����õģ����統��һ�����ܴ���Ҳ���ܲ����ڵ��ļ�ʱ�����ѡ��Ҳ������CStdioFile.  
		//	�� CFile::modeRead ���ļ���������  
		//	�� CFile::modeReadWrite ���ļ�����д��  
		//	�� CFile::modeWrite ���ļ�����д��  
		//	�� CFile::modeNoInherit ��ֹ�ļ����ӽ��̼̳С�  
		//	�� CFile::ShareDenyNone ����ֹ�������̶���д���ʣ����ļ�������ļ��ѱ����������Լ���ģʽ�򿪣���Createʧ�ܡ�  
		//	�� CFile::ShareDenyRead ���ļ�����ֹ�������̶����ļ�������ļ��ѱ����������Լ���ģʽ�򿪣����������̶�����Createʧ�ܡ�  
		//	�� CFile::ShareDenyWrite ���ļ�����ֹ��������д���ļ�������ļ��ѱ����������Լ���ģʽ�򿪣�����������д����Createʧ�ܡ�  
		//	�� CFile::ShareExclusive �Զ�ռģʽ���ļ�����ֹ�������̶��ļ��Ķ�д������ļ��Ѿ�������ģʽ�򿪶�д����ʹ����ǰ���̣�������ʧ�ܡ�  
		//	�� CFile::ShareCompat �˱�־��32λMFC����Ч���˱�־��ʹ��CFile::Openʱӳ��ΪCFile::ShareExclusive��  
		//	�� CFile::typeText �Իس���������������̣������������ࣩ��  
		//	�� CFile::typeBinary ���ö�����ģʽ�������������ࣩ��  
		CFileUtil(TCHAR ptszFileName, UINT nOpenFlags)
		{

		}
		throw(CFileException);

	};
}
#endif

