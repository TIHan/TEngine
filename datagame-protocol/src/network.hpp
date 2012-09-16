#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "types.hpp"
#include "socket.hpp"

#define MAX_BUFFER_LENGTH 1024

namespace dgp {

  class network {
    socket *m_pSocket;
    dgpByte *m_bStream;

  public:
    network (dgpUshort usPort);
    ~network ();

    void printAddresses ();
  };
}

#endif // NETWORK_HPP