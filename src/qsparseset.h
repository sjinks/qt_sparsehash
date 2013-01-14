#ifndef QSPARSESET_H
#define QSPARSESET_H

#include <google/sparse_hash_set>
#include "qgoogleset.h"

namespace {
    template<typename T, typename H> class SparseHashSet : public google::sparse_hash_set<T, H>, public QSharedData
    {
    };
}

template<typename Key, typename T, typename H = qHashWrapper<T> >
class QSparseSet: public QGoogleSet<SparseHashSet<T, H>, T> {
public:
    QSparseSet(void)
    {
    }

    QSparseSet(const Key& deleted)
    {
        this->set_deleted_key(deleted);
    }
};

Q_DECLARE_SEQUENTIAL_ITERATOR(SparseSet)
Q_DECLARE_MUTABLE_SEQUENTIAL_ITERATOR(SparseSet)

#endif // QSPARSESET_H
