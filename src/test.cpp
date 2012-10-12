#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main ()
{
  unique_ptr<INetwork> network (new Network (512));
  network->Host (46767);

  TEuint64 asdf = System::GetTicks ();
  for (int i = 0; i < 5000; i++) {
    shared_ptr<ByteStream> byteStream (new ByteStream (sizeof (TEint)));
    byteStream->Write<TEint> (50);
    Packet *packet = new Packet (byteStream);
    network->Send (packet);
    delete packet;
    packet = network->Receive ();
    //cout << packet->GetByteStream ()->Read<TEint> () << endl;
    delete packet;
  }
  cout << System::GetTicks () - asdf << endl;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
