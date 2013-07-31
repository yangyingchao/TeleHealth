#!/usr/bin/env python
# -*- coding: utf-8 -*-

import THMessage_pb2
import zmq

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

    def __init__(self, type=None, ctx=None):
        """Initializer of ZmqSocket
        """
        self.ctx = ctx
        if ctx is None:
            self.ctx = zmq.Context()
        self.sock = self.ctx.socket(type)
        pass

    def Recv(self):
        """
        """
        pass

    def Send(self, zmsg):
        """

        Arguments:
        - `zmsg`:
        """
        pass


if __name__ == '__main__':
    sock = ZmqSocket(REQ)
    print("ok\n")
