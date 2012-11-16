/*
  Copyright (c) 2012, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ASocket.hpp"
#include "PASocket.hpp"

/*!
 *
 */
static int CreateSocket(const int& domain,
    const int& type,
    const int& protocol) {
  return static_cast<int>(socket(domain, type, protocol));
}

/*!
 *
 */
static int BindSocket(const int& sockfd,
    const struct sockaddr* const& my_addr,
    const int& addrlen) {
  return bind(sockfd, my_addr, addrlen);
}

/*!
 *
 */
static int CloseSocket(const int& sockfd) {
#ifdef __GNUC__
    return close(sockfd);
#elif _MSC_VER
    return closesocket(sockfd);
#endif
}

/*!
 *
 */
static std::string GetSocketAddress(const struct sockaddr_storage* const& pAddr) {
    if (pAddr) {
      std::string address(INET6_ADDRSTRLEN, '\0');

      switch (pAddr->ss_family) {
      case AF_INET6:
        inet_ntop(pAddr->ss_family, &reinterpret_cast<struct sockaddr_in6*>(const_cast<struct sockaddr_storage*>(pAddr))->sin6_addr,
          reinterpret_cast<char*>(const_cast<char*>(address.data())), INET6_ADDRSTRLEN);
        break;
      default:
        inet_ntop(pAddr->ss_family, &reinterpret_cast<struct sockaddr_in*>(const_cast<struct sockaddr_storage*>(pAddr))->sin_addr,
          reinterpret_cast<char*>(const_cast<char*>(address.data())), INET_ADDRSTRLEN);
        break;
      }
      return address.data();
    } else {
      return TE::String::Empty();
    }
}

namespace TE {
  namespace Socket {
    string GetAddress(const shared_ptr<const address_t>& address) {
      return GetSocketAddress(&address->ssAddress);
    }
  }
  /*!
   *
   */
  void PASocket::Open(const unsigned char& bSocketType,
      const unsigned char& bFamily,
      const unsigned char& bFlags,
      const string& szNodeName,
      const string& szServiceName) {
    m_iSocket = -1;
    m_pAddress = 0;

#ifdef _MSC_VER
    WSADATA wsaData;
    int wsaResult;

    // Initialize Winsock
    wsaResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    ERROR_IF_FORMAT(wsaResult != 0, "WSAStartup failed with result %i", wsaResult)
#endif

    struct addrinfo hints, *p;
    const char* nodeName = nullptr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = bFamily;
    hints.ai_socktype = bSocketType;
    hints.ai_flags = bFlags;

    // If the node name is empty, only pass 0 to getaddrinfo.
    if (!szNodeName.empty()) {
      nodeName = static_cast<const char*>(szNodeName.c_str());
    }

    if (getaddrinfo(nodeName, szServiceName.c_str(), &hints, &m_pAddressInfo) != 0) {
      throw runtime_error("Unable to get address information");
    }

    for (p = m_pAddressInfo; p != 0; p = p->ai_next) {
      // If the family is unspecified, get the first one in the linked list.
      if (p->ai_family == bFamily || bFamily == AF_UNSPEC) {
        m_iSocket = CreateSocket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (m_iSocket != -1) {
          m_bFamily = p->ai_family;
          m_pAddress = p;
          break;
        }
      }
    }

    if (m_iSocket == -1) {
      throw runtime_error("Unable to create socket");
    }
  }

  /*!
   *
   */
  void PASocket::SetFamily(const SocketFamily& family) {
    switch (family) {
    case SOCKET_UNSPECIFIED:
      m_bFamily = AF_UNSPEC;
      break;
    case SOCKET_IPV4:
      m_bFamily = AF_INET;
      break;
    case SOCKET_IPV6:
      m_bFamily = AF_INET6;
      break;
    default:
      throw invalid_argument("Invalid socket family");
    }
  }

  /*!
   *
   */
  ASocket::ASocket() :
      priv(make_unique<PASocket>()) {
  }

  /*!
   *
   */
  ASocket::~ASocket() {
    Close();
  }

  /*!
   *
   */
  int ASocket::Bind(const unsigned short& usPort) {
    if (priv->m_iSocket != -1 && priv->m_pAddress) {
      struct sockaddr_in* sockaddr = reinterpret_cast<struct sockaddr_in*>(priv->m_pAddress->ai_addr);

      sockaddr->sin_port = ntohs(usPort);
      if (BindSocket(priv->m_iSocket, priv->m_pAddress->ai_addr, static_cast<int>(priv->m_pAddress->ai_addrlen)) != 0) {
        return -1;
      }
      return 0;
    }
    return -1;
  }

  /*!
   *
   */
  string ASocket::GetAddress() {
    return GetSocketAddress(reinterpret_cast<struct sockaddr_storage*>(priv->m_pAddressInfo->ai_addr));
  }

  /*!
   *
   */
  SocketFamily ASocket::GetFamily() {
    switch (priv->m_bFamily) {
    case AF_UNSPEC:
      return SOCKET_UNSPECIFIED;
    case AF_INET:
      return SOCKET_IPV4;
      break;
    case AF_INET6:
      return SOCKET_IPV6;
    default:
      throw logic_error("Invalid socket family");
    }
  }

  /*!
   *
   */
  void ASocket::Close() {
    if (priv->m_pAddressInfo) {
      freeaddrinfo(priv->m_pAddressInfo);
    }

    if (priv->m_iSocket != -1) {
      CloseSocket(priv->m_iSocket);
    }
  }
}
