// MP3ColExt.cpp : Implementation of CMP3ColExt
#include "stdafx.h"
#include "MP3TagViewer.h"
#include "MP3ColExt.h"
#include <atlconv.h>
#include <shlwapi.h>

/////////////////////////////////////////////////////////////////////////////
// CMP3ColExt

STDMETHODIMP CMP3ColExt::GetColumnInfo ( DWORD dwIndex, SHCOLUMNINFO* psci )
{
    // We have 6 columns, so if dwIndex is 6 or greater, return S_FALSE to
    // indicate we've enumerated all our columns.
    if ( dwIndex > 5 )
        return S_FALSE;

    switch ( dwIndex )
        {
        case 0:     // MP3 Album - separate column
            {
            psci->scid.fmtid = *_Module.pguidVer;   // This is a handy GUID
            psci->scid.pid   = 0;                   // Any ol' ID will do, but using the col # is easiest
            psci->vt         = VT_LPSTR;            // We'll return the data as a string
            psci->fmt        = LVCFMT_LEFT;         // Text will be left-aligned in the column
            psci->csFlags    = SHCOLSTATE_TYPE_STR | SHCOLSTATE_SLOW; // Data should be sorted as strings
            psci->cChars     = 32;                  // Default col width in chars

            lstrcpynW ( psci->wszTitle, L"MP3 Album", MAX_COLUMN_NAME_LEN );
            lstrcpynW ( psci->wszDescription, L"Album name of an MP3", MAX_COLUMN_DESC_LEN );
            }
        break;

        case 1:     // MP3 year - separate column
            {
            psci->scid.fmtid = *_Module.pguidVer;   // This is a handy GUID
            psci->scid.pid   = 1;                   // Any ol' ID will do, but using the col # is easiest
            psci->vt         = VT_LPSTR;            // We'll return the data as a string
            psci->fmt        = LVCFMT_RIGHT;        // Text will be right-aligned in the column
            psci->csFlags    = SHCOLSTATE_TYPE_INT | SHCOLSTATE_SLOW; // Data should be sorted as ints
            psci->cChars     = 6;                   // Default col width in chars

            lstrcpynW ( psci->wszTitle, L"MP3 Year", MAX_COLUMN_NAME_LEN );
            lstrcpynW ( psci->wszDescription, L"Year of an MP3", MAX_COLUMN_DESC_LEN );
            }
        break;

        case 2:     // MP3 genre - separate column
            {
            psci->scid.fmtid = *_Module.pguidVer;   // This is a handy GUID
            psci->scid.pid   = 2;                   // Any ol' ID will do, but using the col # is easiest
            psci->vt         = VT_LPSTR;            // We'll return the data as a string
            psci->fmt        = LVCFMT_LEFT;         // Text will be left-aligned in the column
            psci->csFlags    = SHCOLSTATE_TYPE_STR | SHCOLSTATE_SLOW; // Data should be sorted as strings
            psci->cChars     = 32;                  // Default col width in chars

            lstrcpynW ( psci->wszTitle, L"MP3 Genre", MAX_COLUMN_NAME_LEN );
            lstrcpynW ( psci->wszDescription, L"Genre of an MP3", MAX_COLUMN_DESC_LEN );
            }
        break;

        case 3:     // MP3 artist - reusing the built-in Author column
            {
            psci->scid.fmtid = FMTID_SummaryInformation;    // predefined FMTID
            psci->scid.pid   = 4;                   // Predefined - author
            psci->vt         = VT_LPSTR;            // We'll return the data as a string
            psci->fmt        = LVCFMT_LEFT;         // Text will be left-aligned in the column
            psci->csFlags    = SHCOLSTATE_TYPE_STR | SHCOLSTATE_SLOW; // Data should be sorted as strings
            psci->cChars     = 32;                  // Default col width in chars
            }
        break;

        case 4:     // MP3 song title - reusing the built-in Title column
            {
            psci->scid.fmtid = FMTID_SummaryInformation;    // predefined FMTID
            psci->scid.pid   = 2;                   // Predefined - title
            psci->vt         = VT_LPSTR;            // We'll return the data as a string
            psci->fmt        = LVCFMT_LEFT;         // Text will be left-aligned in the column
            psci->csFlags    = SHCOLSTATE_TYPE_STR | SHCOLSTATE_SLOW; // Data should be sorted as strings
            psci->cChars     = 32;                  // Default col width in chars
            }
        break;

        case 5:     // ID3 comment - reusing the built-in Comments column
            {
            psci->scid.fmtid = FMTID_SummaryInformation;    // predefined FMTID
            psci->scid.pid   = 6;                   // Predefined - comments
            psci->vt         = VT_LPSTR;            // We'll return the data as a string
            psci->fmt        = LVCFMT_LEFT;         // Text will be left-aligned in the column
            psci->csFlags    = SHCOLSTATE_TYPE_STR | SHCOLSTATE_SLOW; // Data should be sorted as strings
            psci->cChars     = 32;                  // Default col width in chars
            }
        break;

        DEFAULT_UNREACHABLE;
        }

    return S_OK;
}

