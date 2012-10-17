#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main ()
{
  /*unique_ptr<INetwork> network (new Network (512));
  network->Host (46767);*/
  unique_ptr<Server> server (new Server (512, "", "5666"));

  TEuint64 asdf = System::GetTicks ();
  for (int i = 0; i < 5; i++) {
    /*shared_ptr<ByteStream> byteStream (new ByteStream (sizeof (TEint)));
    byteStream->WriteString ("HI");
    shared_ptr<Packet> packet (new Packet (byteStream));
    network->Send (packet);
    string ip;
    packet = network->Receive (ip);
    cout << packet->GetByteStream ()->ReadString () << endl;*/
    shared_ptr<ByteStream> byteStream (new ByteStream (sizeof (TEint)));
    byteStream->WriteString ("HI");
    shared_ptr<Packet> packet (new Packet (byteStream));
    server->Send (packet);
    server->Receive ();
  }
  cout << System::GetTicks () - asdf << endl;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
