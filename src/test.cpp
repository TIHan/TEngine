#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main ()
{
  INetwork *network = new Network (512);
  network->Host (46767);

  for (int i = 0; i < 100; i++) {
    IPacket *packet = new Packet (512, "127.0.0.1", "46767");
    packet->Write<TEint> (50);
    packet->Read<TEint> ();
    delete packet;
  }
  cout << "HAY\n";
  delete network;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
