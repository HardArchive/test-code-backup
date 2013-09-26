/*******************************************************************|
|                                                                   |
|       GetOpt.h                                                    |
|       Geert van Horrik, visit http://www.gvhsoftware.org          |
|       Version: 1.1                                                |
|       Date: 2006-03-07                                            |
|                                                                   |
|       License:                                                    |
|         This software is released into the public domain.  You    |
|         are free to use it in any way you like, except that you   |
|         may not sell this source code.                            |
|                                                                   |
|         This software is provided "as is" with no expressed or    |
|         implied warranty. I accept no liability for any damage or |
|         loss of business that this software may cause.            |
|                                                                   |
|*******************************************************************/

// Redefinition
#ifndef GETOPT_H
#define GETOPT_H

//********************************************************************
// INCLUDES
//********************************************************************

#include <string>
#include <exception>
using namespace std;

//********************************************************************
// EXCEPTIONS
//********************************************************************

class WrongTypeException : public exception
{
public:
	virtual const char * what() const
	{
		return "Wrong parameter type is used";
	}
};

class ValueExpectedException : public exception
{
public:
	virtual const char * what() const
	{
		return "Value was expected, but none was given";
	}
};

class UnknownParameterException : public exception
{
public:
	virtual const char * what() const
	{
		return "Unknown parameters are used";
	}
};

class DoubleParametersException : public exception
{
public:
	virtual const char * what() const
	{
		return "Parameters are used twice";
	}
};

//********************************************************************
// TYPEDEFS
//********************************************************************

enum ParameterType
{
	PARAMETERTYPE_UNKNOWN,
	PARAMETERTYPE_INTEGER,
	PARAMETERTYPE_BOOL, 
	PARAMETERTYPE_STRING
};

struct Param
{
	string			sParameter;
	string			sValue;
	bool			bValueExpected;
	ParameterType	oType;

	Param()
	{
		// Initialize values
		sParameter = "";
		sValue = "";
		bValueExpected = true;
		oType = PARAMETERTYPE_UNKNOWN;
	}
};

//********************************************************************
// CGetOpt
//********************************************************************

class CGetOpt
{
public:
	// Constructor & destructor
	CGetOpt();
	~CGetOpt();

	// Functions
	void	CompareCaseSensitive(bool bCaseSensitive = false);
	void	AddExpectedParameter(string sParameter, bool bValueExpected = true,
				ParameterType oParamType = PARAMETERTYPE_UNKNOWN);

	void	ParseCommandLine(string sCommandLine);
	int		GetParameterResultCount();
	bool	ParameterUsed(string sParameter);
	Param	GetParameterResultInfo(string sParameter);
	Param	GetParameterResultInfo(int iIndex);

protected:
	// Functions
	void	ResetParamData();
	void	AddParameterToUsedList(int iParameterIndex, string sValue = "");
	string	GetFirstTextBlock(string sText, bool bAllowQuotes = false);
	bool	ValidType(string sValue, ParameterType oParamType);
	string	TrimSpaces(string sValue);
	string	TrimQuotes(string sValue);
	string	MakeLowerCase(string sValue);

	// Variables
	Param	m_arrParametersExpected[256];
	Param	m_arrParametersUsed[256];
	int		m_iParamExpectedCount;
	int		m_iParamUsedCount;
	bool	m_bCompareCaseSensitive;
};

// End of redefinition protection
#endif // GETOPT_H
