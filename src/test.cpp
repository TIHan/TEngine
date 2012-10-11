#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main ()
{
  INetwork *network = new Network (512);
  network->Host (46767);

  TEuint64 asdf = System::GetTicks ();
  for (int i = 0; i < 5000; i++) {
    ByteStream *byteStream = new ByteStream (512);
    byteStream->Write<TEint> (50);
    Packet *packet = new Packet (byteStream);
    network->Send (packet);
    delete packet;
    packet = network->Receive ();
    delete packet;
  }
  cout << System::GetTicks () - asdf << endl;
  delete network;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
