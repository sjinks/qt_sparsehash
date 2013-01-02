#ifndef QSPARSEHASH_H
#define QSPARSEHASH_H

#include <google/sparse_hash_map>
#include "qgooglehash.h"

namespace {
    template<typename Key, typename T, typename H> class SparseHashMap : public google::sparse_hash_map<Key, T, H>, public QSharedData
    {
    };
}

template<typename Key, typename T, typename H = qHashWrapper<Key> >
class QSparseHash: public QGoogleHash<SparseHashMap<Key, T, H>, Key, T>
{
};

Q_DECLARE_ASSOCIATIVE_ITERATOR(SparseHash)
Q_DECLARE_MUTABLE_ASSOCIATIVE_ITERATOR(SparseHash)

#endif // QSPARSEHASH_H
