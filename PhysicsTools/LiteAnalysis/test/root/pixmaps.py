#!/usr/bin/env python
#-----------------------------------------------------------------------------
# File:        pixmaps.py
# Description: Display pixmaps
# Created:     26-Sep-2003 Harrison B. Prosper
#-----------------------------------------------------------------------------
#$Revision: $
#-----------------------------------------------------------------------------
import sys, os
from ROOT import *
from PhysicsTools.LiteAnalysis import *
from glob import glob
from array import array
#-----------------------------------------------------------------------------
# CONSTANTS
#-----------------------------------------------------------------------------
VERSION        = \
"""
pixmaps.py v1.0, 26-Sep-2003
Python 2.3
Root   3.05/7
"""

ICONDIR        = os.environ["ROOTSYS"]+"/icons"

XDIV           = 12             # Canvas X-divisions
YDIV           = 8              # Canvas Y-divisions
MAXPADS        = XDIV * YDIV    # Maximum number of pads
MAXPAGES       = 10             # Maximum number of pages
PIXSIZE        = 64             # Pixmap size in pixels
WIDTH          = PIXSIZE*XDIV   # Canvas width in pixels
HEIGHT         = PIXSIZE*YDIV   # Canvas height in pixels
STATUS_PARTS   = array('i')     # Status bar division in percentage
STATUS_PARTS.append(23)
STATUS_PARTS.append(77)
CLOCK_DELAY    = 1000           # milliseconds
FILETYPES      = ['.xpm','.png']# Pixmap file types
PSTYPES        = {'.ps' :112,   # Postscript file types
                  '.eps':113
                  }
    
# Layouts

TOP                = TGLayoutHints(kLHintsTop)
BOTTOM             = TGLayoutHints(kLHintsBottom)
CENTERED           = TGLayoutHints(kLHintsCenterX |
                                   kLHintsCenterY)
LEFT               = TGLayoutHints(kLHintsLeft)
RIGHT              = TGLayoutHints(kLHintsRight)

fToolBarLayout     = TGLayoutHints(kLHintsTop |
                                   kLHintsExpandX)
fMenuBarLayout     = TGLayoutHints(kLHintsTop  |
                                   kLHintsLeft |
                                   kLHintsExpandX)
fMenuBarItemLayout = TGLayoutHints(kLHintsTop |
                                   kLHintsLeft)
fMenuBarHelpLayout = TGLayoutHints(kLHintsTop |
                                   kLHintsRight)
fStatusBarLayout   = TGLayoutHints(kLHintsTop  |
                                   kLHintsLeft |
                                   kLHintsExpandX)


# Menu codes and callbacks

M_CALLBACK         = {} # Map between menu items and callbacks
                        # Each menu item is identified by a unique code
                        # and is associated with a unique callback, specified
                        # by name

M_FILE_OPEN        = 1; M_CALLBACK[M_FILE_OPEN]       = "self.add()"
M_FILE_SAVEAS_PDF  = 2; M_CALLBACK[M_FILE_SAVEAS_PDF] = "self.saveAsPDF()"
M_FILE_SAVEAS_GIF  = 3; M_CALLBACK[M_FILE_SAVEAS_GIF] = "self.saveAsGIF()"
M_FILE_PRINT       = 4; M_CALLBACK[M_FILE_PRINT]      = "self.printIt()"
M_FILE_PRINTSETUP  = 5; M_CALLBACK[M_FILE_PRINTSETUP] = "self.printSetup()"
M_FILE_EXIT        = 6; M_CALLBACK[M_FILE_EXIT]       = "self.exit()"

M_EDIT_UNDO        = 21; M_CALLBACK[M_EDIT_UNDO]      = "self.undo()"
M_EDIT_ADD         = 22; M_CALLBACK[M_EDIT_ADD]       = "self.add()"
M_EDIT_DELETE      = 23; M_CALLBACK[M_EDIT_DELETE]    = "self.delete()"

