// EPOS Binding Utility Declarations

#ifndef __binding_h
#define	__binding_h

#include <system/config.h>
#include "hash.h"

__BEGIN_UTIL

template<typename A, typename Key_A, typename B, typename Key_B, unsigned int ENTRIES = 10>
class Binding
{
private:
    typedef Simple_Hash<Binding, ENTRIES, Key_A> Bindings;
    typedef typename Bindings::Element Element;

public:
    Binding(A * a, const Key_A & ka, B * b, const Key_B & kb): _a(a), _b(b), _ka(ka), _kb(kb), _link(this, ka) {
        db<Lists>(TRC) << "Binding::Binding(a=" << a << ",ka=" << ka << ",b=" << b << ",kb=" << kb << ")" << endl;
        _bindings.insert(&_link);
    }

    bool bound() const { return _a && _b; }

    A * a() { return _a; }
    B * b() { return _b; }
    Key_A key() { return _ka; }
    Key_A key_a() { return _ka; }
    Key_B key_b() { return _kb; }

    static bool rebind(const Key_A & oka, const Key_A & nka) {
        db<Lists>(TRC) << "Binding::rebind(old=" << oka << ",new=" << nka << ")" << endl;
        for(typename Bindings::Iterator it = _bindings.begin(); it != _bindings.end(); it++) {
            if(it->object()->_ka == oka) {
                _bindings.remove(&it->object()->_link);
                it->object()->_ka = nka;
                _bindings.insert(&it->object()->_link);
                return true;
            }
        }
        return false;
    }

    static bool unbind(const Key_A & ka) {
        db<Lists>(TRC) << "Binding::unbind(key=" << ka << ")" << endl;
        for(typename Bindings::Iterator it = _bindings.begin(); it != _bindings.end(); it++) {
            if(it->object()->bound() && (it->object()->_ka = ka)) {
                it->object()->_ka = 0;
                return true;
            }
        }
        return false;
    }

    static Binding * get_by_key(const Key_A & ka) {
        db<Lists>(TRC) << "Binding::get_by_key(key=" << ka << ")" << endl;
        for(typename Bindings::Iterator it = _bindings.begin(); it != _bindings.end(); it++)
            if(it->object()->_a && (it->object()->_ka == ka))
                return it->object();
        return 0;
    }

    static Binding * get_by_key_a(const Key_A & ka) { return get_by_key(ka); }

    static Binding * get_by_key_b(const Key_B & kb) {
        for(typename Bindings::Iterator it = _bindings.begin(); it != _bindings.end(); it++)
            if(it->object()->_b && (it->object()->_kb == kb))
                return it->object();
        return 0;
    }

private:
    A * _a;
    B * _b;
    Key_A _ka;
    Key_B _kb;
    Element _link;

    static Bindings _bindings;
};

template<typename A, typename Key_A, typename B, typename Key_B, unsigned int ENTRIES>
typename Binding<A, Key_A, B, Key_B, ENTRIES>::Bindings Binding<A, Key_A, B, Key_B, ENTRIES>::_bindings;

__END_UTIL

#endif
