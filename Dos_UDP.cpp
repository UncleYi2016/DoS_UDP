// Client.cpp : Defines the entry point for the console application.
//
// 1. open the *.c in the Visual C++, then "rebuild all".
// 2. click "yes" to create a project workspace.
// 3. You need to -add the library 'ws2_32.lib' to your project 
//    (Project -> Properties -> Linker -> Input -> Additional Dependencies) 
// 4. recompile the source.


#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock.h>

#define DEFAULT_PORT	27015
#define _WINSOCK_DEPRECATED_NO_WARNINGS

char flush[100];

int main(int argc, char **argv) {

    char szBuff[200];
    char tt[4096] = { 'g' };
    int msg_len;
    int count = 0;
    //int addr_len;
    struct sockaddr_in server_addr;
    struct hostent *hp;
    SOCKET connect_sock;
    WSADATA wsaData;

    char			server_name[30];
    char            cmd[100];
    unsigned short	port = DEFAULT_PORT;
    unsigned int	addr;

    scanf("%s", &server_name);
    scanf("%d", &port);

    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
        // stderr: standard error are printed to the screen.
        fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
        //WSACleanup function terminates use of the Windows Sockets DLL. 
        WSACleanup();
        return -1;
    }

    if (isalpha(server_name[0]))
        hp = gethostbyname(server_name);
    else {
        addr = inet_addr(server_name);
        hp = gethostbyaddr((char*)&addr, 4, AF_INET);
    }

    if (hp == NULL)
    {
        fprintf(stderr, "Cannot resolve address: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    //copy the resolved information into the sockaddr_in structure
    memset(&server_addr, 0, sizeof(server_addr));
    memcpy(&(server_addr.sin_addr), hp->h_addr, hp->h_length);
    server_addr.sin_family = hp->h_addrtype;
    server_addr.sin_port = htons(port);


    connect_sock = socket(AF_INET, SOCK_DGRAM, 0);	//TCp socket


    if (connect_sock == INVALID_SOCKET) {
        fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Client connecting to: %s\n", hp->h_name);

    if (connect(connect_sock, (struct sockaddr *)&server_addr, sizeof(server_addr))
        == SOCKET_ERROR) {
        fprintf(stderr, "connect() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    while (1) {
        scanf("%s", &cmd);
        msg_len = send(connect_sock, cmd, sizeof(cmd), 0);
        printf("send %d\n", ++count);
        if (msg_len == SOCKET_ERROR) {
            fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
            WSACleanup();
            return -1;
        }
        msg_len = recv(connect_sock, szBuff, sizeof(szBuff), 0);
        if (msg_len == SOCKET_ERROR) {
            fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
            WSACleanup();
            return -1;
        }
        printf("%s", szBuff);

    }
    
    closesocket(connect_sock);
    WSACleanup();
    system("Pause");
}
