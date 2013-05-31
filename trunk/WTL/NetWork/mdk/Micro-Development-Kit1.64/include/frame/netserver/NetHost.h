#ifndef MDK_NETHOST_H
#define MDK_NETHOST_H

#include "../../../include/mdk/FixLengthInt.h"
#include <string>

namespace mdk
{
class NetConnect;
class Socket;
/**
	����������
	��ʾһ������������������˽�й��캯����ֻ�������洴�����û�ʹ��

	������һ����������࣬��������ָ�룬����ͨ�����Ʋ�����NetHost������һ��NetConnectָ�롣
	���Բ�Ҫ���Ƶ�ַ�����ã���Ϊ�������ü�������ı䣬��ɿ��ܷ���һ���Ѿ����ͷŵ�NetConnectָ��
	��������
	NetHost *pHost = &host;//���Ƶ�ַ�����ü�������ı�
	vector<NetHost*> hostList;
	hostList.push_back(&host);//���Ƶ�ַ�����ü�������ı�
	��ȷ������
	NetHost safeHost = host;//���ƶ������ü���������1����safeHost����֮ǰ��NetConnectָ��ָ����ڴ���Բ��ᱻ�ͷ�
	vector<NetHost> hostList;
	hostList.push_back(host);//���ƶ������ü���������1���ڶ����hostList��ɾ��֮ǰ��NetConnectָ��ָ����ڴ���Բ��ᱻ�ͷ�
*/
class NetHost
{
	friend class NetConnect;
public:
	NetHost();
	virtual ~NetHost();
	NetHost(const NetHost& obj);	
	NetHost& operator=(const NetHost& obj);
	/*
		����Ψһ��ʶ
		��ʵ�ʾ�������������ӵ�SOCKET�����������ֱ��ʹ��socket���api��������socket��io��close��
		��Ϊcloseʱ���ײ���Ҫ�������������ֱ��ʹ��socketclose()����ײ����û����ִ��������,������Ӳ�����
		io�����ײ��Ѿ�ʹ��io�������ֱ��ʹ��api io������io��������һ���ײ�io���������������ݴ���
	*/
		
	int ID();
	/*
		�ӽ��ջ����ж�����
			������
				pMsg		��Ϣ������յ�����
				uLength		��Ҫ���յĳ���
				bClearCache	�Ƿ񽫽��յ������ݴӻ����������trueɾ����false����
				���籨�ĸ�ʽ��2byte���ݳ���+��������
				OnMsg()�ڽ����߼�����
					1.Recv(msg, 2, true);
					2.����msg�õ����ݳ��ȣ�����Ϊ256
					3.Recv(msg, 256, true)
					��
					���3.���ﷵ��false����ʾʵ�ʵ�������<256��������ȡ
					
					��ʱ���û���2��ѡ��
					ѡ��1��
					ѭ��ִ��Recv ֱ���ɹ��������sleep��CPUֱ��100%�����sleep����ӦЧ�ʽ���
					ѡ��2��
					��256����������ֱ��return�˳�OnMsg���´�OnMsg����ʱ���ٳ���Recv
					�ŵ㣬û��sleep,����CPU
					ȱ�㣺�û�����������֯���û���ҪΪ��������ά��һ��int����������ճ��ȣ�
					Ҳ�����û���Ҫ�Լ�ά��һ���б������ӶϿ�ʱ��Ҫ���б�ɾ������������
					
					����false��bClearCache��������������
					1.Recv(msg, 2, false);
					2.����msg�õ����ݳ��ȣ�����Ϊ256
					3.Recv(msg, 256+2, true)//�������ĳ�����256+2
						���Recv�ɹ���ֱ�Ӵ���
						���Recvʧ�ܣ���ʾ�������ݲ�������Ϊ1.���ﴫ����false��
						���ĳ�����Ϣ����ӽ��ջ�����ɾ�������ԣ��û�����ֱ��return�˳�OnMsg,
						�´�OnMsg����ʱ�������Դ������϶����������ݳ�����Ϣ
			
			����ֵ��
			���ݲ�����ֱ�ӷ���false
			��������ģʽ�������Ѿ����û��������Ϣ�ȴ�����Ϣ����ʱ����OnMsg������
	*/
	bool Recv(unsigned char* pMsg, unsigned short uLength, bool bClearCache = true );
	/**
		��������
		����ֵ��
			��������Чʱ������false
	*/
	bool Send(const unsigned char* pMsg, unsigned short uLength);
	void Close();//�ر�����
	bool IsServer();//������һ������
	void InGroup( int groupID );//����ĳ���飬ͬһ�������ɶ�ε��ø÷���������������
	void OutGroup( int groupID );//��ĳ����ɾ��
	/*
		������ַ
		�������NetServerϣ���õ��Է���ַ��Ӧ�õ��ñ�������������GetServerAddress
		��ΪNetHost��ʾ�ľ��ǶԷ�����������NetHost��������ַ���ǶԷ���ַ
	 */
	void GetAddress( std::string &ip, int &port );//������ַ
	/*
		��������ַ
		�����ϣ��֪���Է��������ӵ��Լ����ĸ��˿ڣ�Ӧ�õ��ñ�������������GetAddress]
		��ΪGetAddress��ʾ���ǶԷ�
	 */
	void GetServerAddress( std::string &ip, int &port );
private:
	NetConnect* m_pConnect;//���Ӷ���ָ��,����NetConnect��ҵ���ӿڣ�����NetConnect��ͨ�Ų�ӿ�
	
};

}  // namespace mdk
#endif//MDK_NETHOST_H
