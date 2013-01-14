#ifndef QGOOGLESET_H
#define QGOOGLESET_H

#include <QtCore/QSharedDataPointer>
#include "qhashwrapper.h"

template<typename Container, typename T>
class QGoogleSet {
protected:
    QSharedDataPointer<Container> d;
public:
    class iterator {
        typename Container::iterator m_it;
        Container* m_c;
        friend class const_iterator;
        friend class QGoogleSet;
    public:
        typedef std::bidirectional_iterator_tag     iterator_category;
        typedef typename Container::difference_type difference_type;
        typedef typename Container::value_type      value_type;
        typedef typename Container::pointer         pointer;
        typedef typename Container::reference       reference;

        iterator(void) : m_it(), m_c(0) {}
        iterator(typename Container::iterator const& it, Container* c) : m_it(it), m_c(c) {}

        const T& operator*() const   { return *this->m_it; }
        const T* operator->() const  { return &this->m_it; }

        bool operator==(const iterator& o) const { return this->m_it == o.m_it; }
        bool operator!=(const iterator& o) const { return this->m_it != o.m_it; }

        iterator& operator++()   { ++this->m_it; return *this; }
        iterator operator++(int) { iterator r(*this); ++this->m_it; return r; }

        iterator& operator--()
        {
            typename Container::iterator b = this->m_c->begin();
            difference_type dist = std::distance(b, this->m_it) - 1;
            this->m_it = b;
            std::advance(this->m_it, dist);
            return *this;
        }

        iterator operator--(int)
        {
            iterator r(*this);
            this->operator--();
            return r;
        }

        iterator operator+(int j) const
        {
            if (j >= 0) {
                iterator r = *this;
                std::advance(r.m_it, j);
                return r;
            }

            return this->operator-(-j);
        }

        iterator operator-(int j) const
        {
            if (j > 0) {
                iterator r = *this;
                typename Container::iterator b = r.m_c->begin();
                difference_type dist = std::distance(b, r.m_it) - j;
                r.m_it = b;
                std::advance(r.m_it, dist);
                return r;
            }

            return this->operator+(-j);
        }

        iterator& operator+=(int j) { return *this = *this + j; }
        iterator& operator-=(int j) { return *this = *this - j; }
    };

    class const_iterator {
        typename Container::const_iterator m_it;
        const Container* m_c;
    public:
        typedef std::bidirectional_iterator_tag     iterator_category;
        typedef typename Container::difference_type difference_type;
        typedef typename Container::value_type      value_type;
        typedef typename Container::pointer         pointer;
        typedef typename Container::const_reference reference;
        typedef typename Container::const_reference const_reference;

        const_iterator(typename Container::const_iterator const& it, const Container* c) : m_it(it), m_c(c) {}
        const_iterator(const iterator& it) : m_it(it.m_it), m_c(it.m_c) {}
        const_iterator(const const_iterator& it) : m_it(it.m_it), m_c(it.m_c) {}

        const T& operator*() const   { return *this->m_it; }
        const T* operator->() const  { return &this->m_it; }

        bool operator==(const const_iterator& o) const { return this->m_it == o.m_it; }
        bool operator!=(const const_iterator& o) const { return this->m_it != o.m_it; }

        const_iterator& operator++()   { ++this->m_it; return *this; }
        const_iterator operator++(int) { const_iterator r = *this; ++this->m_it; return r; }

        const_iterator& operator--()
        {
            typename Container::const_iterator b = this->m_c->begin();
            difference_type dist = std::distance(b, this->m_it) - 1;
            this->m_it = b;
            std::advance(this->m_it, dist);
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator r(*this);
            this->operator--();
            return r;
        }

        const_iterator operator+(int j) const
        {
            if (j >= 0) {
                const_iterator r = *this;
                std::advance(r.m_it, j);
                return r;
            }

            return this->operator-(-j);
        }

        const_iterator operator-(int j) const
        {
            if (j > 0) {
                const_iterator r = *this;
                typename Container::const_iterator b = r.m_c->begin();
                difference_type dist = std::distance(b, r.m_it) - j;
                r.m_it = b;
                std::advance(r.m_it, dist);
                return r;
            }

            return this->operator+(-j);
        }

        const_iterator& operator+=(int j) { return *this = *this + j; }
        const_iterator& operator-=(int j) { return *this = *this - j; }
    };

