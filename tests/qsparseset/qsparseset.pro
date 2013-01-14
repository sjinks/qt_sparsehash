CONFIG  += testcase
TARGET   = tst_qsparseset
QT       = core testlib
SOURCES  = tst_qsparseset.cpp
DESTDIR  = ../

INCLUDEPATH += ../../src ../../

*g++* {
	QMAKE_CXXFLAGS += -fno-strict-aliasing
}

include(../common.pri)
