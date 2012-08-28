#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "SDL.h"
#include "SDL_thread.h"

#define MAX_BUFFER_LENGTH 65536

static int server_socket_func (void *data);
static int client_socket_func (void *data);

static int g_iSocketFileDescriptorIpV4 = -1;
static int g_iSocketFileDescriptorIpV6 = -1;

int dgp_init (unsigned short port)
{
  struct addrinfo hints, *res;
  char portbuf[sizeof port];
  
  memset (&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;          // IPv4 and/or IPv6
  hints.ai_socktype = SOCK_DGRAM;       // User Datagram Protocol
  hints.ai_flags = AI_PASSIVE;          // Use my local address.
  
 // itoa (port, portbuff, 10);
  
  if (getaddrinfo (NULL, portbuf, &hints, &res) != 0)
  {
    fprintf (stderr, "(dgp_init) Error: Unable to get address info.\n");
    return 1;
  }
  
  
  
  return 0;
}

int main ()
{
  // [WS] Initialize SDL.
  SDL_Init (SDL_INIT_EVERYTHING);
  
  SDL_Thread *thread = SDL_CreateThread (server_socket_func, NULL);
 // SDL_Thread *client_thread = SDL_CreateThread (client_socket_func, NULL);
  
  SDL_WaitThread (thread, NULL);
 // SDL_WaitThread (client_thread, NULL);

  return -1;
}

static int server_socket_func (void *data)
{
  struct addrinfo hints, *res;
  int sockfd;

  memset (&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;		// IPv4
  hints.ai_socktype = SOCK_DGRAM;	// User Datagram Protocrol
  hints.ai_flags = AI_PASSIVE;		// Use my local address.
  
  int result_getaddrinfo = getaddrinfo (NULL, "4767", &hints, &res);

  if (result_getaddrinfo != 0)
  {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (result_getaddrinfo));
    exit (1);
  }

  sockfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
  
  if (sockfd == -1)
  {
    fprintf (stderr, "socket: Unable to create socket.\n");
    exit (1);
  }

  int result_bind = bind (sockfd, res->ai_addr, res->ai_addrlen);
  
  if (result_bind == -1)
  {
    shutdown (sockfd, SHUT_RDWR);
    fprintf (stderr, "bind: Unable to bind socket to port.\n");
    exit (1);
  }
  
  while (1)
  {
    unsigned char buffer[MAX_BUFFER_LENGTH];
    struct sockaddr sock_addr;
    socklen_t addr_len = sizeof sock_addr;
    int receive_length = recvfrom (sockfd, buffer, MAX_BUFFER_LENGTH - 1, 0, &sock_addr, &addr_len);
    if (receive_length == -1)
    {
      fprintf (stderr, "recvfrom: Unable to get data.\n");
      exit (1);
    }
    
    printf ("Server Received: %d bytes.\n", receive_length);
    
    union
    {
      int i;
      unsigned char buffer[4];
    } unpack;
        
    unpack.buffer[0] = buffer[0];
    unpack.buffer[1] = buffer[1];
    unpack.buffer[2] = buffer[2];
    unpack.buffer[3] = buffer[3];

    printf ("Server Received Number: %d\n", unpack.i);
    SDL_Delay (10);
  }
  
  return 0;
}

static int client_socket_func (void *data)
{
  struct addrinfo hints, *res;
  int sockfd;

  memset (&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;		// IPv4
  hints.ai_socktype = SOCK_DGRAM;	// User Datagram Protocrol
  
  int result_getaddrinfo = getaddrinfo ("localhost", "4767", &hints, &res);

  if (result_getaddrinfo != 0)
  {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (result_getaddrinfo));
    exit (1);
  }

  sockfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
  
  if (sockfd == -1)
  {
    fprintf (stderr, "socket: Unable to create socket.\n");
    exit (1);
  }
  
  while (1)
  {
    int i; 
    printf ("Enter Number: ");
    scanf ("%d", &i);
    
    unsigned char buffer[MAX_BUFFER_LENGTH];
    
    buffer[0] = i;
    buffer[1] = i >> 8;
    buffer[2] = i >> 16;
    buffer[3] = i >> 24;
    
    printf ("Client Sent Number: %d\n", i);
    
    int send_length = sendto (sockfd, buffer, 4, 0, res->ai_addr, res->ai_addrlen);
    if (send_length == -1)
    {
      fprintf (stderr, "sendto: Unable to send data.\n");
      exit (1);
    }
    
    printf ("\nClient Sent: %d bytes.\n", send_length);
    
    char cont;
    printf ("Continue? ");
    scanf ("%1s", &	cont);
    
    if (cont == 'n')
      exit(1);
    
    SDL_Delay (10);
  } 
}
