#include <utility/ostream.h>
#include <real-time.h>

using namespace EPOS;

OStream cout;

int testing();

int main()
{
    Periodic_Thread::Configuration conf_a{5000, 1000, Periodic_Thread::SAME, Periodic_Thread::UNKNOWN, 10, Thread::READY, Periodic_Thread::NORMAL, 16 * 1024};
    Periodic_Thread::Configuration conf_b{1500, 500, Periodic_Thread::SAME, Periodic_Thread::UNKNOWN, 10, Thread::READY, Periodic_Thread::NORMAL, 16 * 1024};
    Periodic_Thread::Configuration conf_c{25000, 5000, Periodic_Thread::SAME, Periodic_Thread::UNKNOWN, 10, Thread::READY, Periodic_Thread::NORMAL, 16 * 1024};

    Periodic_Thread * a = new Periodic_Thread(conf_a, &testing);
    Periodic_Thread * b = new Periodic_Thread(conf_b, &testing);
    Periodic_Thread * c = new Periodic_Thread(conf_c, &testing);

    a -> join();
    b -> join();
    c -> join();


    delete a;
    delete b;
    delete c;

    cout << "Finishing test..." << endl;

    return 0;
}

int testing() {
  int i = 0;
  while ((i < 10) and Periodic_Thread::wait_next()) // iterator stops infinite runs.
  {
    cout << "Job finished\n" << endl;
    i++;
  }

  return 0;
}