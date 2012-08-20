#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main ()
{
  struct addrinfo hints, *res;
  int sockfd;

  memset (&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;		// IPv4
  hints.ai_socktype = SOCK_DGRAM;	// User Datagram Protocrol
  hints.ai_flags = AI_PASSIVE;		// Use my local address.

  getaddrinfo (NULL, "4767", &hints, &res);

  sockfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);

  bind (sockfd, res->ai_addr, res->ai_addrlen);
  return -1;
}
