/*******************************************************************|
|                                                                   |
|       GetOpt.cpp                                                  |
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

//********************************************************************
// INCLUDES
//********************************************************************

#include "stdafx.h"
#include "GetOpt.h"

#include <algorithm>
#include <cstdlib>

//*******************************************************************
// CONSTRUCTOR & DESTRUCTOR
//*******************************************************************

CGetOpt::CGetOpt()
{
	// Initialize variables
	m_bCompareCaseSensitive = false;

	// Clear results
	ResetParamData();
}

//===================================================================

CGetOpt::~CGetOpt()
{
	
}

//*******************************************************************
// PUBLIC FUNCTIONS
//*******************************************************************

void CGetOpt::CompareCaseSensitive(bool bCaseSensitive /* = false */)
{
	// Set value
	m_bCompareCaseSensitive = bCaseSensitive;
}

//===================================================================

void CGetOpt::AddExpectedParameter(string sParameter, bool bValueExpected /* = true */,
				ParameterType oParamType /* = PARAMETERTYPE_UNKNOWN */)
{
	// Declare variables
	Param oParamInfo;

	// Add values to parameter info
	oParamInfo.sParameter = sParameter;
	oParamInfo.bValueExpected = bValueExpected;
	oParamInfo.oType = oParamType;

	// Add struct to array
	m_arrParametersExpected[m_iParamExpectedCount++] = oParamInfo;
}

//===================================================================

void CGetOpt::ParseCommandLine(string sCommandLine)
{
	// Declare variables
	int iParameterIndex;
	string sParameter, sValue;

	// Reset old data
	m_iParamUsedCount = 0;

	// Loop as long as we have a parameter line
	while (sCommandLine.length() > 0)
	{
		// Clear previous data
		sParameter = "";
		sValue = "";

		// Trim spaces from command line
		sCommandLine = TrimSpaces(sCommandLine);

		// Get first block (quotes are not allowed)
		sParameter = GetFirstTextBlock(sCommandLine, false);

		// Strip parameter
		sCommandLine.erase(0, sParameter.length() + 1);

		// Trim spaces from command line
		sCommandLine = TrimSpaces(sCommandLine);

		// Is block a parameter?
		iParameterIndex = -1;
		for (int i = 0; i < m_iParamExpectedCount; i++)
		{
			// Compare
			if (m_bCompareCaseSensitive)
			{
				if (strcmp(m_arrParametersExpected[i].sParameter.c_str(), sParameter.c_str()) == 0)
				{
					// We found it
					iParameterIndex = i;
				}
			}
			else
			{
				if (_stricmp(m_arrParametersExpected[i].sParameter.c_str(), sParameter.c_str()) == 0)
				{
					// We found it
					iParameterIndex = i;
				}
			}
		}

		// Did we find the parameter?
		if (iParameterIndex == -1)
		{
			// No, but maybe a part of the text is a parameter
			for (int i = 0; i < m_iParamExpectedCount; i++)
			{
				// Compare
				if (m_bCompareCaseSensitive)
				{
					if (sParameter.find(m_arrParametersExpected[i].sParameter) == 0)
					{
						// We found it
						iParameterIndex = i;
					}
				}
				else
				{
					if (MakeLowerCase(sParameter).find(MakeLowerCase(m_arrParametersExpected[i].sParameter)) == 0)
					{
						// We found it
						iParameterIndex = i;
					}
				}
			}
		}

		// Did we find any parameter?
		if (iParameterIndex == -1)
		{
			// Unknown parameters are used!
			throw UnknownParameterException();
		}

		// Strip parameter from text block
		sParameter.erase(0, m_arrParametersExpected[iParameterIndex].sParameter.length());

		// Do we expect a value?
		if (m_arrParametersExpected[iParameterIndex].bValueExpected)
		{
			// Yes, but is parameter length larget than 0?
			if (sParameter.length() > 0)
			{
				// Value is located in the parameter
				sValue = sParameter;
			}
			else
			{
				// Get next textblock, quotes are allowed
				sValue = GetFirstTextBlock(sCommandLine, true);

				// Remove value from command line
				sCommandLine.erase(0, sValue.length());

				// Trim quotes
				sValue = TrimQuotes(sValue);
			}

			// Do we have a valid parameter value?
			if (sValue.length() <= 0)
			{
				// No, throw exception!
				throw ValueExpectedException();
			}

			// Check if value is valid
			if (!ValidType(sValue, m_arrParametersExpected[iParameterIndex].oType))
			{
				// Invalid type!
				throw WrongTypeException();
			}
		}
		else
		{
			// Is there still text left in the parameter?
			if (sParameter.length() > 0)
			{
				// Unknown parameters are used!
				throw UnknownParameterException();
			}
		}

		// Add parameter to used parameters list
		AddParameterToUsedList(iParameterIndex, sValue);
	}

	// If we get here, check if function is successful
	if (TrimSpaces(sCommandLine).length() > 0)
	{
		// Unknown parameters are used
		throw UnknownParameterException();
	}
}

