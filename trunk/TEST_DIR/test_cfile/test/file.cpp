
#include "stdafx.h"
#include "file.h"
//#include <Windows.h>
using namespace RG;



// ----------------------------------------------------------------------------
// 打开文件
// ----------------------------------------------------------------------------
typedef unsigned char byte;
#define MAX_BUFFER_LEN 1024*4    //一次最大读写长度

const int g_iInvalidOffset = -1;

CFile::CFile(const string& filename, const string& mode)
{
	Detach();

	(void)Open(filename, mode);
}

bool CFile::Open(const string& filename, const string& mode)
{
	//wxASSERT_MSG( !m_fp, wxT("should close or detach the old file first") );
#ifdef UNICODE
	m_fp = _wfopen(filename, mode);
#else
	m_fp = fopen(filename.c_str(), mode.c_str());
#endif 
	//fopen_s
	if ( !m_fp )
	{
		//wxLogSysError(_("can't open file '%s'"), filename);

		return false;
	}

	m_name = filename;

	return true;
}

bool CFile::Close()
{
	if ( IsOpened() )
	{
		if ( fclose(m_fp) != 0 )
		{
			//wxLogSysError(_("can't close file '%s'"), m_name.c_str());

			return false;
		}

		Detach();
	}

	return true;
}

// ----------------------------------------------------------------------------
// 读/写
// ----------------------------------------------------------------------------

bool CFile::ReadAll(string *str/*, const wxMBConv& conv*/)
{
	//wxCHECK_MSG( str, false, wxT("invalid parameter") );
	//wxCHECK_MSG( IsOpened(), false, wxT("can't read from closed file") );
	//wxCHECK_MSG( Length() >= 0, false, wxT("invalid length") );
	//size_t length = wx_truncate_cast(size_t, Length());
	//wxCHECK_MSG( (long)length == Length(), false, wxT("huge file not supported") );

	byte szbyBuf[MAX_BUFFER_LEN] = {0};
	size_t length = (size_t)Length();
	memset(szbyBuf, 0, MAX_BUFFER_LEN);
	clearerr(m_fp);

	//wxCharBuffer buf(length + 1);

	// note that real length may be less than file length for text files with DOS EOLs
	// ('\r's get dropped by CRT when reading which means that we have
	// realLen = fileLen - numOfLinesInTheFile)
	length = fread(szbyBuf, sizeof(byte), length, m_fp);

	if ( Error() )
	{
		//wxLogSysError(_("Read error on file '%s'"), m_name.c_str());

		return false;
	}

	szbyBuf[length] = 0;
	*str = string((char*)szbyBuf);

	return true;
}

size_t CFile::Read(void *pBuf, size_t nCount)
{
	//wxCHECK_MSG( pBuf, 0, wxT("invalid parameter") );
	//wxCHECK_MSG( IsOpened(), 0, wxT("can't read from closed file") );

	size_t nRead = fread(pBuf, 1, nCount, m_fp);
	if ( (nRead < nCount) && Error() )
	{
		//wxLogSysError(_("Read error on file '%s'"), m_name.c_str());
	}

	return nRead;
}

size_t CFile::Write(const void *pBuf, size_t nCount)
{
	//wxCHECK_MSG( pBuf, 0, wxT("invalid parameter") );
	//wxCHECK_MSG( IsOpened(), 0, wxT("can't write to closed file") );

	size_t nWritten = fwrite(pBuf, 1, nCount, m_fp);
	if ( nWritten < nCount )
	{
		//wxLogSysError(_("Write error on file '%s'"), m_name.c_str());
	}

	return nWritten;
}

bool CFile::Write(const string& s/*, const wxMBConv& conv*/)
{
	//const wxWX2MBbuf buf = s.mb_str(conv);
	byte szbybuf[MAX_BUFFER_LEN] = {0};


	if ( !szbybuf )
		return false;

	const size_t size = strlen((char*)szbybuf); // FIXME: use buf.length() when available
	return Write(szbybuf, size) == size;
}

//清除文件缓冲区，文件以写方式打开时将缓冲区内容写入文件 
bool CFile::Flush()
{
	if ( IsOpened() )
	{
		//清除文件缓冲区，文件以写方式打开时将缓冲区内容写入文件 
		if ( fflush(m_fp) != 0 )
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
	//wxCHECK_MSG( IsOpened(), false, wxT("can't seek on closed file") );

	int origin;
	switch ( mode )
	{
	default:
		//wxFAIL_MSG(wxT("unknown seek mode"));
		// still fall through

	case FromStart:
		origin = SEEK_SET;
		break;

	case FromCurrent:
		origin = SEEK_CUR;
		break;

	case FromEnd:
		origin = SEEK_END;
		break;
	}

#ifndef wxHAS_LARGE_FFILES
	if ((long)ofs != ofs)
	{
		//wxLogError(_("Seek error on file '%s' (large files not supported by stdio)"), m_name.c_str());

		return false;
	}

	if ( _fseeki64(m_fp, (long)ofs, origin) != 0 )
#else 
	if ( _fseeki64(m_fp, ofs, origin) != 0 )
#endif
	{
		//wxLogSysError(_("Seek error on file '%s'"), m_name.c_str());

		return false;
	}

	return true;
}

long CFile::Tell() const
{
	//wxCHECK_MSG( IsOpened(), g_iInvalidOffset,
	//             wxT("CFile::Tell(): file is closed!") );

	long rc = (long)_ftelli64(m_fp);
	if ( rc == g_iInvalidOffset )
	{
		//wxLogSysError(_("Can't find current position in file '%s'"),
		//              m_name.c_str());
	}

	return rc;
}

long CFile::Length() const
{
	//wxCHECK_MSG( IsOpened(), g_iInvalidOffset,
	//	wxT("CFile::Length(): file is closed!") );

	CFile& self = *const_cast<CFile *>(this);

	long posOld = Tell();
	if ( posOld != g_iInvalidOffset )
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

//#endif // USE_FILE
