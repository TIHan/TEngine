#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main ()
{
   // Network *network = new Network ();
  for (int i = 0; i < 50000; i++) {
    Packet *packet = new Packet (new ByteStream (512), "192.168.1.1", "888");
    delete packet;
  }
    //delete network;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
