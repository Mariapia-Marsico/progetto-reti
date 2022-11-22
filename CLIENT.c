#include <string.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#define BUFFERSIZE 512

void ErrorHandler(char *errorMessage) {
printf("%s",errorMessage);
}
void ClearWinSock() {
#if defined WIN32
WSACleanup();
#endif
}

int main(void) {

#if defined WIN32

WSADATA wsaData;

typedef struct wsaData {
WORD wVersion;
WORD wHighVersion;
char szDescription[WSADESCRIPTION_LEN+1];
char szSystemStatus[WSASYS_STATUS_LEN+1];
unsigned short iMaxSockets;
unsigned short iMaxUdpDg;
char FAR* lpVendorInfo;
} WSADATA;

int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
if (iResult != 0){
printf("error at WSASturtup\n");
return -1;
}
#endif


//CREAZIONE DELLA SOCKET
int MyCSocket;
char accettazione[BUFFERSIZE];//Stringa per connessione al server avvenuta.//
char risposta[BUFFERSIZE];//Stringa di risposta da parte del server che può assumere valore "bye" oppure C.//
char BYE[]="bye";

MyCSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

if (MyCSocket < 0) {
ErrorHandler("socket creation failed.\n");
closesocket(MyCSocket);
ClearWinSock();
return -1;
}

//COSTRUZIONE DELL'INDIRIZZO DEL SERVER
char s_name[BUFFERSIZE];//Indirizzo IP.//
printf("Inserire l'ip dell'host\n");
scanf("%s", s_name);
int port;//Numero di porta da utilizzare per contattare il server.//
printf("Inserire il numero di porta\n");
scanf("%d", &port);

struct sockaddr_in sad;
memset(&sad, 0, sizeof(sad));
sad.sin_family = AF_INET;
sad.sin_addr.s_addr = inet_addr(s_name);//IP del server.//
sad.sin_port = htons(port);//Porta del server.//

//CONNESSIONE AL SERVER
if (connect(MyCSocket,(struct sockaddr *) &sad, sizeof(sad)) < 0){
ErrorHandler( "Failed to connect.\n" );
closesocket(MyCSocket);
ClearWinSock();
return -1;
}

//Controllo ricezione accettazione.//
if (recv(MyCSocket,accettazione,BUFFERSIZE,0)<=0) {
	printf("non e' stato ricevuto nulla");
	
}
else puts(accettazione);//Il client riceve la stringa "Connessione avvenuta".//
int stringlen;//Determina la lunghezza della stringa.//


//Ciclo che controlla l'invio delle stringhe al server .//
do{
char A[BUFFERSIZE]={0}; 
char B[BUFFERSIZE]={0};
printf ("inserisci prima stringa:\n");
scanf ("%s", A);
stringlen = strlen(A);//Controllo lunghezza.//
	if (send(MyCSocket, A, stringlen, 0) != stringlen) {
	printf("client-send() sent a different number of bytes than expected");
	    closesocket(MyCSocket);
	    ClearWinSock();
	    system ("pause");
	    return 0;
	}
	printf("Inserisci seconda stringa:\n");
	scanf("%s", B);
	stringlen = strlen(B);//Controllo lunghezza.//
	if(send(MyCSocket,B,stringlen,0)!= stringlen){
		printf("client-send() sent a different number of bytes than expected");
	    closesocket(MyCSocket);
	    ClearWinSock();
	    system ("pause");
	    return 0;
	}
	
//RICEZIONE STRINGA ELABORATA DAL SERVER
if (recv(MyCSocket, risposta, BUFFERSIZE-1, 0) <= 0) 
puts("Non e' stato ricevuto nulla");
			else 
			{
				printf("Il server manda %s\n", risposta);
			}
}while(strcmp(risposta,BYE)!=0); 

if(strcmp(risposta,BYE)==0){
		closesocket(MyCSocket);
		printf("Connessione chiusa");
		}//Se il confronto tra risposta e bye restituisce 0, la connessione viene chiusa.//	
}//End main.//
