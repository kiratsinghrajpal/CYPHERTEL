#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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

int main(int argc , char *argv[]){
	if(argc < 2 ){
	  fprintf(stderr,"port number not provided\n");
	  exit(1);				
	}
	int sockfd , newsockfd , portno,n;
	char buffer[255];
	
	struct sockaddr_in serv_addr , cli_addr;
	socklen_t clilen;

	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	if(sockfd <0){
	   error("error opening socket");
	
	}
	bzero((char *) &serv_addr , sizeof(serv_addr));
	portno = atoi(argv[1]);
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);


	if(bind(sockfd, (struct sockaddr *) &serv_addr , sizeof(serv_addr)) < 0)
		error("binding failed");



	listen(sockfd , 5);
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd , (struct sockaddr *) &cli_addr , &clilen);

	if(newsockfd < 0)
	error("error on accept");	


	while(1)
	{
		
		bzero(buffer,255);
		n = read(newsockfd,buffer,255);
		if(n<0)
			error("error on reading");
                printf("client_encrypted_msg:%s \n",buffer);
                printf("client_decrypted_msg:");
                decrypt(buffer,strlen(buffer)-1);
                bzero(buffer,255);
                fgets(buffer,255,stdin);
                char* encrypted=encryption(buffer);
                n= write(newsockfd,encrypted,255);
		if(n<0)
			error("error on writing");
                
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}

		
