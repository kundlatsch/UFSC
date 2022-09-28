// EPOS Buffer Declarations

#ifndef __buffer_h
#define __buffer_h

#include <architecture.h>

__BEGIN_UTIL

// This Buffer was designed to move data across a zero-copy communication stack, but can be used for several other purposes
template<typename Owner, typename Data, typename Shadow = void, typename _Metadata = Dummy<>>
class Buffer: private Data, public _Metadata
{
public:
    using Frame = Data;
    using Packet = Data;
    using Message = Data;
    using Metadata = _Metadata;

    typedef Simple_List<Buffer<Owner, Data, Shadow, Metadata> > List;
    typedef typename List::Element Element;

public:
    Buffer(Owner * o, Shadow * s): _lock(false), _owner(o), _shadow(s), _size(sizeof(Data)), _link1(this), _link2(this) {}

    template<typename ... Tn>
    void fill(unsigned int s, Tn ... an) {
        _size = s;
        Data data(an ...);
        memcpy(this, &data, s);
    }

    Data * data() { return this; }
    Data * frame() { return data(); }
    Data * message() { return data(); }

    bool lock() { return !CPU::tsl(_lock); }
    void unlock() { _lock = 0; }

    Owner * owner() const { return _owner; }
    Owner * nic() const { return owner(); }
    void owner(Owner * o) { _owner = o; }
    void nic(Owner * o) { owner(o); }

    Shadow * shadow() const { return _shadow; }
    Shadow * back() const { return shadow(); }

    unsigned int size() const { return _size; }
    void size(unsigned int s) { _size = s; }

    Element * link1() { return &_link1; }
    Element * link() { return link1(); }
    Element * lint() { return link1(); }
    Element * link2() { return &_link2; }
    Element * lext() { return link2(); }

    friend Debug & operator<<(Debug & db, const Buffer & b) {
        db << "{md=" << b._owner << ",lk=" << b._lock << ",sz=" << b._size << ",sd=" << b._shadow << "}";
        return db;
    }

private:
    volatile bool _lock;
    Owner * _owner;
    Shadow * _shadow;
    unsigned int _size;
    Element _link1;
    Element _link2;
};

// Circular Buffer
template<typename T, unsigned int N_ELEMENTS>
class Circular_Buffer
{
private:
    static const unsigned int SIZE = N_ELEMENTS * sizeof(T);

public:
    typedef T Object_Type;

public:
    Circular_Buffer(): _size(0), _head(0), _tail(0) {}
    Circular_Buffer(const void * data, unsigned int size) { copy_and_pad(data, size); }
    template <typename U>
    Circular_Buffer(const U & o) { copy_and_pad(&o, sizeof(U)); }

    unsigned int size() const { return _size; }
    bool empty() const { return (_size == 0); }
    bool full()  { return (_tail + 1) % N_ELEMENTS == _head ? true : false; }

    Object_Type & operator[](const size_t i) {  assert(i < N_ELEMENTS); return _data[(_head + i) % N_ELEMENTS]; }
    const Object_Type & operator[](const size_t i) const {  assert(i < N_ELEMENTS); return _data[(_head + i) % N_ELEMENTS]; }
    operator const Object_Type *() const { return &_data[_head]; }
    operator Object_Type *() { return &_data[_head]; }

    Object_Type & head() { return _data[_head]; }
    const Object_Type & head() const { return _data[_head]; }
    Object_Type & tail() { return _data[_tail]; }
    const Object_Type & tail() const { return _data[_tail]; }

    void insert(const Object_Type & o, unsigned int i) {
        if(!_data[i])
            _size++;
        _data[i] = o;
    }

    void insert(const Object_Type & o) {
        if(full())
            _head = (_head + 1) % N_ELEMENTS;
        if(!empty())
            _tail = (_tail + 1) % N_ELEMENTS;
        if(!_data[_tail])
            _size++;
        _data[_tail] = o;
    }

    Object_Type remove() {
        if(_data[_head]) {
            Object_Type o = _data[_head];
            _data[_head] = 0;
            _size--;
            return o;
        }
        return 0;
    }

    size_t search(const Object_Type & obj) {
        size_t i = 0;
        for(; i < N_ELEMENTS; i++)
            if(_data[i] == obj)
                break;
        return i;
    }


private:
    void copy_and_pad(const void * data, unsigned int size) {
        if(SIZE <= size)
            memcpy(_data, data, SIZE);
        else {
            memset(_data, 0, SIZE);
            memcpy(_data, data, size);
        }
    }

    void reset() {
        _size = _head = _tail = 0;
    }

private:
    unsigned int _size;
    unsigned int _head;
    unsigned int _tail;
    T _data[N_ELEMENTS];
};
__END_UTIL

#endif
