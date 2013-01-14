#ifndef QSPARSESET_H
#define QSPARSESET_H

#include <google/sparse_hash_set>
#include "qgoogleset.h"

namespace {
    template<typename T, typename H> class SparseHashSet : public google::sparse_hash_set<T, H>, public QSharedData
    {
    };
}

template<typename T, typename H = qHashWrapper<T> >
class QSparseSet: public QGoogleSet<SparseHashSet<T, H>, T> {
public:
    QSparseSet(void)
    {
    }

    QSparseSet(const QGoogleSet<SparseHashSet<T, H>, T>& other) : QGoogleSet<SparseHashSet<T, H>, T>(other) {}

    QSparseSet(const T& deleted)
    {
        this->set_deleted_key(deleted);
    }
};

Q_DECLARE_SEQUENTIAL_ITERATOR(SparseSet)

template <typename T>
class QMutableSparseSetIterator : public QMutableGoogleSetIterator<QSparseSet<T>, T> {
public:
    QMutableSparseSetIterator(QSparseSet<T>& container) : QMutableGoogleSetIterator<QSparseSet<T>, T>(container) {}
};

#endif // QSPARSESET_H
