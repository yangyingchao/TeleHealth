# Automatically Generated Makefile by EDE.
# For use with: automake
#
# DO NOT MODIFY THIS FILE OR YOUR CHANGES MAY BE LOST.
# EDE is the Emacs Development Environment.
# http://cedet.sourceforge.net/ede.shtml
#
AM_LDFLAGS=-lprotobuf -lz  -lzmq -pthread -lthpub -L../public -lsqlite3 -lleveldb
AM_CPPFLAGS=-DDEBUG -I../public -I../public/generated/c

top=../../
ede_FILES=Project.ede Makefile.am

SUBDIRS= InternalMessages/ unittest/
EXTRA_DIST=Project.ede
bin_PROGRAMS=THServer DbServer ServerNode
lib_LTLIBRARIES=libthserver.la
THServer_SOURCES=THServer.cpp
DbServer_SOURCES=DBServer.cpp KVDB.cpp KVDBImplLevelDB.cpp
ServerNode_SOURCES=WorkerNode.cpp MessageProcessor.cpp WorkerThread.cpp
libthserver_la_SOURCES=InternalMessages.pb.cc InternalMessages.cpp
THServer_LDADD=
DbServer_LDADD=
ServerNode_LDADD= -lthserver

# End of Makefile.am
