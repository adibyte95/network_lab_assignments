#include<stdio.h>
#include<sys/types.h>//socket
#include<sys/socket.h>//socket
#include<string.h>//memset
#include<stdlib.h>//sizeof
#include<netinet/in.h>//INADDR_ANY

#define PORT 8888
#define SERVER_IP "127.0.0.1"
#define MAXSZ 100
int main()
{
 int sockfd;//to create socket

 struct sockaddr_in serverAddress;//client will connect on this

 int n;
 char msg1[MAXSZ];
 char msg2[MAXSZ];

 //create socket
 sockfd=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=INADDR_ANY;
 serverAddress.sin_port=htons(PORT);

 //client  connect to server on port
 connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
 //send to sever and receive from server
 while(1)
 {
  printf("\nEnter message to send to server:\n");
  scanf ("%[^\n]%*c", msg1);
  if(msg1[0]=='-' && msg1[1]=='1')
   break;

  n=strlen(msg1)+1;
  send(sockfd,msg1,n,0);

  n=recv(sockfd,msg2,MAXSZ,0);

  printf("Receive message from  server::%s\n",msg2);
 }

 return 0;
}
