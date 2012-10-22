#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main()
{
  TEuint asdf = System::GetTicks();
  for (int i = 0; i < 5; i++) {
    unique_ptr<Thread> thread (new Thread([&] { cout << "Hello " << i + 1 << "\n"; }));
  }
  cout << System::GetTicks() - asdf << endl;
#ifdef _MSC_VER
    system("pause");
#endif
  return 0;
}
