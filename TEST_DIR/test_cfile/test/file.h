/***************************************************************************************************
* 1、 File       ： file.h
* 2、 Version    ： 1.0
* 3、 Description： File - 封装 "FILE *" 流
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-12-18 14:08:59
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/

#ifndef   _FILE_H_
#define   _FILE_H_

#include <string>
using namespace std;
#include <stdio.h>

namespace RG
{

	//查找模式
	enum SeekMode
	{
		FromStart,
		FromCurrent,
		FromEnd
	};

	class CFile
	{
	public:
		//构建函数/默认构造函数
		CFile() { m_fp = NULL; }
		CFile(const string& filename, const string& mode = _T("r"));//打开指定的文件 （可能会失败，使用 IsOpened())
		CFile(FILE *lfp) { m_fp = lfp; } //将附加到（已打开）的文件

		//打开/关闭
		bool Open(const string& filename, const string& mode = _T("r"));//打开一个文件(如果不存在-由mode控制）
		bool Close();  //关闭已经打开的文件 （如果未打开，这是 NOP）

		//指定一个现有的文件描述符并取得 CFile 对象
		void Attach(FILE *lfp, const string& name = _T(""))
		{
			Close();
			m_fp = lfp;
			m_name = name; 
		}

		void Detach() { m_fp = NULL; } //释放 CFile 对象
		FILE *fp() const { return m_fp; }

		// 读/写(无缓冲)
		bool ReadAll(string *str/*, const wxMBConv& conv = wxConvAuto()*/); //从文件将所有数据都读入字符串 （文本文件很有用）
		size_t Read(void *pBuf, size_t nCount); // 返回读取字节数-使用 Eof() 和 Error()查看错误// 或不发生		
		size_t Write(const void *pBuf, size_t nCount);                          //写放指定长度的数据	
		bool Write(const string& s/*, const wxMBConv& conv = wxConvAuto()*/);	//写放成功返回true		
		bool Flush();                                                           //刷新未写入的数据

		//文件指针的操作（在失败时间返回ofsInvalid）
		//移动指针ofs在文件 开始/当前/结束位置的的偏移字节数
		bool Seek(long ofs, SeekMode mode = FromStart);
		//移动指针到文件结尾之前ofs个字节
		bool SeekEnd(long ofs = 0) { return Seek(ofs, FromEnd); }
		//获取指针ofs在该文件中获取当前位置
		long Tell() const;
		//取得当前文件长度
		long Length() const;

		//简单的存取：请注意，不支持Eof()和Error()的话，只能调用 IsOpened()!
		bool IsOpened() const { return m_fp != NULL; }     //判断文件是否打开
		bool Eof() const { return feof(m_fp) != 0; }       //判断是否指针已经到达文件末尾		
		bool Error() const { return ferror(m_fp) != 0; }   //有错误发生？		
		const string& GetName() const { return m_name; }   //取得文件名
		//wxFileKind GetKind() const { return wxGetFileKind(m_fp); }		// 类型，如磁盘或管

		//析构函数 文件如果是打开则关闭文件，
		~CFile() { Close(); }

	private:
		// copy ctor and assignment operator are private because it doesn't make
		// sense to copy files this way: attempt to do it will provoke a compile-time
		// error.
		CFile(const CFile&);
		CFile& operator=(const CFile&);

		FILE *m_fp;       //如果文件没有打开IO流为NULL

		string m_name;  //（用于诊断消息) 文件名称
	};
}
#endif // _FILE_H_

