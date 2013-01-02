#ifndef QDENSEHASH_H
#define QDENSEHASH_H

#include <google/dense_hash_map>
#include "qgooglehash.h"

namespace {
    template<typename Key, typename T, typename H> class DenseHashMap : public google::dense_hash_map<Key, T, H>, public QSharedData
    {
    };
}

template<typename Key, typename T, typename H = qHashWrapper<Key> >
class QDenseHash : public QGoogleHash<DenseHashMap<Key, T, H>, Key, T> {
public:
    QDenseHash()
    {
        this->set_empty_key(Key());
    }

    QDenseHash(const Key& empty)
    {
        this->set_empty_key(empty);
    }

    QDenseHash(const Key& empty, const Key& deleted)
    {
        this->set_empty_key(empty);
        this->set_deleted_key(deleted);
    }

    void set_empty_key(const Key& key)
    {
        this->d->set_empty_key(key);
    }
};

Q_DECLARE_ASSOCIATIVE_ITERATOR(DenseHash)
Q_DECLARE_MUTABLE_ASSOCIATIVE_ITERATOR(DenseHash)

#endif // QDENSEHASH_H
