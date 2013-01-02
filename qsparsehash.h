#ifndef QSPARSEHASH_H
#define QSPARSEHASH_H

#include <google/sparse_hash_map>
#include <google/dense_hash_map>
#include <QHash>
#include <QDataStream>
#include <QDebug>

/*
 * QHash-like wrapper for google::sparse_hash_map and google::dense_hash_map
 */


/*
 * Basic hash wrapper which maintains partial compatibility with QHash
 */
template<typename Container, typename Key, typename T>
class QGoogleHash {
    QSharedDataPointer<Container> d;
public:
    QGoogleHash(void) : d(new Container()) {}
    QGoogleHash(const QGoogleHash<Container, Key, T>& other) : d(other.d) {}
    QGoogleHash<Container, Key, T>& operator=(const QGoogleHash<Container, Key, T>& other)
    {
        if (this->d != other.d) {
            this->d = other.d;
        }

        return *this;
    }

    void swap(QGoogleHash<Container, Key, T>& other)
    {
        qSwap(this->d, other.d);
    }

    class iterator {
        typename Container::iterator m_it;
        Container* m_c;
        friend class QGoogleHash;
    public:
        typedef std::bidirectional_iterator_tag     iterator_category;
        typedef typename Container::difference_type difference_type;
        typedef typename Container::value_type      value_type;
        typedef typename Container::pointer         pointer;
        typedef typename Container::reference       reference;

        iterator(typename Container::iterator const& it, Container* c) : m_it(it), m_c(c) {}

        const Key& key() const { return this->m_it->first; }
        T& value() const       { return this->m_it->second; }
        T& operator*() const   { return this->m_it->second; }
        T* operator->() const  { return &this->m_it->second; }

        bool operator==(const iterator &o) const { return this->m_it == o.m_it; }
        bool operator!=(const iterator &o) const { return this->m_it != o.m_it; }

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

        const Key& key() const       { return this->m_it->first; }
        const T& value() const       { return this->m_it->second; }
        const T& operator*() const   { return this->m_it->second; }
        const T* operator->() const  { return &this->m_it->second;; }

        bool operator==(const const_iterator &o) const { return this->m_it == o.m_it; }
        bool operator!=(const const_iterator &o) const { return this->m_it != o.m_it; }

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
    typedef T              mapped_type;
    typedef T              data_type;
    typedef Key            key_type;
    typedef qptrdiff       difference_type;
    typedef int            size_type;
    typedef T*             pointer;
    typedef T const*       const_pointer;
    typedef T&             reference;
    typedef T const&       const_reference;

    iterator begin()                  { return iterator(this->d->begin(), this->d); }
    const_iterator begin() const      { return const_iterator(this->d->begin(), this->d); }
    const_iterator cbegin() const     { return this->begin(); }
    const_iterator constBegin() const { return this->begin(); }

    iterator end()                    { return iterator(this->d->end(), this->d); }
    const_iterator end() const        { return const_iterator(this->d->end(), this->d); }
    const_iterator cend() const       { return this->end(); }
    const_iterator constEnd() const   { return this->end(); }

    size_type capacity() const   { return static_cast<size_type>(this->d->bucket_count()); }
    void reserve(int size)       { this->d->resize(size); }
    void squeeze()               { this->d->resize(0); }

    bool contains(const Key& key) const            { return this->d->find(key) != this->d->end(); }
    const_iterator constFind(const Key& key) const { return const_iterator(this->d->find(key), this->d); }
    iterator find(const Key& key)                  { return iterator(this->d->find(key), this->d); }
    const_iterator find(const Key &key) const      { return const_iterator(this->d->find(key), this->d); }

    size_type count(const Key &key) const { return static_cast<size_type>(this->d->count(key)); }
    size_type count() const               { return static_cast<size_type>(this->d->size()); }
    size_type size() const                { return this->count(); }
    bool isEmpty() const                  { return this->empty(); }
    bool empty() const                    { return this->d->empty(); }
    void clear()                          { this->d->clear(); }

    iterator insert(const Key& key, const T& value)
    {
        typename Container::value_type v = std::make_pair<const Key, T>(key, value);
        std::pair<typename Container::iterator, bool> res = this->d->insert(v);
        if (!res.second) {
            res.first->second = value;
        }

        return iterator(res.first, this->d);
    }

    iterator insertMulti(const Key& key, const T& value)
    {
        qWarning("insert_multi() is not supported");
        return this->insert(key, value);
    }

    const Key key(const T& value) const { return this->key(value, Key()); }
    const Key key(const T& value, const Key& defaultKey) const
    {
        typename Container::const_iterator it = this->d->begin();
        while (it != this->d->end()) {
            if (value == it->second) {
                return it->first;
            }

            ++it;
        }

        return defaultKey;
    }