M_HELP_ABOUT       = 51; M_CALLBACK[M_HELP_ABOUT]     = "self.about()"
M_HELP_USAGE       = 52; M_CALLBACK[M_HELP_USAGE]     = "self.usage()"


# Help

HelpUsage = \
"""
(A) Running scripts

 o Examples
 
    pixmaps.py                     Launch program
    
    pixmaps.py /usr/share/pixmaps  Launch program, scan specified directory
                                   for files with extensions ".xpm" or ".png"
                                   and load pixmaps into one or more canvases.
                                   
 o To view pixmap filename, hold mouse near edge of desired pad.
 

(B) Commands
 
 o File
       Open        ..... Select directory and add icon images to canvases
       Save As ps  ..... Save selected canvas as a postscript file
       Save As eps ..... Save current canvas as an encapsulated postscript file

 o Edit
       Add ............. Select directory and add icon images to one or
                         more canvases. Can have a maximum of 10 canvases. 
"""
HU_WIDTH  = 600
HU_HEIGHT = 250


HelpAbout = \
"""
%s

\tHarrison B. Prosper
\tCMS Experiment
\tCERN, Geneva, Switzerland
\te-mail: Harrison.Prosper@cern.ch

\tPermanent Address
\t\tDepartment of Physics
\t\tFlorida State University
\t\tTallahassee, FL 32306, USA
\t\te-mail: harry@hep.fsu.edu
""" % VERSION

HA_WIDTH  = 350
HA_HEIGHT = 300


#-----------------------------------------------------------------------------
# (A) Root Graphical User Interfaces (GUI)
#
#   A Root GUI is a double tree of widgets (that is, graphical objects) with
#   which the user can interact in order to orchestrate a series of actions.
#   One tree describes the child-to-parent relationships, while the other
#   describes the parent-to-child relationships. The latter describes the
#   graphical layout of widgets. In the Root GUI system the two trees are not
#   isomorphic. For example, the child-to-parent relationship of a TGPopupMenu
#   is TGPopupMenu -> TGWindow, however, TGMenuBar -> TGPopupMenu is 
#   a typical parent-to-child relationship.
#
#   o A child-to-parent relationship is defined by the child widget when the
#     latter is created.
#
#   o A parent-to-child relationship, that is, a specific layout of a widget
#     within another, is defined by the parent after it has been created.
#
#   Each widget can emit one or more signals, usually triggered by some user
#   manipulation of, or action on, the widget. For example, clicking on a
#   tab of a  TGTab, that is, a notebook, will cause the notebook to emit
#   the signal "Selected(Int_t)" along with the identity of the selected tab.
#   Signals are connected to "Slots", that is, actions. This is how a user
#   GUI interaction can be made to yield one or more actions. Any signal can be
#   connected to any slot. Indeed, the relationship between signals and slots
#   can be many-many. In practice, a slot is modeled as a procedure such as the
#   a method of the GUI class. In this GUI, the notebook's Selected(Int_t)
#   signal is connected to the "setPage" method of the GUI. Therefore,
#   whenever a tab is selected the method "setPage" is invoked.
#   
#   In summary, a Root GUI is a (double) tree of widgets with which the user
#   can interact, whose signals---usually generated by user interactions---are
#   connected to slots, that is, actions modeled as procedures.
#
# (B) This Example
#
#   We model this GUI as a TApplication that contains a TGMainFrame. Here is
#   the layout hierarchy (that is, the parent-to-child relationships).
#
#   Window                   (TGWindow)
#
#     Main                      (TGMainFrame)
#
#       MenuBar                    (TGMenuBar)
#         MenuFile                    (TGPopupMenu)
#         MenuEdit                    (TGPopupMenu)
#         MenuHelp                    (TGPopupMenu)
#
#       ToolBar                    (TGToolBar)
#         AddButton                   (TGPictureButton)
#         SaveButton                  (TGPictureButton)
#         PrintButton                 (TGPictureButton)
#
#       Notebook                   (TGTab)
#         page 1                      (TGCompositeFrame)
#           ecanvas 1                    (TRootEmbeddedCanvas)
#          :
#         page n                      (TGCompositeFrame)
#                                        (TRootEmbeddedCanvas)
#
#       StatusBar                  (TGSTatusBar)
#
#-----------------------------------------------------------------------------