    typedef iterator       Iterator;
    typedef const_iterator ConstIterator;
    typedef T              key_type;
    typedef T              value_type;
    typedef T*             pointer;
    typedef const T*       const_pointer;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef qptrdiff       difference_type;
    typedef int            size_type;

    QGoogleSet(void) : d(new Container()) {}
    QGoogleSet(const QGoogleSet<Container, T>& other) : d(other.d) {}

    QGoogleSet<Container, T>& operator=(const QGoogleSet<Container, T>& other)
    {
        this->d = other.d;
        return *this;
    }

#ifdef Q_COMPILER_RVALUE_REFS
    QGoogleSet<Container, T>& operator=(QGoogleSet<Container, T>&& other)
    {
        qSwap(this->d, other.d);
        return *this;
    }
#endif

    void swap(QGoogleSet<Container, T>& other)
    {
        qSwap(this->d, other.d);
    }

    iterator begin()                  { return iterator(this->d->begin(), this->d); }
    const_iterator begin() const      { return const_iterator(this->d->begin(), this->d); }
    const_iterator cbegin() const     { return this->begin(); }
    const_iterator constBegin() const { return this->begin(); }

    iterator end()                    { return iterator(this->d->end(), this->d); }
    const_iterator end() const        { return const_iterator(this->d->end(), this->d); }
    const_iterator cend() const       { return this->end(); }
    const_iterator constEnd() const   { return this->end(); }

    bool contains(const T& key) const            { return this->d->find(key) != this->d->end(); }
    const_iterator constFind(const T& key) const { return const_iterator(this->d->find(key), this->d); }
    iterator find(const T& key)                  { return iterator(this->d->find(key), this->d); }
    const_iterator find(const T &key) const      { return const_iterator(this->d->find(key), this->d); }

    size_type capacity() const   { return static_cast<size_type>(this->d->bucket_count()); }
    void reserve(int size)       { this->d->resize(size); }
    void squeeze()               { this->d->resize(0); }

    bool operator==(const QGoogleSet<Container, T>& other) const { return *(this->d) == *(other.d); }
    bool operator!=(const QGoogleSet<Container, T>& other) const { return *(this->d) != *(other.d); }

    size_type size() const  { return this->count(); }
    size_type count() const { return static_cast<size_type>(this->d->size()); }
    bool isEmpty() const    { return this->empty(); }
    bool empty() const      { return this->d->empty(); }
    void clear()            { this->d->clear(); }

    bool contains(const QGoogleSet<Container, T>& other) const
    {
        typename QGoogleSet<Container, T>::const_iterator i = other.constBegin();
        while (i != other.constEnd()) {
            if (!this->contains(*i)) {
                return false;
            }

            ++i;
        }

        return true;
    }

    iterator insert(const T& value)
    {
        std::pair<typename Container::iterator, bool> res = this->d->insert(value);
        return iterator(res.first, this->d);
    }


    iterator erase(iterator pos)
    {
        iterator next = pos+1;
        this->d->erase(pos.m_it);
        return next;
    }

    size_type remove(const T& key) { return static_cast<size_type>(this->d->erase(key)); }

    QGoogleSet<Container, T>& unite(const QGoogleSet<Container, T>& other)
    {
        if (this->d != other.d) {
            this->d->insert(other.d->begin(), other.d->end());
        }

        return *this;
    }

    QGoogleSet<Container, T>& intersect(const QGoogleSet<Container, T>& other)
    {
        if (this->d != other.d) {
            typename Container::const_iterator i = this->d->begin();
            while (i != this->d->end()) {
                if (!other.contains(*i)) {
                    this->d->erase(*i);
                }

                ++i;
            }
        }

        return *this;
    }

    QGoogleSet<Container, T>& subtract(const QGoogleSet<Container, T>& other)
    {
        if (this->d != other.d) {
            typename Container::const_iterator i = this->d->begin();
            while (i != this->d->end()) {
                if (other.contains(*i)) {
                    this->d->erase(*i);
                }

                ++i;
            }
        }
        else {
            this->clear();
        }

        return *this;
    }

    QList<T> toList() const { return this->values(); }