    QList<Key> keys() const
    {
        QList<Key> result;
        typename Container::const_iterator it = this->d->begin();
        while (it != this->d->end()) {
            result.append(it->first);
            ++it;
        }

        return result;
    }

    QList<Key> keys(const T& value) const
    {
        QList<Key> result;
        typename Container::const_iterator it = this->d->begin();
        while (it != this->d->end()) {
            if (value == it->second()) {
                result.append(it->first());
            }

            ++it;
        }

        return result;
    }

    QList<Key> uniqueKeys() const { return this->keys(); }

    iterator erase(iterator pos)
    {
        iterator next = pos+1;
        this->d->erase(pos.m_it);
        return next;
    }

    size_type remove(const Key& key) { return static_cast<size_type>(this->d->erase(key)); }

    T take(const Key &key)
    {
        typename Container::iterator it = this->d->find(key);
        if (it == this->d->end()) {
            return T();
        }

        T result = it->second;
        this->d->erase(it);
        return result;
    }

    QGoogleHash<Container, Key, T>& unite(const QGoogleHash<Container, Key, T>& other)
    {
        this->insert(other.begin(), other.end());
        return *this;
    }

    const T value(const Key& key) const { return this->value(key, T()); }
    const T value(const Key& key, const T& defaultValue) const
    {
        typename Container::const_iterator it = this->d->find(key);
        return (it == this->d->end()) ? defaultValue : it->second;
    }

    QList<T> values() const
    {
        QList<T> result;
        typename Container::const_iterator it = this->d->begin();
        while (it != this->d->end()) {
            result.append(it->second);
            ++it;
        }

        return result;
    }

    QList<T> values(const Key& key) const
    {
        QList<T> result;
        typename Container::const_iterator it = Container::find(key);
        if (it != Container::end()) {
            result.append(it->second);
        }

        return result;
    }

    reference operator[](const Key& key)      { return this->d->operator[](key); }
    const T	operator[](const Key & key) const { return this->value(key); }
    bool operator!=(const QGoogleHash<Container, Key, T>& other) const { return *(this->d) != *(other.d); }
    bool operator==(const QGoogleHash<Container, Key, T>& other) const { return *(this->d) == *(other.d); }

    void detach() { /*this->d.detach();*/ }

    void set_deleted_key(const Key& key)
    {
        this->d->set_deleted_key(key);
    }

    void clear_deleted_key()
    {
        this->d->clear_deleted_key();
    }

    template<class C, class K, class U>
    friend QDebug operator<<(QDebug dbg, const QGoogleHash<C, K, U>& ctr);
};

template<class Container, class Key, class T>
QDebug operator<<(QDebug dbg, const QGoogleHash<Container, Key, T>& ctr)
{
    dbg.nospace();
    typename Container::const_iterator it = ctr.d->begin();
    while (it != ctr.d->end()) {
        dbg << "key: " << it->first << ", value: " << it->second << '\n';
        ++it;
    }

    return dbg.space();
}

template<typename T>
class qHashWrapper {
public:
    uint operator()(const T& v) const
    {
        return qHash(v);
    }
};

template<typename Key, typename T, typename H>
class SparseHashMap : public google::sparse_hash_map<Key, T, H>, public QSharedData {};

template<typename Key, typename T, typename H>
class DenseHashMap : public google::dense_hash_map<Key, T, H>, public QSharedData {};

/* typedef-like class definition for QSparseHash as a wrapper to google::sparse_hash_map
 */
template<typename Key, typename T, typename H = qHashWrapper<Key> >
class QSparseHash: public QGoogleHash<SparseHashMap<Key, T, H>, Key, T> {};

template<typename Key, typename T, typename H = qHashWrapper<Key> >
class QDenseHash: public QGoogleHash<DenseHashMap<Key, T, H>, Key, T> {};

Q_DECLARE_ASSOCIATIVE_ITERATOR(SparseHash)
Q_DECLARE_MUTABLE_ASSOCIATIVE_ITERATOR(SparseHash)
Q_DECLARE_ASSOCIATIVE_ITERATOR(DenseHash)
Q_DECLARE_MUTABLE_ASSOCIATIVE_ITERATOR(DenseHash)


/* Serialization and deserialization routines
 */
template<typename Container, typename Key, typename T>
QDataStream& operator<< (QDataStream& out, const QGoogleHash<Container, Key, T>& hash)
{
    out << hash.count();
    typename QGoogleHash<Container, Key, T>::const_iterator i = hash.begin();
    while (i != hash.end()) {
        out << i.key() << i.value();
        ++i;
    }

    return out;
}

template<typename Container, typename Key, typename T>
QDataStream& operator>>(QDataStream& in, QGoogleHash<Container, Key, T>& hash) {
    int size;
    in >> size;
    for (int i=0; i<size; ++i) {
        Key key;
        T value;
        in >> key >> value;
        hash.insert(key, value);
    }

    return in;
}

#endif // QSPARSEHASH_H
