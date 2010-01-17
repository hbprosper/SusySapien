#!/usr/bin/env python
#-----------------------------------------------------------------------------
# File:        mkntuplecfi.py
# Description: Gui to create cfi fragment for Mkntuple
# Created:     06-Jan-2010 Harrison B. Prosper
# Updated:     16-Jan-2010 HBP - simplify command line
#-----------------------------------------------------------------------------
#$Revision: 1.1.2.1 $
#-----------------------------------------------------------------------------
import sys, os, re, platform
from ROOT import *
#-----------------------------------------------------------------------------
def usage():
	print """
Usage:
      mkntuplecfi.py [root-file-dir]
   
      root-file-dir     directory containing root files. If omitted,
		                  the default is the current directory
		"""
	sys.exit(0)
ARGV = sys.argv[1:]
if len(ARGV) > 0 and ARGV[0] == "?": usage()

# Make sure CMSSW is set up
if not os.environ.has_key("CMSSW_BASE"):
	print "\t*** please set up CMSSW first\n"
	sys.exit(0)

BASE           = os.environ["CMSSW_BASE"]
VERSION        = \
"""
mkntuplecfi.py v1.0.1 January 2010
Python %s
Root   %s
""" % (platform.python_version(),
	   gROOT.GetVersion())
ICONDIR        = "%s/icons" % os.environ["ROOTSYS"]
METHODDIR      = "%s/src/PhysicsTools/LiteAnalysis/dataformats/methods" % \
				 BASE

if not os.path.exists(METHODDIR):
	print "\t** cannot find methods directory:\n%s" % METHODDIR
	sys.exit(0)

print VERSION
#-----------------------------------------------------------------------------
from string import *
from time import *
from glob import glob
from array import array
from PhysicsTools.LiteAnalysis import *
#-----------------------------------------------------------------------------

WIDTH          = 750            # Width of GUI in pixels
HEIGHT         = 500            # Height of GUI in pixels
# StatusBar components
STATUS_PARTS   = array('i')     # Status bar division in percentage
STATUS_PARTS.append(23)
STATUS_PARTS.append(77)



# We need a few simple regular expressions to extract sub-strings

# Extract branches ending in
getbranch    = re.compile(r'(?<=\_).+\. / .+$',re.M)

# Extract class name from branch name
getclass  = re.compile(r'(?<=Wrapper\<).+(?=\>)')

# Extract getByLabel string from branch name
getlabel  = re.compile(r'.+(?=\_\_)|.+(?=\_)')

# Strip away namespace, vector< etc.
stripname = re.compile(r'::|vector\<|\>| ')

def isVector(name):
	return find(name, "vector<") > -1

# GUI widget Layouts

TOP          = TGLayoutHints(kLHintsTop)
LEFT         = TGLayoutHints(kLHintsLeft)
RIGHT        = TGLayoutHints(kLHintsRight)
LEFT_X       = TGLayoutHints(kLHintsLeft | kLHintsExpandX)
LEFT_X       = TGLayoutHints(kLHintsLeft | kLHintsExpandX)
TOP_LEFT     = TGLayoutHints(kLHintsTop  | kLHintsLeft)
TOP_LEFT_PADDED     = TGLayoutHints(kLHintsTop  | kLHintsLeft, 5, 5, 2, 2)
TOP_X        = TGLayoutHints(kLHintsTop  | kLHintsExpandX)
TOP_LEFT_X   = TGLayoutHints(kLHintsTop  | kLHintsLeft | kLHintsExpandX)
TOP_LEFT_X_Y = TGLayoutHints(kLHintsTop  | kLHintsLeft | kLHintsExpandX |
							 kLHintsExpandY)
TOP_RIGHT    = TGLayoutHints(kLHintsTop  | kLHintsRight)
TOP_X_Y      = TGLayoutHints(kLHintsTop  | kLHintsExpandX | kLHintsExpandY)


# Menu codes and callbacks

