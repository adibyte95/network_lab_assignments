/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int sockfd; /* socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char client_message[BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /* 
   * check command line arguments 
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /*
     * recvfrom: receive a UDP datagram from a client
     */
    bzero(client_message, BUFSIZE);
    n = recvfrom(sockfd, client_message, BUFSIZE, 0,
		 (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");

    /* 
     * gethostbyaddr: determine who sent the datagram
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server received datagram from %s (%s)\n", 
	   hostp->h_name, hostaddrp);
	   
	   /*
	      my code begins here
	   */
	        
	        // printf("%s\n",client_message);
         char news[101];
         client_message[strlen(client_message)]='+' ;
        int i;
        int counter=0;
  for(i=0;i<strlen(client_message); i++)
  {
    if(client_message[i]==' ')
    continue;
    else
    {
    news[counter] = client_message[i];
    counter++;
    }
  }
  news[counter]='\0';
   //printf("new is %s\n",news);
   int stack[1000]={0};
   int top=0;
   char value[101];
   counter=0;
   int insert;
   for(int i=0;i<strlen(news); i++)
   {
      if(news[i]=='+') 
      {
         value[counter]='\0';
         counter=0;
         sscanf(value, "%d", &insert);
         stack[top]=insert;
         top++;
         stack[top]=-1;
         top++;
      }
      
      else if(news[i]=='-') 
      {
         value[counter]='\0';
         counter=0;
         sscanf(value, "%d", &insert);
         stack[top]=insert;
         top++;
         stack[top]=-2;
         top++;
      }
      
      else if(news[i]=='*') 
      {
         value[counter]='\0';
         counter=0;
         sscanf(value, "%d", &insert);
         stack[top]=insert;
         top++;
         stack[top]=-3;
         top++;
      }
      
      else if(news[i]=='/') 
      {
         value[counter]='\0';
         counter=0;
         sscanf(value, "%d", &insert);
         stack[top]=insert;
         top++;
         stack[top]=-4;
         top++;
      }
      else
      {
        value[counter]=news[i];
        counter++;
      }
   }  
     /*
    printf("stack is: \n");
    for(int i=0;i<top-1;i++)
    printf("%d\n",stack[i]);
    printf("a[o] is : %d\n",stack[0]);
    */
    i=0;
    float ans=0;
    ans=stack[i];
    while(i<=top-2)
    {
      i++;
      int indicator = stack[i];
      if(indicator==-1)
      {
        i++;
        ans +=stack[i];
      }
      else if(indicator==-2)
      {
        i++;
        ans -=stack[i];
      }
      else if(indicator==-3)
      {
        i++;
        ans *=stack[i];
      }
      else if(indicator==-4)
      {
        i++;
        ans /=stack[i];
      } 
    }
    memset(client_message, 0, 255);
       char send_message[100];
       top=0;
       sprintf(send_message, "%f", ans);
       printf("replied\n");
	   printf("server is going to send %s\n",send_message);
	   
	   /*
	       my code ends here
	   */
   // printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);
    
    
     /* 
     * sendto: echo the input back to the client 
     */
     
     
    n = sendto(sockfd, send_message, strlen(send_message), 0, 
	       (struct sockaddr *) &clientaddr, clientlen);
    if (n < 0) 
      error("ERROR in sendto");
  }
}
