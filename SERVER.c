#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#define PROTOPORT 27015
#define BUFFERSIZE 512

void ErrorHandler(char *errorMessage) {
printf ("%s", errorMessage);
}

void ClearWinSock() {
#if defined WIN32
WSACleanup();
#endif
}

int main(int argc, char *argv[]) {

	int port;
	if (argc > 1) {
	port = atoi(argv[1]); // if argument specified convert argument to binary
	}
	else
	port = PROTOPORT; // use default port number
	if (port < 0) {
	printf("bad port number %s \n", argv[1]);
	return 0;
	}

#if defined WIN32

WSADATA wsaData; // initialize Winsock

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
return 0;
}
#endif

//CREAZIONE DELLA SOCKET
int Mysocket;
char accettazione[]= "Connessione avvenuta";
Mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

if (Mysocket < 0) {
ErrorHandler("socket creation failed.\n");
return -1;
}

struct sockaddr_in {
short sin_family;
u_short sin_port;
struct in_addr sin_addr;
char sin_zero[8];
};


//ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
struct sockaddr_in sad;
memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
sad.sin_family = AF_INET;
sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
sad.sin_port = htons( 27015 ); // converts values between the host and network byte order. Specifically, htons() converts 16-bit quantities from host byte order to network byte order. //


if (bind(Mysocket, (struct sockaddr*) &sad, sizeof(sad)) < 0) { 
      printf ("bind() failed\n");
       closesocket(Mysocket);
       return -1;
} //The function bind returns 0 in case of success, otherwise -1//

//SETTAGGIO DELLA SOCKET ALL'ASCOLTO
int qlen = 1;
if (listen (Mysocket, qlen) < 0) {
ErrorHandler("listen() failed.\n");
closesocket(Mysocket);
return -1;
}

//ACCETTARE UNA NUOVA CONNESSIONE
/* Main server loop - accept and handle requests */
struct sockaddr_in cad; //structure for the client address
int MyCSocket; //socket descriptor for the client
int clientLen; //the size of the client address
printf("Waiting for a client to connect...\n");
while (1) {
clientLen = sizeof(cad); //set the size of the client address
if ( (MyCSocket=accept(Mysocket, (struct sockaddr *)&cad, &clientLen)) < 0) {
ErrorHandler("accept() failed.\n");

//CHIUSURA DELLA CONNESSIONE
closesocket(Mysocket);
ClearWinSock();
return -1; 
}

//5) Il server invia al client la stringa "connessione avvenuta".//
 if (send(MyCSocket, accettazione, BUFFERSIZE,0)<=0)
 		{printf("messaggio non inviato");
		}

int flag; //Indicatore che segnala il verificarsi di una condizione particolare.//
	
//4) Stabilita la connessione, il server visualizza sullo std output un messaggio contenente il l’ip del client con cui ha stabilito la connessione. //
printf("Connessione stabilita con il client %s\n", inet_ntoa(cad.sin_addr));

//Ciclo che regola l'invio di stringhe dal server al client.//
do{
   char A[BUFFERSIZE]={0}; //prima stringa//
    char B[BUFFERSIZE]={0}; //seconda stringa//
	char QUIT[]="quit"; //possibile stringa concatenata che comporta la chiusura della connessione e la terminazione del processo.//
	char C[BUFFERSIZE]="";//stringa concatenata A+B//
	char BYE[]="bye";//stringa inviata dal server in caso di ricezione della stringa "quit".//

	flag=0;//inizializzazione flag.//

		//Controllo ricezione stringa 1.//
		if(recv(MyCSocket, A, BUFFERSIZE, 0) <= 0) {
			puts("Messaggio 1 non ricevuto");
		}

		//Controllo ricezione stringa 2.//
		if(recv(MyCSocket, B, BUFFERSIZE, 0) <= 0) {
			puts("Messaggio 2 non ricevuto");
		}	
			
		 strcmp(A,QUIT);//Compara la prima stringa con la parola "quit".//
		 strcmp(B,QUIT);//Compara la seconda stringa con la parola "quit".//
		 
		 if((strcmp(A,QUIT)==0)||(strcmp(B,QUIT)==0))
		 { if (send (MyCSocket, BYE, BUFFERSIZE,0)<=0)
 		   {printf("messaggio non inviato");
		   }//Controllo invio stringa "bye".//
		   flag=1;//Quando il flag raggiunge 1, raggiunge lo stato desiderato.//
		 }//Se dal confronto delle stringhe otteniamo 0, significa che esse sono uguali.//
		 else
		 strcat(A,B);//La funzione strcat accoda il contenuto della stringa B alla fine della stringa A.//
		 strncat(C,A,BUFFERSIZE);//La funzione strncat trasferisce il contenuto della stringa A che contiene la concatenazione di AB in C.//
	
	//Controllo invio stringa C.//
		if(send(MyCSocket, C, BUFFERSIZE-1, 0) <= 0) {
		puts("Messaggio non inviato");
	    }
}while(flag!=1);
}//end of the while loop.//
}//end main.//
