#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MYPORT "5000"
#define MAX 4096

void *convert(struct sockaddr *sa)
{
	if ( sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "you are entering wrong number of string\n" );
		exit(EXIT_FAILURE);
	}

	struct addrinfo hints, *ref, *p;
	int status, client_sockfd, yes = 1;
	int  nbytes;
	size_t count=MAX;
	struct sockaddr_storage addr;
	char ip_str[INET6_ADDRSTRLEN], buffer[MAX];

	memset(&hints, 0, sizeof hints);

	if ( (status = getaddrinfo(argv[1], MYPORT, &hints, &ref)) != 0)
	{
		fprintf(stderr, "getaddrinfo : %s\n", gai_strerror(status));
		return 2;
	}


	 for (p = ref;p != NULL; p = p->ai_next)
	 {
	 	if ( (client_sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
	 	{
	 		perror("client_socket: error");
	 		continue;
	 	}

	 	

	 	if ( (connect(client_sockfd, p->ai_addr, p->ai_addrlen)) == -1)
	 	{
	 		perror("connect: error");
	 		close(client_sockfd);
	 		continue;
	 	}break;
	 }
	 if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(EXIT_FAILURE);
    }
    //inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), ip_str, sizeof ip_str);
    inet_ntop(p->ai_family, convert((struct sockaddr *)p->ai_addr), ip_str, sizeof ip_str);
    //printf("%s\n",convert((struct sockaddr *)p->ai_addr));
    printf("client: connecting to %s\n", ip_str);

    freeaddrinfo(ref);
	while(1)
	{
	    printf("please enter the instruction: ");
	    fgets(buffer,4096, stdin);
		if (strlen(buffer) > 4096)
		{
			printf("you are overflowing my buffer. Try again with less than 4096 char\n");
			continue;
		}
		buffer[strlen(buffer) -1] = '\0';
		if((strcmp(buffer, "quit") == 0) | (strcmp(buffer, "create") == 0) | (strcmp(buffer, "run") == 0))
		{
		    if((nbytes = send(client_sockfd, buffer, strlen(buffer) + 1, 0)) <= 0)
			{
				if(nbytes == 0)
				{
					printf("CLIENT: server is closed\n");
					close(client_sockfd);
					exit(EXIT_SUCCESS);
				}
				else
				{
					perror("CLIENT:");
					close(client_sockfd);
					exit(EXIT_FAILURE);
				}
			}
		}
		else
		{
			printf("CLIENT:You entered invalid input. TRY AGAIN !!!\n");
			continue;
		}
	    memset(buffer,0, MAX	);
	    if((nbytes = recv(client_sockfd, buffer, MAX, 0)) <= 0)
		{
			if(nbytes == 0)
			{
				printf("CLIENT: server is closed\n");
				close(client_sockfd);
				exit(EXIT_SUCCESS);
			}
			else
			{
				perror("CLIENT:");
				close(client_sockfd);
				exit(EXIT_FAILURE);
			}
		}
	    else
	    {//printf("bug_point 2\n");
	    	if(strcmp(buffer, "quit") == 0)
	    	{//printf("bug_point 3\n");
	    		close(client_sockfd);
	    		exit(EXIT_SUCCESS);
	    	}
	    	else
	    	{//printf("bug_point 4\n");
	    		printf("CLIENT: message from server > %s\n",buffer );
	    	}
	    	
	    }
	}
    
    close(client_sockfd);

}