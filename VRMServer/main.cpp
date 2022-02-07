// ������ �������� TCP-���-�������

#include "INIReader.h"
#include "TextFile.h"

using namespace std;

CINIFile c_sINIFile; 
CTextFile c_sTXTFile;

#define MY_PORT 666 // ����, ������� ������� ������ 666

// ������ ��� ������ ���������� �������� �������������
#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); \
         else printf("No User on line\n");

// �������� �������, ������������� �������������� �������������
 DWORD WINAPI SexToClient(LPVOID client_socket);
 float cclient;

// ���������� ���������� - ���������� �������� �������������
 int nclients = 0;

int main(int argc, char* argv[])
 {
     char buff[1024]; // ����� ��� ��������� ����

     printf("TCP SERVER DEMO\n");
     // ��� 1 - ������������� ���������� �������
     // �.�. ������������ �������� ���������� �� ������������
     // �� ���������� ��������� �� ������� �����, ������������� � ���������
     // �� ��������� WSADATA.
     // ����� ����� ��������� ���������� ���� ����������, ������, �����
     // ������ ���� �� ����� ������������ �������� (��������� WSADATA
     // �������� 400 ����)
     if (WSAStartup(0x0202, (WSADATA *)&buff[0]))
     {
         // ������!
         printf("Error WSAStartup %d\n", WSAGetLastError());
         return -1;
     }

     // ��� 2 - �������� ������
     SOCKET mysocket;
     // AF_INET - ����� ���������
     // SOCK_STREAM - ��������� ����� (� ���������� ����������)
     // 0 - �� ��������� ���������� TCP ��������
     if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
     {
         // ������!
         printf("Error socket %d\n", WSAGetLastError());
         WSACleanup(); // �������������� ���������� Winsock
         return -1;
     }

     // ��� 3 - ���������� ������ � ��������� �������
     sockaddr_in local_addr;
     local_addr.sin_family = AF_INET;
     local_addr.sin_port = htons(MY_PORT); // �� �������� � ������� �������!!!
     local_addr.sin_addr.s_addr = 0; // ������ ��������� �����������
                                     // �� ��� ���� IP-������

     // �������� bind ��� ����������
     if (bind(mysocket, (sockaddr *)&local_addr, sizeof(local_addr)))
     {
         // ������
         printf("Error bind %d\n", WSAGetLastError());
         closesocket(mysocket); // ��������� �����!
         WSACleanup();
         return -1;
     }

     // ��� 4 - �������� �����������
     // ������ ������� - 0x100
     if (listen(mysocket, 0x100))
     {
         // ������
         printf("Error listen %d\n", WSAGetLastError());
         closesocket(mysocket);
         WSACleanup();
         return -1;
     }

     printf("�������� �����������...\n");

     // ��� 5 - ��������� ��������� �� �������
     SOCKET client_socket; // ����� ��� �������
     sockaddr_in client_addr; // ����� ������� (����������� ��������)

     // ������� accept ���������� �������� ������ ���������
     int client_addr_size = sizeof(client_addr);

     // ���� ���������� �������� �� ����������� �� �������
     while ((client_socket = accept(mysocket, (sockaddr *)&client_addr, \
            &client_addr_size)))
     {
         nclients++; // ����������� ������� �������������� ��������

         // �������� �������� ��� �����
         HOSTENT *hst;
         hst = gethostbyaddr((char *)&client_addr. sin_addr.s_addr, 4, AF_INET);

         // ����� �������� � �������
         printf("+%s [%s] new connect!\n",
         (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
         PRINTNUSERS

         // ����� ������ ������ ��� ����������� �������
         // ��, ��� ����� ������������� ������������ _beginthreadex
         // ��, ��������� ������� ������� ������� ����������� �� ����������
         // ����� �� ������, ����� �������� � CreateThread
         DWORD thID;
         CreateThread(NULL, NULL, SexToClient, &client_socket, NULL, &thID);
     }
     return 0;
 }

// ��� ������� ��������� � ��������� ������
// � ���������� ���������� ��������������� ������� ���������� �� ���������
 DWORD WINAPI SexToClient(LPVOID client_socket)
 {
     SOCKET my_sock;
	 char destbuff[256];
	 char email[256];
	 char upassword[256];
	 char ubdate[256];
	 char usercmd[256];
	 char sznickbuff[256];
	 char zsupasswbuff[256];
	 char szunick[256];
	 char zsupassw[256];
	 char szdefuser[256];
	 char szdefpassw[256];
	 char uprofile[256];
     my_sock = ((SOCKET *)client_socket)[0];
     char buff[20 * 1024];
     // ���� ���-�������: ����� ������ �� ������� � ����������� �� �������
     int bytes_recv;
     while ((bytes_recv = recv(my_sock, buff,sizeof(buff), 0)) &&
     bytes_recv != SOCKET_ERROR)
	 {
	 if(strcmp(buff,"-register") == 0)
	 {
	 size_t nprsize = strlen(buff)+1;
	 c_sINIFile.OpenINIFile("./Users.ini");
	 sprintf(destbuff,"Profile_%s",&buff[nprsize]);
	 c_sINIFile.WriteChar(destbuff,"UserNick",&buff[nprsize]);
     size_t nprsize4 = nprsize + strlen(&buff[nprsize])+3;
	 sscanf(&buff[nprsize4],"%s",email);
	 c_sINIFile.WriteChar(destbuff,"UserEMail",email);
	 size_t nprsize2 = nprsize4+(strlen(email)+1);
	 sscanf(&buff[nprsize2],"%s",upassword);
	 c_sINIFile.WriteChar(destbuff,"UserPassword",upassword);
	 size_t nprsize3 = nprsize2+(strlen(&buff[nprsize2])+1);
	 sscanf(&buff[nprsize3],"%s",ubdate);
	 c_sINIFile.WriteChar(destbuff,"UserBirthDate",ubdate);
	 printf("User %s was registered",&buff[nprsize]);
	 ZeroMemory(buff,sizeof(buff));
	 }
	 if(strcmp(buff,"-signup")==0)
	 {
		char error[256];
		char succeeded[256];
		char login[256];
		strcpy(error,"-suerror");
		strcpy(succeeded,"-susuccess");
		strcpy(login,"-login");
		c_sINIFile.OpenINIFile("./Users.ini");
		printf("User Sign Up!\n");
        strcpy(szdefuser,"defaultuser");
		strcpy(szdefpassw,"0000");
		size_t nprsize = strlen(buff)+1;
		sprintf(sznickbuff,"%s",&buff[nprsize]);
		size_t nprsize2 = nprsize + strlen(sznickbuff)+1;
		sprintf(zsupasswbuff,"%s",&buff[nprsize2]);
		sprintf(uprofile,"Profile_%s",sznickbuff);
		strcpy(szunick,c_sINIFile.GetChar(uprofile,"UserNick",szdefuser));
		strcpy(zsupassw,c_sINIFile.GetChar(uprofile,"UserPassword",szdefpassw));
		if(strcmp(sznickbuff,szunick)!=0 || strcmp(zsupasswbuff,zsupassw)!=0)
		{
          printf("user connection error");
	      send(my_sock,error,strlen(error)+1,0);
		}
		else
		{
		  send(my_sock,succeeded,strlen(succeeded)+1,0);
		  printf("user was registered (nick): ");
		  printf(szunick);
		  ZeroMemory(buff,sizeof(buff));
		  recv(my_sock,buff,sizeof(buff),0);
		  if(strcmp(buff,"-login")==0)
		  {
           send(my_sock,szunick,strlen(szunick)+1,0);
		  }
		}
	 }
	 if(strcmp(buff,"-user_search")==0)
	 {
	 }
     if(strcmp(buff,"-user_search_all")==0)
	 {
 char sznickbuff[256];
 ifstream f;
 f.open("./Users.ini");
 int nstr=1;
 while(!f.eof())
 {
	 nstr+5;
	 strcpy(sznickbuff,c_sTXTFile.ReadTXTFile("./Users.ini",6,"%s"));
 }
	 }
	 if(my_sock != 0)
	 {
	 }
	 else
	 {
     char disc[256];
	 strcpy(disc,"-disconnect");
	 send(my_sock,disc,strlen(disc)+1,0);
	 }
	 }
	 

     // ���� �� �����, �� ��������� ����� �� ����� �� �������
     // ���������� �������� recv ������ - ���������� � �������� ���������
     nclients--; // ��������� ������� �������� ��������
     printf("\n-disconnect\n"); PRINTNUSERS

     // ��������� �����
     closesocket(my_sock);
     return 0;
 }