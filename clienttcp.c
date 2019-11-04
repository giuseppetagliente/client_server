//codice client

#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void clearwinsock(){
#if defined WIN32
	WSACleanup();
#endif
}

int main(){

#if defined WIN32
	WSADATA wsadata;
	int iresult=WSAStartup(MAKEWORD(2,2),&wsadata);
	if(iresult != 0){
		printf("error\n");
		system("PAUSE");
		return 0;
	}
#endif

	//creazione socket
	int descrittoresocket;
	if((descrittoresocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0){
		printf("errore socket\n");
		return 0;
	}else printf("socket()\n");

	struct sockaddr_in client;

	char nomeserver[100];
	printf("dammi il nome del server\n");
	scanf("%s",nomeserver);

	int porta;
	printf("dammi la porta del server\n");
	scanf("%d",&porta);

	//rivedere questo
	struct hostent *server = gethostbyname(nomeserver);
	struct in_addr *ina = (struct in_addr *) server->h_addr_list[0];

	client.sin_family =AF_INET;
	client.sin_port = htons(porta);
	client.sin_addr.s_addr = inet_addr(inet_ntoa(*ina));

	//connect
	int clientlen = sizeof(client);
	if(connect(descrittoresocket,(struct sockaddr *)&client,&clientlen) < 0){
		printf("errore connect\n");
		closesocket(descrittoresocket);
		clearwinsock();
		return 0;
	}else printf("connect()\n");


	for(;;){
		struct{
			char prima[100];
			char seconda[100];
			char terza[100];
		}stringhe;

		printf("dammi la prima stringa\n");
		scanf("%s",stringhe.prima);

		printf("dammi la seconda stringa\n");
		scanf("%s",stringhe.seconda);

		//send
		if(send(descrittoresocket,&stringhe,sizeof(stringhe),0) <= 0){
			printf("errore send\n");
			closesocket(descrittoresocket);
			clearwinsock();
			return 0;
		}else printf("send()\n");


		if(recv(descrittoresocket,&stringhe,sizeof(stringhe),0) <= 0){
			printf("errore recv\n");
			closesocket(descrittoresocket);
			clearwinsock();
			return 0;
		}else printf("risposta dal server : %s\n",stringhe.terza);

		char bye[4] = "bye";
		int controllo;
		controllo = strcmp(stringhe.terza,"bye");
		if(controllo == 0){
			closesocket(descrittoresocket);
			clearwinsock();
			printf("close()\n");
			return 0;
			}
	}//for

	printf("close ()\n");
	closesocket(descrittoresocket);
	clearwinsock();
	return 0;
}
