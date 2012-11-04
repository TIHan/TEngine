#include <TELib.hpp>
#include <Network.hpp>
#include "network/UdpSocket.hpp"

using namespace TE;

TEint main()
{
  auto l = make_shared<List<shared_ptr<UdpSocket>>>();
  auto hey = make_shared<UdpSocket>(SOCKET_IPV4);
  l->Add(hey);
  l->Add(hey);
  l->Remove(hey);
  l->Add(make_shared<UdpSocket>(SOCKET_IPV6));
  l->Add(make_shared<UdpSocket>(SOCKET_IPV4));
  auto l2 = l->Where([] (shared_ptr<UdpSocket> i) {return i->GetFamily() == SOCKET_IPV4;});
  l->Clear();
  auto testSeq = make_shared<Sequence<TEbyte>>();
  TEbyte x = 5;
  testSeq->Add(x);
  testSeq->Add(6);
  testSeq->Add(x);
  testSeq->Add(6);
  testSeq->Remove(5);
  testSeq->Clear();
 /* TEint answer = 0;
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
        cout << buffer->data() << endl;
        count++;

        shared_ptr<address_t> asdf = get<1>(tupleReceive);
        shared_ptr<ByteStream> byteStream(new ByteStream(512));
        byteStream->WriteString("PONG");

        cout << "Sending: " << socket->Send(byteStream->GetBuffer(), asdf) << endl;

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
        socket->Send(byteStream->GetBuffer());

        auto tupleReceive = socket->Receive();
        auto buffer = get<0>(tupleReceive);
        cout << buffer->data() << endl;
    } else {
      cout << "Unable to send.\n";
    }
    break;
  }*/
#ifdef _MSC_VER
    system("pause");
#endif
  return 0;
}
