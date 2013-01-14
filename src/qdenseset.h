#ifndef QDENSESET_H
#define QDENSESET_H

#include <google/dense_hash_set>
#include "qgoogleset.h"

namespace {
    template<typename T, typename H> class DenseHashSet : public google::dense_hash_set<T, H>, public QSharedData
    {
    };
}

template<typename T, typename H = qHashWrapper<T> >
class QDenseSet : public QGoogleSet<DenseHashSet<T, H>, T> {
public:
    QDenseSet()
    {
        this->set_empty_key(T());
    }

    QDenseSet(const QGoogleSet<DenseHashSet<T, H>, T>& other) : QGoogleSet<DenseHashSet<T, H>, T>(other) {}

    QDenseSet(const T& empty)
    {
        this->set_empty_key(empty);
    }

    QDenseSet(const T& empty, const T& deleted)
    {
        this->set_empty_key(empty);
        this->set_deleted_key(deleted);
    }

    void set_empty_key(const T& key)
    {
        this->d->set_empty_key(key);
    }
};

Q_DECLARE_SEQUENTIAL_ITERATOR(DenseSet)

template <typename T>
class QMutableDenseSetIterator : public QMutableGoogleSetIterator<QDenseSet<T>, T> {
public:
    QMutableDenseSetIterator(QDenseSet<T>& container) : QMutableGoogleSetIterator<QDenseSet<T>, T>(container) {}
};

#endif // QDENSESET_H
