#include "TextFile.h"

CTextFile::CTextFile()
{
}

CTextFile::~CTextFile()
{
}

void CTextFile::WriteTXTFile(char * filename,char * writemsg)
{
	f.open(filename,ios::app);
	f << writemsg << "\n";
	f.close();
}

char * CTextFile::ReadTXTFile(char * filename,int nstr,char * format)
{
file = fopen(filename,"r");
if(!file)
{return 0;}
for(int i = 0;i<nstr;i++)
{fgets(textbuff , sizeof(textbuff), file)[i];}
sscanf(textbuff,format,readmsg);
fclose(file);
ZeroMemory(file,sizeof(file));
return readmsg;
}

void CTextFile::ClearTXTFile(char * filename)
{
	f.open(filename);
	f.close();
}