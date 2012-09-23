#ifndef __NETWORK_HPP_
#define __NETWORK_HPP_

#include "Types.hpp"
#include "Socket.hpp"
#include "ByteStream.hpp"

namespace TE {
  class INetwork {
  public:
    virtual ~INetwork () {};
    virtual void PrintAddresses () = 0;
    virtual void Connect (const TEchar *szAddress, const TEchar *szPort) = 0;
  };

  class Network : public INetwork {
    ISocket *m_pSocket;
    IByteStream *m_pByteStream;

  public:
    explicit Network (const TEushort usPort);
    ~Network ();

    void PrintAddresses ();
    void Connect (const TEchar *szAddress, const TEchar *szPort);
  };
}

#endif /* __NETWORK_HPP_ */