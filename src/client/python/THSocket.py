#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import THMessage
import struct

SERVER_PORT = 5678
SERVER_HOST = "localhost"

TMP_FLAG = 1

HEADER_FMT    = "BBBBL"
HEADER_LENGTH = 16 # XXX: Update this.

class ClientSocket:
    """
    This class create a socket that connects to specified host server and
    port. Then user can use Send() and Receive() to send or receive messages
    from server.
    Note: At the beginning of Send() and Receive(), a package header must be
    sent/received before the message body. Refer to src/public/MessageBase.h
    for details of message header.
    """

    def __init__(self, host=SERVER_HOST, port=SERVER_PORT):
        """
        """
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((host, port))
            self.isOK = True
        except:
            self.isOk = False
        finally:
            pass


    def Send(self, msg):
        """
        Send msg with header. Procedure:
        1. Send Header.
        2. Send MessageBody
        """
        totalsent = 0
        while totalsent < HEADER_LENGTH:
            sent = self.sock.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent

    def Receive(self):
        """
        """
        msg = ''
        while len(msg) < MSGLEN:
            chunk = self.sock.recv(MSGLEN-len(msg))
            if chunk == '':
                raise RuntimeError("socket connection broken")
            msg = msg + chunk
        return msg
