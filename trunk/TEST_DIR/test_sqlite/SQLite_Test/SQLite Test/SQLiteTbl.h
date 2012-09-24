/*
SQLiteTbl - Version 1.3

You are free to use and modify this code without restriction to fit your needs. 
Here is the sqlite license, which sounds good to me:
************************************************************************
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
************************************************************************

SQLiteTbl is a SQLite database wrapper class.  It provides the ability to
have data member access to the data values in the specified table.

I wanted to replace the MFC CRecordSet class with a new class that provides
simlar functionality but uses the free SQLite database engine.

SQLite is a free database.  Here is their blurb from www.sqlite.org
"SQLite is a in-process library that implements a self-contained, serverless, 
zero-configuration, transactional SQL database engine. The code for SQLite 
is in the public domain and is thus free for use for any purpose, commercial or private."

In order to use SQLiteTbl, you will need to download sqlite as and add it to your project.
SQLiteTbl expects to find sqlite3.h and sqliteInt.h in your include path.

There is no dependance on MFC or STL.  I still wanted to have a CString like object,
so I derived a simple string class called SQLiteString.  I also need somewhere to store 
the blob data, so I created a SQLiteBlob.

SQLiteTbl is an abstract base class.  You will need to instantiate a derived SQLiteTbl in 
order for this work. 

example:
cars.h
-=-=-=-=
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
-=-=-=

cars.cpp
-=-=-=
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
int CarsTbl::ItemCount() { return 7; } //must match items listed above

const char* CarsTbl::TableName() { return "DMV Cars"; }

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
-=-=-=

Brad Kremer, Fluke Networks - December 2008

*/

#ifndef SQLITETBL_H
#define SQLITETBL_H

#include "sqlite3.h"
#include "sqliteInt.h"

//this #define enables the auto-vacuum mode, which will keep the file sizes small
#define MYSQLITE_AUTOVAC_ON

enum MYDB_TYPE {
    MYDB_TYPE_NONE,
    MYDB_TYPE_TEXT,
    MYDB_TYPE_INT,
    MYDB_TYPE_DOUBLE,
    MYDB_TYPE_BLOB,
};

struct MYDB_ITEM {
    MYDB_TYPE type;
    void* pData;  
    int item_col; //which column is the item
    char name[100];
};

//basic blob class
class SQLiteBlob {
public:
    SQLiteBlob() { m_pData = NULL; m_size = 0; }
    ~SQLiteBlob() { Clean(); }

    void Clean();
    void Set(const char* pData, int size);
    const char* GetData() { return (m_size>0) ? m_pData : NULL; }
    int GetSize() { return m_size; }

private:
    char* m_pData;
    int m_size;
};

//I didn't want to be dependant on MFC or STL, so I made my own string obj
//feel free to expand it if needed
class SQLiteString : public SQLiteBlob 
{
public:
    SQLiteString() { SQLiteBlob::SQLiteBlob();  Set("", 1); }
    SQLiteString(const char* pData) { SetString(pData); }

    void SetString(const char* pData);
    void Format(const char* fmt, ...);
    int GetLength() { int l = GetSize(); return (l>1) ? l-1 : 0; } //string length, not buffer
    bool IsEmpty() { return GetLength() < 1; }
    void SetAt(int i, char c); //only use if necessary

    const char* operator =(const char* pData) { SetString(pData);  return GetData(); }
    SQLiteString& operator =(SQLiteString& data) { Set(data.GetData(), data.GetSize()); return *this; }
    operator const char*()  { return GetData(); }
    char operator [](int i); 
    void operator +=(const char* pData);
    void operator +=(char c) { char t[2]; t[0]=c; t[1]='\0'; *this+=t; }
    bool operator ==(const char* pS);
    bool operator !=(const char* pS) { return !(*this==pS); }
    bool operator <(const char* pS);
    bool operator >(const char* pS);
};


//SQLiteTbl is the base class for each of your SQLite tables in your project.  You
//would instantiate a new class for each table with this as your base class.
class SQLiteTbl
{
public:
    SQLiteTbl();
    virtual ~SQLiteTbl();

    //virtual functions that need to be defined in the derived class
    virtual const char* TableName() = 0;     
    virtual int ItemCount() = 0; //number of items we are using.  can be less than the actual columns
    virtual bool Open(const char* pFile);

    bool Create(const char* pFile);    
    bool IsOpen() { return m_is_open; }
    bool Compact(); //be careful using this, as it will take time, plus if you use the auto-vacuum, you don't need it
    const char* Path() { return m_path; }

    void SetSort(const char* pSort) { m_sort = pSort; }
    const char* GetSort() { return m_sort; }
    void SetFilter(const char* pFilter) { m_filter = pFilter; }
    const char* GetFilter() { return m_filter; }
    bool Query(); //run the select string
    bool Close();
    bool IsBOF(); //beginning of file
    bool IsEOF(); //end of file
    int CurrentPos() { return m_row_pos; } //current position
    int GetCount() { return m_row_count; }//get the current count of items
    int ReadCount();  //executes the sql to ge the count
    const char* GetSQL(bool rowid_only=false, bool use_filter=true, bool use_sort=true);
    
    bool ExeSQL(const char* pSQL); //provides mechanism to run SQL command directly
    bool SetOneItem(const char* pItem, const char* pValue, int rowid=-1); //sets one item in the table quickly.  doesn't use edit/update

    bool MoveFirst();
    bool MoveNext();
    bool MovePrev();
    bool MoveLast();
    bool Move(int idx); //move to a specific index.  One-based, because of I was making it like CRecordSet

    bool Edit();    //call edit first, then change values, must call Update when finished
    bool InEdit() { return m_rowid_edit > 0; }
    void EditCancel() { m_rowid_edit = 0; } //cancel the edit
    bool AddNew(); //must call Update when finished
    bool Update();  //call Update after you have changed the values

    bool Delete();

    //caution - be careful how you use the sqlite direct access
    sqlite3* GetDB() { return m_pDb; }
    MYDB_ITEM* Items() { return m_pItems; }
    int* GetRowids() { return m_pRowids; } //again, be careful using this
    int GetQueryCount() { return m_query_count; }

protected:
    void InitItems();
    void SetItem(int item, const char* pName, MYDB_TYPE type, void* pData);
    void LoadItems(sqlite3_stmt* pStmt);
    bool LoadColumnNumbers();      
    bool InsertTable();
    bool SlowAdd();
    void AddRowid(int id);
    
    sqlite3* m_pDb;
    MYDB_ITEM* m_pItems; //this must be set by the derived class
    bool m_is_open;
    SQLiteString m_path;
    SQLiteString m_filter;
    SQLiteString m_sort;
    int m_row_count;
    int m_row_pos; //current position
    int m_col_count; //number of actual columns in table
    bool m_col_loaded; //set true after we have loaded the column indexes

    int* m_pRowids;
    int m_rowid_size; //bigger to allow us to add new records
    int m_query_count; //used so we know how far we are into the query

    int m_rowid_edit; //used for edit/update
    bool m_add; //used to show we are adding
};


#endif //SQLITETBL_H
