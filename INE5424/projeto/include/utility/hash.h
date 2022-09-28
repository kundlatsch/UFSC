// EPOS Hash Utility Declarations

#ifndef __hash_h
#define	__hash_h

#include <system/config.h>
#include "list.h"
#include "vector.h"

__BEGIN_UTIL

// Hash Table with a single Synonym List
// In order to change the hash function, simply redefine the operator % for
// objects of type T and Key.
template<typename T, unsigned int SIZE, typename Key = int>
class Simple_Hash
{
public:
    typedef T Object_Type;
    typedef Key Rank_Type;
    typedef typename List_Elements::Singly_Linked_Ordered<T, Key> Element;

    class Forward: public List_Iterators::Forward<Element>
    {
        typedef List_Iterators::Forward<Element> Base;
        using Base::_current;

    public:
        enum Begin { BEGIN = 0 };
        enum End { END = -1 };

    public:
        Forward(Simple_Hash * hash, const Begin & b): _hash(hash), _counter(0) {}
        Forward(Simple_Hash * hash, const End & e): _hash(hash), _counter(END) {}

        Forward & operator++() {
            for(; (_counter < SIZE) && !_hash->_vector[_counter]; _counter++);

            if(_counter < SIZE) {
                _current = _hash->_vector[_counter];
                _counter++;
            } else if(_counter == SIZE) {
                _current = _hash->_synonyms.head();
                _counter++;
            } else
                _current = _current->next();

            if(!_current)
                _counter = END;

            return *this;
        }

        Forward operator++(int) {
            Forward tmp = *this; ++*this; return tmp;
        }

        bool operator==(const Forward & i) const {
            return _counter == i._counter;
        }
        bool operator!=(const Forward & i) const {
            return _counter != i._counter;
        }

    private:
        Simple_Hash * _hash;
        unsigned int _counter;
    };

    typedef Forward Iterator;


public:
    Simple_Hash() {}

    Iterator begin() {
        Iterator it = Iterator(this, Iterator::BEGIN); // this sets on position 0, which may not be used in a Hash;
        it++;  // sends Iterator to the first valid element, or END if empty;
        return it;
    }

    Iterator end() { return Iterator(this, Iterator::END); }

    bool empty() const {
        return (_vector.size() == 0) && (_synonyms.size() == 0);
    }
    unsigned int size() const {
        return _vector.size() + _synonyms.size();
    }

    void insert(Element * e) {
        if(!_vector.insert(e, e->key() % SIZE))
            _synonyms.insert(e);
    }

    Element * remove(Element * e) {
        if(_vector.remove(e))
            return e;
        return _synonyms.remove(e);
    }
    Element * remove(const Object_Type * obj) {
        Element * e = _vector.remove(obj);
        if(e)
            return e;
        return _synonyms.remove(obj);
    }

    Element * search(const Object_Type * obj) {
        Element * e = _vector.search(obj);
        if(e)
            return e;
        return _synonyms.search(obj);
    }

    Element * search_key(const Key & key) {
        Element * e = _vector[key % SIZE];
        if(e && (e->key() == key))
            return e;
        return _synonyms.search_rank(key);
    }

    Element * remove_key(const Key & key) {
        Element * e = _vector[key % SIZE];
        if(e && (e->key() == key))
            return _vector.remove(key % SIZE);
        return _synonyms.remove_rank(key);
    }

private:
    Vector<T, SIZE, Element> _vector;
    Simple_Ordered_List<T, Key, Element> _synonyms;
};


// Hash Table with a Synonym List for each Key
template<typename T,
         unsigned int SIZE,
         typename Key = int,
         typename El = List_Elements::Singly_Linked_Ordered<T, Key>,
         typename L = Simple_Ordered_List<T, Key, El> >
class Hash
{
public:
    typedef T Object_Type;
    typedef El Element;
    typedef L List;

    // TODO: a forward iterator is missing here!

public:
    Hash() {}

    bool empty() const {
        for(unsigned int i = 0; i < SIZE; i++)
            if(!_table[i].empty())
        	return false;
        return true;
    }

    unsigned int size() const {
        unsigned int size = 0;
        for(unsigned int i = 0; i < SIZE; i++)
            size += _table[i].size();
        return size;
    }

    void insert(Element * e) {
        _table[e->key() % SIZE].insert(e);
    }

    Element * remove(Element * e) {
        return _table[e->key() % SIZE].remove(e);
    }
    Element * remove(const Object_Type * obj) {
        for(unsigned int i = 0; i < SIZE; i++) {
            Element * e = _table[i].remove(obj);
            if(e)
        	return e;
        }
        return 0;
    }

    Element * search(const Object_Type * obj) {
        for(unsigned int i = 0; i < SIZE; i++) {
            Element * e = _table[i].search(obj);
            if(e)
        	return e;
        }
        return 0;
    }

    Element * search_key(const Key & key) {
        return _table[key % SIZE].search_rank(key);
    }

    Element * remove_key(const Key & key) {
        return _table[key % SIZE].remove_rank(key);
    }

    List * operator[](const Key & key) {
        return &_table[key % SIZE];
    }

private:
    List _table[SIZE];
};

__END_UTIL

#endif
