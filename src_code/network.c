/******************************************/
/* Author: NAZMUL HUSSAIN                 */
/* Roll no - 18CS60R63                    */
/* Mid Sem exam: Networking problem       */
/******************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>     // This is for string related call.
#include<errno.h>      // This is for error handling.
#include<limits.h>     // this is for different type limit checking.
#include<ctype.h>      // This is for different type checking.
#include<fcntl.h>      // This is for unblocking socket.
#include<unistd.h>     // this is for different type typical system call.
#include<sys/socket.h> // This header file defines miscellaneous socket related constant, types, and function.
#include<sys/types.h>  // This header file defines miscellaneous types 
#include<sys/sem.h>    // This header file defines semaphores related types, function.
#include<sys/wait.h>   // This header file defines blocking and wait related types and function.
#include<sys/ipc.h>    // This header file defines mainly interprocess communication related structures.
#include<netinet/in.h> // This header file defines system parameter related to address.
#include<arpa/inet.h>  // This header file defines different types macros .
#include<time.h>       // This header file is included for getting times.
#include<signal.h>     
#include<netdb.h>
#include<sys/shm.h>    // This header file is included for shared memory .

#define PORT "5000"   // This is the port server will be listening on
#define MAX 4096  
#define NAZMUL
extern void parse(FILE*,FILE*);


int listener;         // Server will be listening on this socket id.
int new_fd;           // newly accept()ed socket descriptor.
int pid;              // This will save the pid of main process
char filenameout[128], file_name[128];


/*.....................This function convert the IP address................................*/
void *convert(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *)sa)->sin_addr);
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}
/*.........................................................................................*/



/*...............We are handling SIGINT SIGNAL here..........................................*/
void sigint_handler(int sig)
{
	int nbytes;
	char buffer[MAX];
	strcpy(buffer, "quit");
	if (pid != getpid())
	{
		remove(file_name);
		remove(filenameout);
		close(new_fd);
	}
	else
	{
		close(listener);
	}

	#if hi
	if((nbytes = send(new_fd, buffer, strlen(buffer) + 1, 0)) < 0)
	{
		if(nbytes == 0)
		{
			printf("SERVER: client is closed\n");
			close(new_fd);
			close(listener);
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("SERVER:");
			close(new_fd);
			close(listener);
			exit(EXIT_FAILURE);
		}
	}
	
	#endif
	exit(EXIT_SUCCESS);
}
/*............................................................................................*/



/*.......................We are handling SIGCHLD signal here..................................*/
void sigchild_handler(int sig)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}
/*..............................................................................................*/



/*...........................We are generating random number here...............................*/

int get_total_line_number(char *file_name)
{
	FILE *fp;
	int i=1;
	char text[256];
	fp = fopen(file_name, "r");
	
	if (fp == NULL)
	{
		perror("\nCould not open file");
		exit(-1);   
	}
	while (fgets(text, 255, fp) != NULL)
	{
		i++;
	}
	fclose(fp);
	return (i-1);
}





