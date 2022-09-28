// EPOS Observer Utility Declarations

// Observation about the lack of virtual destructors in the following classes:
// Observed x Observer is used in mediators, so they appear very early in the system.
// To be more precise, they are used in SETUP, where we cannot yet handle a heap.
// Since the purpose of the destructors is only to trace the classes, we accepted to
// declare them as non-virtual. But it must be clear that this is one of the few uses
// for them.

#ifndef __observer_h
#define	__observer_h

#include <utility/list.h>

__BEGIN_UTIL

// Observer x Observed
class Observer;

class Observed
{
    friend class Observer;

private:
    typedef Simple_List<Observer>::Element Element;

public:
    Observed() {
        db<Observers>(TRC) << "Observed() => " << this << endl;
    }
    ~Observed() {
        db<Observers>(TRC) << "~Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Observer * o);
    virtual void detach(Observer * o);
    virtual bool notify();

    virtual unsigned int observers() const { return _observers.size(); }

private:
    Simple_List<Observer> _observers;
};

class Observer
{
    friend class Observed;

protected:
    Observer(): _link(this) {
        db<Observers>(TRC) << "Observer() => " << this << endl;
    }

public:
    ~Observer() {
        db<Observers>(TRC) << "~Observer(this=" << this << ")" << endl;
    }

    virtual void update(Observed * o) = 0;

private:
    Observed::Element _link;
};

inline void Observed::attach(Observer * o)
{
    db<Observers>(TRC) << "Observed::attach(obs=" << o << ")" << endl;

    _observers.insert(&o->_link);
}

inline void Observed::detach(Observer * o)
{
    db<Observers>(TRC) << "Observed::detach(obs=" << o << ")" << endl;

    _observers.remove(&o->_link);
}

inline bool Observed::notify()
{
    bool notified = false;

    db<Observers>(TRC) << "Observed::notify()" << endl;

    for(Element * e = _observers.head(); e; e = e->next()) {
        db<Observers>(INF) << "Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;

        e->object()->update(this);
        notified = true;
    }

    return notified;
}


// Conditional Observer x Conditionally Observed
template<typename C = int>
class Conditional_Observer;

template<typename C = int>
class Conditionally_Observed
{
    friend class Conditional_Observer<C>;

private:
    typedef typename Simple_Ordered_List<Conditional_Observer<C>, C>::Element Element;

public:
    typedef C Observing_Condition;

public:
    Conditionally_Observed() {
        db<Observers>(TRC) << "Conditionally_Observed() => " << this << endl;
    }

    virtual ~Conditionally_Observed() {
        db<Observers>(TRC) << "~Conditionally_Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Conditional_Observer<C> * o, const C & c) {
        db<Observers>(TRC) << "Conditionally_Observed::attach(o=" << o << ",c=" << c << ")" << endl;

        o->_link = Element(o, c);
        _observers.insert(&o->_link);
    }

    virtual void detach(Conditional_Observer<C> * o, const C & c) {
        db<Observers>(TRC) << "Conditionally_Observed::detach(obs=" << o << ",c=" << c << ")" << endl;

        _observers.remove(&o->_link);
    }

    virtual bool notify(const C & c) {
        bool notified = false;

        db<Observers>(TRC) << "Conditionally_Observed::notify(cond=" << hex << c << ")" << endl;

        for(Element * e = _observers.head(); e; e = e->next()) {
            if(e->rank() == c) {
                db<Observers>(INF) << "Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;
                e->object()->update(this, c);
                notified = true;
            }
        }

        return notified;
    }

    virtual unsigned int observers() const { return _observers.size(); }

private:
    Simple_Ordered_List<Conditional_Observer<C>, C> _observers;
};

template<typename C>
class Conditional_Observer
{
    friend class Conditionally_Observed<C>;

public:
    typedef C Observing_Condition;

protected:
    Conditional_Observer(): _link(this) {
        db<Observers>(TRC) << "Conditionally_Observer() => " << this << endl;
    }

public:
    virtual ~Conditional_Observer() {
        db<Observers>(TRC) << "~Conditionally_Observer(this=" << this << ")" << endl;
    }

    virtual void update(Conditionally_Observed<C> * o, const C & c) = 0;

private:
    typename Conditionally_Observed<C>::Element _link;
};


// (Conditional) Observer x (Conditionally) Observed with Data
template<typename D, typename C = void>
class Data_Observer;

template<typename D, typename C = void>
class Data_Observed
{
    friend class Data_Observer<D, C>;

private:
    typedef Data_Observer<D, C> _Observer;
    typedef typename Simple_Ordered_List<Data_Observer<D, C>, C>::Element Element;

public:
    typedef D Observed_Data;
    typedef C Observing_Condition;

public:
    Data_Observed() {
        db<Observers>(TRC) << "Data_Observed() => " << this << endl;
    }

