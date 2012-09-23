#include <stdio.h>
#include <iostream>

#include "TENetwork.hpp"

using namespace TE;

#define DEFAULT_PORT 4767

TEint main ()
{
    Network *network = new Network ();
    delete network;
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
