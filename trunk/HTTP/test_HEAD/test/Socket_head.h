#ifndef _SOCKET_HEAD_H_
#define _SOCKET_HEAD_H_

/******************************************************************************* 
* 1�� Class      �� Socket_head
* 2�� Version    �� 1.00
* 3�� Description:  ��������վ����Head������֤����������
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-8-2 17:14:30
* 6�� History    �� 
* 7�� Remark     �� �׽����ڹ�������ʼ�����۹������ͷ�  ��������Ҫ��������
********************************************************************************/ 
#include "Socket.h"
namespace QNA
{

	class CSocketHEAD
	{
	public:
		bool CheckEffective()
		{
			return false;
		}

	private:
		SOCKET m_sock;
		bool m_bIsClient;        //�ͻ��˱�־��trueΪ�ͻ��ˣ�falseΪ�����
	};


}

#endif  //_SOCKET_HEAD_H_