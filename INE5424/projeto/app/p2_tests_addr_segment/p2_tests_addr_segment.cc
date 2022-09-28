#include <time.h>
#include <process.h>
#include <machine.h>
#include <system.h>

using namespace EPOS;

int print_inf();
int func_a();

OStream cout;

int main()
{

    cout << "-----------------------------" << endl;
    cout << "Addres Space and Segment Test" << endl;
    cout << "-----------------------------" << endl;
    
    System_Info * si = System::info();
    cout << "System_Info app location" << endl;
    cout << "App code segment = " << CPU::Phy_Addr(si->pmm.app_code) << endl;
    cout << "App data segment = " << CPU::Phy_Addr(si->pmm.app_data) << endl;

    Task * task = Task::self();
    Address_Space * as = task->address_space();
    cout << "Task address_space pd = " << as->pd() << endl;

    CPU::Log_Addr code = task->code();
    cout << "Testing APP_CODE..." << endl;
    assert(code == Memory_Map::APP_CODE);

    CPU::Log_Addr data = task->data();
    cout << "Testing APP_DATA.." << endl;
    assert(data == Memory_Map::APP_DATA);

    cout << "Finishing test..." << endl;
    
}