int main()
{
	int yes = 1;              // this for setsockopt()
	int status;           // this is for holding return value of getaddrinfo()
	int proces_id_gedit, proces_id_ter;        // this will hold return value of fork()
	int flags;             // this for fcntl() call
	//char file_name[128];		// This is for saving filename.
	char command[128];
	FILE *fp, *output_fp;
	int create_flag = 0;  // To check whether create command came or not
	int run_flag = 0;     // To check whether run command came or not

	char line[255];
	size_t length = 255;
	ssize_t nread;


	struct sigaction for_sigint, for_sigchild; // these struct, we have to pass to sigaction()

	char buffer[MAX];    // this will be used in send(), recv()
	int nbytes;          // This will hold return value of send(), recv()

	struct sockaddr_storage client_addr; //it will hold client socket address.
	char client_IP[INET6_ADDRSTRLEN];       // It will hold the ip address of client ip address.
	socklen_t addr_len;                   // It will hold length of client ip address.



	/*................GET us an successful socket and bind it...........................................*/
	struct addrinfo hints, *ref, *p;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((status = getaddrinfo(NULL, PORT, &hints, &ref)) != 0)
	{
		fprintf(stderr, "SERVER: getaddrinfo:%s\n",gai_strerror(status) );
		exit(EXIT_FAILURE);
	}

	for(p = ref; p != NULL; p = p->ai_next)
	{
		if((listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			continue;
		}

		if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			perror("SERVER:");
			exit(EXIT_FAILURE);
		}
		 /*....Setting the socket in non-blocking mode...*/
		if ((flags = fcntl(listener, F_GETFL, 0)) < 0) 
		{ 
    		perror("SERVER(flags = fcntl):");
    		continue;
		} 

		if (fcntl(listener, F_SETFL, flags | O_NONBLOCK) < 0) 
		{ 
			perror("SERVER( fcntl):");
    		continue;
		} 
		/*..............................................*/

		if(bind(listener, p->ai_addr, p->ai_addrlen) == -1)
		{
			continue;
		}

		break; // we get successfull socket and bind, so we do not need to traverse anymore.
	}


	if(p == NULL) //checking whether we get a successful binding socket or not
	{
		fprintf(stderr, "we did not get successful binding socket\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(ref);

	/*..................we are handling here SIGINT SIGNAL....................................*/
	for_sigint.sa_handler = sigint_handler;
	for_sigint.sa_flags = 0;
	sigemptyset(&for_sigint.sa_mask);
	if(sigaction(SIGINT, &for_sigint, NULL) < 0)
	{
		perror("SERVER(sigint):");
		exit(EXIT_FAILURE);
	}
	/*............................................................................................*/


	/*.......................We are handling SIGCHLD signal here.................................*/
	for_sigchild.sa_handler = sigchild_handler;
	for_sigchild.sa_flags = SA_RESTART;
	sigemptyset(&for_sigchild.sa_mask);
	if(sigaction(SIGCHLD, &for_sigchild, NULL) < 0)
	{
		perror("SERVER(SIGCHLD):");
		exit(EXIT_FAILURE);
	}
	/*.............................................................................................*/



	listen(listener, 5);  // we are listening here.
	printf("SERVER:We are waiting for connection......................................\n");


	pid = getpid() ;// save the process id in pid.
	for(;;)
	{	
		addr_len = sizeof(client_addr);
		if((new_fd = accept(listener, (struct sockaddr *)&client_addr, &addr_len)) < 0)  // we are accepting new connection here.
		{
			
		}	
		else
		{
			/*....Setting the socket in non-blocking mode...*/
			if ((flags = fcntl(new_fd, F_GETFL, 0)) < 0) 
			{ 
	    		perror("SERVER(flags = fcntl):");
	    		continue;
			} 

			if (fcntl(new_fd, F_SETFL, flags | O_NONBLOCK) < 0) 
			{ 
				perror("SERVER( fcntl):");
	    		continue;
			} 
			/*..............................................*/

			
			inet_ntop(client_addr.ss_family, convert((struct sockaddr *)&client_addr), client_IP, sizeof client_IP);
			printf("SERVER: We got connection from %s\n",client_IP );

			if( fork() == 0)
			{	
				close(listener); // child process does not need this.
				for(;;)
				{

					memset(buffer, 0, 4096);
					if((nbytes = recv(new_fd, buffer, MAX, 0)) == 0)
					{
						printf("SERVER: client is closed on socket Id:%d \n",new_fd);
						close(new_fd);
						close(listener);
						exit(EXIT_SUCCESS);
						
					}
					else
					{
					
						
						if(strcmp(buffer, "quit") == 0)
						{
							memset(buffer, 0, 4096);
							strcpy(buffer, "quit");
							if((nbytes = send(new_fd, buffer, strlen(buffer) + 1, 0)) < 0)
							{
								if(nbytes == 0)
								{
									printf("SERVER: client is closed\n");
									close(new_fd);
									close(listener);
									exit(EXIT_SUCCESS);
								}
								else
								{
									
								}
							}
							else
							{
								if(create_flag == 1 && run_flag ==0)
								{
									fclose(output_fp);
									//printf("%s\n",buffer );
									
									char temp[128];
									sprintf(temp, "kill -9 %d",proces_id_gedit);
									//printf("gedit %s %d\n",temp,proces_id_gedit);
									system(temp);
									sprintf(temp, "kill -9 %d",proces_id_ter);
									//printf("%s %d\n",temp,proces_id_ter);
									system(temp);
									//system("killall xterm");
									//system("killall gedit");
									//printf("%s %s\n", file_name,filenameout);
										remove(file_name);
									remove(filenameout);
									create_flag = 0;
								}
								printf("SERVER: Connection with client %d is closed\n",new_fd );
								close(new_fd);
								close(listener);
								exit(EXIT_SUCCESS);
							}
							
						}
						else if(strcmp(buffer, "create") == 0)
						{
							printf("create command came\n");
							memset(buffer, 0, 4096);
							strcpy(buffer, "we got create command");
							if((nbytes = send(new_fd, buffer, strlen(buffer) + 1, 0)) == 0)
							{
								
								printf("SERVER: client is closed\n");
								close(new_fd);
								close(listener);
								exit(EXIT_SUCCESS);
								
								
							}

							sprintf(file_name, "%d.txt",getpid() );
							sprintf(filenameout,"%dout.txt",getpid());
							sprintf(command,"gedit %s",file_name);
							signal(SIGCHLD, SIG_IGN);
							proces_id_gedit= fork();
							if(proces_id_gedit== 0)
							{close(new_fd);
								system(command);
								//exit(EXIT_SUCCESS);
							}
							else
							{
								proces_id_ter=fork();
								if(proces_id_ter==0)
								{
									close(new_fd);
									memset(command, 0, 128);
									sprintf(command, "xterm -e \"./sample %s; $SHELL\"",filenameout);
									system(command );
								}
								else
								{
									create_flag = 1;
									run_flag = 0;
								}	
							}

							
							
						}
						else if(strcmp(buffer, "run") == 0 )
						{
							if(create_flag == 1 && run_flag == 0)
							{
								int line_no;
								run_flag = 1;
								printf("run command came\n");
								memset(buffer, 0, 4096);
								output_fp = fopen(filenameout, "r");
								while(fgets(line, length, output_fp) != NULL)
								{
									strncat(buffer, line, 255);
									
								}
								line_no = get_total_line_number(file_name);
								sprintf(line, "\nTotal no of lines in file: %d",line_no);
								strcat(buffer,line);

								if((nbytes = send(new_fd, buffer, strlen(buffer) + 1, 0)) == 0)
								{
									if(nbytes == 0)
									{
										printf("SERVER: client is closed\n");
										close(new_fd);
										close(listener);
										exit(EXIT_SUCCESS);
									}
									else
									{
										
									}
								}
								//free(line);
								fclose(output_fp);
								//printf("%s\n",buffer );
								
								char temp[128];
								sprintf(temp, "kill -9 %d",proces_id_gedit);
								//printf("gedit %s %d\n",temp,proces_id_gedit);
								system(temp);
								sprintf(temp, "kill -9 %d",proces_id_ter);
								//printf("%s %d\n",temp,proces_id_ter);
								system(temp);
								//system("killall xterm");
								//system("killall gedit");
								//printf("%s %s\n", file_name,filenameout);
								remove(filenameout);
								remove(file_name);
								create_flag = 0;
								run_flag =1;
							}
							else if(run_flag ==1 && create_flag == 0)
							{
								printf("run command came\n");
								memset(buffer, 0, 4096);
								strcpy(buffer, "you can not give run command without giving first create command. TRY AGAIN.");
								if((nbytes = send(new_fd, buffer, strlen(buffer) + 1, 0)) == 0)
								{
									
									printf("SERVER: client is closed\n");
									close(new_fd);
									close(listener);
									exit(EXIT_SUCCESS);
									
									
								}

							}
							else
							{
								memset(buffer, 0, 4096);
								strcpy(buffer, "I am no longer able to process your any further request. You better type CTL+C");
								if((nbytes = send(new_fd, buffer, strlen(buffer) + 1, 0)) == 0)
								{
									
									printf("SERVER: client is closed\n");
									close(new_fd);
									close(listener);
									exit(EXIT_SUCCESS);
									
									
								}
							}

						}
						else
						{
							#if nazmul
							memset(buffer, 0, 4096);
							strcpy(buffer, "you entered invalid command");
							if((nbytes = send(new_fd, buffer, strlen(buffer) + 1, 0)) < 0)
							{
								if(nbytes == 0)
								{
									printf("SERVER: client is closed\n");
									close(new_fd);
									close(listener);
									exit(EXIT_SUCCESS);
								}
								else
								{}
									
							}
							#endif
						}

						if(create_flag == 1)
						{
							sleep(1);
							fp = fopen(file_name, "r");
							output_fp=fopen(filenameout, "w");
							if ( fp != NULL)
							{
								//printf("we are calling parse\n");
								parse(fp, output_fp);
								fclose(fp);
							}
							
							fclose(output_fp);
							
						}
					} 
	
				}/*..........................................END OF FOR LOOP OF EACH CLIENT HANDLING PROCESS................*/

			}/*..............................................END OF FORK IF BLOCK...........................................*/
			else
			{
				close(new_fd);  // parent does not need this
				continue;
			}

		}/*..................................................END OF ACCEPT ELSE BLOCK....................................*/

	}/*......................................................END OF OF MAIN FOR LOOP.......................................*/

}/*..........................................................END OF MAIN BLOCK.........................................................*/	

