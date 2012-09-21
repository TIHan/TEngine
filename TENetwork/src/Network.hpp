#ifndef __NETWORK_HPP_
#define __NETWORK_HPP_

#include "Types.hpp"
#include "Socket.hpp"
#include "ByteStream.hpp"

namespace TE {
  class INetwork {
  public:
    virtual void PrintAddresses () = 0;
    virtual void Connect (const TEchar *szAddress, const TEchar *szPort) = 0;
  };

  class Network : INetwork {
    Socket *m_pSocket;
    ByteStream *m_pByteStream;

  public:
    Network (TEushort usPort);
    ~Network ();

    void PrintAddresses ();
    void Connect (const TEchar *szAddress, const TEchar *szPort);
  };
}

#endif /* __NETWORK_HPP_ */