class Gui:
    """

    Display pixmaps

    Usage:
        pixmaps.py [icon-directory]
        
    """
    
    def __init__(self, name):
        
        self.Window = gClient.GetRoot()

        #-------------------------------------------------------------------
        # Check if icon directory has been specified on command line. If it
        # has store in IconDir variable, otherwise set IconDir to '.'
        #-------------------------------------------------------------------
        
        name = ""
        if len(sys.argv) > 1:
            name = glob(sys.argv[1])[0]# Make sure directory exists!
        if name <> "":
            self.IconDir = name
        else:
            self.IconDir = ICONDIR
                   
        self.SaveDir    = os.environ['PWD'] # Directory for postscript files
        self.IniDir     = "."               # Initial directory for file dialog
        self.Connection = []                # List of Signal/Slot connections
        self.Images     = []                # List of (icon-image, filename)
        self.Pages      = []                # List of (page, ecanvas, canvas)
        self.lastPage   = 0
        self.lastImage  = 0
        self.lastPad    = 0
        self.currentPage= 0
        self.fi  = TGFileInfo()
        
        #-------------------------------------------------------------------
        # Create main frame
        #-------------------------------------------------------------------
        # Establish a connection between the main frame's "CloseWindow()"
        # signal and the gui's "close" slot, modeled as a method.
        # When the main frame issues the signal CloseWindow() this
        # triggers a call to the close method of the gui.

        self.Main = TGMainFrame(self.Window, WIDTH, HEIGHT)
        self.Connection.append(Connection(self.Main, "CloseWindow()",
                                          self,      "close"))
        
        #-------------------------------------------------------------------
        # Create menu bar
        #-------------------------------------------------------------------
        # MenuBar's parent is Main, a fact made explicit in MenuBar's
        # constructor. This establishes a child-to-parent relationship.
        # We also define how MenuBar is to displayed within Main and thereby
        # establish a parent-to-child relationship. This is done by invoking
        # Main's AddFrame method, wherein we also specify how precisely
        # the MenuBar is to be positioned within Main.
        #-------------------------------------------------------------------
        # The hint objects are used to place and group
        # widgets with respect to each other.

        self.MenuBar = TGMenuBar(self.Main)
        self.Main.AddFrame(self.MenuBar, fMenuBarLayout)

        self.H3DBar  = TGHorizontal3DLine(self.Main)
        self.Main.AddFrame(self.H3DBar, fToolBarLayout)
        
        #-------------------------------------------------------------------
        # Create menus
        #-------------------------------------------------------------------

        self.MenuFile = TGPopupMenu(self.Window)
        self.MenuFile.AddEntry("&Open",           M_FILE_OPEN)
        self.MenuFile.AddEntry("Save As pdf",     M_FILE_SAVEAS_PDF)
        self.MenuFile.AddEntry("&Save As gif",    M_FILE_SAVEAS_GIF)
        self.MenuFile.AddSeparator()
        self.MenuFile.AddEntry("&Print",          M_FILE_PRINT)
        self.MenuFile.AddEntry("P&rint Setup..." ,M_FILE_PRINTSETUP)
        self.MenuFile.AddSeparator()
        self.MenuFile.AddEntry("E&xit",           M_FILE_EXIT)        

        self.Connection.append(Connection(self.MenuFile, "Activated(Int_t)",
                                          self,          "menu"))
        self.MenuBar.AddPopup("&File", self.MenuFile, fMenuBarItemLayout)


        self.MenuEdit = TGPopupMenu(self.Window)
        self.MenuEdit.AddEntry("Undo",            M_EDIT_UNDO)
        self.MenuEdit.AddSeparator()        
        self.MenuEdit.AddEntry("Add",             M_EDIT_ADD)
        self.MenuEdit.AddEntry("Delete",          M_EDIT_DELETE)

        self.Connection.append(Connection(self.MenuEdit, "Activated(Int_t)",
                                          self,          "menu"))
        self.MenuBar.AddPopup("&Edit", self.MenuEdit, fMenuBarItemLayout)

        
        self.MenuHelp = TGPopupMenu(self.Window)
        self.MenuHelp.AddEntry    ("&About",      M_HELP_ABOUT)
        self.MenuHelp.AddSeparator()
        self.MenuHelp.AddEntry    ("&Usage",      M_HELP_USAGE)

        self.Connection.append(Connection(self.MenuHelp, "Activated(Int_t)",
                                          self,          "menu"))
        self.MenuBar.AddPopup("&Help", self.MenuHelp, fMenuBarHelpLayout)

        #-------------------------------------------------------------------
        # Create a notebook
        #-------------------------------------------------------------------

        self.NoteBook = TGTab(self.Main, 1, 1)
        self.Main.AddFrame(self.NoteBook, TOP)
        self.Connection.append(Connection(self.NoteBook, "Selected(Int_t)",
                                          self,          "setPage"))

        #-------------------------------------------------------------------
        # Create a status bar, divided into two parts
        #-------------------------------------------------------------------

        self.StatusBar = TGStatusBar(self.Main, 1, 1)
        status_parts = array('i')
        status_parts.append(23)
        status_parts.append(77)
        self.StatusBar.SetParts(status_parts, len(status_parts))
        self.Main.AddFrame(self.StatusBar, fStatusBarLayout)

        #-------------------------------------------------------------------
        # Add pages to notebook
        #-------------------------------------------------------------------
        
        self.addFirstPage()
        self.addPages()
        self.setPage(0)
        
        self.mapWindows()
        
    def __del__(self):
        pass


    def mapWindows(self):

        self.Main.SetWindowName("PixMap Display")

        # Map all subwindows of main frame
        self.Main.MapSubwindows()

        # Initialize layout        
        self.Main.Resize(self.Main.GetDefaultSize())

        # Finally map the main frame (render GUI visible)        
        self.Main.MapWindow()
        
    #---------------------------
    # Slots
    #---------------------------

    # Responds to: CloseWindow()    issued by Main

    def close(self):

        gApplication.Terminate()
        
    # Responds to: Activated(Int_t) issued by MenuFile, MenuEdit, MenuHelp
    
    def menu(self,id):
        if id in M_CALLBACK:
            print "%5d %s" % (id,M_CALLBACK[id])
            exec(M_CALLBACK[id])

        else:
            print "Unrecognized menu id = %d" % id

    # Responds to: Selected(Int_t)  issued by Notebook

    def setPage(self,id):

        # Before changing current tab's color,  re-color previous tab to the
        # TGMainframe's default color
        
        tab = self.NoteBook.GetTabTab(self.currentPage)
        tab.ChangeBackground(self.Main.GetDefaultFrameBackground())

        # Now re-color current tab
        
        self.currentPage = id
        
        tab = self.NoteBook.GetTabTab(self.currentPage)
        tab.ChangeBackground(root.ColorByName("yellow"))

        self.canvas = self.Pages[self.currentPage][2]
        
    #---------------------------
    # Methods
    #---------------------------

    def loadImages(self):
        self.StatusBar.SetText("Loading pixmaps ...", 0)
        for Ext in FILETYPES:
            command = "%s/*%s" % (self.IconDir, Ext)
            filelist = glob(command)
            filelist.sort()
            for filename in filelist:
                image = TImage.Open(filename)
                if not image:
                    print "*** Failed to read %s " % filename
                else:
                    self.Images.append((image,filename))
                    self.StatusBar.SetText(filename, 1)
                    self.StatusBar.SetText("Number of pixmaps %d" % \
                                           len(self.Images), 0)

    def addFirstPage(self):
        pageNumber = 1
        page       = self.NoteBook.AddTab("pixmaps%2.2d" % pageNumber)
        ecanvas    = TRootEmbeddedCanvas("ecanvas%2.2d"  % pageNumber, page,
                                         WIDTH, HEIGHT)
        page.AddFrame(ecanvas, TOP)

        canvas     = ecanvas.GetCanvas()   # Return TCanvas         
        canvas.Divide(XDIV, YDIV)          # Divide into pads
        canvas.SetFillColor(0)             # White background
        canvas.SetBorderMode(0)            # No border
        
        self.Pages.append((page,ecanvas,canvas))
        self.lastPage = pageNumber
        
    def addPages(self):
        
        if self.lastPage >= MAXPAGES:
            return
        
        self.loadImages()
        
        pageNumber = self.lastPage
        padNumber  = self.lastPad
        
        for image, filename in self.Images[self.lastImage:]:

            padNumber = padNumber + 1
            
            if padNumber > MAXPADS:
                pageNumber= pageNumber + 1
                padNumber = 1

                page      = self.NoteBook.AddTab("pixmaps%2.2d" %pageNumber)
                ecanvas   = TRootEmbeddedCanvas("ECanvas%d" % pageNumber, page,
                                                WIDTH, HEIGHT)
                page.AddFrame(ecanvas, TOP)

                canvas    = ecanvas.GetCanvas()            
                canvas.Divide(XDIV, YDIV)
                canvas.SetFillColor(0)    # White
                canvas.SetBorderMode(0)   # No border
                
                self.Pages.append((page,ecanvas,canvas))

            else:
                page, ecanvas, canvas = self.Pages[pageNumber-1]

            # Move to current pad, draw current image
            # and set the text in the tool tip to be the filename
            # of the image
            
            canvas.cd(padNumber)
            image.Draw() 
            gPad.SetToolTipText(filename)
            
        # Update canvases
        
        for page, ecanvas, canvas in self.Pages:
            canvas.Update()
        
        self.lastPage  = len(self.Pages)
        self.lastImage = len(self.Images)
        self.lastPad   = padNumber
        
    def undo(self):
        self.notdone()

    def add(self):

        print "Call dialog"
        
        self.fdialog = TFileDialog(self.Window,
                                   self.Main,
                                   kFDOpen,
                                   self.IniDir)

        self.IconDir = self.fdialog.Filename()
        self.IniDir  = self.fdialog.IniDir()

        self.addPages()
        
        # Need to remap Notebook subwindows
        
        self.NoteBook.MapSubwindows()
        self.NoteBook.Layout()
        
    def delete(self):
        self.notdone()
        
    def saveAsPDF(self):
        self.saveAs(".pdf")

    def saveAsGIF(self):
        self.saveAs(".gif")

    def saveAs(self, ext):
        filename = self.saveFilename(ext)
        ps = TPostscript(filename, PSTYPE[ext])
        self.canvas.Update()
        ps.Close()
        
    def saveFilename(self, ext):
        name = "pixmaps%2.2d%s" % (self.currentPage+1, ext)
        self.StatusBar.SetText("Saving canvas to file %s in directory %s"
                               % (name, self.SaveDir), 1)
        return self.SaveDir + "/" + name
    
    def printIt(self):
        self.notdone()

    def printSetup(self):
        self.notdone()

    def usage(self):
        THelpDialog(self.Window, "Usage", HelpUsage, HU_WIDTH, HU_HEIGHT)

    def about(self):
        THelpDialog(self.Window, "About", HelpAbout, HA_WIDTH, HA_HEIGHT)

    def exit(self):
        self.close()
        
    def notdone(self):
        print "Not done"
        THelpDialog(self.Window,"Not Yet Done!", "\tGo boil your head!",
                    220, 30)

    def Run(self):
        gApplication.Run()
        
#------------------------------------------------------------------------------
#---- Main program ------------------------------------------------------------
#------------------------------------------------------------------------------

def main():
    gui = Gui("PixMap")
    gui.Run()
#------------------------------------------------------------------------------
main()



