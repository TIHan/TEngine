#ifdef __GNUC__
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <cstring>
#elif _MSC_VER
  #include <winsock2.h>
  #include <ws2tcpip.h>

  #pragma comment(lib, "ws2_32.lib")
#endif
#include <stdio.h>

#ifdef __GNUC__
  typedef int8_t dgp_int8;
  typedef uint8_t dgp_uint8;
  typedef int16_t dgp_int16;
  typedef uint16_t dgp_uint16;
  typedef int32_t dgp_int32;
  typedef uint32_t dgp_uint32;
  typedef int64_t dgp_int64;
  typedef uint64_t dgp_uint64;
#elif _MSC_VER
  typedef signed __int8 dgp_int8;
  typedef unsigned __int8 dgp_uint8;
  typedef signed __int16 dgp_int16;
  typedef unsigned __int16 dgp_uint16;
  typedef signed __int32 dgp_int32;
  typedef unsigned __int32 dgp_uint32;
  typedef signed __int64 dgp_int64;
  typedef unsigned __int64 dgp_uint64;
#endif

typedef struct _dgp_uint128
{
  dgp_uint16 value[8];
} dgp_uint128;
  
typedef void* dgp_pointer;
typedef float dgp_float;
typedef double dgp_double;

#define MAX_BUFFER_LENGTH 1024

static dgp_int32 g_iSocketFileDescriptorIpV4 = -1;
static dgp_int32 g_iSocketFileDescriptorIpV6 = -1;

dgp_int32 dgp_init (dgp_uint16 port) {
  struct addrinfo hints, *res, *p;
  char portbuf[sizeof port], ip[INET6_ADDRSTRLEN];

#ifdef _MSC_VER
  WSADATA wsaData;
  int iResult;

  // Initialize Winsock
  iResult = WSAStartup (MAKEWORD(2,2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
  }
#endif
  
  memset (&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;          /* IPv4 and/or IPv6 */
  hints.ai_socktype = SOCK_DGRAM;       /* User Datagram Protocol */
  
  if (getaddrinfo (NULL, "46767", &hints, &res) != 0) {
    fprintf (stderr, "(dgp_init) Error: Unable to get address info.\n");
    return 1;
  }
  
  for (p = res;p != NULL; p = p->ai_next) {
    if (p->ai_family == AF_INET) {
      g_iSocketFileDescriptorIpV4 = socket (p->ai_family, p->ai_socktype, p->ai_protocol);
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      inet_ntop (p->ai_family, &ipv4->sin_addr, ip, sizeof ip);
      printf ("IPv4: %s\n", ip);
    } else if (p->ai_family == AF_INET6) {
      g_iSocketFileDescriptorIpV6 = socket (p->ai_family, p->ai_socktype, p->ai_protocol);
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      inet_ntop (p->ai_family, &ipv6->sin6_addr, ip, sizeof ip);
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
  //WSACleanup ();
#ifdef _MSC_VER
    system ("pause");
#endif
  return -1;
}
