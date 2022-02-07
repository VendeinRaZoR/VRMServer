// Пример простого TCP-эхо-сервера

#include "INIReader.h"
#include "TextFile.h"

using namespace std;

CINIFile c_sINIFile; 
CTextFile c_sTXTFile;

#define MY_PORT 666 // Порт, который слушает сервер 666

// макрос для печати количества активных пользователей
#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); \
         else printf("No User on line\n");

// прототип функции, обслуживающий подключившихся пользователей
 DWORD WINAPI SexToClient(LPVOID client_socket);
 float cclient;

// глобальная переменная - количество активных пользователей
 int nclients = 0;

int main(int argc, char* argv[])
 {
     char buff[1024]; // Буфер для различных нужд

     printf("TCP SERVER DEMO\n");
     // Шаг 1 - Инициализация Библиотеки Сокетов
     // т.к. возвращенная функцией информация не используется
     // ей передается указатель на рабочий буфер, преобразуемый к указателю
     // на структуру WSADATA.
     // Такой прием позволяет сэкономить одну переменную, однако, буфер
     // должен быть не менее полкилобайта размером (структура WSADATA
     // занимает 400 байт)
     if (WSAStartup(0x0202, (WSADATA *)&buff[0]))
     {
         // Ошибка!
         printf("Error WSAStartup %d\n", WSAGetLastError());
         return -1;
     }

     // Шаг 2 - создание сокета
     SOCKET mysocket;
     // AF_INET - сокет Интернета
     // SOCK_STREAM - потоковый сокет (с установкой соединения)
     // 0 - по умолчанию выбирается TCP протокол
     if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
     {
         // Ошибка!
         printf("Error socket %d\n", WSAGetLastError());
         WSACleanup(); // Деиницилизация библиотеки Winsock
         return -1;
     }

     // Шаг 3 - связывание сокета с локальным адресом
     sockaddr_in local_addr;
     local_addr.sin_family = AF_INET;
     local_addr.sin_port = htons(MY_PORT); // не забываем о сетевом порядке!!!
     local_addr.sin_addr.s_addr = 0; // сервер принимает подключения
                                     // на все свои IP-адреса

     // вызываем bind для связывания
     if (bind(mysocket, (sockaddr *)&local_addr, sizeof(local_addr)))
     {
         // Ошибка
         printf("Error bind %d\n", WSAGetLastError());
         closesocket(mysocket); // закрываем сокет!
         WSACleanup();
         return -1;
     }

     // Шаг 4 - ожидание подключений
     // размер очереди - 0x100
     if (listen(mysocket, 0x100))
     {
         // Ошибка
         printf("Error listen %d\n", WSAGetLastError());
         closesocket(mysocket);
         WSACleanup();
         return -1;
     }

     printf("Ожидание подключений...\n");

     // Шаг 5 - извлекаем сообщение из очереди
     SOCKET client_socket; // сокет для клиента
     sockaddr_in client_addr; // адрес клиента (заполняется системой)

     // функции accept необходимо передать размер структуры
     int client_addr_size = sizeof(client_addr);

     // цикл извлечения запросов на подключение из очереди
     while ((client_socket = accept(mysocket, (sockaddr *)&client_addr, \
            &client_addr_size)))
     {
         nclients++; // увеличиваем счетчик подключившихся клиентов

         // пытаемся получить имя хоста
         HOSTENT *hst;
         hst = gethostbyaddr((char *)&client_addr. sin_addr.s_addr, 4, AF_INET);

         // вывод сведений о клиенте
         printf("+%s [%s] new connect!\n",
         (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
         PRINTNUSERS

         // Вызов нового потока для обслужвания клиента
         // Да, для этого рекомендуется использовать _beginthreadex
         // но, поскольку никаких вызовов функций стандартной Си библиотеки
         // поток не делает, можно обойтись и CreateThread
         DWORD thID;
         CreateThread(NULL, NULL, SexToClient, &client_socket, NULL, &thID);
     }
     return 0;
 }

// Эта функция создается в отдельном потоке
// и обсуживает очередного подключившегося клиента независимо от остальных
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
     // цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
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
	 

     // если мы здесь, то произошел выход из цикла по причине
     // возращения функцией recv ошибки - соединение с клиентом разорвано
     nclients--; // уменьшаем счетчик активных клиентов
     printf("\n-disconnect\n"); PRINTNUSERS

     // закрываем сокет
     closesocket(my_sock);
     return 0;
 }