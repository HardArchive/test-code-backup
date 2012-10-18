// html to mhtml.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "html to mhtml.h"

list<string> lista;
string path;

const string BOUNDARYDECL="----=_NextPart_000_0076_01C29953.BE473C30";
const string BOUNDARY="--"+BOUNDARYDECL;

string locationFromPath(const char *file)
{
	char *p= NULL;
	char *f=strdup(file);

	for(p=f; p<strlen(f)+f; p++)
		if(*p=='\\')
			*p='/';

	p=f;
	if(*p=='/')
		p++;

	string ret="file:///"+string(p);
	free(f);
	return ret;
}

string processFile(const char *file)
{
	HANDLE hFile=CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
		return "";

	LARGE_INTEGER size;
	GetFileSizeEx(hFile, &size);
	char *buffer=new char[size.LowPart];
	int read;
	ReadFile(hFile, buffer, size.LowPart, (LPDWORD)&read, NULL);
	CloseHandle(hFile);

	int len=Base64EncodeGetRequiredLength(size.LowPart);
	char *b64=new char[len];
	Base64Encode((const BYTE *)buffer, size.LowPart, b64, &len);
	b64[len]=0;
	delete[] buffer;

	string ret=BOUNDARY+"\r\nContent-Type: application/octet-stream;\r\nContent-Transfer-Encoding: base64\r\nContent-Location: "
		+locationFromPath(file)+"\r\n\r\n"+string(b64)+"\r\n\r\n";
	delete[] b64;

	return ret;
}

string processText(const char *file)
{
	HANDLE hFile=CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
		return "";

	LARGE_INTEGER size;
	GetFileSizeEx(hFile, &size);
	char *buffer=new char[size.LowPart];
	int read;
	ReadFile(hFile, buffer, size.LowPart, (LPDWORD)&read, NULL);
	CloseHandle(hFile);

	string ret=BOUNDARY+"\r\nContent-Type: application/octet-stream;\r\nContent-Transfer-Encoding: text/plain\r\nContent-Location: "
		+locationFromPath(file)+"\r\n\r\n"+string(buffer,size.LowPart)+"\r\n\r\n";
	delete[] buffer;

	return ret;
}

inline string CONVERT(char p)
{
	string ret;

	if(p=='\n')
		ret="=20";
	else if(p=='=')
		ret="=3D";
	else
		ret=p;

	return ret;
}
#define EATNONALPHA(p) while(!isalpha(*p)) p++
#define EATSPACE(p) while(isspace(*p)) p++
#define EATNONALPHACOPY(p) while(!isalpha(*p)) tmp+=CONVERT(*(p++))
#define EATSPACECOPY(p) while(isspace(*p)) tmp+=CONVERT(*(p++))

