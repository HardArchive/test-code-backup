//
//	TREE.C
//
//	Console program to recurse through a directory structure and
//  display the directory levels using ASCII graphics
//
//	Usage: "tree [-d depth] path"
//
//	www.catch22.net
//
#include <windows.h>
#include <stdio.h>

#define TABCHARS  4
#define MAXLEVELS 16

#pragma comment(linker, "/opt:nowin98")

struct _treenode
{
	struct _treenode * parent;
	int				   children;
};

typedef struct _treenode treenode;

void null_term(char *buf, int level)
{
	buf[level * TABCHARS] = 0;
}

void print_line(char *buf, int level, int nodechar, int fillchar)
{
	int i;

	buf[level * TABCHARS] = nodechar;

	for(i = 1; i < TABCHARS; i++)
	{
		buf[i + level * TABCHARS] = fillchar;
	}
}

#define OEMCHAR_HORZLINE 0xC4		//'-'
#define OEMCHAR_VERTLINE 0xB3		//'|'
#define OEMCHAR_ELL		 0xC0		//'\'
#define OEMCHAR_3BAR	 0xC3		//'+'
#define OEMCHAR_CROSS    0xC5		//'+'
#define OEMCHAR_BLANK	 0x20		//' '

void print_item(char *szItemName, int level, treenode *ptn)
{
	char szFill[MAXLEVELS * TABCHARS + 1];

	if(level == MAXLEVELS)
		return;

	// null-terminate in correct place
	null_term(szFill, level+1);

	if(ptn->children)
		print_line(szFill, level, OEMCHAR_3BAR, OEMCHAR_HORZLINE);
	else
		print_line(szFill, level, OEMCHAR_ELL, OEMCHAR_HORZLINE);

	ptn = ptn->parent;

	// traverse BACKWARDS through the hierarchy (i.e. left <- right)
	while(level--)
	{
		if(ptn->children)
			print_line(szFill, level, OEMCHAR_VERTLINE, OEMCHAR_BLANK);
		else
			print_line(szFill, level, OEMCHAR_BLANK, OEMCHAR_BLANK);

		ptn = ptn->parent;
	}
	
	// now print the whole tree graphic from left -> right
	printf(" %s%s", szFill, szItemName);
}

BOOL IsDirectory(WIN32_FIND_DATA *win32fd)
{
	if(win32fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		if(win32fd->cFileName[0] == '.'  && win32fd->cFileName[1] == '\0' ||
		   win32fd->cFileName[0] == '.'  && win32fd->cFileName[1] == '.'  && win32fd->cFileName[2] == '\0')
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}

//
//	Wrapper around FindFirstFile - restricts to directories only
//
HANDLE FindFirstDirectory(char *szFileName, WIN32_FIND_DATA *pFindFileData)
{
	HANDLE hFind;

	if((hFind = FindFirstFile(szFileName, pFindFileData)) == INVALID_HANDLE_VALUE)
		return INVALID_HANDLE_VALUE;

	do
	{
		// look out for directories
		if(IsDirectory(pFindFileData))
			return hFind;

	} while(FindNextFile(hFind, pFindFileData));

	// didn't find any directories...close the enumeration
	FindClose(hFind);

	return INVALID_HANDLE_VALUE;
}

//
//	Wrapper around FindNextFile - restricts to directories only
//
static BOOL FindNextDirectory(HANDLE hFindFile, WIN32_FIND_DATA *pFindFileData)
{
	// enumerate files until we find a directory
	while(FindNextFile(hFindFile, pFindFileData))
	{
		if(IsDirectory(pFindFileData))
			return TRUE;
	} 

	return FALSE;
}

//
//	Append the specified string onto the base string
//
char *appendpath(char *base, char *str)
{
	// find end of string
	char *ptr = base + lstrlen(base);

	// add on a slash if necessary
	if(*(ptr-1) != '\\')
		*ptr++  = '\\';

	// append the string
	lstrcpy(ptr, str);

	return ptr;
}

int recurse(char *szPath, int level, int maxdepth, treenode *ptn)
{
	int				cursubdir  = 0;
	int				nextsubdir = 1;
	WIN32_FIND_DATA win32fd[2];
	BOOL			fRet[2] =  { TRUE, TRUE };

	char *			ptr;
	HANDLE			hFind;

	int				subdirs = 0;

	// allocate (on the stack) a new treenode for this level
	// when we recurse this function call, subsequent recursions link
	// back through each other, resulting in a hierarchy we can traverse.
	treenode tn = { ptn, 0 };

	// add on the search criteria
	ptr = appendpath(szPath, "*");

	// Find a sub-directory
	hFind   = FindFirstDirectory(szPath, &win32fd[0]);
	fRet[0] = (hFind != INVALID_HANDLE_VALUE);

	// restore to normal
	*ptr = '\0';

	// if this tree has subtrees, but we are at maxdepth, then exit out :)
	if(maxdepth == 0 && fRet[0] == TRUE)
	{
		// terminate the LAST line just printed - indicate that there
		// are further subtrees that we havn't shown
		printf(" +\n");

		FindClose(hFind);
		return 0;
	}
	else if(level > 0)
	{
		// terminate the LAST line that was displayed
		printf("\n");
	}
	
	// Loop until there are no more subtrees. 
	while(fRet[nextsubdir])
	{ 
		// Find the next subdirectory
		fRet[nextsubdir] = FindNextDirectory(hFind, &win32fd[nextsubdir]);

		// indicate to sublevels if we have another sibling
		tn.children = fRet[nextsubdir];

		if(fRet[cursubdir] == TRUE)
		{ 
			// Append subdirectory name
			ptr = appendpath(szPath, "");
			lstrcat(ptr, win32fd[cursubdir].cFileName);

			// print current subtree
			print_item(win32fd[cursubdir].cFileName, level, &tn);

			// step down another level
			recurse(szPath, level + 1, maxdepth - 1, &tn);

			// Restore 
			*ptr = '\0';

			subdirs++;
		} 

		// toggle 
		nextsubdir ^= 1;
		cursubdir  ^= 1;
	} 

	FindClose(hFind);

	return subdirs;
}


void tree(const char *szPath, int maxdepth)
{
	char szCopy[MAX_PATH];

	// make a "writable" copy of the specified path - 
	// must be big enough to allow the recursion to append 
	// subdirectories to it
	lstrcpy(szCopy, szPath);

	printf("%s:\n", szCopy);
	
	// do the real work
	recurse(szCopy, 0, maxdepth, 0);
	
	printf("\n");
}

void showhelp()
{
	printf("Usage: tree [-d depth] [path]\n\n");
}

//
//	usage: tree [-d:num] [path]
//
int main(int argc, char *argv[])
{
	int		depth = 1;
	char *	path;

	if(argc == 1)
	{
		showhelp();
		return 0;
	}
	else
	{
		// depth argument supplies
		if(lstrcmp(argv[1], "-d") == 0)
		{
			if(argc != 4)
			{
				showhelp();
				return 0;
			}
			else
			{
				depth = atoi(argv[2]);
				path  = argv[3];
			}
		}
		// default depth is 1 deep
		else
		{
			if(argc != 2)
			{
				showhelp();
				return 0;
			}
			else
			{
				depth = 1;			// default
				path  = argv[1];
			}
		}
	}
	
	// do it!
	tree(path, depth);

	getchar();

	return 0;
}
