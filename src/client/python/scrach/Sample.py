#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import struct
import socket

sys.path.append("../../../public/generated/python")
from THMessage_pb2 import *

HEADER_LENGTH = 48


class THMessage:
    """ THMessage is Basic Message that exchange data with server. It contains
    a header of form (MessageHeader) and kinds of real message body.
    """

    def __init__(self):
        """
        """
        self._header      = None
        self._body         = None
        self._body_blob   = ""
        self._header_blob = ""
        self._packer = struct.Struct("%H%s");
        pass

    def SetMessageHeader(self, header):
        """

        Arguments:
        - `header`:
        """
        self._header = header
        __UpdateHeaderSize()
        pass

    def SetMessageBody(self, body):
        """

        Arguments:
        - `body`:
        """
        self.body = body
        __UpdateHeaderSize()
        pass

    def LoadHeaderFromBlob(self, blob):
        """

        Arguments:
        - `blob`:
        """
        self._header_blob = blob
        pass

    def GetHeaderString(self):
        """
        """
        if self._header:
            self._header_blob = self._packer.pack(self._header.ByteSize(),
                                                  self._header.SerializeToString())
            length = HEADER_LENGTH - len(self._header_blob)
            if length > 0:
                self._header_blob += ' '*length

        return self._header_blob;

    def GetBodyString(self):
        """
        """
        if self._body:
            self.body_blob = self._body.SerializeToString()

        return self._body_blob

    def IsValid(self):
        """
        """
        return True

    def __UpdateHeaderSize():
        """
        """
        if self._header and self.body:
            self._header.length = self.body.ByteSize()
        pass


class Socket:
    '''demonstration class only
    - coded for clarity, not efficiency
    '''

    def __init__(self, sock=None):
        if sock is None:
            self.sock = socket.socket(
                socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock

    def Connect(self, host, port):
        self.sock.connect((host, port))

    def Send(self, msg):

        # Check if it is valid.
        if not msg.IsValid():
            return False

        # Send header first.
        result = __SendProc(msg.GetHeaderString(), HEADER_LENGTH)
        if result:
            # Send body
            data = msg.GetBodyString()
            result = __SendProc(data, len(data))
        return result

    def __SendProc(self, data, length):
        """

        Arguments:
        - `data`:
        - `length`:
        """
        # Send header first.
        totalsent = 0
        while totalsent < length:
            sent = self.sock.send(msg[totalsent:])
            if sent == 0:
                return False
            totalsent = totalsent + sent
        return True

    def Recv(self):
        return ""

    def __RecvProc(self, length):
        """
        """
        msg = ''
        while len(msg) < length:
            chunk = self.sock.recv(length-len(msg))
            if chunk == '':
                raise RuntimeError("socket connection broken")
            msg = msg + chunk
        return msg


    def Close(self):
        """
        """
        self.sock.close()

HOST = '127.0.0.1'        # The remote host
PORT = 5678              # The same port as used by the server

sock = Socket();
sock.Connect(HOST, PORT)

a = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";

sock.Send(a, len(a))
data = sock.Recv(1024)
sock.Close()
print 'Received', repr(data)
