#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "dgp_types.h"

#define MAX_BUFFER_LENGTH 1024

static dgp_int32 g_iSocketFileDescriptorIpV4 = -1;
static dgp_int32 g_iSocketFileDescriptorIpV6 = -1;

dgp_int32 dgp_init (dgp_uint16 port) {
  struct addrinfo hints, *res, *p;
  char portbuf[sizeof port], ip[INET6_ADDRSTRLEN];
  
  memset (&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;          /* IPv4 and/or IPv6 */
  hints.ai_socktype = SOCK_DGRAM;       /* User Datagram Protocol */
  hints.ai_flags = AI_PASSIVE;          /* Use my local address. */
  
  if (getaddrinfo (NULL, "46767", &hints, &res) != 0) {
    fprintf (stderr, "(dgp_init) Error: Unable to get address info.\n");
    return 1;
  }
  
  for (p = res;p != NULL; p = p->ai_next) {
    if (p->ai_family == AF_INET) {
      g_iSocketFileDescriptorIpV4 = socket (p->ai_family, p->ai_socktype, p->ai_protocol);
      inet_ntop (p->ai_family, p, ip, sizeof ip);
      printf ("IPv4: %s\n", ip);
    } else if (p->ai_family == AF_INET6) {
      g_iSocketFileDescriptorIpV6 = socket (p->ai_family, p->ai_socktype, p->ai_protocol);
      inet_ntop (p->ai_family, p, ip, sizeof ip);
      printf ("IPv6: %s\n", ip);
    }
  }
  
  if (g_iSocketFileDescriptorIpV4 == -1 && g_iSocketFileDescriptorIpV6 == -1) {
    fprintf (stderr, "(dgp_init) Error: Unable to create a socket.\n");
    return 1;
  }
  
  
  
  
  return 0;
}

dgp_int32 main ()
{
  dgp_init (46767);

  return -1;
}