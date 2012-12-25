/***************************************************************************************************
* 1、 File       ： file.h
* 2、 Version    ： 1.1
* 3、 Description： CFile - 封装 C标准库的文件读写函数类
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-12-18 14:08:59
* 6、 History    ： 使用string
* 7、 Remark     ： 支持宽窄字符
****************************************************************************************************/

#ifndef   _FILE_H_
#define   _FILE_H_

#include <string>
using namespace std;
#include <stdio.h>

namespace RG
{
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#define MAX_BUFFER_LEN 1024*4    //一次最大读写长度

    typedef unsigned char byte;
    const int g_iInvalidOffset = -1;  //全局偏移变量

    //查找模式
    enum SeekMode
    {
		//文件开始
        FromStart,     
        FromCurrent,
        FromEnd
    };

    class CFile
    {
    public:
        //构建函数/默认构造函数
        CFile()
        {
            m_fp = NULL;
			m_tstrFileName.clear();
        }
       // CFile(const tstring &filename, const tstring &mode = _T("r"));//打开指定的文件 （可能会失败，使用 IsOpened())
		CFile(const tstring& tstrFileName, const tstring& tstrMode = _T("r"));
        CFile(FILE *lfp)
        {
            m_fp = lfp;    //将附加到（已打开）的文件
        }

        //打开/关闭
        bool Open(const tstring& tstrFileName, const tstring& tstrMode = _T("r"));//打开一个文件(如果不存在-由mode控制）
        bool Close();  //关闭已经打开的文件 （如果未打开，这是 NOP）

        //指定一个现有的文件描述符并取得 CFile 对象
        void Attach(FILE *lfp, const tstring& tstrFileName = _T(""))
        {
            Close();
            m_fp = lfp;
			m_tstrFileName = tstrFileName;
        }

        void Detach()
        {
            m_fp = NULL;    //释放 CFile 对象
        }
        FILE *fp() const
        {
            return m_fp;
        }

        // 读/写
        bool ReadAll(PBYTE pbyBuf, const size_t nBufLen);     //从文件将所有数据都读入字符串 （文本文件很有用）
        size_t Read(PBYTE pbyBuf, const size_t nBufLen);      // 返回读取字节数-使用 Eof() 和 Error()查看错误// 或不发生
        size_t Write(PBYTE pbyBuf, const size_t nBufLen);     //写放指定长度的数据
        bool Write(tstring& tstrBuf);	                          //写放成功返回true
        bool Flush();                                         //刷新未写入的数据

        //文件指针的操作（在失败时间返回ofsInvalid）
        //移动指针ofs在文件 开始/当前/结束位置的的偏移字节数
        bool Seek(long ofs, SeekMode mode = FromStart);
        //移动指针到文件结尾之前ofs个字节
        bool SeekEnd(long ofs = 0)
        {
            return Seek(ofs, FromEnd);
        }
		//移动指针到文件开始之后ofs个字节
		bool SeekStart(long ofs = 0)
		{
			return Seek(ofs, FromStart);
		}
        //获取指针ofs在该文件中获取当前位置
        long Tell() const;
        //取得当前文件长度
        long Length() const;

        //简单的存取：请注意，不支持Eof()和Error()的话，只能调用 IsOpened()!
        bool IsOpened() const
        {
            return m_fp != NULL;    //判断文件是否打开
        }
        bool Eof() const
        {
            return feof(m_fp) != 0;    //判断是否指针已经到达文件末尾
        }
        bool Error() const
        {
            return ferror(m_fp) != 0;    //有错误发生？
        }
        const void GetName(tstring& tstrFileName) const
        {
			tstrFileName = m_tstrFileName;
        }
        //wxFileKind GetKind() const { return wxGetFileKind(m_fp); }		// 类型，如磁盘或管

        //析构函数 文件如果是打开则关闭文件，
        ~CFile()
        {
            Close();
        }

    private:
        CFile(const CFile &);
        CFile &operator=(const CFile &);

        FILE *m_fp;       //如果文件没有打开IO流为NULL
		tstring m_tstrFileName;
    };


    CFile::CFile(const tstring& tstrFileName, const tstring& tstrMode)
    {
        Detach();

        (void)Open(tstrFileName, tstrMode);
    }


    bool CFile::Open(const tstring& tstrFileName, const tstring& tstrMode)
    {
        int iRet = 0;
		m_fp = NULL;
        //iRet = _tfopen_s(&m_fp, ptszFileName, ptszMode);
		m_fp = _tfopen(tstrFileName.c_str(), tstrMode.c_str());

        if (!(m_fp /*&& !iRet*/)) return false;

		m_tstrFileName = tstrFileName;
        return true;
    }

