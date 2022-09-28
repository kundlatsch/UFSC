// EPOS Vector Utility Declarations

#ifndef __vector_h
#define	__vector_h

#include "list.h"

__BEGIN_UTIL

// Vector
template<typename T, unsigned int N_ELEMENTS, typename El = List_Elements::Pointer<T>>
class Vector
{
public:
    typedef T Object_Type;
    typedef El Element;

public:
    Vector(): _size(0) {
        for(unsigned int i = 0; i < N_ELEMENTS; i++)
            _vector[i] = 0;
    }

    unsigned int size() const { return _size; }
    bool empty() const { return (_size == 0); }

    Element * operator[](unsigned int i) const { assert(i < N_ELEMENTS); return _vector[i]; }

    bool insert(Element * e, unsigned int i) {
        if(_vector[i])
            return false;

        _vector[i] = e;
        _size++;
        return true;
    }

    Element * remove(unsigned int i) {
        if(_vector[i]) {
            Element * e = _vector[i];
            _vector[i] = 0;
            _size--;
            return e;
        }
        return 0;
    }
    Element * remove(Element * e) {
        for(unsigned int i = 0; i < N_ELEMENTS; i++)
            if(_vector[i] == e) {
        	_vector[i] = 0;
        	_size--;
        	return e;
            }
        return 0;
    }
    Element * remove(const Object_Type * obj) {
        for(unsigned int i = 0; i < N_ELEMENTS; i++)
            if(_vector[i]->object() == obj) {
        	Element * e = _vector[i];
        	_vector[i] = 0;
        	_size--;
        	return e;
            }
        return 0;
    }

    Element * search(const Object_Type * obj) {
        for(unsigned int i = 0; i < N_ELEMENTS; i++)
            if(_vector[i]->object() == obj)
        	return _vector[i];
        return 0;
    }

protected:
    unsigned int _size;
    Element * _vector[N_ELEMENTS];
};

__END_UTIL

#endif
