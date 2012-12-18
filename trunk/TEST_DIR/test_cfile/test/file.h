/***************************************************************************************************
* 1�� File       �� file.h
* 2�� Version    �� 1.0
* 3�� Description�� File - ��װ "FILE *" ��
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-12-18 14:08:59
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/

#ifndef   _FILE_H_
#define   _FILE_H_

#include <string>
using namespace std;
#include <stdio.h>

namespace RG
{

	//����ģʽ
	enum SeekMode
	{
		FromStart,
		FromCurrent,
		FromEnd
	};

	class CFile
	{
	public:
		//��������/Ĭ�Ϲ��캯��
		CFile() { m_fp = NULL; }
		CFile(const string& filename, const string& mode = _T("r"));//��ָ�����ļ� �����ܻ�ʧ�ܣ�ʹ�� IsOpened())
		CFile(FILE *lfp) { m_fp = lfp; } //�����ӵ����Ѵ򿪣����ļ�

		//��/�ر�
		bool Open(const string& filename, const string& mode = _T("r"));//��һ���ļ�(���������-��mode���ƣ�
		bool Close();  //�ر��Ѿ��򿪵��ļ� �����δ�򿪣����� NOP��

		//ָ��һ�����е��ļ���������ȡ�� CFile ����
		void Attach(FILE *lfp, const string& name = _T(""))
		{
			Close();
			m_fp = lfp;
			m_name = name; 
		}

		void Detach() { m_fp = NULL; } //�ͷ� CFile ����
		FILE *fp() const { return m_fp; }

		// ��/д(�޻���)
		bool ReadAll(string *str/*, const wxMBConv& conv = wxConvAuto()*/); //���ļ����������ݶ������ַ��� ���ı��ļ������ã�
		size_t Read(void *pBuf, size_t nCount); // ���ض�ȡ�ֽ���-ʹ�� Eof() �� Error()�鿴����// �򲻷���		
		size_t Write(const void *pBuf, size_t nCount);                          //д��ָ�����ȵ�����	
		bool Write(const string& s/*, const wxMBConv& conv = wxConvAuto()*/);	//д�ųɹ�����true		
		bool Flush();                                                           //ˢ��δд�������

		//�ļ�ָ��Ĳ�������ʧ��ʱ�䷵��ofsInvalid��
		//�ƶ�ָ��ofs���ļ� ��ʼ/��ǰ/����λ�õĵ�ƫ���ֽ���
		bool Seek(long ofs, SeekMode mode = FromStart);
		//�ƶ�ָ�뵽�ļ���β֮ǰofs���ֽ�
		bool SeekEnd(long ofs = 0) { return Seek(ofs, FromEnd); }
		//��ȡָ��ofs�ڸ��ļ��л�ȡ��ǰλ��
		long Tell() const;
		//ȡ�õ�ǰ�ļ�����
		long Length() const;

		//�򵥵Ĵ�ȡ����ע�⣬��֧��Eof()��Error()�Ļ���ֻ�ܵ��� IsOpened()!
		bool IsOpened() const { return m_fp != NULL; }     //�ж��ļ��Ƿ��
		bool Eof() const { return feof(m_fp) != 0; }       //�ж��Ƿ�ָ���Ѿ������ļ�ĩβ		
		bool Error() const { return ferror(m_fp) != 0; }   //�д�������		
		const string& GetName() const { return m_name; }   //ȡ���ļ���
		//wxFileKind GetKind() const { return wxGetFileKind(m_fp); }		// ���ͣ�����̻��

		//�������� �ļ�����Ǵ���ر��ļ���
		~CFile() { Close(); }

	private:
		// copy ctor and assignment operator are private because it doesn't make
		// sense to copy files this way: attempt to do it will provoke a compile-time
		// error.
		CFile(const CFile&);
		CFile& operator=(const CFile&);

		FILE *m_fp;       //����ļ�û�д�IO��ΪNULL

		string m_name;  //�����������Ϣ) �ļ�����
	};
}
#endif // _FILE_H_

