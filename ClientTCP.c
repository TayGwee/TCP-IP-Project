/*	ClientTCP.c 
	TTP3121 TCP/IP Programming 
	1. Tay Ka Huey 1122701276
	2. Gwee Chee Wei 1122700976
*/


#include "inet.h"


#define PORT 8000
#define MAXSZ 1024
#define S_IP "127.0.0.1"

int main()
{
int sockfd;// create a socket

struct sockaddr_in serv_addr;//client connect this address

int n;
int rec = 0;
char rmv[25];
char mk[25];
char data[MAXSZ+1];


static struct sigaction act;

void catch_exception(int);

act.sa_handler = catch_exception;
sigfillset(&(act.sa_mask));

sigaction(SIGINT, &act, (void *) 0);


//create socket
sockfd=socket(AF_INET,SOCK_STREAM,0);
//initialize the socket addresses
memset(&serv_addr,0,sizeof(serv_addr));
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=inet_addr(S_IP);
serv_addr.sin_port=htons(PORT);

//client  connect to server on port
connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

 
/* Get the linux's username of the connected client */ 
	char *usr; 
	usr=(char *)malloc(10*sizeof(char)); 
	usr=getlogin(); 

/* set the 'C_file' path */ 
	char str[30]; 
	strcpy(str, "/home/"); 
	strcat(str, usr); 
	strcat(str, "/C_file/"); 

/* Make the C_FIle directory if the directory */ 
	struct stat s; 
	if(stat(str, &s) == -1){ 
	mkdir(str, 0700); }

//send to sever and receive from server
do
{
bzero( data, sizeof(data));
recv(sockfd,data,MAXSZ,0);

printf("%s\n",data);


gets(data);

send(sockfd,data,MAXSZ,0);

 if(!strcmp(data, "1"))
{

	DIR *dir;
	struct dirent *r;

	char directoryName[30];
   	strcpy(directoryName, "/home/"); 
    	strcat(directoryName, usr); 
    	strcat(directoryName, "/C_file/");

	if ((dir = opendir (directoryName)) != NULL) {

		
		printf("\n[List files in Client Directory]\n");
  		// print all the files and directories within directory 
  		while ((r = readdir (dir)) != NULL) {

		printf("%s\n", r->d_name);    }

 		closedir (dir);
	}

	printf("\nPlease enter filename from above:\n");
	
	bzero( data, sizeof(data));
	
	gets(data);
	send(sockfd,data, MAXSZ, 0);

	char NamaFile[40];
	strcpy(NamaFile, "/home/"); 
	strcat(NamaFile, usr); 
	strcat(NamaFile, "/serverFile/");
	    
	strcat(NamaFile, data);

	FILE *file;
    	file = fopen(NamaFile, "r"); 

	bzero( data, sizeof(data));
	int nread = fread(data,1,256,file);
    	rec = recv(sockfd, data, MAXSZ, 0);
    	fwrite(data,1,rec,file);

}


else if(!strcmp(data, "2"))
{
	
printf("Please write down filename to download to [C_file]\n\n" );

	bzero( data, sizeof(data));
	recv(sockfd, data, MAXSZ, 0);
	printf("\n%s\n", data); 
	gets(data);
	send(sockfd,data, MAXSZ, 0);
	
   	char NamaFile[40];
    	strcpy(NamaFile, "/home/"); 
    	strcat(NamaFile, usr); 
    	strcat(NamaFile, "/C_file/");
    	strcat(NamaFile, data);
    
    	FILE *file;
    	file = fopen(NamaFile, "ab"); 

		if(NULL == file)
    	{
        	printf("WARNING!!! ERROR DETECTED!!!");
        
    	}
    
    	bzero( data, sizeof(data));
    
    	rec = recv(sockfd, data, MAXSZ, 0);
    	fwrite(data,1,rec,file);
   
}



else if(!strcmp(data, "3"))
{
	printf("Please key in Directory to create: ");
	scanf("%s", mk);

	/* create the path of the directory */ 
	char cr8Direc[40]; 
	strcpy(cr8Direc, "/home/"); 
	strcat(cr8Direc, usr); 
	strcat(cr8Direc, "/"); 
	strcat(cr8Direc, mk);

	/* Make Directory if the path of the directory no occur */ 
	struct stat s; 
	if(stat(cr8Direc, &s) == -1){ 
	mkdir(cr8Direc, 0700); } 
}


else if(!strcmp(data, "4"))
{
	printf("Please key in Directory to delete: ");
	scanf("%s", rmv);

	/* set the path of the directory that want to delete */ 
	char dltDirec[40]; 
	strcpy(dltDirec, "/home/"); 
	strcat(dltDirec, usr); 
	strcat(dltDirec, "/"); 
	strcat(dltDirec, rmv);

	/* select all the files inside the directory that want to delete */
	char slctSubDirec[60];
	strcpy(slctSubDirec, "exec rm -r ");
	strcat(slctSubDirec, "/home/"); 
	strcat(slctSubDirec, usr); 
	strcat(slctSubDirec, "/"); 
	strcat(slctSubDirec, rmv);
	strcat(slctSubDirec, "/*"); 

	/* Check the path exist or not, if exist, delete it */ 
	struct stat s; 
	if(stat(dltDirec, &s) != -1){
	system(slctSubDirec);
	rmdir(dltDirec); } 
}

}while (strcmp(data, "/E"));
close(sockfd);
return 0;
}

void catch_exception(int signo){
	printf("\n[ Interuption ~~~~ Occured.....]\n");
}

