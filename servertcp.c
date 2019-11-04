//codice server

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
#define PORTA 1234

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
		return 0;
	}
#endif

	//creazione socket
	int descrittoresocket;
	if((descrittoresocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
		printf("errore socket\n");
		return 0;
	}else printf("socket()\n");

	//struct
	struct sockaddr_in server;

	server.sin_family=AF_INET;
	server.sin_port=htons(PORTA);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");

	//bind
	if(bind(descrittoresocket,(struct sockaddr *)&server,sizeof(server))<0){
		printf("errore bind\n");
		closesocket (descrittoresocket);
		clearwinsock();
		return 0;
	}else printf("bind()\n");

	for(;;){
		//listen
		if(listen(descrittoresocket,1)<0){
			printf("errore listen\n");
			closesocket (descrittoresocket);
			clearwinsock();
			return 0;
		}else printf("listen()\n");

		//accept
		int clientsock;
		struct sockaddr_in remoto;
		int clientlen=sizeof(remoto);

		if((clientsock=accept(descrittoresocket,(struct sockaddr *)&remoto,&clientlen)) < 0){
			printf("errore accept\n");
			closesocket (descrittoresocket);
			clearwinsock();
			return 0;

			//rivedere questo
		}else {	struct hostent *host;
				host = gethostbyaddr((char *)&remoto.sin_addr,4,AF_INET);
				char* nomeclient = host->h_name;
				printf("comunicazione accettata con l'host %s\n",nomeclient);

		}

		int i = 1;
		while(i != 0){
			struct{
				char prima[100];
				char seconda[100];
				char terza[100];
			}stringhe;

			//recv
			if(recv(clientsock,&stringhe,sizeof(stringhe),0) <= 0){
				printf("error recv\n");
				closesocket(clientsock);
				clearwinsock();
				return 0;
			}else printf("recv()\n");

			char s1[100];
			char s2[100];

			strcpy(s1,stringhe.prima);
			strcpy(s2,stringhe.seconda);

			char bye[4]="bye";
			char quit[5]="quit";
			int prima;
			int seconda;

			prima=strcmp(s1,quit);
			seconda=strcmp(s2,quit);

			if(prima == 0 || seconda == 0){
				strcpy(stringhe.terza,bye);
				send(clientsock,&stringhe,sizeof(stringhe),0);
				printf("send()\n");
				i = 0;
			}else {
				strcpy(stringhe.terza,stringhe.prima);
				strcat(stringhe.terza,stringhe.seconda);
				send(clientsock,&stringhe,sizeof(stringhe),0);
				printf("send()\n");
			}
		}//while
	}//for

	return 0;
}
