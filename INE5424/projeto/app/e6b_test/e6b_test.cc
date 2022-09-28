#include <utility/ostream.h>
#include <process.h>

using namespace EPOS;

OStream cout;


char * test_char = new (SHARED) char('a');

int test1_b() {
    *test_char = 'b';
    return 0;
}

int test1_c() {
    *test_char = 'c';
    return 0;
}

int main() {

    cout << "-----------------------" << endl;
    cout << "Shared memory test" << endl;
    cout << "-----------------------" << endl;

    // Test 1: create a global variable in the shared memory 
    // and manipulate it with two different threads
    Thread * test1_b_thread = new Thread(&test1_b);
    Thread * test1_c_thread = new Thread(&test1_c);

    test1_b_thread->join();
    test1_c_thread->join();

    cout << "Asserting test char value" << endl;
    assert(*test_char != 'a');
    assert(*test_char != 'b');
    assert(*test_char == 'c');

    delete test_char;

    // Test 2: after deleting a variable from the shared memory,
    // create another one and change it's value
    int * test_int = new (SHARED) int;
    *test_int = 0;
    *test_int += 1;

    cout << "Asserting test int value" << endl;
    assert(*test_int != 0);
    assert(*test_int == 1);
    assert(*test_int != 2);

    delete test_int;

    // Test 3: create and change the value of a variable
    // using the default and the shared new operator

    cout << "Asserting values of different operators" << endl;

    int * test_shared_new = new (SHARED) int;
    int * test_default_new = new int;
    
    *test_shared_new = 0;
    *test_default_new = 1;

    ++*test_shared_new;
    ++*test_default_new;

    assert(*test_shared_new == 1);
    assert(*test_default_new == 2);

    delete test_shared_new;
    delete test_default_new;
    
    cout << "Finishing test..." << endl;
}