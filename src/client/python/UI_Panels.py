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


class UI_SingleUserFrame(UI_BasePanel):
    """
    A mini frame to show single user's information
    """

    def __init__(self, parent, user):
        """
        Constructor
        """
        self.user = user

        UI_BasePanel.__init__(self, parent)
        pass

    def Draw(self):
        """
        Create necessary widgets and fill into sizer.
        """
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.label = wx.StaticText(self, -1, "Current User:%s"%self.user.name,
                                   style=wx.ALIGN_CENTER)
        self.label.SetBackgroundColour('Yellow')
        self.sizer.Add(self.label, 0, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER, 5)

        self.gridView = UI_SimpleGrid(self)
        self.sizer.Add(self.gridView, 1, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER, 5)

        self.SetSizerAndFit(self.sizer)
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

        #TODO: Replace this listCtrl with customized view.
        self.lstCtrl = UI_MainPanelListCtrl(self)
        #TODO: Load external data here!
        self.lstCtrl.LoadData(None)
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
        pass


    def OnBtnOpenClicked(self, evt):
        """
        Handling AddButton clicked event.
        Arguments:
        - `evt`:
        """
        print("OnBtnOpenClicked called!\n")
        #TODO: replace this mini-frame with a in-place panel.
        user = self.lstCtrl.GetItemDataEnhanced()
        if user is not None:
            frame = UI_SingleUserFrame(self, user)
            frame.Show()
        pass

    #XXX: Callback functions called by its children
    #TODO: Update parameters ...
    def ListItemSelected(self, evt):
        """
        Notify that a list item is selected.
        Arguments:
        - `idx`: index of item.
        """

        #TODO: Check if those buttons should be enabled or not.
        self.btnOpen.Enable(True)
        self.btnRemove.Enable(True)

        print("ListItemSelected with ID: %d\n"%evt.GetIndex())

        pass
