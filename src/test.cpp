#include <TELib.hpp>
#include <Network.hpp>
#include "network/UdpSocket.hpp"

using namespace TE;

TEint main()
{
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

      shared_ptr<ByteStream> byteStream(new ByteStream(512));
      shared_ptr<UdpSocket> socket(new UdpSocket(SOCKET_IPV4, String::Empty()));
      if (socket->Bind(port) == -1) {
        cout << "Unable to bind.\n";
      }
      while (1 && !socket->HasErrors()) {
        auto tupleReceive = socket->Receive();
        auto buffer = get<0>(tupleReceive);
        cout << buffer << endl;
        count++;
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

    string text;
    cout << "Send a message: ";
    cin >> text;

    shared_ptr<ByteStream> byteStream(new ByteStream(512));
    byteStream->WriteString(text);
    shared_ptr<UdpSocket> socket(new UdpSocket(SOCKET_UNSPEC, address, port));
    if (!socket->HasErrors()) {
        socket->Send(byteStream->GetBuffer(), byteStream->GetSize());
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
