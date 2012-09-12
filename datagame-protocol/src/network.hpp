#ifndef DGPNETWORK_HPP
#define DGPNETWORK_HPP

#include "types.hpp"

#define MAX_BUFFER_LENGTH 1024

namespace dgp {

  typedef struct sockaddr_in dgpAddress;
  typedef struct sockaddr_in6 dgpAddress6;

  class network {
    dgpInt m_iFileDescriptorSocket;
    dgpInt m_iFileDescriptorSocket6;
    dgpAddress *m_pAddress;
    dgpAddress6 *m_pAddress6;

    dgpByte *m_bStream;

    dgpInt closeSocket (dgpInt sockfd);

  public:
    network (dgpUshort usPort);
    ~network ();

    void printAddresses ();
  };
}

#endif // DGPNETWORK_HPP