M_CALLBACK         = {} # Map between menu items and callbacks
						# Each menu item is identified by a unique code
						# and is associated with a unique callback, specified
						# by name

M_FILE_OPEN        = 1;  M_CALLBACK[M_FILE_OPEN]  = "self.open()"
M_FILE_SAVE        = 2;  M_CALLBACK[M_FILE_SAVE]  = "self.save()"
M_FILE_EXIT        = 6;  M_CALLBACK[M_FILE_EXIT]  = "self.exit()"
M_EDIT_CLEAR       = 21; M_CALLBACK[M_EDIT_CLEAR] = "self.clear()"
M_EDIT_UNDO        = 22; M_CALLBACK[M_EDIT_UNDO]  = "self.undo()"
M_HELP_ABOUT       = 51; M_CALLBACK[M_HELP_ABOUT] = "self.about()"
M_HELP_USAGE       = 52; M_CALLBACK[M_HELP_USAGE] = "self.usage()"

L_CLASSBOX         = 1
L_LABELBOX         = 2
L_METHODBOX        = 3

B_OPEN             = 1
B_SAVE             = 2
B_OK               = 3

K_MENUBAR_HEIGHT   = 26    # pixels
K_TOOLBAR_HEIGHT   = 32
K_STATUSBAR_HEIGHT = 26
K_LISTBOX_WIDTH    = WIDTH/3
K_LISTBOX_HEIGHT   = 450

K_PROG_MAX         = 20.0
K_MAX_COUNT        = 500

# Help

