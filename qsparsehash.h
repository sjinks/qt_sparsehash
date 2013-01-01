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
    void insert(const Key &key, const T &value) {
        (*this)[key] = value;
    }
    T value(const Key &key) const {
        typename Container::const_iterator i;
        i = this->find(key);
        if (i != this->end())
            return i->second;
        else
            return T();
    }
    bool contains(const Key &key) const {
        return (this->find(key) != this->end());
    }
    int count() const {
        return this->size();
    }
    void print() const {
        typename Container::const_iterator i = this->begin();
        while (i != this->end()) {
            qDebug() << "key: " << i->first << " value: " << i->second;
            i++;
        }
    }
    void remove(const Key &key) {
        this->erase(key);
    }
    void squeeze() {
        // there's nothing to squeeze
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
        i++;
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
        i++;
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
