#include <TELib.hpp>
#include <Network.hpp>
#include "network/UdpSocket.hpp"

using namespace TE;

TEint main()
{
  TEuint asdf = System::GetTicks();
  shared_ptr<ByteStream> byteStream(new ByteStream(512));
  byteStream->WriteString("Hello");
  shared_ptr<UdpSocket> socket(new UdpSocket(SOCKET_IPV4, ""));
  socket->Bind(5776);
  socket->Send(byteStream->GetBuffer(), byteStream->GetSize());
  socket->Receive();
  cout << socket->GetAddress() << endl;
  cout << System::GetTicks() - asdf << endl;
#ifdef _MSC_VER
    system("pause");
#endif
  return 0;
}
