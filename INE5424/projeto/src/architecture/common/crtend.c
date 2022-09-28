void _init() __attribute__ ((section(".init")));

typedef void (*fptr)(void);

static fptr __CTOR_END__[1] __attribute__((section(".init_array"))) = { (fptr)0 };
static fptr __DTOR_END__[1] __attribute__((used, section(".fini_array"))) = { (fptr)0 };

static void __do_global_ctors_aux()
{
    fptr * p;
    for(p = __CTOR_END__ - 1; *p != (fptr) -1; p--)
        if(p)
            (*p)();
}

void _init()
{
    __do_global_ctors_aux();
}
