CONFIG  += testcase
TARGET   = tst_qdenseset
QT       = core testlib
SOURCES  = tst_qdenseset.cpp
DESTDIR  = ../

INCLUDEPATH += ../../src ../../

*g++* {
	QMAKE_CXXFLAGS += -fno-strict-aliasing
}
