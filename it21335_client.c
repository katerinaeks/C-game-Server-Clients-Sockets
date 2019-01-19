#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_SIZE 50

int isInteger(char *string);

int main(int argc,char *argv[])
{
    int sock_desc,server_port,connected=0,read_size;
    char *protocolresponse="quit";
    struct sockaddr_in serv_addr;
    char sbuff[MAX_SIZE],rbuff[MAX_SIZE];

    if (argc != 3)
    {
      printf("Usage:it21335_client <Server IP address> <Server Port number>\n");	
      exit(0);
    }
    else 
    {
      if (isInteger(argv[2]))
      server_port=atoi(argv[2]);
      else
      {
	printf("The server port number must be an integer number\n");
	exit(0);
      }
      
      if ((server_port < 0) || (server_port > 65535))
      {
	printf("The server port number must be an integer between 0-65535!!\n");
	exit(0);
      }
      
      if((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
         printf("Failed creating socket\n");

      bzero((char *) &serv_addr, sizeof (serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
      serv_addr.sin_port = htons(server_port);

      if (connect(sock_desc, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
         printf("Failed to connect to server\n");
         return -1;
       }
       else
       {
	 connected=1;
         printf("Connected successfully\n");
         printf("Write one of the following commands:(Rock | Paper | Scissors)\n");
         printf("Or type quit to disconnect from server!!\n");
         while(connected)
         {
	   printf("Give command:\n");
	   bzero(sbuff,MAX_SIZE);
	   scanf("%s",&sbuff);
	   write(sock_desc , sbuff , strlen(sbuff));
           if (strcmp(sbuff,protocolresponse) == 0)
	   {
	     printf("disconnected from server!!\n");
	     connected=0;
	     break;
	   } 
	   read_size=recv(sock_desc,rbuff,MAX_SIZE,0);
           if(read_size ==0)
            printf("Receive Error\n");
           else
	   {
	    rbuff[read_size]='\0';
	    printf("Server Response:%s\n",rbuff);
	   }
           bzero(rbuff,MAX_SIZE);//to clean buffer-->IMP otherwise previous word characters also came
          }
           close(sock_desc);
       }
     }
     
     return 0;
}

int isInteger(char *string)
{
        int i, stringLength = strlen(string),boolean;
	boolean=1;

        for(i = 0; i < stringLength; i++)
        {
                if(isdigit(string[i]) == 0)
		{
		  boolean=0;
                  break;
		}
        }

      return boolean;
}
