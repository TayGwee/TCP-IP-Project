/*	ServerTCP.c 
	TTP3121 TCP/IP Programming 
	1. Tay Ka Huey 1122701276
	2. Gwee Chee Wei 1122700976
*/

#include "inet.h"

#define PORT 8000
#define MAXSZ 1024
#define MSGSIZE 2048
int main()
{
int sockfd;//to create socket
int n_sock;//to accept connection
int clilen;
char msg1[MSGSIZE+1];

struct sockaddr_in serv_addr;
struct sockaddr_in cli_addr;

int n;
char data[MAXSZ];
int cli_addrLength;
int pid;
int rec = 0;

//create socket
sockfd=socket(AF_INET,SOCK_STREAM,0);
//initialize the socket addresses
memset(&serv_addr,0,sizeof(serv_addr));
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
serv_addr.sin_port=htons(PORT);

//bind the socket with the server address and port
bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));


/* Get the linux's username of the connected client */ 
	char *usr; 
	usr=(char *)malloc(10*sizeof(char)); 
	usr=getlogin(); 

	/* set the 'server_file' path */ 
	char str[30]; 
	strcpy(str, "/home/"); 
	strcat(str, usr); 
	strcat(str, "/S_File/"); 

/* Make the C_FIle directory if the directory */ 
	struct stat s; 
	if(stat(str, &s) == -1){ 
	mkdir(str, 0700); }



listen(sockfd,5);

while(1)
{
//parent process waiting to accept a new connection
printf("\n[===========Waiting Connection===========]\n");
cli_addrLength=sizeof(cli_addr);
n_sock=accept(sockfd,(struct sockaddr*)&cli_addr,&cli_addrLength);
printf("**Client connected**\n");

//child process is created for serving each new clients
pid=fork();
if(pid==0)//child process rec and send
{
 strcpy(msg1,"**Hello, you are connecting to the server** \n\nSelect your action...\n1)Send File\n2)Download File\n3)Create Directory\n4)Delete Directory\n[type /E to quit] : \nEnter (1,2,3,4):\n");
	  send(n_sock, msg1, MSGSIZE, 0);

do
{
n=recv(n_sock,data,MAXSZ,0);

 if(!strcmp(data, "1"))
	{
		
		bzero( data, sizeof(data));
		recv(n_sock, data, MSGSIZE, 0);
                /* Make the S_File path */ 
		char filename[30];
		strcpy(filename, "/home/"); 
		strcat(filename, usr); 
		strcat(filename, "/S_File/");
		strcat(filename, data);
		
/* Print out the file of the S_File path Directory */ 
		FILE *file;
   	 	file = fopen(filename, "ab"); 
		bzero( data, sizeof(data));
		rec = recv(n_sock, data, MSGSIZE, 0);
		fwrite(data,1,rec,file);
		fclose(file);

		bzero( data, sizeof(data));
		strcat(data,"To send... [/E to exit]");
	}

 if(!strcmp(data, "2"))
	{	
		bzero( data, sizeof(data));
		strcat(data,"Please enter[/E]\n\n");

		DIR *dir;
		struct dirent *ent;

		char directoryName[30];
   		strcpy(directoryName, "/home/"); 
    		strcat(directoryName, usr); 
    		strcat(directoryName, "/S_File/");

		if ((dir = opendir (directoryName)) != NULL) {
		
			//print all the files and directories within directory 
 			while ((ent = readdir (dir)) != NULL) {

			strcat(data, ent->d_name);
			strcat(data, "\n");
 			}
 		 closedir (dir);
		send(n_sock, data, MSGSIZE, 0);
		 
		}else {

  		
 		 perror ("Directory does not exist...");
  		return EXIT_FAILURE;
		}

	bool exist = true; 

	do{

        
	bzero( data, sizeof(data));
	recv(n_sock, data, MSGSIZE, 0);

	char filename[30];
    	strcpy(filename, "/home/"); 
    	strcat(filename, usr); 
    	strcat(filename, "/S_File/");
    	strcat(filename, data);
    
	
	FILE *file = fopen(filename, "r");
	if(file==NULL)
        {
            strcpy(data,"File not found in server...Please re-enter the correct filename");
	    send(n_sock, data, MSGSIZE, 0);  
	    exist = false; 
	   
        }  
	
	if(exist == true )
	{
	bzero( data, sizeof(data));
	int nread = fread(data,1,256,file);
	send(n_sock, data, nread, 0);
	}

	bzero( data, sizeof(data));
	strcpy(data,"To Download... [/E to exit]");

	}while(exist == false);
	}
		
	 
	if(!strcmp(data, "3"))
	{
		bzero( data, sizeof(data));
		strcat(data,"To create Directory... [/E to exit]");
	}

	if(!strcmp(data, "4"))
	{
		bzero( data, sizeof(data));
		strcat(data,"To delete Directory... [/E to exit]");
	}
if(n==0)
{
close(n_sock);
break;
}
data[n]=0;
send(n_sock,data,MSGSIZE,0);


}while(strcmp(msg1, "/E"));
exit(0);
}
else
{
close(n_sock);//sock is closed BY PARENT
}
}

return 0;
}

