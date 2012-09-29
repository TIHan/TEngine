#include <TELib.hpp>
#include <Network.hpp>
//#include <windows.h>

using namespace TE;

TEint main ()
{
    //Network *network = new Network ();
  Network *network = new Network (512);
  network->Host (46767);
  while (1)
  {
    cin.get ();
 // TEuint count = (TEuint)GetTickCount ();
  for (int i = 0; i < 100; i++) {
    IByteStream *stream = new ByteStream (512);
    stream->WriteString ("CONNECT");

    IPacket *packet = new Packet (stream, "127.0.0.1", "46767");
    network->Send (packet);
    delete packet;
    packet = network->Receive ();
    delete packet;
  }
  cout << "HAY\n";
  //  cout << GetTickCount () - count << endl;
  }
  delete network;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
