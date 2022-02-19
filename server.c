#include <stdio.h> 
#include <string.h>    
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 


#define MAX_USERS 30

int main(int argc, int** argv)
{
	
	int fdListen, fdConn[MAX_USERS];
	int portNum = 3005;
 	fd_set sockets; 

	memset(fdConn + 0, 0, sizeof(int) * MAX_USERS);

	// step 1: creating the socket
	fdListen = socket (AF_INET, SOCK_STREAM, 0);
	if (fdListen < 0)
	{
		perror ("Error at creating the socket\n");
		return errno;	
	}

	// step 2: bind the socket to the server address
	struct sockaddr_in server_addr; 
	socklen_t size = sizeof (server_addr); 

	bzero (&server_addr, sizeof (server_addr));  // zero structure out 
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons (portNum);
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	
	if (bind(fdListen, (struct sockaddr *) &server_addr, size) < 0)
	{
		perror ("Error at binding the socket\n");
		return errno;
	}

	// step 3: make the server listen to the port
	if (listen (fdListen, 5) < 0)   
	{
		perror ("Error at listenting\n");
		return errno;
	}
	
	puts ("Server is up and running...");

	/// step 4: manage the clients	
	int max_sd;
	int bufSize = 1025;
	char buffer[bufSize];
	
	while (1)  
    	{  
        	// clear the socket set 
        	FD_ZERO (&sockets);
	
		
		// add master socket to set 
        	FD_SET (fdListen, &sockets);  
        	max_sd = fdListen; 

		// add child sockets to set
 
        	for (int i = 0; i < MAX_USERS; i++)
		{	
			int sd = fdConn[i];
			//printf("%d ", sd);
			if(sd > 0)
				FD_SET(sd , &sockets);  

			if(sd > max_sd)  
				max_sd = sd;
		}


		// wait for an activity on one of the sockets
        	int activity = select (max_sd + 1, &sockets, NULL, NULL, NULL);  
		if (activity < 0)  
            		puts("Error at select");  
		
		// if something happened on the master socket , 
        	// then its an incoming connection
		int new_socket = 0; 
        	if (FD_ISSET(fdListen, &sockets))  
        	{  	
			//accept the new connection
			new_socket = accept (fdListen, (struct sockaddr *) &server_addr, &size);
            		if (new_socket < 0)  
            		{  
                		perror("Error at accepting clients");  
                		return errno;  
            		}  
       		  	
			/// show on server side who has connected			
			printf ("New connection, socket fd is %d, ip is: %s, port: %d \n", new_socket, inet_ntoa (server_addr.sin_addr), ntohs (server_addr.sin_port));  
			
			/// tell the client that he is connected
			
			char* ok = "Connected...\n";
			send(new_socket, ok, strlen(ok), 0);
			
			// add new socket to array of sockets 
            		for (int i = 0; i < MAX_USERS; i++)   
                		//if position is empty 
                		if (!fdConn[i])  
                		{  
                    			fdConn[i] = new_socket;  
					break;  
                		}  
        	}  

		/// else it is some request from another server
		for (int i = 0; i < MAX_USERS; i++)  
        	{  
            		int sd = fdConn[i];  
                 	
			/// if the socket was active handle the requests
            		if (FD_ISSET ( sd , &sockets))  
            		{  	
				int valRead = recv (sd, buffer, bufSize, MSG_NOSIGNAL);
   				if (!valRead)  
                		{  
                    			// somebody disconnected, get his details and print 
                    			getpeername (sd , (struct sockaddr *) &server_addr, &size);  
                    			printf ("Host disconnected, ip %s, port %d\n" , inet_ntoa(server_addr.sin_addr) , ntohs(server_addr.sin_port));  
                         
                    			// close the socket and mark as 0 in list for reuse 
                    			close (sd);  
                    			fdConn[i] = 0;  
                		}

				else
				{
					buffer[valRead] = '\0';  
                    			printf("Client %d: %s", sd, buffer);  
	
					for (int j = 0; j < MAX_USERS; j++)
                    			{	
						char msg[bufSize];
                        			sprintf (msg, "%d,%s", sd, buffer);
                        			send (fdConn[j], msg, strlen(msg), MSG_NOSIGNAL);
                    			}
				}
			valRead = 0;
                	strcpy(buffer, ""); 
			}
		}
	}
     

	return 0;
}














