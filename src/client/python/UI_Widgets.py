#!/usr/bin/env python
# -*- coding: utf-8 -*-

import wx
from DBTables_pb2 import *
import  wx.grid   as  gridlib

class UI_MainPanelListCtrl(wx.ListCtrl):
    """
    ListCtrl used by MainPanel
    """
    def __init__(self, parent):
        """
        Constructor.
        """
        wx.ListCtrl.__init__(self, parent, -1, size=parent.GetSize(),
                             style=wx.LC_ICON|wx.LC_EDIT_LABELS)
        self.parent = parent

        # Create a sample item, should always be added at the end of this list.
        self.dummyItem = wx.ListItem();
        self.dummyItem.SetText("TestItem")
        # self.dummyItem.SetImage(0)

        self.userTable=[]

        # Binding event handlers.
        self.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnItemSelected, self)
        self.Bind(wx.EVT_LIST_BEGIN_LABEL_EDIT, self.OnItemEditBegin, self)
        self.Bind(wx.EVT_LIST_END_LABEL_EDIT, self.OnItemEditEnd, self)


    def LoadData(self, data=None):
        """
        Load data and fill into this ListCtrl
        Arguments:
        - `data`: data to be loaded.
        """

        #TODO: Load real data.
        if isinstance(data, list):
            self.data = data
            #TODO: fill listctrl with data.

        self.lastItem = self.InsertItem(self.dummyItem)
        pass

    def GetItemDataEnhanced(self, index = -1):
        """
        Get enhanced data.
        Arguments:
        - `index`:
        """
        user = None
        if index < len(self.userTable) and index != -1:
            user = self.userTable[index]

        #XXX: Remove test code!
        user = User();
        user.name = "TestUser"
        user.uuid = "xxxxxxxxxxxx";
        return user

    ### Event Handlers ....
    def OnItemSelected(self, evt):
        """
        Notify that a list item is selected.
        Arguments:
        - `idx`: index of item.
        """
        self.parent.ListItemSelected(evt)
        pass

    def OnItemEditBegin(self, evt):
        """
        Begin to edit label
        Arguments:
        - `evt`:
        """
        pass

    def OnItemEditEnd(self, evt):
        """
        Finished to edit label
        Arguments:
        - `evt`:
        """
        pass