    bool CFile::Close()
    {
        if (IsOpened())
        {
            if ( fclose(m_fp) != 0 ) return false;
            Detach();
        }
        return true;
    }

    // ----------------------------------------------------------------------------
    // 读/写
    // ----------------------------------------------------------------------------

	//读取文件所有内容 PBYTE pbyBuf BUF指针, int& iBufLen 传入缓冲区长度 
    bool CFile::ReadAll(PBYTE pbyBuf, const size_t nBufLen)
    {
        byte szbyBuf[MAX_BUFFER_LEN] = {0};
        size_t nLength = (size_t)Length();
        memset(szbyBuf, 0, MAX_BUFFER_LEN);
        clearerr(m_fp);
		if (NULL == pbyBuf) return false;

        nLength = fread(szbyBuf, sizeof(byte), nLength, m_fp);

        if (Error())  return false;
		if (nBufLen < nLength) return false;   //buf太小		

		memcpy(pbyBuf, szbyBuf, nLength);
        return true;
    }

	//读取指定长度  PBYTE pbyBuf BUF指针, size_t& nBufLen 需读取长度 
	//返回实际读取长度 此函数必须保证有足够的缓冲区空间
    size_t CFile::Read(PBYTE pbyBuf, const size_t nBufLen)
    {
        size_t nRead = 0;
		nRead = fread(pbyBuf, sizeof(byte), nBufLen, m_fp);
        if ((nRead < nBufLen) && Error())
        {
            //TRACE(_("Read error on file '%s'"), m_name.c_str());
        }
        return nRead;
    }

	//写入数据   返回写入的实际长度
    size_t CFile::Write(PBYTE pbyBuf, const size_t nBufLen)
    {
        //wxCHECK_MSG( pBuf, 0, wxT("invalid parameter") );
        //wxCHECK_MSG( IsOpened(), 0, wxT("can't write to closed file") );

        size_t nWritten = fwrite(pbyBuf, sizeof(byte), nBufLen, m_fp);	
        if (nWritten < nBufLen)
        {
            //wxLogSysError(_("Write error on file '%s'"), m_name.c_str());
        }

        return nWritten;
    }

    bool CFile::Write(tstring& tstrBuf)
    {
		size_t size = 0;
        BYTE szbybuf[MAX_BUFFER_LEN] = {0};
		memset(szbybuf, 0, MAX_BUFFER_LEN);
		memcpy(szbybuf, tstrBuf.c_str(), _tcslen(tstrBuf.c_str())*sizeof(TCHAR));
		size = _tcslen((TCHAR *)szbybuf)*sizeof(TCHAR);

        if (!size) return false;

        return Write(szbybuf, size) == size;
    }

    //清除文件缓冲区，文件以写方式打开时将缓冲区内容写入文件
    bool CFile::Flush()
    {
        if ( IsOpened() )
        {
            //清除文件缓冲区，文件以写方式打开时将缓冲区内容写入文件
            if (fflush(m_fp) != 0)
            {
                //wxLogSysError(_("failed to flush the file '%s'"), m_name.c_str());
                return false;
            }
        }
        return true;
    }

    // ----------------------------------------------------------------------------
    // 查找
    // ----------------------------------------------------------------------------

    bool CFile::Seek(long ofs, SeekMode mode)
    {
        int origin = 0;
        switch (mode)
        {
        default:
            //wxFAIL_MSG(wxT("unknown seek mode"));
            // still fall through
        case FromStart:
            origin = SEEK_SET; break;
        case FromCurrent:
            origin = SEEK_CUR; break;
        case FromEnd:
            origin = SEEK_END; break;
        }

        if (_fseeki64(m_fp, ofs, origin) != 0)
        {
            //wxLogSysError(_("Seek error on file '%s'"), m_name.c_str());
            return false;
        }
        return true;
    }

    long CFile::Tell() const
    {
        long rc = (long)_ftelli64(m_fp);
        if (rc == g_iInvalidOffset)
        {
            //wxLogSysError(_("Can't find current position in file '%s'"),
            //              m_name.c_str());
        }
        return rc;
    }

    long CFile::Length() const
    {
        CFile &self = *const_cast<CFile *>(this);

        long posOld = Tell();
        if (posOld != g_iInvalidOffset)
        {
            if ( self.SeekEnd() )
            {
                long len = Tell();

                (void)self.Seek(posOld);

                return len;
            }
        }
        return g_iInvalidOffset;
    }
}
#endif // _FILE_H_

