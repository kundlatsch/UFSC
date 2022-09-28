#include <time.h>
#include <process.h>
#include <machine.h>
#include <utility/fork.h>
#include <system.h>

using namespace EPOS;

int print_inf();
int func_a();

OStream cout;

int main()
{
    if (Task::self()->id() == 0) {
        cout << "-----------------------" << endl;
        cout << "Task Tests" << endl;
        cout << "-----------------------" << endl;
        fork(&main);
    }
    if (Task::self()->id() == 1) {
        cout << "    -> Forked from main function " << endl;
        int t_id = Task::self()->id();
        assert(t_id == 1);
        return 0;
    }

    Address_Space * as = Task::self()->address_space();
    cout << "Address_Space pd = " << as->pd() << endl;

    cout << "Task code addr = " << static_cast<void *>(Task::self()->code()) << endl;
    cout << "Address_Space code physical addr = " << static_cast<void *>(as->physical(Task::self()->code())) << endl;
    cout << "code_segment size = " << Task::self()->code_segment()->size() << endl;

    cout << "Task data addr = " << static_cast<void *>(Task::self()->data()) << endl;
    cout << "Address_Space data physical addr = " << static_cast<void *>(as->physical(Task::self()->data())) << endl;
    cout << "data_segment size = " << Task::self()->data_segment()->size() << endl;

    cout << "Finishing test..." << endl;
    return 0;
}
