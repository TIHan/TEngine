#include <stdio.h>
#include <iostream>
#include "dgp.hpp"

using namespace dgp;

dgpInt main ()
{
  network *network = new dgp::network (46767);
  network->printAddresses ();
  delete network;
  //WSACleanup ();
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
