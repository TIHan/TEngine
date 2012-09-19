#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "types.hpp"
#include "socket.hpp"

#define MAX_BUFFER_LENGTH 1024

namespace dgp {

  class network {
    socket *m_pSocket;
    dgpByte *m_bStream;
    size_t m_streamSize;
    dgpByte *m_bReceiveStream;

  public:
    network (dgpUshort usPort);
    ~network ();

    void printAddresses ();
    void connect (const dgpChar *szAddress, const dgpChar *szPort);
  };
}

#endif // NETWORK_HPP