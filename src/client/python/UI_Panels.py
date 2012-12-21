#!/usr/bin/env python
# -*- coding: utf-8 -*-

import wx

from UI_Widgets import *


class UI_BasePanel(wx.Panel):
    """
    Base UI Panel.
    """

    def __init__(self, parent=None):
        """
        Constructor it will call constructor of parent class, then call
        virtual function Draw() to create necessary widgets.
        """
        wx.Panel.__init__(self, parent, wx.ID_ANY, size=parent.GetSize())

        self.parent = parent
        self.sizer = None

        self.Draw()

        self.Show(True)


    def Draw(self):
        """
        Virtual function to be implemented by all children.
        """
        pass



class UI_MainPanel(UI_BasePanel):
    """
    MainPanel.
    """

    def __init__(self, parent=None):
        """
        Constructor.
        """
        UI_BasePanel.__init__(self, parent)

    def Draw(self):
        """
        Create necessary widgets and fill into sizer.
        """
        self.sizer   = wx.BoxSizer(wx.VERTICAL)
        self.lstCtrl = UI_MainPanelListCtrl(self)
        self.sizer.Add(self.lstCtrl, 1, wx.ALL, 5)

         # TODO: Add Images to buttons.
        btnSizer = wx.BoxSizer(wx.HORIZONTAL)
        self.btnAdd = wx.Button(self,wx.ID_ANY,label = 'Add')
        self.btnAdd.Bind(wx.EVT_LEFT_DOWN, self.OnBtnAddClicked)
        self.btnAdd.Enable(True)
        btnSizer.Add(self.btnAdd, 1, wx.ALL|wx.EXPAND|wx.ALIGN_CENTRE_HORIZONTAL, 5)

        self.btnRemove = wx.Button(self,wx.ID_ANY,label = 'Remove')
        self.btnRemove.Bind(wx.EVT_LEFT_DOWN, self.OnBtnRemoveClicked)
        self.btnRemove.Enable(False)
        btnSizer.Add(self.btnRemove, 1, wx.ALL|wx.EXPAND|wx.ALIGN_CENTRE_HORIZONTAL, 5)

        self.btnOpen = wx.Button(self,wx.ID_ANY,label = 'Open')
        self.btnOpen.Bind(wx.EVT_LEFT_DOWN, self.OnBtnOpenClicked)
        self.btnOpen.Enable(False)
        btnSizer.Add(self.btnOpen, 1, wx.ALL|wx.EXPAND|wx.ALIGN_CENTRE_HORIZONTAL, 5)

        self.sizer.Add(btnSizer, 0, wx.ALL)

        self.SetSizerAndFit(self.sizer)

        self.Bind(wx.EVT_LIST_ITEM_DESELECTED, self.ListItemSelected, self.lstCtrl)


    def OnBtnAddClicked(self, evt):
        """
        Handling AddButton clicked.
        Arguments:
        - `evt`:
        """
        print("OnBtnAddClicked called!\n",)



    def OnBtnRemoveClicked(self, evt):
        """
        Handling AddButton clicked event.
        Arguments:
        - `evt`:
        """
        print("OnBtnRemoveClicked called!\n",)
        print("OnBtnRemoveClicked called!\n")



    def OnBtnOpenClicked(self, evt):
        """
        Handling AddButton clicked event.
        Arguments:
        - `evt`:
        """
        print("OnBtnOpenClicked called!\n")

    def ListItemSelected(self, idx):
        """
        Notify that a list item is selected.
        Arguments:
        - `idx`: index of item.
        """
        self.btnOpen.Enable(True)
        print("ListItemSelected with ID: %d\n"%idx)
