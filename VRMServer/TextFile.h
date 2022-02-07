#include "IncludesBase.h"

using namespace std;

class CTextFile
{
public:
CTextFile();
~CTextFile();
void WriteTXTFile(char * filename,char * writemsg);
char * ReadTXTFile(char * filename,int nstr,char * format);
void ClearTXTFile(char * filename);
private:
ofstream f;
FILE *file;
char textbuff[256];
char readmsg[256];
};