//
//	Display process tree under NT/2000/XP/2003
//
//	www.catch22.net
//
#include <windows.h>
#include <stdio.h>
#include "ntheader.h"

#pragma comment(linker, "/opt:nowin98")

BOOL GetQuerySystemInfoBuffer(ULONG InfoClass, PVOID *SysInfo, PULONG MemSize);
VOID FreeQuerySystemInfoBuffer(PVOID SysInfo);

typedef struct _PROCINFO
{
	// Per-process information
	WCHAR		ProcessName[MAX_PATH];
	ULONG		ProcessId;
	ULONG		ParentId;

	FILETIME	CreateTime;

	// Process hierarchy links
	struct _PROCINFO *FirstChild;
	struct _PROCINFO *NextSibling;
	struct _PROCINFO *Parent;

} PROCINFO, *PPROCINFO;

//
//	Flatten the tree
//
void DrawProcessTree(PROCINFO *node, int indent)
{
	int i;

	PROCINFO *ptr = node;

	for(; ptr != 0; ptr = ptr->NextSibling)
	{
		printf("%5d ", ptr->ProcessId);

		for(i = 0; i < indent; i++)
			printf("  ");

		printf(" %-32ls\n", ptr->ProcessName);

		if(ptr->FirstChild != 0)
			DrawProcessTree(ptr->FirstChild, indent+1);
	}
}

//
//	Build a tree from the process Ids and their parent's
//
void BuildProcessTree(PROCINFO *ProcInfo, ULONG ProcCount)
{
	ULONG i, j;

	for(i = 0; i < ProcCount; i++)
	{
		PROCINFO *parent = 0;

		// find the parent process
		for(j = 0; j < ProcCount; j++)
		{
			if(i != j && ProcInfo[i].ParentId == ProcInfo[j].ProcessId)
			{
				parent = &ProcInfo[j];
				ProcInfo[i].Parent = parent;
				break;
			}
		}

		// Does this process have a parent?
		if(parent != 0 && CompareFileTime(&parent->CreateTime, &ProcInfo[i].CreateTime) < 0)
		{
			// we found a parent node...add as a child of this node
			if(parent->FirstChild == 0)
			{
				parent->FirstChild = &ProcInfo[i];
			}
			// already has a child...add as a sibling of that child
			else
			{
				PROCINFO *ptr = parent->FirstChild;

				// find last child
				for(; ptr->NextSibling != 0; ptr = ptr->NextSibling);

				// append
				ptr->NextSibling = &ProcInfo[i];
			}
		}
		// Couldn't find a parent....add as a "root" process
		else if(i > 0)
		{
			// no parent, add as a sibling of root node..
			PROCINFO *ptr = &ProcInfo[0];
			
			// insert as a child of parent
			for(; ptr->NextSibling != 0; ptr = ptr->NextSibling);

			// append
			ptr->NextSibling = &ProcInfo[i];
		}
	}
}

//
//	Return list of processes. 
//
BOOL GetProcessList(PROCINFO *pProcInfo, ULONG *nProcCount)
{
	ULONG Offset = 0;
	ULONG MemSize;
	PBYTE base;
	SYSTEM_PROCESSES *spti;
	ULONG i = 0;

	if(!GetQuerySystemInfoBuffer(5, (PVOID*)&base, &MemSize))
		return FALSE;

	spti = (SYSTEM_PROCESSES *)base;

	while(Offset < MemSize)
	{
		spti = (SYSTEM_PROCESSES *)(base + Offset);

		Offset += spti->NextEntryDelta;

		if(i < *nProcCount)
		{
			pProcInfo[i].ProcessId   = spti->ProcessId;
			pProcInfo[i].ParentId    = spti->InheritiedFromProcessId;
			
			pProcInfo[i].CreateTime.dwHighDateTime = spti->CreateTime.HighPart;
			pProcInfo[i].CreateTime.dwLowDateTime  = spti->CreateTime.LowPart;
			
			lstrcpyW(pProcInfo[i].ProcessName, spti->ProcessName.Buffer);

			pProcInfo[i].FirstChild  = 0;
			pProcInfo[i].NextSibling = 0;
			pProcInfo[i].Parent      = 0;

			i++;
		}

		if(spti->NextEntryDelta == 0)
			break;
	}

	FreeQuerySystemInfoBuffer(base);

	*nProcCount = i;

	return TRUE;
}

int main()
{
	ULONG	  ProcCount;
	PROCINFO *ProcList = malloc(100 * sizeof(PROCINFO));

	if(ProcList)
	{
		// Get the list of processes
		GetProcessList(ProcList, &ProcCount);

		BuildProcessTree(ProcList, ProcCount);

		// "Flatten" tree
		DrawProcessTree(&ProcList[0], 0);

		free(ProcList);
	}
	getchar();
	return 0;
}