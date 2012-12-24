#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import wx

sys.path.append("../../public/generated/python")

from eventids import *
from UI_Panels import *

class Client(wx.Frame):
    """
    """

    def __init__(self):
        """
        """
        wx.Frame.__init__(self, None, wx.ID_ANY, title='TempClient',
                          size=wx.Size(800, 600))

        self.m_mainPanel = None

        self.CreateMenuBar()
        self.CreateMainPanel()

        self.Show()
        self.Fit()

    def CreateMenuBar(self):
        """
        """
        menubar = wx.MenuBar()
        fileMenu = wx.Menu()
        qmi = wx.MenuItem(fileMenu, APP_EXIT, '&Quit\tCtrl+Q')
        fileMenu.AppendItem(qmi)
        self.Bind(wx.EVT_MENU, self.OnQuit, id=APP_EXIT)

        menubar.Append(fileMenu, '&File')
        self.SetMenuBar(menubar)


    def CreateStatusBar(self):
        """
        """
        statusBar = wx.StatusBar(self, wx.ID_ANY, name="Welcome to this test cleint!")
        statusBar.Show()
        self.SetStatusBar(statusBar)
        pass

    def CreateMainPanel(self):
        """
        Create MainPanel.
        """
        self.m_mainPanel = UI_MainPanel(self);
        pass


    def helloevent(self,event):
        msg = 'hello'
        msgbox = wx.MessageDialog(self,message = msg,style = wx.OK)
        if msgbox.ShowModal() == wx.ID_OK:
            msgbox.Destroy()

    def OnQuit(self, event):
        """
        """
        self.Close()

def main(redirect):
    """
    Run this application.
    """
    ex = wx.App(redirect)
    Client()
    ex.MainLoop()


if __name__ == '__main__':
    redirect = False
    if len(sys.argv) > 1:
        redirect = True

    main(redirect)
