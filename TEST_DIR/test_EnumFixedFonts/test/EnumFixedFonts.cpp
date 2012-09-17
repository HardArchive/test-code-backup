#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

HDC  hdc;
char currentfonts[200][64];
int  curfont = 0;

int  currentsizes[200];
int  cursize = 0;

int CALLBACK FontSizesProc(
	LOGFONT *plf,      /* pointer to logical-font data */
	TEXTMETRIC *ptm,   /* pointer to physical-font data */
	DWORD FontType,    /* font type */
	LPARAM lParam      /* pointer to application-defined data */
	)
{
	static int truetypesize[] = { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72 };
	int i;

	if(FontType != TRUETYPE_FONTTYPE)
	{
		int  logsize    = ptm->tmHeight - ptm->tmInternalLeading;
		long pointsize  = MulDiv(logsize, 72, GetDeviceCaps(hdc, LOGPIXELSY));

		for(i = 0; i < cursize; i++)
			if(currentsizes[i] == pointsize) return 1;

		printf("%d ", pointsize);
		
		currentsizes[cursize] = pointsize;
		if(++cursize == 200) return 0;
		
		return 1;	
	}
	else
	{
		for(i = 0; i < (sizeof(truetypesize) / sizeof(truetypesize[0])); i++)
		{
			printf("%d ", truetypesize[i]);
		}

		return 0;
	}

}


int EnumFontSizes(char *fontname)
{
	LOGFONT logfont;

	ZeroMemory(&logfont, sizeof logfont);

	logfont.lfHeight = 0;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	lstrcpy(logfont.lfFaceName, fontname);

	EnumFontFamiliesEx(hdc, &logfont, (FONTENUMPROC)FontSizesProc, 0, 0);

	return 0;
}

int CALLBACK FontNameProc(
	ENUMLOGFONTEX    *lpelfe,   /* pointer to logical-font data */
	NEWTEXTMETRICEX  *lpntme,   /* pointer to physical-font data */
	int              FontType,  /* type of font */
	LPARAM			 lParam     /* a combo box HWND */
	)
{
	int i;

	if(lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH)
	{
		/* Make sure the fonts are only added once */
		for(i = 0; i < curfont; i++)
		{
			if(lstrcmp(currentfonts[i], (char *)lpelfe->elfFullName) == 0)
				return 1;
		}

		printf("%-16s: ", lpelfe->elfFullName);
		
		cursize = 0;
		EnumFontSizes((char *)lpelfe->elfFullName);
		printf("\n");

		lstrcpy(currentfonts[curfont], (char *)lpelfe->elfFullName);
		
		if(++curfont == 200)
			return 0;
	
	}

	return 1;
}

int EnumFixedFonts(void)
{
	LOGFONT logfont;

	ZeroMemory(&logfont, sizeof logfont);

	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	lstrcpy(logfont.lfFaceName, "\0");

	hdc = GetDC(0);

	EnumFontFamiliesEx(hdc, &logfont, (FONTENUMPROC)FontNameProc, 0, 0);

	ReleaseDC(0, hdc);

	return 0;
}

int main(void)
{
	EnumFixedFonts();
	return 0;
}