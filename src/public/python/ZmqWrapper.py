#!/usr/bin/env python
# -*- coding: utf-8 -*-

import THMessage_pb2
import zmq
import sys

from zmq import REQ, REP, PUB, SUB, PAIR, DEALER, ROUTER, PULL, PUSH, XSUB, XPUB


class ZmqMessage:
    """Wrapper of zmq_message_t
    """

    def __init__(self):
        """
        """
        pass

    def get(self):
        """Get raw zmq_msg_t
        """
        pass

    def data(self):
        """Return raw data.
        """
        pass


class ZmqSocket:
    """Wrapper of zmq_socket.
    """

    def __init__(self, stype=None, ctx=None):
        """Initializer of ZmqSocket
        """
        self.ctx = ctx
        if ctx is None:
            self.ctx = zmq.Context()
        self.sock = self.ctx.socket(stype)
        self.stype = stype
        pass

    def Recv(self):
        """Recv a ZmqMessage
        """
        pass

    def Send(self, zmsg):
        """Send a ZmqMessage

        Arguments:
        - `zmsg`: message to be sent.
        """
        pass

    def SendRaw(self, data):
        """Send raw data

        Arguments:
        - `data`: raw data to be send
        """
        self.sock.send(data)

    def RecvRaw(self):
        """
        """
        return self.sock.recv()


    def Bind(self, addr):
        """Bind to an address

        Arguments:
        - `addr`: address to be bound.
        """
        result = False
        if self.stype == REP:
            try:
                self.sock.bind(addr)
            except:
                print sys.exc_info()
            else:
                result = True
        return result

    def Connect(self, addr):
        """Connect to an address

        Arguments:
        - `addr`: address to be connected.
        """
        result = False
        if self.stype == REQ:
            try:
                self.sock.connect(addr)
            except:
                print sys.exc_info()
            else:
                result = True
                return result

if __name__ == '__main__':

    # 1. Test socket.
    # 1.1 REP socket.
    sock = ZmqSocket(REP)
    ret = sock.Bind("tcp://127.0.0.1:5880")
    print ret

    # 1.2 REQ socket
    sock2 = ZmqSocket(REQ)
    sock2.Connect("tcp://127.0.0.1:5880")
    print ret
