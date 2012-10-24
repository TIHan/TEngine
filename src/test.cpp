#include <TELib.hpp>
#include <Network.hpp>
#include "network/Socket.hpp"

using namespace TE;

TEint main()
{
  TEuint asdf = System::GetTicks();
  auto byteStream = new ByteStream(512);
  byteStream->WriteString("Hello");
  auto socket = new Socket(SOCKET_IPV4, "");
  socket->Bind(5775);
  socket->Send(byteStream->GetBuffer(), byteStream->GetSize());
  socket->Receive();
  cout << socket->GetAddress() << endl;
  cout << System::GetTicks() - asdf << endl;
#ifdef _MSC_VER
    system("pause");
#endif
  return 0;
}
