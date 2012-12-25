/***************************************************************************************************
* 1�� File       �� file.h
* 2�� Version    �� 1.1
* 3�� Description�� CFile - ��װ C��׼����ļ���д������
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-12-18 14:08:59
* 6�� History    �� ʹ��string
* 7�� Remark     �� ֧�ֿ�խ�ַ�
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

#define MAX_BUFFER_LEN 1024*4    //һ������д����

    typedef unsigned char byte;
    const int g_iInvalidOffset = -1;  //ȫ��ƫ�Ʊ���

    //����ģʽ
    enum SeekMode
    {
		//�ļ���ʼ
        FromStart,     
        FromCurrent,
        FromEnd
    };

    class CFile
    {
    public:
        //��������/Ĭ�Ϲ��캯��
        CFile()
        {
            m_fp = NULL;
			m_tstrFileName.clear();
        }
       // CFile(const tstring &filename, const tstring &mode = _T("r"));//��ָ�����ļ� �����ܻ�ʧ�ܣ�ʹ�� IsOpened())
		CFile(const tstring& tstrFileName, const tstring& tstrMode = _T("r"));
        CFile(FILE *lfp)
        {
            m_fp = lfp;    //�����ӵ����Ѵ򿪣����ļ�
        }

        //��/�ر�
        bool Open(const tstring& tstrFileName, const tstring& tstrMode = _T("r"));//��һ���ļ�(���������-��mode���ƣ�
        bool Close();  //�ر��Ѿ��򿪵��ļ� �����δ�򿪣����� NOP��

        //ָ��һ�����е��ļ���������ȡ�� CFile ����
        void Attach(FILE *lfp, const tstring& tstrFileName = _T(""))
        {
            Close();
            m_fp = lfp;
			m_tstrFileName = tstrFileName;
        }

        void Detach()
        {
            m_fp = NULL;    //�ͷ� CFile ����
        }
        FILE *fp() const
        {
            return m_fp;
        }

        // ��/д
        bool ReadAll(PBYTE pbyBuf, const size_t nBufLen);     //���ļ����������ݶ������ַ��� ���ı��ļ������ã�
        size_t Read(PBYTE pbyBuf, const size_t nBufLen);      // ���ض�ȡ�ֽ���-ʹ�� Eof() �� Error()�鿴����// �򲻷���
        size_t Write(PBYTE pbyBuf, const size_t nBufLen);     //д��ָ�����ȵ�����
        bool Write(tstring& tstrBuf);	                          //д�ųɹ�����true
        bool Flush();                                         //ˢ��δд�������

        //�ļ�ָ��Ĳ�������ʧ��ʱ�䷵��ofsInvalid��
        //�ƶ�ָ��ofs���ļ� ��ʼ/��ǰ/����λ�õĵ�ƫ���ֽ���
        bool Seek(long ofs, SeekMode mode = FromStart);
        //�ƶ�ָ�뵽�ļ���β֮ǰofs���ֽ�
        bool SeekEnd(long ofs = 0)
        {
            return Seek(ofs, FromEnd);
        }
		//�ƶ�ָ�뵽�ļ���ʼ֮��ofs���ֽ�
		bool SeekStart(long ofs = 0)
		{
			return Seek(ofs, FromStart);
		}
        //��ȡָ��ofs�ڸ��ļ��л�ȡ��ǰλ��
        long Tell() const;
        //ȡ�õ�ǰ�ļ�����
        long Length() const;

        //�򵥵Ĵ�ȡ����ע�⣬��֧��Eof()��Error()�Ļ���ֻ�ܵ��� IsOpened()!
        bool IsOpened() const
        {
            return m_fp != NULL;    //�ж��ļ��Ƿ��
        }
        bool Eof() const
        {
            return feof(m_fp) != 0;    //�ж��Ƿ�ָ���Ѿ������ļ�ĩβ
        }
        bool Error() const
        {
            return ferror(m_fp) != 0;    //�д�������
        }
        const void GetName(tstring& tstrFileName) const
        {
			tstrFileName = m_tstrFileName;
        }
        //wxFileKind GetKind() const { return wxGetFileKind(m_fp); }		// ���ͣ�����̻��

        //�������� �ļ�����Ǵ���ر��ļ���
        ~CFile()
        {
            Close();
        }

    private:
        CFile(const CFile &);
        CFile &operator=(const CFile &);

        FILE *m_fp;       //����ļ�û�д�IO��ΪNULL
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
    // ��/д
    // ----------------------------------------------------------------------------

	//��ȡ�ļ��������� PBYTE pbyBuf BUFָ��, int& iBufLen ���뻺�������� 
    bool CFile::ReadAll(PBYTE pbyBuf, const size_t nBufLen)
    {
        byte szbyBuf[MAX_BUFFER_LEN] = {0};
        size_t nLength = (size_t)Length();
        memset(szbyBuf, 0, MAX_BUFFER_LEN);
        clearerr(m_fp);
		if (NULL == pbyBuf) return false;

        nLength = fread(szbyBuf, sizeof(byte), nLength, m_fp);

        if (Error())  return false;
		if (nBufLen < nLength) return false;   //buf̫С		

		memcpy(pbyBuf, szbyBuf, nLength);
        return true;
    }

	//��ȡָ������  PBYTE pbyBuf BUFָ��, size_t& nBufLen ���ȡ���� 
	//����ʵ�ʶ�ȡ���� �˺������뱣֤���㹻�Ļ������ռ�
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

	//д������   ����д���ʵ�ʳ���
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

    //����ļ����������ļ���д��ʽ��ʱ������������д���ļ�
    bool CFile::Flush()
    {
        if ( IsOpened() )
        {
            //����ļ����������ļ���д��ʽ��ʱ������������д���ļ�
            if (fflush(m_fp) != 0)
            {
                //wxLogSysError(_("failed to flush the file '%s'"), m_name.c_str());
                return false;
            }
        }
        return true;
    }

    // ----------------------------------------------------------------------------
    // ����
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

