#ifndef DGPNETWORK_HPP
#define DGPNETWORK_HPP

#include "types.hpp"

#define MAX_BUFFER_LENGTH 1024

namespace dgp {

  typedef struct sockaddr_in dgpAddress;
  typedef struct sockaddr_in6 dgpAddress6;
  typedef struct addrinfo dgpAddressInfo;

  class network {
    dgpInt m_iFileDescriptorSocket;
    dgpInt m_iFileDescriptorSocket6;
    dgpAddress *m_pAddress;
    dgpAddress6 *m_pAddress6;
    dgpAddressInfo *m_pAddressInfo;

    dgpByte *m_bStream;

    dgpInt close (dgpInt sockfd);

  public:
    network (dgpUshort usPort);
    ~network ();

    void printAddresses ();
  };
}

#endif // DGPNETWORK_HPP