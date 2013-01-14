#ifndef QHASHWRAPPER_H
#define QHASHWRAPPER_H

#include <QtCore/QHash>

template<typename T>
class qHashWrapper {
public:
    uint operator()(const T& v) const
    {
        return qHash(v);
    }
};

#endif // QHASHWRAPPER_H
