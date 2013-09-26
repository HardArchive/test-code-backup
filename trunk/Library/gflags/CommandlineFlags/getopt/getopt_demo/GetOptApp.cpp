/*******************************************************************|
|                                                                   |
|       GetOptApp demo application                                  |
|       Geert van Horrik, visit http://www.gvhsoftware.org          |
|       Date: 2006-02-28                                            |
|                                                                   |
|*******************************************************************/

//********************************************************************
// INCLUDES
//********************************************************************

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "GetOpt.h"

//*******************************************************************
// MAIN FUNCTION
//*******************************************************************

int main(int argc, char* argv[])
{
	// Declare variables
	char szCommandLine[256];
	CGetOpt oParamParsing;
	int i;

	// Add some example parameters
	oParamParsing.AddExpectedParameter("-check", true, PARAMETERTYPE_BOOL);
	oParamParsing.AddExpectedParameter("-url", true, PARAMETERTYPE_STRING);
	oParamParsing.AddExpectedParameter("-log", false);
	oParamParsing.AddExpectedParameter("-booltest", true, PARAMETERTYPE_BOOL);

	// Show app header
	printf("CGetOpt Test application\n");
	printf("************************\n");
	
	// Loop as long as the user wants to loop
	do
	{
		// Show text
		printf("\nEnter a parameter line (max 256 parameters)... (empty parameter = exit)\n");

		// Wait for user input
		cin.getline(szCommandLine, 256);

		// If user input is not empty, show results
		if (strlen(szCommandLine) > 0)
		{
			// Show start of results
			printf("\nResults of %s\n", szCommandLine);
			printf("==========\n");

			try
			{
				// Parse parameters
				oParamParsing.ParseCommandLine(szCommandLine);

				// Show results
				for (i = 0; i < oParamParsing.GetParameterResultCount(); i++)
				{
					Param oInfo = oParamParsing.GetParameterResultInfo(i);
					printf("%s => %s\n", oInfo.sParameter.c_str(),
						oInfo.sValue.c_str());
				}
			}
			catch (exception & ex)
			{
				printf("%s\n", ex.what());
			}

			// Show end of results
			printf("**********\n");
		}
	}
	while (strlen(szCommandLine) > 0);

	// Exit
	return 0;
}