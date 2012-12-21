#!/usr/bin/env python
# -*- coding: utf-8 -*-

import wx

class UI_MainPanelListCtrl(wx.ListCtrl):
    """
    ListCtrl used by MainPanel
    """
    def __init__(self, parent):
        """
        Constructor.
        """
        wx.ListCtrl.__init__(self, parent, -1, size=parent.GetSize(), style=wx.LC_ICON)
        self.parent = parent

        # Create a sample item, should always be added at the end of this list.
        self.dummyItem = wx.ListItem();
        self.dummyItem.SetText("TestItem")
        # self.dummyItem.SetImage(0)

        self.LoadData()

    def LoadData(self, data=None):
        """
        Load data and fill into this ListCtrl
        Arguments:
        - `data`: data to be loaded.
        """

        #TODO: Load real data.
        pass

        self.lastItem = self.InsertItem(self.dummyItem)
