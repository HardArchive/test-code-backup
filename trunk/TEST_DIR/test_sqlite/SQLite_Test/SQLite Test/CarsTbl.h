// CarsTbl.h : header file
//

#ifndef CarsTbl_h
#define CarsTbl_h

#include "SQLiteTbl.h"

class CCarsTbl : public SQLiteTbl
{
public:
    CCarsTbl();

    virtual bool Open(const char* pFile);
    virtual const char* TableName();
    virtual int ItemCount();

    SQLiteString m_owner;    
    SQLiteString m_make;
    SQLiteString m_model;
    SQLiteString m_year;
    SQLiteString m_color;
//5
    int m_vin;
    SQLiteString m_license;
    SQLiteBlob m_picture;
};



#endif