    virtual ~Data_Observed() {
        db<Observers>(TRC) << "~Data_Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Data_Observer<D, C> * o, const C & c) {
        db<Observers>(TRC) << "Data_Observed::attach(obs=" << o << ",cond=" << c << ")" << endl;

        o->_link = Element(o, c);
        _observers.insert(&o->_link);
    }

    virtual void detach(Data_Observer<D, C> * o, const C & c) {
        db<Observers>(TRC) << "Data_Observed::detach(obs=" << o << ",cond=" << c << ")" << endl;

        _observers.remove(&o->_link);
    }

    virtual bool notify(const C & c, D * d) {
        bool notified = false;

        db<Observers>(TRC) << "Data_Observed::notify(this=" << this << ",cond=" << c << ",obss= " << _observers.size() << ")" << endl;

        for(Element * e = _observers.head(); e; e = e->next()) {
            if(e->rank() == c) {
                db<Observers>(INF) << "Data_Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;
                e->object()->update(this, c, d);
                notified = true;
            }
        }

        return notified;
    }

    virtual _Observer * observer(const C & c, unsigned int index = 0) {
        _Observer * o = 0;
        for(Element * e = _observers.head(); e; e = e->next()) {
            if(e->rank() == c) {
                if(!index)
                    o =  e->object();
                else
                    index--;
            }
        }
        return o;
    }

    virtual unsigned int observers() const { return _observers.size(); }

private:
    Simple_Ordered_List<Data_Observer<D, C>, C> _observers;
};

template<typename D, typename C>
class Data_Observer
{
    friend class Data_Observed<D, C>;

public:
    typedef D Observed_Data;
    typedef C Observing_Condition;

protected:
    Data_Observer(): _link(this) {
        db<Observers>(TRC) << "Data_Observer() => " << this << endl;
    }

public:
    virtual ~Data_Observer() {
        db<Observers>(TRC) << "~Data_Observer(this=" << this << ")" << endl;
    }

    virtual void update(Data_Observed<D, C> * o, const C & c, D * d) = 0;

private:
    typename Data_Observed<D, C>::Element _link;
};


// (Unconditional) Observer x (Unconditionally) Observed with Data
template<typename D>
class Data_Observed<D, void>
{
    friend class Data_Observer<D, void>;

private:
    typedef Data_Observer<D, void> _Observer;
    typedef typename Simple_List<Data_Observer<D, void>>::Element Element;

public:
    typedef D Observed_Data;

public:
    Data_Observed() {
        db<Observers>(TRC) << "Data_Observed() => " << this << endl;
    }

    virtual ~Data_Observed() {
        db<Observers>(TRC) << "~Data_Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Data_Observer<D, void> * o) {
        db<Observers>(TRC) << "Data_Observed::attach(obs=" << o << ")" << endl;

        o->_link = Element(o);
        _observers.insert(&o->_link);
    }

    virtual void detach(Data_Observer<D, void> * o) {
        db<Observers>(TRC) << "Data_Observed::detach(obs=" << o << ")" << endl;

        _observers.remove(&o->_link);
    }

    virtual bool notify(D * d) {
        bool notified = false;

        db<Observers>(TRC) << "Data_Observed::notify(this=" << this << ")" << endl;

        for(Element * e = _observers.head(); e; e = e->next()) {
            db<Observers>(INF) << "Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;
            e->object()->update(this, d);
            notified = true;
        }

        return notified;
    }

    virtual _Observer * observer(unsigned int index = 0) {
        _Observer * o = 0;
        for(Element * e = _observers.head(); e; e = e->next()) {
            if(!index)
                o =  e->object();
            else
                index--;
        }
        return o;
    }

    virtual unsigned int observers() const { return _observers.size(); }

private:
    Simple_List<Data_Observer<D, void>> _observers;
};

template<typename D>
class Data_Observer<D, void>
{
    friend class Data_Observed<D, void>;

public:
    typedef D Observed_Data;

protected:
    Data_Observer(): _link(this) {
        db<Observers>(TRC) << "Data_Observer() => " << this << endl;
    }

public:
    virtual ~Data_Observer() {
        db<Observers>(TRC) << "~Data_Observer(this=" << this << ")" << endl;
    }

    virtual void update(Data_Observed<D, void> * o, D * d) = 0;

private:
    typename Data_Observed<D, void>::Element _link;
};

__END_UTIL

#endif
