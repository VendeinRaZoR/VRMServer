#include "IncludesBase.h"

class CINIFile
{
public:
CINIFile();
~CINIFile();
int GetInt(char * szappname,char * szkeyname,int ndefault );
char * GetChar(char * szappname,char * szkeyname,char * szdefault );
void OpenINIFile(char * filename);
void WriteChar(char * szappname,char * szkeyname,char * string);
private:
char _filename[256];
char  szrstring[256];
int nrvalue;
};