#include <stdio.h>
#include <iostream>

#include "dgp.hpp"

using namespace dgp;

#define DEFAULT_PORT 46767

dgpInt main ()
{
  network *network = new dgp::network (DEFAULT_PORT);
  network->printAddresses ();
#ifdef _MSC_VER
    system ("pause");
#endif
  return 0;
}