HelpUsage = \
"""
The time has come the walrus said
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

YELLOW      = root.Color("yellow")
GREEN       = root.Color("green")
LIGHTYELLOW = root.Color("lightyellow")
LIGHTGREEN  = root.Color("lightgreen")

P_METHODS  = 0
P_SELECTED = 1
P_COLOR = (LIGHTYELLOW, LIGHTGREEN)

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
#     within another, is defined by the parent after it has been created
#     using its AddFrame method in which the child is specified.
#
#   Each widget can emit one or more signals, usually triggered by some user
#   manipulation of, or action on, the widget. For example, clicking on a
#   tab of a  TGTab, that is, a notebook, will cause the notebook to emit
#   the signal "Selected(Int_t)" along with the identity of the selected tab.
#   Signals are connected to "Slots", that is, actions. This is how a user
#   GUI interaction can be made to yield one or more actions. Any signal can be
#   connected to any slot. Indeed, the relationship between signals and slots
#   can be many-many. In practice, a slot is modeled as a procedure such as
#   a method of the GUI class. In this GUI, the listboxs' Selected(Int_t)
#   signal is connected to the corresponding listbox method of the GUI so 
#   that whenever an item in the listbox is selected that method is invoked.
#   
#   In summary, a Root GUI is a (double) tree of widgets with which the user
#   can interact, whose signals---usually generated by user interactions---are
#   connected to slots, that is, actions modeled as procedures.
#
# (B) This GUI
#
#   We model this GUI as a TApplication that contains a TGMainFrame. Here is
#   the layout hierarchy (that is, the parent-to-child relationships).
#
#   window                   (TGWindow)
#
#     main                      (TGMainFrame)
#
#       menuBar                    (TGMenuBar)
#         menuFile                    (TGPopupMenu)
#         menuEdit                    (TGPopupMenu)
#         menuHelp                    (TGPopupMenu)
#
#       vframe                     (TGVerticalFrame)
#         toolBar                    (TGToolBar)
#           openButton                  (TGPictureButton)
#           saveButton                  (TGPictureButton)
#           progressBar                 (TGHProgressBar)
#
#         hframe                     (TGHorizontalFrame)
#           vcframe                     (TGVerticalFrame)
#             clabel                      (TGLabel)
#             classBox                    (TGListBox)
#
#           vmframe                     (TGVerticalFrame)
#             mlabel                      (TGLabel)
#             methodBox                   (TGListBox)
#
#           vlframe                     (TGVerticalFrame)
#             llabel                      (TGLabel)
#             labelBox                    (TGListBox)
#
#         statusBar                  (TGSTatusBar)
#-----------------------------------------------------------------------------

class Gui:
	"""
	gui = Gui(title, iniDir)
	"""

	def __init__(self, name, inidir):

		self.methodDir  = METHODDIR
		self.iniDir     = inidir            # Initial directory for file dialog
		self.iconDir    = ICONDIR				 
		self.connection = []                # List of Signal/Slot connections

		#-------------------------------------------------------------------
		# Create main frame
		#-------------------------------------------------------------------
		# Establish a connection between the main frame's "CloseWindow()"
		# signal and the GUI's "close" slot, modeled as a method.
		# When the main frame issues the signal CloseWindow() this
		# triggers a call to the close method of the class Gui.

		self.window = gClient.GetRoot()
		self.main = TGMainFrame(self.window, WIDTH, HEIGHT)

		self.connection.append(Connection(self.main, "CloseWindow()",
										  self,      "close"))

		#-------------------------------------------------------------------
		# Create menu bar
		#-------------------------------------------------------------------
		# MenuBar's parent is main, a fact made explicit in MenuBar's
		# constructor. This establishes a child-to-parent relationship.
		# We also define how MenuBar is to displayed within main and thereby
		# establish a parent-to-child relationship. This is done by invoking
		# main's AddFrame method, wherein we also specify how precisely
		# the MenuBar is to be positioned within main.
		#-------------------------------------------------------------------
		# The hint objects are used to place and group
		# widgets with respect to each other.

		self.menuBar = TGMenuBar(self.main)
		self.main.AddFrame(self.menuBar, TOP_X)

		self.H3DBar  = TGHorizontal3DLine(self.main)
		self.main.AddFrame(self.H3DBar, TOP_X)

		#-------------------------------------------------------------------
		# Create menus
		#-------------------------------------------------------------------
		self.menuFile = TGPopupMenu(self.window)
		self.menuBar.AddPopup("&File", self.menuFile, TOP_LEFT_PADDED)
		self.menuFile.AddEntry("&Open", M_FILE_OPEN)
		self.menuFile.AddEntry("&Save", M_FILE_SAVE)
		self.menuFile.AddSeparator()
		self.menuFile.AddEntry("E&xit",M_FILE_EXIT)        
		self.connection.append(Connection(self.menuFile, "Activated(Int_t)",
										  self,          "menu"))

		self.menuEdit = TGPopupMenu(self.window)
		self.menuBar.AddPopup("&Edit", self.menuEdit, TOP_LEFT_PADDED)
		self.menuEdit.AddEntry("Clear", M_EDIT_CLEAR)
		self.connection.append(Connection(self.menuEdit, "Activated(Int_t)",
										  self,          "menu"))

		self.menuHelp = TGPopupMenu(self.window)
		self.menuBar.AddPopup("&Help", self.menuHelp, TOP_LEFT_PADDED)
		self.menuHelp.AddEntry("&Usage", M_HELP_USAGE)
		self.menuHelp.AddEntry("&About", M_HELP_ABOUT)
		self.menuHelp.AddSeparator()
		self.connection.append(Connection(self.menuHelp, "Activated(Int_t)",
										  self,          "menu"))

		#-------------------------------------------------------------------
		# Add vertical frame to contain notebook, toolbar and
		# status window
		#-------------------------------------------------------------------
		self.vframe = TGVerticalFrame(self.main, 1, 1)
		self.main.AddFrame(self.vframe, TOP_X_Y)

		#-------------------------------------------------------------------
		# Add horizontal frame to hold toolbar
		#-------------------------------------------------------------------
		self.toolBar = TGHorizontalFrame(self.vframe, 1, 1)
		self.vframe.AddFrame(self.toolBar, TOP_X)
		
		# Add picture buttons

		BUTTON_LAYOUT = TGLayoutHints(kLHintsLeft, 10, 10, 2, 2)
		self.picpool = TGPicturePool(gClient, ICONDIR)
		if self.picpool:
			self.openIcon   = self.picpool.GetPicture("open.xpm")
			self.openButton = TGPictureButton(self.toolBar,
											  self.openIcon,
											  B_OPEN)
			self.openButton.SetToolTipText("Open a RECO, AOD or PAT root file")
			self.toolBar.AddFrame(self.openButton, BUTTON_LAYOUT)
			self.connection.append(Connection(self.openButton,
											  "Clicked()",
											  self, "open"))

			self.saveIcon   = self.picpool.GetPicture("save.xpm")
			self.saveButton = TGPictureButton(self.toolBar,
											  self.saveIcon,
											  B_SAVE)
			self.saveButton.SetToolTipText("Save configuration file fragment")
			self.toolBar.AddFrame(self.saveButton, BUTTON_LAYOUT)
			self.connection.append(Connection(self.saveButton,
											  "Clicked()",
											  self, "save"))	

		#-------------------------------------------------------------------
		# Add a horizontal progress bar
		#-------------------------------------------------------------------
		self.progressBar = TGHProgressBar(self.toolBar,
										  TGProgressBar.kFancy, 1)
		self.progressBar.SetBarColor("green")
		self.progressBar.SetRange(0, K_PROG_MAX)
		PB_LAYOUT = TGLayoutHints(kLHintsLeft |
								  kLHintsExpandX |
								  kLHintsExpandY, 10, 10, 2, 2)
		self.toolBar.AddFrame(self.progressBar, PB_LAYOUT)

		# Set up a timer for progress bar
		self.progTimer = TTimer(1000)
		self.connection.append(Connection(self.progTimer,
										  "Timeout()",
										  self, "progTimeout"))

		#-------------------------------------------------------------------
		# Add a notebook with two pages
		# 1. The Methods page lists available methods
		# 2. The Selected Methods page lists selected methods
		#-------------------------------------------------------------------
		self.noteBook = TGTab(self.vframe, 1, 1)
		self.vframe.AddFrame(self.noteBook, TOP_X_Y)
		self.connection.append(Connection(self.noteBook, "Selected(Int_t)",
										  self,          "setPage"))

		self.pages = []
		self.pages.append(self.noteBook.AddTab("Methods"))
		self.pages.append(self.noteBook.AddTab("Selected Methods"))
		self.currentPage = 0
		self.noteBook.SetTab("Methods")
		tab = self.noteBook.GetTabTab("Methods")
		tab.ChangeBackground(LIGHTYELLOW)
		
		#-------------------------------------------------------------------
		# Add horizontal frame to contain labels and listboxes
		#-------------------------------------------------------------------
		self.hframe    = [0,0]
		self.clabel    = [0,0]
		self.mlabel    = [0,0]
		self.llabel    = [0,0]
		self.vcframe   = [0,0]
		self.vmframe   = [0,0]
		self.vlframe   = [0,0]
		self.classBox  = [0,0]
		self.labelBox  = [0,0]
		self.methodBox = [0,0]
		
		for id in [0,1]:
			idoffset = id*10
			
			self.hframe[id] = TGHorizontalFrame(self.pages[id], 1, 1)
			self.pages[id].AddFrame(self.hframe[id], TOP_X_Y)

			# Create a labeled list box for by creating a vertical frame,
			# inserting it into the horizontal frame, and then inserting the
			# label followed by the listbox into the vertical frame
			
			self.vcframe[id] = TGVerticalFrame(self.hframe[id], 1, 1)
			self.hframe[id].AddFrame(self.vcframe[id], TOP_X_Y)
			self.hframe[id].ChangeBackground(YELLOW)
			
			self.clabel[id] = TGLabel(self.vcframe[id], "Classes")
			self.vcframe[id].AddFrame(self.clabel[id], TOP_X)

			self.classBox[id] = TGListBox(self.vcframe[id],
										  L_CLASSBOX+idoffset,
										  kSunkenFrame)
			self.classBox[id].Resize(K_LISTBOX_WIDTH, K_LISTBOX_HEIGHT)
			self.vcframe[id].AddFrame(self.classBox[id], TOP_X_Y)
			self.connection.append(Connection(self.classBox[id],
											  "Selected(Int_t)",
											  self,
											  "classListBox"))
			
			# Create a list box for methods

			self.vmframe[id] = TGVerticalFrame(self.hframe[id], 1, 1)
			self.hframe[id].AddFrame(self.vmframe[id], TOP_X_Y)
			
			self.mlabel[id] = TGLabel(self.vmframe[id], "Methods")
			self.vmframe[id].AddFrame(self.mlabel[id], TOP_X)
			
			self.methodBox[id] = TGListBox(self.vmframe[id],
										   L_METHODBOX+idoffset,
										   kSunkenFrame)
			self.methodBox[id].Resize(K_LISTBOX_WIDTH, K_LISTBOX_HEIGHT)
			self.methodBox[id].SetMultipleSelections()
			self.vmframe[id].AddFrame(self.methodBox[id], TOP_X_Y)
			self.connection.append(Connection(self.methodBox[id],
											  "Selected(Int_t)",
											  self,
											  "methodListBox"))

			# Create a list box for getByLabel string

			self.vlframe[id] = TGVerticalFrame(self.hframe[id], 1, 1)
			self.hframe[id].AddFrame(self.vlframe[id], TOP_X_Y)
			
			self.llabel[id] = TGLabel(self.vlframe[id], "getByLabel")
			self.vlframe[id].AddFrame(self.llabel[id], TOP_X)
			
			self.labelBox[id] = TGListBox(self.vlframe[id],
										  L_LABELBOX+idoffset,
										  kSunkenFrame)
			self.labelBox[id].Resize(K_LISTBOX_WIDTH, K_LISTBOX_HEIGHT)
			self.labelBox[id].SetMultipleSelections()
			self.vlframe[id].AddFrame(self.labelBox[id], TOP_X_Y)
			self.connection.append(Connection(self.labelBox[id],
											  "Selected(Int_t)",
											  self,
											  "labelListBox"))
		#-------------------------------------------------------------------
		# Create a status bar, divided into two parts
		#-------------------------------------------------------------------
		self.statusBar = TGStatusBar(self.vframe, 1, 1)
		status_parts = array('i')
		status_parts.append(23)
		status_parts.append(77)
		self.statusBar.SetParts(status_parts, len(status_parts))
		self.vframe.AddFrame(self.statusBar, TOP_X)

		self.mapWindows()

	def __del__(self):
		pass

#----------------------------------------------------------------------------
# Methods
#----------------------------------------------------------------------------
	def mapWindows(self):

		self.main.SetWindowName("Make n-tuple configuration fragment")

		# Map all subwindows of main frame
		self.main.MapSubwindows()

		# Initialize layout        
		self.main.Resize(self.main.GetDefaultSize())

		# Finally map the main frame (render GUI visible)        
		self.main.MapWindow()

#---------------------------------------------------------------------------
	def loadData(self, filename):

		# List root file

		cmd = "rlist.py %s Events" % filename

		self.statusBar.SetText("Listing file ...", 0)
		self.statusBar.SetText(filename, 1)

		self.progTimer.Start()
		record = os.popen(cmd).read()
		self.progTimer.Stop()
		self.progressBar.Reset()
		
		# Get branches

		records = getbranch.findall(record)
		if len(records) == 0:
			self.statusBar.SetText("** Error", 0)
			self.statusBar.SetText("No branches found", 1)
			return
		self.statusBar.SetText("Done!", 0)

		open("mkntuple_cfi.lis","w").writelines(joinfields(records,'\n'))
		
		# Creat a map to keep track of selections
		
		self.cmap = {}
		
		for record in records:
			label = getlabel.findall(record)[0]
			cname = getclass.findall(record)[0]
			fname = "%s/a%s.txt" % (self.methodDir, stripname.sub("", cname))
			if not os.path.exists(fname): continue

			# methods file found, so read methods
			
			if not self.cmap.has_key(cname):
				methods = filter(lambda x: x != "",
								 map(strip, open(fname).readlines()))
				methods = map(lambda x: replace(x,"\t","   "), methods)
				methods.sort()
				mmap = {}
				for method in methods:
					mmap[method] = False
				self.cmap[cname] = {'selected': False,
									'labels': {},
									'methods': mmap}

			self.statusBar.SetText(fname, 1)
			self.cmap[cname]['labels'][label] = False

		# Fill classBox on Methods page

		self.statusBar.SetText("Number of classes", 0)
		cnames = self.cmap.keys()
		self.statusBar.SetText("%d" % len(cnames), 1)
		cnames.sort()

		for id in [P_METHODS, P_SELECTED]:
			self.classBox[id].RemoveAll()
			self.labelBox[id].RemoveAll()
			self.methodBox[id].RemoveAll()
		
		for index, entry in enumerate(cnames):
			self.classBox[P_METHODS].AddEntry(entry, index)
		self.classBox[P_METHODS].Layout()
		
#---------------------------------------------------------------------------
# Slots
#---------------------------------------------------------------------------

#---------------------------------------------------------------------------
# Responds to: Selected(Int_t)  issued by Edit/Clear

	def clear(self):
		pageNumber = self.noteBook.GetCurrent()
 		if pageNumber == P_SELECTED: return

		# We are in methods page
		
		cid = self.classBox[pageNumber].GetSelected()
		cname = self.classBox[pageNumber].GetEntry(cid).GetText().Data()
		self.cmap[cname]['selected'] = False
		self.classBox[P_METHODS].Select(cid, kFALSE)
		self.labelBox[P_METHODS].RemoveAll()
		self.methodBox[P_METHODS].RemoveAll()		
#---------------------------------------------------------------------------
# Responds to: Selected(Int_t)  issued by Notebook

	def setPage(self, id):
		
		# Before changing current tab's color,  re-color previous tab to the
		# TGMainframe's default color

		tab = self.noteBook.GetTabTab(self.currentPage)
		tab.ChangeBackground(self.main.GetDefaultFrameBackground())

		# Now re-color current tab

		self.currentPage = id
		tab = self.noteBook.GetTabTab(self.currentPage)
		tab.ChangeBackground(P_COLOR[id])

		# Disable delete menu item if this is the Selected Methods page
		
		if id == P_METHODS: return
		
		# This is the selected methods page, so update it

		# Note which class was selected

		cid = self.classBox[P_SELECTED].GetSelected()
		if cid < 0:
			cname = ''
		else:
			cname = self.classBox[P_SELECTED].GetEntry(cid).GetText().Data()

		self.classBox[P_SELECTED].RemoveAll()
		self.labelBox[P_SELECTED].RemoveAll()
		self.methodBox[P_SELECTED].RemoveAll()

		# List selected classes
		
		cnames = self.cmap.keys()
		cnames.sort()
		for index, entry in enumerate(cnames):

			if not self.cmap[entry]['selected']: continue
			
			self.classBox[P_SELECTED].AddEntry(entry, index)
			if entry != cname: continue

			# The current class was selected in Selected Methods page
			
			self.classBox[P_SELECTED].Select(index)

			# List selected getByLabels
		
			labels= self.cmap[cname]['labels']
			names = labels.keys()
			names.sort()
			self.labelBox[P_SELECTED].RemoveAll()
			for ind, name in enumerate(names):
				if not labels[name]: continue
				self.labelBox[P_SELECTED].AddEntry(name, ind)
			self.labelBox[P_SELECTED].Layout()

			# List selected methods
		
			methods = self.cmap[cname]['methods']
			names = methods.keys()
			names.sort()
			self.methodBox[P_SELECTED].RemoveAll()
			for ind, name in enumerate(names):
				if not methods[name]: continue
				self.methodBox[P_SELECTED].AddEntry(name, ind)
			self.methodBox[P_SELECTED].Layout()
			
		self.classBox[P_SELECTED].Layout()
		
#---------------------------------------------------------------------------
# Responds to: Timeout()    issued by Timer

	def progTimeout(self):
		if self.progressBar.GetPosition() < K_PROG_MAX:
			self.progressBar.Increment(1.0)
		else:
			self.progressBar.Reset()
		gClient.NeedRedraw(self.progressBar) # Force immediate redraw
				
#---------------------------------------------------------------------------
# Responds to: CloseWindow()    issued by Main

	def close(self):
		gApplication.Terminate()

#---------------------------------------------------------------------------
# Responds to: Activated(Int_t) issued by menuFile, menuEdit, menuHelp

	def menu(self, id):
		if id in M_CALLBACK:
			exec(M_CALLBACK[id])
		else:
			print "Unrecognized menu id = %d" % id

#---------------------------------------------------------------------------
# Responds to: Selected(Int_t)

	def classListBox(self, id):
		
		pageNumber = self.noteBook.GetCurrent()
		cname = self.classBox[pageNumber].GetEntry(id).GetText().Data()
		self.cmap[cname]['selected'] = True
		
		SelectedPage = pageNumber == P_SELECTED
		
		# List methods
		
		methods = self.cmap[cname]['methods']
		names = methods.keys()
		names.sort()
		self.methodBox[pageNumber].RemoveAll()
		for index, name in enumerate(names):
			if SelectedPage:
				if not methods[name]: continue
			self.methodBox[pageNumber].AddEntry(name, index)
		self.methodBox[pageNumber].Layout()

		# List getByLabels
		
		labels= self.cmap[cname]['labels']
		names = labels.keys()
		names.sort()
		self.labelBox[pageNumber].RemoveAll()
		for index, name in enumerate(names):
			if SelectedPage:
				if not labels[name]: continue
			self.labelBox[pageNumber].AddEntry(name, index)
		self.labelBox[pageNumber].Layout()
		
#---------------------------------------------------------------------------
	def methodListBox(self, id):

		#print "methodBox %d" % id	

		pageNumber = self.noteBook.GetCurrent()

		# Disable method selection in Selected Methods page
		
		if pageNumber == P_SELECTED:
			self.methodBox[pageNumber].Select(id, kFALSE)
			return
		
		###D
		#print " methodBox pageNumber(%d)" % pageNumber
		
		# This is the Methods page, so flag method as selected	
			
		cid   = self.classBox[pageNumber].GetSelected()
		cname = self.classBox[pageNumber].GetEntry(cid).GetText().Data()
		name  = self.methodBox[pageNumber].GetEntry(id).GetText().Data()
		on    = self.cmap[cname]['methods'][name]
		self.cmap[cname]['methods'][name] = not on

		###D
		#print "    class( %s ) method( %s ) %s" % \
		#	  (cname, name, self.cmap[cname]['methods'][name])

#---------------------------------------------------------------------------
	def labelListBox(self, id):
		
		#print "labelBox %d" % id

		pageNumber = self.noteBook.GetCurrent()

		# Disable label selection in Selected Methods page
		
		if pageNumber == P_SELECTED:
			self.labelBox[pageNumber].Select(id, kFALSE)
			return

		# Check that at least one method has been selected

		if self.methodBox[pageNumber].GetSelected() < 0:
			self.labelBox[pageNumber].Select(id, kFALSE)
			THelpDialog(self.window,"Warning!",
						"Either:\n  go boil your head...or\n  "\
						"select at least one method!", 220, 80)
			return
		# This is the Methods page, so flag label as selected
		
		cid   = self.classBox[pageNumber].GetSelected()
		cname = self.classBox[pageNumber].GetEntry(cid).GetText().Data()
		name  = self.labelBox[pageNumber].GetEntry(id).GetText().Data()
		status= self.cmap[cname]['labels'][name]
		self.cmap[cname]['labels'][name] = not status

## 		###D
## 		print "    class( %s ) label( %s ) %s" % \
## 			  (cname, name, self.cmap[cname]['labels'][name]) 

#---------------------------------------------------------------------------
	def open(self):
		fdialog = TFileDialog(self.window,
							  self.main,
							  kFDOpen,
							  self.iniDir)

		filename = fdialog.Filename()
		self.iniDir  = fdialog.IniDir()
		self.loadData(filename)

#---------------------------------------------------------------------------
	def undo(self):
		self.notdone()

#---------------------------------------------------------------------------
	def save(self):

		filename = "mkntuple_cfi.py"

		self.statusBar.SetText("Saving to file", 0)
		self.statusBar.SetText(filename, 1)

		tab  = ' '*15

		out = open(filename, "w")
		out.write('#------------------------------------'\
				  '-------------------------------------\n')
		out.write("# Created: %s by mkntuplecfi.py\n" % ctime(time()))
		out.write('#------------------------------------'\
				  '-------------------------------------\n')
		out.write("import FWCore.ParameterSet.Config as cms\n")
		out.write("demo =\\\n")
		out.write('cms.EDAnalyzer("Mkntuple",\n')
		out.write('%sdebugLevel = cms.untracked.int32(0),\n' % tab)
		out.write('\n')
		out.write('%sntupleName = cms.untracked.string("ntuple.root"),\n' % \
				  tab)
		out.write('%svariables  =\n' % tab)
		out.write('%scms.untracked.\n' % tab)
		out.write("%svstring(\n" % tab)

		tab = 4*' '

		###D
		#print "get selected classes"
		
		# Get selected classes
		
		names = self.cmap.keys()
		names.sort()
		cnames = []
		for index, name in enumerate(names):
			if not self.cmap[name]['selected']: continue
			cnames.append(name)

		prefix = ''
		for index, cname in enumerate(cnames):
			if isVector(cname):
				maxcount = K_MAX_COUNT
			else:
				maxcount = 1

			# Get selected methods
			
			buffer = stripname.sub("", cname)
			methods = []
			selected = self.cmap[cname]['methods']
			for name in selected.keys():
				if not selected[name]: continue
				methods.append(name)
			methods.sort()

			# Get selected labels
			
			labels = []
			selected = self.cmap[cname]['labels']
			for name in selected.keys():
				if not selected[name]: continue
				labels.append(name)
			labels.sort()

			###D
			#print "  class( %s )" % cname
			
			# loop over class/label pairs

			for index, label in enumerate(labels):

				record = '%s%s"%-31s %-31s %3d",\n' % \
						 (prefix, tab, buffer, label, maxcount)
				###D
				#print record
				out.write(record)
				out.write('%s#--------------------------------'\
						  '-------------------------------------\n' % tab)

				# get selected methods
		
				for index, method in enumerate(methods):
					record = '%s"%s",\n' % (tab, method)
					out.write(record)
				out.write('%s"/"' % tab)
				prefix = ",\n"
				
		out.write("\n%s)\n" % tab)
		tab = 15*' '
		out.write("%s)\n" % tab)
		out.close()

		self.statusBar.SetText("Done!", 0)

					
	def usage(self):
		THelpDialog(self.window, "Usage", HelpUsage, HU_WIDTH, HU_HEIGHT)

	def about(self):
		THelpDialog(self.window, "About", HelpAbout, HA_WIDTH, HA_HEIGHT)

	def exit(self):
		self.close()

	def notdone(self):
		print "Not done"
		THelpDialog(self.window,"Not Yet Done!", "\tGo boil your head!",
					220, 30)

	def Run(self):
		gApplication.Run()

#------------------------------------------------------------------------------
#---- Main program ------------------------------------------------------------
#------------------------------------------------------------------------------
def main():
	if len(ARGV) > 0:
		inidir = ARGV[0]
	else:
		inidir = "."

	gui = Gui("mkntuplecfi", inidir)
	gui.Run()
#------------------------------------------------------------------------------
main()