STDMETHODIMP CMP3ColExt::GetItemData ( LPCSHCOLUMNID pscid, LPCSHCOLUMNDATA pscd,
                                       VARIANT* pvarData )
{
USES_CONVERSION;
LPCTSTR   szFilename = OLE2CT(pscd->wszFile);
char      szField[31];
bool      bUsingBuiltinCol = false;
CID3v1Tag rTag;
bool      bCacheHit = false;

    // Verify that the format id and column numbers are what we expect.
    if ( pscid->fmtid == *_Module.pguidVer )
        {
        if ( pscid->pid > 2 )
            return S_FALSE;
        }
    else if ( pscid->fmtid == FMTID_SummaryInformation )
        {
        bUsingBuiltinCol = true;

        if ( pscid->pid != 2  &&  pscid->pid != 4  &&  pscid->pid != 6 )
            return S_FALSE;
        }
    else
        {
        return S_FALSE;
        }

    // If we're being called with a directory (instead of a file), we can
    // bail immediately.
    // Also bail if the file is offline (that is, backed up to tape or some
    // other storage).  
    if ( pscd->dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_OFFLINE) )
        return S_FALSE;

    // Check the file extension.  If it's not .MP3, we can bail immediately.
    if ( 0 != lstrcmpiW ( pscd->pwszExt, L".mp3" ))
        return S_FALSE;

    // Look for the filename in our cache.
list_ID3Cache::const_iterator it, itEnd;

    for ( it = m_ID3Cache.begin(), itEnd = m_ID3Cache.end();
          !bCacheHit && it != itEnd; it++ )
        {
        if ( 0 == lstrcmpi ( szFilename, it->sFilename.c_str() ))
            {
            CopyMemory ( &rTag, &it->rTag, sizeof(CID3v1Tag) );
            bCacheHit = true;
            }
        }

    // If the file's tag wasn't in our cache, read the tag from the file.
    if ( !bCacheHit )
        {
        if ( !ReadTagFromFile ( szFilename, &rTag ))
            return S_FALSE;

        // We'll keep the tags for the last 5 files cached - remove the oldest
        // entries if the cache is bigger than 4 entries.
        while ( m_ID3Cache.size() > 4 )
            {
            m_ID3Cache.pop_back();
            }

        // Add the new ID3 tag to our cache.
        CID3CacheEntry entry;

        entry.sFilename = szFilename;
        CopyMemory ( &entry.rTag, &rTag, sizeof(CID3v1Tag) );
        
        m_ID3Cache.push_front ( entry );
        }   // end if(!bCacheHit)

    // Check if we really have an ID3 tag by looking for the signature.
    if ( 0 != StrCmpNA ( rTag.szTag, "TAG", 3 ))
        return S_FALSE;

    // Format the details string.
    if ( bUsingBuiltinCol )
        {
        switch ( pscid->pid )
            {
            case 2:                     // song title
                CopyMemory ( szField, rTag.szTitle, countof(rTag.szTitle) );
                szField[30] = '\0';
            break;

            case 4:                     // artist
                CopyMemory ( szField, rTag.szArtist, countof(rTag.szArtist) );
                szField[30] = '\0';
            break;

            case 6:                     // comments
                CopyMemory ( szField, rTag.szComment, countof(rTag.szComment) );
                szField[30] = '\0';
            break;

            DEFAULT_UNREACHABLE;
            }
        }
    else
        {
        switch ( pscid->pid )
            {
            case 0:                     // album
                CopyMemory ( szField, rTag.szAlbum, countof(rTag.szAlbum) );
                szField[30] = '\0';
            break;

            case 1:                     // year
                CopyMemory ( szField, rTag.szYear, countof(rTag.szYear) );
                szField[4] = '\0';
            break;

            case 2:                     // genre
                GetGenreName ( rTag, szField, countof(szField) );
                szField[30] = '\0';
            break;

            DEFAULT_UNREACHABLE;
            }
        }

    // WinAmp will pad strings with spaces instead of nulls, so remove any
    // trailing spaces.
    StrTrimA ( szField, " " );

    // Create a VARIANT with the details string, and return it back to the shell.
CComVariant vData ( szField );

    vData.Detach ( pvarData );

    return S_OK;
}


//////////////////////////////////////////////////////////////////////////
//
// GetGenreName() : Returns a textual description given a genre ID.
//
// tag: ID3 tag that contains the ID.
// szField: Buffer to receive the description.
// cchBuffSize: Buffer size in chars.
//
// Note: szField is always an ANSI string.
//
//////////////////////////////////////////////////////////////////////////

