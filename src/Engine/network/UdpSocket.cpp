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

#include "UdpSocket.hpp"
#include "PASocket.hpp"

namespace TE {
  /*!
   *
   */
  UdpSocket::UdpSocket(const SocketFamily& family) {
    priv->SetFamily(family);
  }

  /*!
   *
   */
  UdpSocket::UdpSocket() {
    priv->SetFamily(SOCKET_UNSPECIFIED);
  }

  /*!
   *
   */
  UdpSocket::~UdpSocket() {
  }

  /*!
   *
   */
  void UdpSocket::Open() {
    priv->Open(SOCK_DGRAM, priv->m_bFamily, AI_PASSIVE, String::Empty(), String::Empty());
  }

  /*!
   *
   */
  void UdpSocket::Open(const string& szAddress, const string& szPort) {
    priv->Open(SOCK_DGRAM, priv->m_bFamily, AI_PASSIVE, szAddress, szPort);
  }

  /*!
   *
   */
  tuple<shared_ptr<ByteSequence>, shared_ptr<address_t>> UdpSocket::ReceiveFrom() {
    struct sockaddr_storage sock_addr;
    socklen_t addr_len = sizeof(sock_addr);
    auto pBuffer = make_shared<ByteSequence>(SOCKET_MAX_BUFFER);

    TEint bytes = recvfrom(priv->m_iSocket, reinterpret_cast<TEchar*>(const_cast<TEbyte*>(pBuffer->GetRawData())),
      pBuffer->GetSize(), 0, reinterpret_cast<sockaddr*>(const_cast<struct sockaddr_storage*>(&sock_addr)), &addr_len);

    if (bytes != -1) {
      pBuffer->ShrinkToSize(bytes);
    } else {
      pBuffer->Clear();
    }

    auto address = make_shared<address_t>(sock_addr, static_cast<TEint>(addr_len));
    return tuple<shared_ptr<ByteSequence>, shared_ptr<address_t>>(pBuffer, address);
  }

  /*!
   *
   */
  TEint UdpSocket::Send(const shared_ptr<const IByteData>& pByteData) {
    TEint bytes = sendto(priv->m_iSocket, reinterpret_cast<TEchar*>(const_cast<TEbyte*>(pByteData->GetRawByteData())),
      pByteData->GetByteDataSize(), 0, priv->m_pAddress->ai_addr, static_cast<TEint>(priv->m_pAddress->ai_addrlen));
    return bytes;
  }

  /*!
   *
   */
  TEint UdpSocket::SendTo(const shared_ptr<const IByteData>& pByteData,
      const shared_ptr<const address_t>& address) {
    TEint bytes = sendto(priv->m_iSocket, reinterpret_cast<const TEchar*>(const_cast<const TEbyte*>(pByteData->GetRawByteData())),
      pByteData->GetByteDataSize(), 0, reinterpret_cast<sockaddr*>(const_cast<struct sockaddr_storage*>(&address->ssAddress)), address->nLength);
    return bytes;
  }
}