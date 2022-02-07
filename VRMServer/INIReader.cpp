#include "INIReader.h"

CINIFile::CINIFile()
{
}

CINIFile::~CINIFile()
{
}

void CINIFile::OpenINIFile(char * filename)
{
strcpy(_filename,filename);
}

int CINIFile::GetInt(char * szappname,char * szkeyname,int ndefault )
{
	nrvalue = GetPrivateProfileInt(szappname,szkeyname,ndefault,_filename);
	return nrvalue;
}

char * CINIFile::GetChar(char * szappname,char * szkeyname,char * szdefault )
{
	GetPrivateProfileString(szappname,szkeyname,szdefault,szrstring,sizeof(szrstring),_filename);
	return szrstring;
}

void CINIFile::WriteChar(char * szappname,char * szkeyname,char * string)
{
	WritePrivateProfileString(szappname,szkeyname,string,_filename);
}