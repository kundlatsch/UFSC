// EPOS Synchronizer Component Test Program

#include <machine.h>
#include <time.h>
#include <synchronizer.h>
#include <process.h>

using namespace EPOS;

const int iterations = 128;

OStream cout;

const int BUF_SIZE = 16;
char buffer[BUF_SIZE];
Semaphore empty(BUF_SIZE);
Semaphore full(0);

int consumer()
{
    int out = 0;
    for(int i = 0; i < iterations; i++) {
        full.p();
        cout << "C<-" << buffer[out] << " ";
        out = (out + 1) % BUF_SIZE;
        Alarm::delay(100000);
        empty.v();
    }

    return 0;
}

int main()
{
    cout << "Producer x Consumer" << endl;

    Thread * cons = new Thread(&consumer);

    // producer
    int in = 0;
    for(int i = 0; i < iterations; i++) {
        empty.p();
        Alarm::delay(100000);
        buffer[in] = 'a' + in;
        cout << "P->" << buffer[in] << " ";
        in = (in + 1) % BUF_SIZE;
        full.v();
    }

    cons->join();

    cout << "The end!" << endl;

    delete cons;

    return 0;
}
