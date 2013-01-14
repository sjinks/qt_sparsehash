# QSparseHash [![Build Status](https://travis-ci.org/sjinks/qsparsehash.png?branch=master)](https://travis-ci.org/sjinks/qsparsehash)

A Qt wrapper for [sparsehash](https://code.google.com/p/sparsehash/)'s `sparse_hash_map` and `dense_hash_map`.

Initially this was a fork of https://github.com/shuttie/qsparsehash but in attempts to make the interfaces compatible with Qt the entire code was rewritten.

## Installation

Add this code to your `.pro` file:

```
INCLUDEPATH += /path/to/qsparsehash
DEPENDPATH  += /path/to/qsparsehash
```

and `#include` the files you need from your Qt code:

```c++
#include <QSparseHash> // if you need QSparseHash
#include <QDenseHash>  // if you need QDenseHash
```

## Compatibility

QSparseHash is tested with Qt 4.8, Qt 5.0 and sparsehash 2.0.2 (will probably work with sparsehash 1.10).
It may work with older versions of Qt / qsparsehash but this has not been tested.

## How to Use

QSparseHash and QDenseHash thrive to be compatible with Qt's QHash class so that they can be used as drop-in replacements.
However, there are a few pecularities:
* neither `sparse_hash_map` nor `dense_hash_map` support non-unique keys;
this means that `insertMulti()` method won't work as expected;
currently it displays a warning message and behaves like `insert()`.
* as a consequence, `QSparseHash::unite()` and `QDenseHash::unite()` will not work the same way as `QHash::unite()`
if both hashes contain the same keys: the resulting hash won't have multiple keys: `unite()` just adds missing keys/values
from the other hash.
* both QSparseHash and QDenseHash have a notion of a "deleted key" [[1](http://sparsehash.googlecode.com/svn/trunk/doc/sparse_hash_map.html#6)] [[2](http://sparsehash.googlecode.com/svn/trunk/doc/dense_hash_map.html#6)].
In brief, if you want to remove something from the hash, you must set the deleted key (using either `set_deleted_key()` method
or by invoking a constructor which takes the deleted key as its argument).
The deleted key should be a key that is **never** used for legitimate hash-map entries.
It is an error to call `remove()` (or any other method that invokes `remove()`) without first setting the deleted key,
and it is also an error to call `insert()` with an item whose key is the "deleted key".
* QDenseHash has a notion of an '[empty key](http://sparsehash.googlecode.com/svn/trunk/doc/dense_hash_map.html#6)'.
The empty key should be a key that is **never** used for legitimate hash-map entries. It **must** be different from the key used for the deleted key.
It is an error to call `insert()` with an item whose key is the "empty key".

## Examples

**Creating, inserting and removing values:**

```c++
QSparseHash<QString, int> hash;
hash.insert("foo", 1);
hash.insert("bar", 2);
hash["baz"] = 3;

// accessing values
qDebug("baz: %d", hash["baz"]);

// removing
hash.set_deleted_key(""); // if you remove items from hash, you must set the deleted key
hash.remove("foo");
qDebug("Number of elements in the hash: %u", hash.count());
```

**Iterating over the hash:**

```c++
QSparseHash<QString, int> hash;
hash.insert("foo", 1);
hash.insert("bar", 2);
QSparseHash<QString, int>::const_iterator i = hash.begin();
while (i != hash.end()) {
    qDebug("%s: %d", qPrintable(i.key()), i.value());
    ++i;
}
```
    
**Check if the hash contains a key:**

```c++
QSparseHash<QString, int> hash;
hash.insert("foo", 1);
hash.insert("bar", 2);
if (hash.contains("baz")) {
    qDebug("hash contains baz");
}

if (hash.contains("foo")) {
    qDebug("hash contains foo");
}
```

**Dense hash example:**

```c++
QDenseHash<QString, int> hash("" /* empty key */, "!" /* deleted key */); 
hash.insert("foo", 1);
hash.insert("bar", 2);
hash.remove("bar");
qDebug("foo: %d", hash["foo"]);
```

## More documentation

* [sparsehash](http://sparsehash.googlecode.com/svn/trunk/doc/index.html);
* [QHash 4.8](http://qt-project.org/doc/qt-4.8/qhash.html), [QHash 5.0](http://qt-project.org/doc/qt-5.0/qtcore/qhash.html);
* QHash, sparsehash, boost::unordered_map, std::unordered_map, ruby and python maps [benchmark](http://blog.aggregateknowledge.com/2011/11/27/big-memory-part-3-5-google-sparsehash/).
