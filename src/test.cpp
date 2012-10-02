#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main ()
{
  INetwork *network = new Network (512);
  network->Host (46767);

  for (int i = 0; i < 100; i++) {
       TEbyte *asdf = new TEbyte[10];
    IPacket *packet = new Packet (asdf, 512, 1, "1", "46767");
    delete asdf;
    delete packet;
  }
  cout << "HAY\n";
  delete network;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
