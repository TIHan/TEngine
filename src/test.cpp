#include <TELib.hpp>
#include <Network.hpp>

using namespace TE;

TEint main()
{
  //TEuint asdf = System::GetTicks();
  //cout << System::GetTicks() - asdf << endl;
  unique_ptr<Thread> thread(new Thread([=] { cout << "Thread 1\n"; }));
  unique_ptr<Thread> thread2(new Thread([=] { cout << "Thread 2\n"; }));
  unique_ptr<Thread> thread3(new Thread([=] { cout << "Thread 3\n"; }));
  unique_ptr<Thread> thread4(new Thread([=] { cout << "Thread 4\n"; }));
#ifdef _MSC_VER
    system("pause");
#endif
  return 0;
}
