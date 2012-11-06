// Copyright 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <iostream>
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char **argv) {
  std::cout << "Running Tests\n";

  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
#ifdef _MSC_VER
  system("pause");
#endif
  return 0;
}

// [WS] Keep this here for now. :)
/*
  TEint answer = 0;
  do {
    cout << "(1) to host. (2) to connect.\n";
    cin >> answer;
  } while (answer != 1 && answer != 2);

  switch (answer) {
  case 1:
    {
      TEushort port;
      cout << "Use port: ";
      cin >> port;
      TEint count = 0;

      shared_ptr<UdpSocket> socket(new UdpSocket(SOCKET_IPV4));
      cout << "Address: " << socket->GetAddress() << endl;
      if (socket->Bind(port) == -1) {
        cout << "Unable to bind.\n";
      }
      while (1 && !socket->HasErrors()) {
        auto tupleReceive = socket->Receive();
        auto buffer = get<0>(tupleReceive);
        cout << buffer->GetRawByteData() << endl;
        count++;

        shared_ptr<address_t> asdf = get<1>(tupleReceive);
        shared_ptr<ByteStream> byteStream(new ByteStream(512));
        byteStream->WriteString("PONG");

        cout << "Sending: " << socket->Send(byteStream, asdf) << endl;

        cout << "Packet Number: " << count << endl;
      }
    }
    break;
  case 2:
    string address;
    cout << "Enter address: ";
    cin >> address;

    string port;
    cout << "Enter port: ";
    cin >> port;

    shared_ptr<ByteStream> byteStream(new ByteStream(512));
    byteStream->WriteString("PING");

    shared_ptr<UdpSocket> socket(new UdpSocket(address, port));
    cout << "Address: " << socket->GetAddress() << endl;

    if (!socket->HasErrors()) {
        socket->Send(byteStream);

        auto tupleReceive = socket->Receive();
        auto buffer = get<0>(tupleReceive);
        cout << buffer->GetRawByteData() << endl;
    } else {
      cout << "Unable to send.\n";
    }
    break;
  }
*/