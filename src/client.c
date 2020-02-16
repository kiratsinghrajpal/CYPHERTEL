#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>
void error(const char *msg){

	perror(msg);
	exit(1);

 }

char *encryption(char *s) {
        int length=strlen(s)-1;
        int sqr=(int)sqrt(length);
        int sqr_2nd=0;
        if(sqr*sqr==length)
        {
            sqr_2nd=sqr;
        }
        else
        {
            sqr_2nd=sqr+1;
        }

        if(sqr*sqr_2nd<length)
        {
            sqr=sqr+1;
        }

        int position=0;
        char encode_arr[sqr][sqr_2nd];
        int v=0;
        for(int i=0;i<sqr;i++)
        {
            for(int j=0;j<sqr_2nd;j++)
            {
              if(v<length)
              {
              encode_arr[i][j]=s[v];
              v++;
              }
              else
               {
              encode_arr[i][j]='`';
                v++;
               }
            }
            
        }
         
        int w=0;
        static char encrypted[255];
        for(int i=0;i<sqr_2nd;i++)
        {
            for(int j=0;j<sqr;j++)
            {
                encrypted[w]=encode_arr[j][i];
                  w++;
            }
        } 

       return encrypted;
}

void decrypt(char* encrypted_msg,int length)
     {
         int sqr=(int)sqrt(length);
         int sqr_2nd=0;
         if(sqr*sqr==length)
         {
             sqr_2nd=sqr;
         }
         else
         {
             sqr_2nd=sqr+1;
         }

         if(sqr*sqr_2nd<length)
         {
             sqr=sqr+1;
         }
       
         int w=0;
         char decrypt[sqr][sqr_2nd];
         for(int i=0;i<sqr_2nd;i++)
         {
             for(int j=0;j<sqr;j++)
             {
                decrypt[j][i]=*(encrypted_msg+w);
                w++;
             }
         }
          
         for(int i=0;i<sqr;i++)
         {
             for(int j=0;j<sqr_2nd;j++)
             {   
                 if(decrypt[i][j]!='`')
                  {
                 printf("%c",decrypt[i][j]);
                  }
             }
         }
         printf("\n");

     }




int main(int argc , char *argv[])
{
	int sockfd , portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	char buffer[255];
	if(argc<3){
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(1);
		}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET , SOCK_STREAM, 0);
	if(sockfd < 0)
		error("error opening socket");

	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
	fprintf(stderr , "ERROR , no such host");
	}
	  
	bzero((char *) &serv_addr , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr , (char *) &serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd , (struct sockaddr *) &serv_addr , sizeof(serv_addr))<0)
		error("connection failed");


	while(1)
	{

		bzero(buffer,255);
                fgets(buffer,255,stdin);
                char* encrypted=encryption(buffer);
                n= write(sockfd,encrypted,255);
		if(n<0)
			error("error on writing");
                bzero(buffer,255);
                n = read(sockfd,buffer,255);
		if(n<0)
			error("error on reading");
                printf("server_encrypted_msg:%s \n",buffer);
                printf("server_decrypted_msg:");
                decrypt(buffer,strlen(buffer)-1);
	}
	close(sockfd);
	return 0;


}	