    QList<T> values() const
    {
        QList<T> result;
        result.reserve(this->size());
        typename Container::const_iterator it = this->d->begin();
        while (it != this->d->end()) {
            result.append(*it);
            ++it;
        }

        return result;
    }

    QGoogleSet<Container, T>& operator<<(const T& value)                        { this->insert(value); return *this; }
    QGoogleSet<Container, T>& operator|=(const QGoogleSet<Container, T> &other) { this->unite(other); return *this; }
    QGoogleSet<Container, T>& operator|=(const T& value)                        { this->insert(value); return *this; }
    QGoogleSet<Container, T>& operator&=(const QGoogleSet<Container, T>& other) { this->intersect(other); return *this; }
    QGoogleSet<Container, T>& operator&=(const T& value)
    {
        bool contains = this->contains(value);
        this->clear();
        if (contains) {
            this->insert(value);
        }

        return *this;
    }

    QGoogleSet<Container, T>& operator+=(const QGoogleSet<Container, T>& other) { this->unite(other); return *this; }
    QGoogleSet<Container, T>& operator+=(const T& value)                        { this->insert(value); return *this; }
    QGoogleSet<Container, T>& operator-=(const QGoogleSet<Container, T>& other) { this->subtract(other); return *this; }
    QGoogleSet<Container, T>& operator-=(const T& value)                        { this->remove(value); return *this; }

    QGoogleSet<Container, T> operator|(const QGoogleSet<Container, T>& other) const { QGoogleSet<Container, T> result = *this; result |= other; return result; }
    QGoogleSet<Container, T> operator&(const QGoogleSet<Container, T>& other) const { QGoogleSet<Container, T> result = *this; result &= other; return result; }
    QGoogleSet<Container, T> operator+(const QGoogleSet<Container, T>& other) const { QGoogleSet<Container, T> result = *this; result += other; return result; }
    QGoogleSet<Container, T> operator-(const QGoogleSet<Container, T>& other) const { QGoogleSet<Container, T> result = *this; result -= other; return result; }

    void detach() { /*this->d.detach();*/ }

    void set_deleted_key(const T& key)
    {
        this->d->set_deleted_key(key);
    }

    void clear_deleted_key()
    {
        this->d->clear_deleted_key();
    }

    template<class C, class K>
    friend QDebug operator<<(QDebug dbg, const QGoogleSet<C, K>& ctr);
};

template<typename Container, typename T>
class QMutableGoogleSetIterator {
    typedef typename Container::iterator iterator;
    Container* c;
    iterator i, n;
    bool item_exists() const { return c->constEnd() != n; }

public:
    QMutableGoogleSetIterator(Container& container)
        : c(&container)
    {
        i = c->begin();
        n = c->end();
    }

    ~QMutableGoogleSetIterator() {}

    QMutableGoogleSetIterator& operator=(QMutableGoogleSetIterator& container)
    {
        c = &container;
        i = c->begin();
        n = c->end();
        return *this;
    }

    void toFront() { i = c->begin(); n = c->end(); }
    void toBack()  { i = c->end(); n = i; }

    bool hasNext() const      { return c->constEnd() != i; }
    const T& next()           { n = i; ++i; return *n; }
    const T& peekNext() const { return *i; }

    bool hasPrevious() const      { return c->constBegin() != i; }
    const T& previous()           { --i; n = i; return *n; }
    const T& peekPrevious() const { iterator p = i; return *--p; }

    void remove()
    {
        if (c->constEnd() != n) {
            i = c->erase(n);
            n = c->end();
        }
    }

    const T& value() const { return *n; }

    bool findNext(const T& t)
    {
        while (c->constEnd() != (n = i)) {
            if (*i++ == t) {
                return true;
            }
        }

        return false;
    }

    bool findPrevious(const T& t)
    {
        while (c->constBegin() != i) {
            if (*(n = --i) == t) {
                return true;
            }
        }

        n = c->end();
        return false;
    }
};

template<class Container, class T>
QDebug operator<<(QDebug dbg, const QGoogleSet<Container, T>& ctr)
{
    dbg.nospace();
    typename Container::const_iterator it = ctr.d->begin();
    dbg << "QGoogleSet(";
    while (it != ctr.d->end()) {
        dbg << *it << ',';
        ++it;
    }

    dbg << ")";
    return dbg.space();
}

#endif // QGOOGLESET_H
