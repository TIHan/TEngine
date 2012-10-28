#include <TELib.hpp>
#include <Network.hpp>
#include "network/UdpSocket.hpp"

using namespace TE;

TEint main()
{
  TEint answer = 0;
  TEuint64 hey = 18446744073709551615;
  List<TEuint64> *asdf = new List<TEuint64>();
  asdf->Add(hey);
  TEuint64 dick = asdf->Find(hey);
  cout << dick << endl;
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
        cout << buffer << endl;
        count++;

        shared_ptr<address_t> asdf = get<2>(tupleReceive);
        shared_ptr<ByteStream> byteStream(new ByteStream(512));
        byteStream->WriteString("PONG");

        cout << "Sending: " << socket->Send(byteStream->GetBuffer(), byteStream->GetSize(), asdf) << endl;

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
        socket->Send(byteStream->GetBuffer(), byteStream->GetSize());

        auto tupleReceive = socket->Receive();
        auto buffer = get<0>(tupleReceive);
        cout << buffer << endl;
    } else {
      cout << "Unable to send.\n";
    }
    break;
  }
#ifdef _MSC_VER
    system("pause");
#endif
  return 0;
}
