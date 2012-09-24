// CarsTbl.cpp : implementation file
//

#include "stdafx.h"
#include "CarsTbl.h"


CCarsTbl::CCarsTbl()
{
    //these must match table above
    SetItem(0, "Owner",             MYDB_TYPE_TEXT, &m_owner);
    SetItem(1, "Make",              MYDB_TYPE_TEXT, &m_make);
    SetItem(2, "Model",             MYDB_TYPE_TEXT, &m_model);
    SetItem(3, "Color",             MYDB_TYPE_TEXT, &m_color);
    SetItem(4, "Year",              MYDB_TYPE_TEXT, &m_year);
//5
    SetItem(5, "VIN",               MYDB_TYPE_INT,  &m_vin);
    SetItem(6, "License",           MYDB_TYPE_TEXT, &m_license);
    SetItem(7, "Color Picture",     MYDB_TYPE_BLOB, &m_picture);
}

const char* CCarsTbl::TableName() { return "DMV Cars"; }
int CCarsTbl::ItemCount() { return 8; }


bool CCarsTbl::Open(const char* pFile)
{
    if (SQLiteTbl::Open(pFile)) {
        if (Query()) 
            return true;
    }
    //anything goes wrong, close and return false
    Close();
    return false;
}






class CarsTbl : public SQLiteTbl
{
public:
    CarsTbl();

    virtual bool Open(const char* pFile);
    virtual const char* TableName();
    virtual int ItemCount();
 
    //data members of the table
//0
    SQLiteString m_make;
    SQLiteString m_model;
    int m_year;
    double m_weight;
    SQLiteString m_color;
//5
    SQLiteString m_license;
    SQLiteBlob m_picture;
};


CarsTbl::CarsTbl()
{
    //these must match table in .h
//0
    SetItem(0, "Make",              MYDB_TYPE_TEXT, &m_make);
    SetItem(1, "Model",             MYDB_TYPE_TEXT, &m_model);
    SetItem(2, "Year",              MYDB_TYPE_INT, &m_year);
    SetItem(3, "Weight",            MYDB_TYPE_DOUBLE, &m_weight);
    SetItem(4, "Color",             MYDB_TYPE_TEXT, &m_color);
//5
    SetItem(5, "License",           MYDB_TYPE_TEXT, &m_license);
    SetItem(6, "Color Picture",     MYDB_TYPE_BLOB, &m_picture);
}

const char* CarsTbl::TableName() { return "DMV Cars"; }
int CarsTbl::ItemCount() { return 7; }


bool CarsTbl::Open(const char* pFile)
{
    if (SQLiteTbl::Open(pFile)) {
        if (Query()) 
            return true;
    }
    //anything goes wrong, close and return false
    Close();
    return false;
}