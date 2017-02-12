#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define PORTNUMBER 8888

struct serverParm {
           int connectionDesc;
       };

void *serverThread(void *parmPtr) 
{
#define PARMPTR ((struct serverParm *) parmPtr)
    int recievedMsgLen;
    char messageBuf[1025];

    /* Server thread code to deal with message processing */
    printf("DEBUG: connection made, connectionDesc=%d\n",
            PARMPTR->connectionDesc);
    if (PARMPTR->connectionDesc < 0) {
        printf("Accept failed\n");
        return(0);    /* Exit thread */
    }
    
    /* Receive messages from sender... */
    while ((recievedMsgLen=
            read(PARMPTR->connectionDesc,messageBuf,sizeof(messageBuf)-1)) > 0) 
    {
        recievedMsgLen[messageBuf] = '\0';
        printf("%s\n",messageBuf);
        /*
          my code begins here
        */
        
          char news[101];
         messageBuf[strlen(messageBuf)]='+' ;
       int i;
       int counter=0;
  for(i=0;i<strlen(messageBuf); i++)
  {
    if(messageBuf[i]==' ')
    continue;
    else
    {
    news[counter] = messageBuf[i];
    counter++;
    }
  }
  
   news[counter]='\0';
  
   int stack[100]={0};
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
    
    //this is for printing stack trails
    
    printf("stack is: \n");
    for(int i=0;i<=top-2;i++)
    printf("%d\n",stack[i]);
    
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
        
        char send_message[100];
        sprintf(send_message, "%f", ans);  
        ans=0;
        top=0;
        memset(messageBuf, 0, 255);
        /*
           my code ends here
        */
         write(PARMPTR->connectionDesc , send_message , strlen(send_message)); 
     /*   if (write(PARMPTR->connectionDesc,send_message,strlen(send_message)) < 0) {
               perror("Server: write error");
               return(0);
           }
           else
           {
           printf("ans send to the client\n");
           }*/
    }
    close(PARMPTR->connectionDesc);  /* Avoid descriptor leaks */
    free(PARMPTR);                   /* And memory leaks */
    return(0);                       /* Exit thread */
}

int main () {
    int listenDesc;
    struct sockaddr_in myAddr;
    struct serverParm *parmPtr;
    int connectionDesc;
    pthread_t threadID;

    /* For testing purposes, make sure process will terminate eventually */
    alarm(1200);  /* Terminate in 120 seconds */

    /* Create socket from which to read */
    if ((listenDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("open error on socket");
        exit(1);
    }

    /* Create "name" of socket */
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = INADDR_ANY;
    myAddr.sin_port = htons(PORTNUMBER);
        
    if (bind(listenDesc, (struct sockaddr *) &myAddr, sizeof(myAddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    /* Start accepting connections.... */
    /* Up to 5 requests for connections can be queued... */
    listen(listenDesc,5);

    while (1) /* Do forever */ {
        /* Wait for a client connection */
        connectionDesc = accept(listenDesc, NULL, NULL);

        /* Create a thread to actually handle this client */
        parmPtr = (struct serverParm *)malloc(sizeof(struct serverParm));
        parmPtr->connectionDesc = connectionDesc;
        if (pthread_create(&threadID, NULL, serverThread, (void *)parmPtr) 
              != 0) {
            perror("Thread create error");
            close(connectionDesc);
            close(listenDesc);
            exit(1);
        }

        printf("Parent ready for another connection\n");
    }
return 0;
}
