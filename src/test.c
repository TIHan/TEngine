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

int dgp_init (dgp_uint16 port)
{
  struct addrinfo hints, *res;
  char portbuf[sizeof port];
  
  memset (&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;          /* IPv4 and/or IPv6 */
  hints.ai_socktype = SOCK_DGRAM;       /* User Datagram Protocol */
  hints.ai_flags = AI_PASSIVE;          /* Use my local address. */
  
  if (getaddrinfo (NULL, portbuf, &hints, &res) != 0)
  {
    fprintf (stderr, "(dgp_init) Error: Unable to get address info.\n");
    return 1;
  }
  
  
  
  
  return 0;
}

dgp_int32 main ()
{
  dgp_init (46767);
  printf ("HEY");

  return -1;
}