#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main ()
{
  INetwork *network = new Network (512);
  network->Host (46767);

  TEuint64 asdf = System::GetTickCount ();
  for (int i = 0; i < 5000; i++) {
    IPacket *packet = new Packet (512, "127.0.0.1", "46767");
    packet->Write<TEint> (50);
    network->Send (packet);
    delete packet;
    packet = network->Receive ();
    delete packet;
  }
  cout << System::GetTickCount () - asdf << endl;
  delete network;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
