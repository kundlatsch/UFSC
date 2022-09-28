// EPOS PC IC Mediator (dispatch) Implementation

#include <machine/ic.h>
#include <process.h>

__BEGIN_SYS

void IC::dispatch(unsigned int i)
{
    bool not_spurious = true;
    if((i >= INT_FIRST_HARD) && (i <= INT_LAST_HARD))
        not_spurious = eoi(i);
    if(not_spurious) {

        if((i != INT_SYS_TIMER) || Traits<IC>::hysterically_debugged)
            db<IC>(TRC) << "IC::dispatch(i=" << i << ")" << endl;

        _int_vector[i](i);
    } else {
        if(i != INT_LAST_HARD)
            db<IC>(TRC) << "IC::spurious interrupt (" << i << ")" << endl;
    }
}

__END_SYS
