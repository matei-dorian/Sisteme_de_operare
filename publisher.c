#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
 
int main()
{
	int client;
	int portNum = 3005;

	char* ip = "127.0.0.1";
	
	/// Step 1: create the socket
	client = socket (AF_INET, SOCK_STREAM, 0);
 
	if (client < 0)
	{
		perror ("Error at creating socket\n");
		return errno;
	}
	
	/// Step 2: set up the port and ip address 
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons (portNum);

	if (inet_pton (AF_INET, ip, &server_addr.sin_addr) <= 0)
	{
		perror ("Error at converting the address\n");
		return errno;
	}


	/// Step 3: connect
	if (connect (client, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
	{
		perror ("Error at connecting to server.\n");
		return errno;
	}
	

	int bufSize = 1024;
	char buffer[bufSize];
	int isRunning = 1;
	char *endOfLine;

	recv (client, buffer, bufSize, 0);  // wait for OK from the server
	printf ("%s", buffer);
 	
	/// Start sending messages
	while (isRunning)
	{
		printf ("Client: ");
		fgets(buffer, bufSize, stdin);
		
	        //remove \n fSrom buffer
		int lg = strlen(buffer);
		if (*buffer && buffer[lg] == '\n')
			buffer[lg] = '\0';

		send (client, buffer, bufSize, 0);
		//printf("%d", strcmp (buffer, "#\n"));
		if (strcmp (buffer, "#\n") == 0)
		{
			send(client, "\nConnection ended by the client.\n", bufSize, 0);
			isRunning = 0;
		} 
	}

	printf("Terminated\n");

	/// Step 4: close the socket
	close(client);


	return 0;
}