string processHTML(const char *file)
{
	HANDLE hFile=CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
		return "";

	LARGE_INTEGER size;
	GetFileSizeEx(hFile, &size);
	unsigned char *buffer=new unsigned char[size.LowPart];
	int read;
	ReadFile(hFile, buffer, size.LowPart, (LPDWORD)&read, NULL);
	CloseHandle(hFile);

	string tmp;
	tmp.reserve(3*size.LowPart);

	for(unsigned char *p=buffer; p<buffer+size.LowPart; p++)
		if(*p=='<')
		{
			if(p[1]=='/')
				goto __skipToNextTag;

			EATSPACECOPY(p);

			if(((p[1]=='a'||p[1]=='A')&&!isalpha(p[2]))||(strnicmp((char *)p+1,"link",4)==0&&!isalpha(p[5])))
			{
				//am gasit tagul!!!
				//acum caut href
				char *p1=StrStrI((char *)p,"href");
				if(!p1)
					goto __skipToNextTag;
				p1=StrStr((char *)p1,"=");
				p1++;
				EATSPACE(p1);
				char *p1end;
				if(*p1=='\"')
				{
					p1++;
					p1end=StrStr(p1,"\"");
				}
				else
					p1end=StrStr(p1," ");
				
				string pathThis(p1,p1end-p1);
				if(PathIsRelative(pathThis.c_str()))
					if(!PathIsURL(pathThis.c_str()))
						pathThis=path+"\\"+pathThis;
				if(PathFileExists(pathThis.c_str()))
				{
					// now just push everything until now unto tmp and add the new stuff at the end
					for(unsigned char *pk=p; pk<(unsigned char *)p1; pk++)
						tmp+=CONVERT(*pk);
					tmp+=locationFromPath(pathThis.c_str());
					p=(unsigned char *)p1end;

					for(list<string>::iterator it=lista.begin(); it!=lista.end(); it++)
						if(*it==pathThis.c_str())
							goto __skipToNextTag;
					lista.push_back(pathThis.c_str());
				}
			}
			else if((strnicmp((char *)p+1,"img",3)==0&&isspace(p[4]))||(strnicmp((char *)p+1,"frame",5)==0)&&isspace(p[6]))
			{
				//am gasit img sau frame
				//caut src
				char *p1=StrStrI((char *)p,"src");
				if(!p1)
					goto __skipToNextTag;
				p1=StrStr((char *)p1,"=");
				p1++;
				EATSPACE(p1);
				char *p1end;
				if(*p1=='\"')
				{
					p1++;
					p1end=StrStr(p1,"\"");
				}
				else
					p1end=StrStr(p1," ");
				
				string pathThis(p1,p1end-p1);
				if(PathIsRelative(pathThis.c_str()))
					if(!PathIsURL(pathThis.c_str()))
						pathThis=path+"\\"+pathThis;
				if(PathFileExists(pathThis.c_str()))
				{
					// now just push everything until now unto tmp and add the new stuff at the end
					for(unsigned char *pk=p; pk<(unsigned char *)p1; pk++)
						tmp+=CONVERT(*pk);
					tmp+=locationFromPath(pathThis.c_str());
					p=(unsigned char *)p1end;

					for(list<string>::iterator it=lista.begin(); it!=lista.end(); it++)
						if(*it==pathThis.c_str())
							goto __skipToNextTag;
					lista.push_back(pathThis.c_str());
				}
			}
			else if(strnicmp((char *)p+1,"body",4)==0&&isspace(p[5]))
			{
				//am gasit img
				//caut background
				char *p1=StrStrI((char *)p,"background");
				if(!p1)
					goto __skipToNextTag;
				p1=StrStr((char *)p1,"=");
				p1++;
				EATSPACE(p1);
				char *p1end;
				if(*p1=='\"')
				{
					p1++;
					p1end=StrStr(p1,"\"");
				}
				else
					p1end=StrStr(p1," ");
				
				string pathThis(p1,p1end-p1);
				if(PathIsRelative(pathThis.c_str()))
					if(!PathIsURL(pathThis.c_str()))
						pathThis=path+"\\"+pathThis;
				if(PathFileExists(pathThis.c_str()))
				{
					// now just push everything until now unto tmp and add the new stuff at the end
					for(unsigned char *pk=p; pk<(unsigned char *)p1; pk++)
						tmp+=CONVERT(*pk);
					tmp+=locationFromPath(pathThis.c_str());
					p=(unsigned char *)p1end;

					for(list<string>::iterator it=lista.begin(); it!=lista.end(); it++)
						if(*it==pathThis.c_str())
							goto __skipToNextTag;
					lista.push_back(pathThis.c_str());
				}
			}

__skipToNextTag:
			while(*p!='>')
				tmp+=CONVERT(*(p++));
			tmp+=CONVERT(*p);
		}
		else
			tmp+=CONVERT(*p);

	tmp=BOUNDARY+"\r\nContent-Type: text/html; \r\nContent-Transfer-Encoding: quoted-printable\r\nContent-Location: "
		+locationFromPath(file)+"\r\n\r\n"+tmp+"\r\n\r\n";
	
	delete[] buffer;

	return tmp;
}

string process(const char *file)
{
	if(StrStrI(file,".htm")||StrStrI(file,".shtm")||StrStrI(file,".phtm"))
		return processHTML(file);
	else if(StrStrI(file,".cpp")||StrStrI(file,".hpp")||StrStrI(file,".h")||StrStrI(file,".c")||StrStrI(file,".inl")||StrStrI(file,".txt"))
		return processText(file);
	else
		return processFile(file);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	OPENFILENAME ofn={0};
	char file[10240]="";
	ofn.hInstance=hInstance;
	ofn.lpstrFilter="HTML files\0*.htm;*.html;*.shtm;*.shtml;*.phtml;*.phtm\0All files\0*.*\0";
	ofn.lpstrFile=file;
	ofn.lpstrTitle="Choose the html file to be converted.";
	ofn.lStructSize=sizeof(ofn);
	ofn.nMaxFile=10240;
	ofn.Flags=OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_EXPLORER;
	if(!GetOpenFileName(&ofn))
		return 0;

	char *thePath=file, *f=file;
	SetCurrentDirectory(thePath);
	f+=strlen(f)+1;
	
	bool onlyone=false;
	if(!*f)
	{
		path=thePath;
		path.resize(path.find_last_of('\\'));
		onlyone=true;
	}
	else
		path=thePath;

	while(*f||onlyone)
	{
		string theFile;
		if(onlyone)
			theFile=thePath;
		else
			theFile=string(thePath)+"\\"+f;
		char *file1=strdup(theFile.c_str());
		PathRenameExtension(file1, ".mht");

		lista.clear();
		string out="MIME-version: 1.0\r\nContent-Type: multipart/related;\r\n\tboundary=\""+BOUNDARYDECL+"\";\r\n\ttype=\"text/html\"\r\nX-MimeOLE: Produced By Microsoft MimeOLE V6.00.2800.1106\r\n\r\nThis is a multi-part message in MIME format.\r\n\r\n";
		lista.push_back(theFile);
		for(list<string>::iterator it=lista.begin(); it!=lista.end(); it++)
			out+=process(it->c_str());

		HANDLE hFile=CreateFile(file1, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD written;
		WriteFile(hFile, out.c_str(), (DWORD)out.size(), &written, NULL);
		CloseHandle(hFile);

		free(file1);

		if(onlyone)
			break;

		f+=strlen(f)+1;
	}

	MessageBox(NULL, "All done.", NULL, MB_OK);

	return 0;
}