class UI_SimpleGrid(gridlib.Grid):
    """
    Simple grid used to show records of some user
    """
    def __init__(self, parent):
        gridlib.Grid.__init__(self, parent, -1)
        ##mixins.GridAutoEditMixin.__init__(self)
        self.moveTo = None

        # self.Bind(wx.EVT_IDLE, self.OnIdle)

        self.CreateGrid(25, 5)#, gridlib.Grid.SelectRows)
        ##self.EnableEditing(False)

        # simple cell formatting
        self.SetColSize(3, 200)
        self.SetRowSize(4, 45)

        self.SetColLabelValue(0, "Custom")
        self.SetColLabelValue(1, "column")
        self.SetColLabelValue(2, "labels")

        # attribute objects let you keep a set of formatting values
        # in one spot, and reuse them if needed
        attr = gridlib.GridCellAttr()
        attr.SetTextColour(wx.BLACK)
        attr.SetBackgroundColour(wx.RED)
        attr.SetFont(wx.Font(10, wx.SWISS, wx.NORMAL, wx.BOLD))

        # you can set cell attributes for the whole row (or column)
        self.SetRowAttr(5, attr)


        self.SetColLabelAlignment(wx.ALIGN_LEFT, wx.ALIGN_BOTTOM)

        self.SetCellValue(0, 0, "First cell")
        self.SetCellValue(1, 1, "Another cell")
        self.SetCellValue(2, 2, "Yet another cell")
        self.SetCellValue(3, 3, "This cell is read-only")
        self.SetCellFont(0, 0, wx.Font(12, wx.ROMAN, wx.ITALIC, wx.NORMAL))
        self.SetCellTextColour(1, 1, wx.RED)
        self.SetCellBackgroundColour(2, 2, wx.CYAN)
        self.SetReadOnly(3, 3, True)

        self.SetCellEditor(5, 0, gridlib.GridCellNumberEditor(1,1000))
        self.SetCellValue(5, 0, "123")
        self.SetCellEditor(6, 0, gridlib.GridCellFloatEditor())
        self.SetCellValue(6, 0, "123.34")
        self.SetCellEditor(7, 0, gridlib.GridCellNumberEditor())

        self.SetCellValue(6, 3, "You can veto editing this cell")

        #self.SetRowLabelSize(0)
        #self.SetColLabelSize(0)


        #self.SetDefaultCellOverflow(False)
        #r = gridlib.GridCellAutoWrapStringRenderer()
        #self.SetCellRenderer(9, 1, r)

        # overflow cells
        self.SetCellValue( 9, 1, "This default cell will overflow into neighboring cells, but not if you turn overflow off.");
        self.SetCellSize(11, 1, 3, 3);
        self.SetCellAlignment(11, 1, wx.ALIGN_CENTRE, wx.ALIGN_CENTRE);
        self.SetCellValue(11, 1, "This cell is set to span 3 rows and 3 columns");


        editor = gridlib.GridCellTextEditor()
        editor.SetParameters('10')
        self.SetCellEditor(0, 4, editor)
        self.SetCellValue(0, 4, "Limited text")

        renderer = gridlib.GridCellAutoWrapStringRenderer()
        self.SetCellRenderer(15,0, renderer)
        self.SetCellValue(15,0, "The text in this cell will be rendered with word-wrapping")


        # test all the events
        self.Bind(gridlib.EVT_GRID_CELL_LEFT_CLICK, self.OnCellLeftClick)
        self.Bind(gridlib.EVT_GRID_CELL_RIGHT_CLICK, self.OnCellRightClick)
        self.Bind(gridlib.EVT_GRID_CELL_LEFT_DCLICK, self.OnCellLeftDClick)
        self.Bind(gridlib.EVT_GRID_CELL_RIGHT_DCLICK, self.OnCellRightDClick)

        self.Bind(gridlib.EVT_GRID_LABEL_LEFT_CLICK, self.OnLabelLeftClick)
        self.Bind(gridlib.EVT_GRID_LABEL_RIGHT_CLICK, self.OnLabelRightClick)
        self.Bind(gridlib.EVT_GRID_LABEL_LEFT_DCLICK, self.OnLabelLeftDClick)
        self.Bind(gridlib.EVT_GRID_LABEL_RIGHT_DCLICK, self.OnLabelRightDClick)

        self.Bind(gridlib.EVT_GRID_ROW_SIZE, self.OnRowSize)
        self.Bind(gridlib.EVT_GRID_COL_SIZE, self.OnColSize)

        self.Bind(gridlib.EVT_GRID_RANGE_SELECT, self.OnRangeSelect)
        self.Bind(gridlib.EVT_GRID_CELL_CHANGE, self.OnCellChange)
        self.Bind(gridlib.EVT_GRID_SELECT_CELL, self.OnSelectCell)

        self.Bind(gridlib.EVT_GRID_EDITOR_SHOWN, self.OnEditorShown)
        self.Bind(gridlib.EVT_GRID_EDITOR_HIDDEN, self.OnEditorHidden)
        self.Bind(gridlib.EVT_GRID_EDITOR_CREATED, self.OnEditorCreated)


    def OnCellLeftClick(self, evt):
        print("OnCellLeftClick: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()

    def OnCellRightClick(self, evt):
        print("OnCellRightClick: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()

    def OnCellLeftDClick(self, evt):
        print("OnCellLeftDClick: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()

    def OnCellRightDClick(self, evt):
        print("OnCellRightDClick: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()

    def OnLabelLeftClick(self, evt):
        print("OnLabelLeftClick: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()

    def OnLabelRightClick(self, evt):
        print("OnLabelRightClick: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()

    def OnLabelLeftDClick(self, evt):
        print("OnLabelLeftDClick: (%d,%d) %s\n" %
              (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()

    def OnLabelRightDClick(self, evt):
        print("OnLabelRightDClick: (%d,%d) %s\n" %
              (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()

    def OnRowSize(self, evt):
        print("OnRowSize: row %d, %s\n" %
                       (evt.GetRowOrCol(), evt.GetPosition()))
        evt.Skip()

    def OnColSize(self, evt):
        print("OnColSize: col %d, %s\n" %
                       (evt.GetRowOrCol(), evt.GetPosition()))
        evt.Skip()

    def OnRangeSelect(self, evt):
        if evt.Selecting():
            msg = 'Selected'
        else:
            msg = 'Deselected'
        print("OnRangeSelect: %s  top-left %s, bottom-right %s\n" %
                       (msg, evt.GetTopLeftCoords(), evt.GetBottomRightCoords()))
        evt.Skip()


    def OnCellChange(self, evt):
        print("OnCellChange: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))

        # Show how to stay in a cell that has bad data.  We can't just
        # call SetGridCursor here since we are nested inside one so it
        # won't have any effect.  Instead, set coordinates to move to in
        # idle time.
        value = self.GetCellValue(evt.GetRow(), evt.GetCol())

        if value == 'no good':
            self.moveTo = evt.GetRow(), evt.GetCol()


            def OnIdle(self, evt):
                if self.moveTo != None:
                    self.SetGridCursor(self.moveTo[0], self.moveTo[1])
            self.moveTo = None

        evt.Skip()


    def OnSelectCell(self, evt):
        if evt.Selecting():
            msg = 'Selected'
        else:
            msg = 'Deselected'
        print("OnSelectCell: %s (%d,%d) %s\n" %
                       (msg, evt.GetRow(), evt.GetCol(), evt.GetPosition()))

        # Another way to stay in a cell that has a bad value...
        row = self.GetGridCursorRow()
        col = self.GetGridCursorCol()

        if self.IsCellEditControlEnabled():
            self.HideCellEditControl()
            self.DisableCellEditControl()

        value = self.GetCellValue(row, col)

        if value == 'no good 2':
            return  # cancels the cell selection

        evt.Skip()


    def OnEditorShown(self, evt):
        if evt.GetRow() == 6 and evt.GetCol() == 3 and \
                wx.MessageBox("Are you sure you wish to edit this cell?",
                              "Checking", wx.YES_NO) == wx.NO:
            evt.Veto()
            return

        print("OnEditorShown: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()


    def OnEditorHidden(self, evt):
        if evt.GetRow() == 6 and evt.GetCol() == 3 and \
                wx.MessageBox("Are you sure you wish to  finish editing this cell?",
                              "Checking", wx.YES_NO) == wx.NO:
            evt.Veto()
            return

        print("OnEditorHidden: (%d,%d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetPosition()))
        evt.Skip()


    def OnEditorCreated(self, evt):
        print("OnEditorCreated: (%d, %d) %s\n" %
                       (evt.GetRow(), evt.GetCol(), evt.GetControl()))
