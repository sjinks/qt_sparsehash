#ifndef QSPARSEHASH_H
#define QSPARSEHASH_H

#include <google/sparse_hash_map>
#include <google/dense_hash_map>
#include <qglobal.h>
#include <QHash>
#include <QDataStream>
#include <QDebug>

/*
 * QHash-like wrapper for google::sparse_hash_map and google::dense_hash_map
 */


/*
 *Basic hash wrapper which maintains partial compatibility with QHash
 */
template <class Container, class Key, class T>
class QGoogleHash: public Container
{
public:
    int capacity() const {
        return this->bucket_count();
    }
    typename Container::const_iterator constBegin() const {
        return this->begin();
    }
    typename Container::const_iterator constEnd() const {
        return this->end();
    }
    typename Container::const_iterator constFind(const Key &key) const {
        return this->find(key);
    }
    bool contains(const Key &key) const {
        return (this->find(key) != this->end());
    }
    int count(const Key &key) const {
        return static_cast<int>(Container::count(key));
    }
    int count() const {
        return static_cast<int>(this->size());
    }
    typename Container::iterator insert(const Key &key, const T &value) {
        typename Container::value_type v = std::make_pair<const Key, T>(key, value);
        std::pair<typename Container::iterator, bool> res = Container::insert(v);
        return res.first;
    }
    bool isEmpty() const {
        return this->empty()
    }
    const Key key(const T &value) const {
        return this->key(value, Key());
    }
    const Key key(const T &value, const Key &defaultKey) const {
        typename Container::const_iterator it = this->begin();
        while (it != this->end()) {
            if (value == it->second()) {
                return it->first;
            }
        }

        return defaultKey;
    }
    QList<Key> keys() const {
        QList<Key> result;
        typename Container::const_iterator it = this->begin();
        if (it != this->end()) {
            result.append(it->first());
        }

        return result;
    }
    QList<Key> keys(const T &value) const {
        QList<Key> result;
        typename Container::const_iterator it = this->begin();
        while (it != this->end()) {
            if (value == it->second()) {
                result.append(it->first());
            }
        }

        return result;
    }
    int remove(const Key &key) {
        return static_cast<int>(this->erase(key));
    }
    void reserve(int size) {
        this->resize(size);
    }
    void squeeze() {
        // there's nothing to squeeze
    }
    T take(const Key &key) {
        typename Container::iterator it = this->find(key);
        if (it == this->end()) {
            return T();
        }

        T result = it->second;
        this->erase(it);
        return result;
    }
    QList<Key> uniqueKeys() const {
        return this->keys();
    }
    QGoogleHash<Container, Key, T> &unite(const QGoogleHash<Container, Key, T> &other) {
        this->insert(other.begin(), other.end());
        return *this;
    }
    const T value(const Key &key) const {
        return this->value(key, T());
    }
    const T value(const Key &key, const T &defaultValue) const {
        typename Container::iterator it = this->find(key);
        return (it == this->end()) ? defaultValue : it->second;
    }
    QList<T> values() const {
        QList<T> result;
        typename Container::const_iterator it = this->begin();
        while (it != this->end()) {
            result.append(it->second);
            ++it;
        }

        return result;
    }
    QList<T> values(const Key &key) const {
        QList<T> result;
        typename Container::iterator it = this->find(key);
        if (it != this->end()) {
            result.append(it->second());
        }

        return result;
    }
    void print() const {
        typename Container::const_iterator i = this->begin();
        while (i != this->end()) {
            qDebug() << "key: " << i->first << " value: " << i->second;
            ++i;
        }
    }
};

template <typename T>
class Q_DECL_HIDDEN qHashWrapper {
    uint operator()(const T &v) {
        return qHash<T>(v);
    }
}

/* typedef-like class definition for QSparseHash as a wrapper to google::sparse_hash_map
 */
template <class Key, class T>
class QSparseHash: public QGoogleHash<google::sparse_hash_map<Key, T, qHashWrapper<T> >, Key, T> {};

template <class Key, class T>
class QDenseHash: public QGoogleHash<google::dense_hash_map<Key, T, qHashWrapper<T> >, Key, T> {};

/* Serialization and deserialization routines
 */
template <class Key, class T>
QDataStream & operator<< ( QDataStream & out, const QSparseHash<Key, T> & hash ) {
    out << hash.count();
    typename QSparseHash<Key, T>::const_iterator i = hash.begin();
    while (i != hash.end()) {
        out << i->first << i->second;
        ++i;
    }
    return out;
}

template <class Key, class T>
QDataStream & operator>> ( QDataStream & in, QSparseHash<Key, T> & hash ) {
    int size;
    in >> size;
    for (int i=0; i<size; ++i) {
        Key key;
        T value;
        in >> key;
        in >> value;
        hash.insert(key, value);
    }
    return in;
}

template <class Key, class T>
QDataStream & operator<< ( QDataStream & out, const QDenseHash<Key, T> & hash ) {
    out << hash.count();
    typename QDenseHash<Key, T>::const_iterator i = hash.begin();
    while (i != hash.end()) {
        out << i->first << i->second;
        ++i;
    }
    return out;
}

template <class Key, class T>
QDataStream & operator>> ( QDataStream & in, QDenseHash<Key, T> & hash ) {
    int size;
    in >> size;
    for (int i=0; i<size; ++i) {
        Key key;
        T value;
        in >> key;
        in >> value;
        hash.insert(key, value);
    }
    return in;
}

#endif // QSPARSEHASH_H