//===================================================================

int	CGetOpt::GetParameterResultCount()
{
	// Return parameter count
	return m_iParamUsedCount;
}

//===================================================================

bool CGetOpt::ParameterUsed(string sParameter)
{
	// Declare variables
	bool bFound = false;

	// Loop all used parameters
	for (int i = 0; i < m_iParamUsedCount; i++)
	{
		// Check if we are working case-sensitive
		if (m_bCompareCaseSensitive)
		{
			// Compare case-sensitive
			bFound = (strcmp(sParameter.c_str(), m_arrParametersUsed[i].sParameter.c_str()) == 0) ? true : false;
		}
		else
		{
			// Compare case-insensitive
			bFound = (_stricmp(sParameter.c_str(), m_arrParametersUsed[i].sParameter.c_str()) == 0) ? true : false;
		}

		// Did we find the parameter?
		if (bFound)
		{
			// We found the parameter!
			return true;
		}
	}

	// If we get here, parameter is not used
	return false;
}

//===================================================================

Param CGetOpt::GetParameterResultInfo(string sParameter)
{
	// Declare variables
	bool bFound = false;
	Param oParamInfo;

	// Loop all used parameters
	for (int i = 0; i < m_iParamUsedCount; i++)
	{
		// Check if we are working case-sensitive
		if (m_bCompareCaseSensitive)
		{
			// Compare case-sensitive
			bFound = (strcmp(sParameter.c_str(), m_arrParametersUsed[i].sParameter.c_str()) == 0) ? true : false;
		}
		else
		{
			// Compare case-insensitive
			bFound = (_stricmp(sParameter.c_str(), m_arrParametersUsed[i].sParameter.c_str()) == 0) ? true : false;
		}

		// Did we find the parameter?
		if (bFound)
		{
			// We found the parameter, was a value expected?
			if (m_arrParametersUsed[i].bValueExpected)
			{
				// Yes, return value
				return m_arrParametersUsed[i];
			}
			else
			{
				// No, return empty value
				return oParamInfo;
			}
		}
	}

	// If we get here, parameter is not used
	return oParamInfo;
}

//===================================================================

Param CGetOpt::GetParameterResultInfo(int iIndex)
{
	// Declare variables
	Param oParamInfo;

	// Is number in range?
	if ((iIndex < 0) || (iIndex > m_iParamUsedCount))
	{
		// No value
		return oParamInfo;
	}

	// Return value
	return m_arrParametersUsed[iIndex];
}

//*******************************************************************
// PROTECTED / PRIVATE FUNCTIONS
//*******************************************************************

void CGetOpt::ResetParamData()
{
	// Clear all data
	m_iParamExpectedCount = 0;
	m_iParamUsedCount = 0;
}

//===================================================================

void CGetOpt::AddParameterToUsedList(int iParameterIndex, string sValue /* = "" */)
{
	// Is parameter already in the used list?
	for (int i = 0; i < m_iParamUsedCount; i++)
	{
		// Compare items
		if (m_bCompareCaseSensitive)
		{
			if (strcmp(m_arrParametersUsed[i].sParameter.c_str(), m_arrParametersExpected[iParameterIndex].sParameter.c_str()) == 0)
			{
				throw DoubleParametersException();
			}
		}
		else
		{
			if (_stricmp(m_arrParametersUsed[i].sParameter.c_str(), m_arrParametersExpected[iParameterIndex].sParameter.c_str()) == 0)
			{
				throw DoubleParametersException();
			}

		}
	}

	// If we get here, add parameter
	m_arrParametersUsed[m_iParamUsedCount++] = m_arrParametersExpected[iParameterIndex];
	m_arrParametersUsed[m_iParamUsedCount-1].sValue = TrimQuotes(sValue);
}

