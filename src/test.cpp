#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main()
{
  TEuint64 asdf = System::GetTicks();
  for (int i = 0; i < 5; i++) {
  }
  cout << System::GetTicks() - asdf << endl;
#ifdef _MSC_VER
    system("pause");
#endif
  return 0;
}
