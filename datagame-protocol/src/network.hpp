#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "types.hpp"
#include "socket.hpp"
#include "bytestream.hpp"

namespace dgp {
  class network {
    socket *m_pSocket;
    byteStream *m_pByteStream;

  public:
    network (dgpUshort usPort);
    ~network ();

    void printAddresses ();
    void connect (const dgpChar *szAddress, const dgpChar *szPort);
  };
}

#endif // NETWORK_HPP