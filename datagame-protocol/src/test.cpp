#include <stdio.h>
#include <iostream>

#include "dgp.hpp"

using namespace dgp;

#define DEFAULT_PORT 4767

dgpInt main ()
{
  network *network = new dgp::network (DEFAULT_PORT);
  network->printAddresses ();
  delete network;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