//===================================================================

string CGetOpt::GetFirstTextBlock(string sText, bool bAllowQuotes /* = false */)
{
	// Declare variables
	int iBlockStart = -1, iBlockEnd;
	string sResult;

	// Trim spaces
	sText = TrimSpaces(sText);

	// Should we allow quotes?
	if (bAllowQuotes)
	{
		// Is first character a "?
		if (sText.substr(0, 1) == "\"")
		{
			// Find next parameter
			iBlockStart = 0;
			iBlockEnd = sText.find('"', 1);

			// Don't forget to also include the quote
			if (iBlockEnd != -1)
			{
				iBlockEnd++;
			}
		}
	}

	// Should we search for next space?
	if (iBlockStart == -1)
	{	
		// Start is first character
		iBlockStart = 0;
		iBlockEnd = sText.find(' ', iBlockStart);
	}

	// Is there an ending?
	if (iBlockEnd == -1)
	{
		// No, treat all text as value
		sResult = sText.substr(iBlockStart, sText.length() - iBlockStart);
	}
	else
	{
		// Yes, use value between the separators
		sResult = sText.substr(iBlockStart, iBlockEnd - iBlockStart);
	}	

	// Return value
	return sResult;
}

//===================================================================

bool CGetOpt::ValidType(string sValue, ParameterType oParamType)
{
	// Declare variables
	bool bValid = false;
	string sTemp;

	// Check which type
	switch (oParamType)
	{
	case PARAMETERTYPE_UNKNOWN:
		// All types are all-right
		bValid = true;
		break;

	case PARAMETERTYPE_INTEGER:
		// Convert to integer
		if ((TrimSpaces(sValue) == "0") || (atoi(sValue.c_str()) != 0))
		{
			// Right type
			bValid = true;
		}
		break;

	case PARAMETERTYPE_BOOL:
		// Trim spaces
		sTemp = TrimSpaces(sValue);

		// Is it yes or no
		if ((MakeLowerCase(sValue) == "yes") ||  (MakeLowerCase(sValue) == "no"))
		{
			// Right type
			bValid = true;
		}

		// Is it true or false
		if ((MakeLowerCase(sValue) == "true") ||  (MakeLowerCase(sValue) == "false"))
		{
			// Right type
			bValid = true;
		}

		// Is it 1 or 0
		if ((sValue == "1") ||  (sValue == "0"))
		{
			// Right type
			bValid = true;
		}
		break;

	case PARAMETERTYPE_STRING:
		// String is always ok
		bValid = true;
		break;

	default:
		// Unknown type
		bValid = false;
		break;
	}

	// Return value
	return bValid;
}

//===================================================================

string CGetOpt::TrimSpaces(string sValue)
{
	// Trim at the left
	while ((sValue.length() > 0) && (sValue.substr(0, 1) == " "))
	{
		// Remove the space
		sValue.erase(0, 1);
	}

	// Trim at the right
	while ((sValue.length() > 0) && (sValue.substr(sValue.length() - 1, 1) == " "))
	{
		// Remove the space
		sValue.erase(sValue.length() - 1, 1);
	}

	// Return trimmed string
	return sValue;
}

//===================================================================

string CGetOpt::TrimQuotes(string sValue)
{
	// Trim at the left
	while ((sValue.length() > 0) && (sValue.substr(0, 1) == "\""))
	{
		// Remove the quotes
		sValue.erase(0, 1);
	}

	// Trim at the right
	while ((sValue.length() > 0) && (sValue.substr(sValue.length() - 1, 1) == "\""))
	{
		// Remove the quotes
		sValue.erase(sValue.length() - 1, 1);
	}

	// Return trimmed string
	return sValue;
}

//===================================================================

string CGetOpt::MakeLowerCase(string sValue)
{
	// Declare variables
	string sResult = sValue;

	// Transform the string
	transform(sValue.begin(), sValue.end(), sResult.begin(), tolower);

	// Return the result
	return sResult;
}