void CMP3ColExt::GetGenreName ( const CID3v1Tag& tag, char* szField, 
                                const UINT cchBuffSize )
{
static const char* aszGenres[] = {
    "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", "Grunge", 
    "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies", "Other", "Pop", "R&B",
    "Rap", "Reggae", "Rock", "Techno", "Industrial", "Alternative", "Ska",
    "Death Metal", "Pranks", "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop",
    "Vocal", "Jazz+Funk", "Fusion", "Trance", "Classical", "Instrumental",
    "Acid", "House", "Game", "Sound Clip", "Gospel", "Noise", "AlternRock", "Bass",
    "Soul", "Punk", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock",
    "Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk",
    "Eurodance", "Dream", "Southern Rock", "Comedy", "Cult", "Gangsta", "Top 40",
    "Christian Rap", "Pop/Funk", "Jungle", "Native American", "Cabaret", "New Wave",
    "Psychadelic", "Rave", "Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk",
    "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll", "Hard Rock", "Folk",
    "Folk-Rock", "National Folk", "Swing",  "Fast Fusion", "Bebob", "Latin", "Revival",
    "Celtic", "Bluegrass", "Avantgarde", "Gothic Rock", "Progressive Rock", 
    "Psychedelic Rock", "Symphonic Rock", "Slow Rock", "Big Band", "Chorus", 
    "Easy Listening", "Acoustic", "Humour", "Speech", "Chanson", "Opera", 
    "Chamber Music", "Sonata", "Symphony", "Booty Bass", "Primus", "Porn Groove",
    "Satire", "Slow Jam", "Club", "Tango", "Samba", "Folklore", "Ballad", 
    "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet", "Punk Rock", "Drum Solo",
    "A capella", "Euro-House", "Dance Hall" };

    if ( tag.byGenre < countof(aszGenres) )
        lstrcpynA ( szField, aszGenres[tag.byGenre], cchBuffSize );
    else
        lstrcpynA ( szField, "(Other)", cchBuffSize );
}


//////////////////////////////////////////////////////////////////////////
//
// ReadTagFromFile() : Reads an ID3 tag from a file.
//
// szFilename: Full path to the file.
// pTag: Pointer to a CID3v1Tag that receives the tag, if one is present.
//
// Returns: true/false on success/failure.
//
// Note: This fn does not check if a tag is actually present, it just reads
//       the part of the file where the tag would be.
//
//////////////////////////////////////////////////////////////////////////

bool CMP3ColExt::ReadTagFromFile ( LPCTSTR szFilename, CID3v1Tag* pTag )
{
HANDLE   hfile;
bool     bReadLastAccessTime;
FILETIME ftLastAccess;

    ATLASSERT(!IsBadStringPtr ( szFilename, -1 ));
    ATLASSERT(!IsBadWritePtr ( pTag, sizeof(CID3v1Tag) ));

    // Open the MP3 file.
    hfile = CreateFile ( szFilename, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,
                         NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if ( INVALID_HANDLE_VALUE == hfile )
        return false;

    // Save the last-accessed time so we can restore it later.  Note that as
    // of today, in 2K (no SPs), by the time we get called, the file's last-
    // accessed time is already set to the current date/time, so this code
    // here doesn't have any effect.  Hopefully MS will fix this....
    bReadLastAccessTime = (0 != GetFileTime ( hfile, NULL, &ftLastAccess, NULL ));

__try
    {
    // Check the file length - it must be >= the size of an ID3v1 tag.
    DWORD dwLenLo, dwLenHi, dwBytesRead;

    dwLenLo = GetFileSize ( hfile, &dwLenHi );

    if ( -1 == dwLenLo  &&  GetLastError() != ERROR_SUCCESS )
        return false;                   // GetFileSize() failed.

    if ( 0 == dwLenHi  &&  dwLenLo < sizeof(CID3v1Tag) )
        return false;                   // File's too small to have a tag.

    // Seek 128 bytes from the end, where the ID3 tag would start.
    if ( INVALID_SET_FILE_POINTER == 
           SetFilePointer ( hfile, -1 * int(sizeof(CID3v1Tag)), NULL, FILE_END ))
        {
        return false;
        }

    // Read in the data - we don't care if it's really an ID3 tag or not.
    if ( !ReadFile ( hfile, pTag, sizeof(CID3v1Tag), &dwBytesRead, NULL ))
        return false;

    if ( sizeof(CID3v1Tag) != dwBytesRead )
        return false;
    }
__finally
    {
    ATLASSERT( INVALID_HANDLE_VALUE != hfile );

    // Restore the file's last-accessed time, and close it.
    if ( bReadLastAccessTime )
        SetFileTime ( hfile, NULL, &ftLastAccess, NULL );

    CloseHandle ( hfile );
    }

    return true;
}
