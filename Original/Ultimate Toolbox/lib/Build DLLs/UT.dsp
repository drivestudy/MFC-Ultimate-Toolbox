# Microsoft Developer Studio Project File - Name="UT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=UT - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UT.mak" CFG="UT - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UT - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UT - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UT - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UT___Win32_Debug"
# PROP BASE Intermediate_Dir "UT___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x1009 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\Lib\libs\jpgd.lib ..\..\Lib\libs\cpuinfd.lib version.lib mapi32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\DLLs/UTD.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ..\..\Lib\libs\jpgd.lib ..\..\Lib\libs\cpuinfd.lib version.lib mapi32.lib msimg32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\DLLs/UTD.dll" /implib:"..\DLLs/UTD.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "UT - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UT___Win32_Release"
# PROP BASE Intermediate_Dir "UT___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x1009 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\Lib\libs\jpgr.lib ..\..\Lib\libs\cpuinfr.lib version.lib mapi32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBC" /out:"..\DLLs/UT.dll"
# ADD LINK32 ..\..\Lib\libs\jpgr.lib ..\..\Lib\libs\cpuinfr.lib version.lib mapi32.lib msimg32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBC" /out:"..\DLLs/UT.dll" /implib:"..\DLLs/UT.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "UT - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UT___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "UT___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x1009 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\Lib\libs\jpgd.lib ..\..\Lib\libs\cpuinfd.lib version.lib mapi32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\DLLs/UTDU.dll" /implib:"..\..\DLLs/UTDU.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ..\..\Lib\libs\jpgd.lib ..\..\Lib\libs\cpuinfd.lib version.lib mapi32.lib msimg32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\DLLs/UTDU.dll" /implib:"..\DLLs/UTDU.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "UT - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UT___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "UT___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x1009 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\Lib\libs\jpgr.lib ..\..\Lib\libs\cpuinfr.lib version.lib mapi32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBC" /out:"..\DLLs/UTu.dll"
# ADD LINK32 ..\..\Lib\libs\jpgr.lib ..\..\Lib\libs\cpuinfr.lib version.lib mapi32.lib msimg32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBC" /out:"..\DLLs/UTU.dll" /implib:"..\DLLs/UTU.lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "UT - Win32 Debug"
# Name "UT - Win32 Release"
# Name "UT - Win32 Unicode Debug"
# Name "UT - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Ultimate ToolBox Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\CONVFILE.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\COPYTREE.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\CPYSTDLG.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\DIR.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\DIRLIST.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\DSTRLIST.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\EVNTLOG.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\FILE.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\FILELIST.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\MDELDLG.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\MFCToolBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OX3DTabView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXActiveDesktop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXAdvancedAssert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXAdvancedAssertMail.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXAppBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXAutoComplete.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXAutoListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBackgroundPainter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBITBUF.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBitmapButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBitmapMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBitmapMenuOrganizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBMPFilePickerCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBMPFle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBrowseColorEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBrowseDirEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXBrowseDirEditList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXButtonAppearanceDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCalculatorCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCalculatorEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCalendar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCalendarEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCalendarPopup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCaptionPainter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXChildFrameState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXClipPocket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCMPFL.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXColorPickerButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXColorPickerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXComboPickerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCOMPR.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXContextHelpDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXContextHelpPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXContextHelpPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCoolBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCoolComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCoolControlOrganizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCoolCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCoolToolBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCRCChk.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCsvFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeBackgroundPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeCaptionPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeCommandsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeCoolControlsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeInstanceManagerPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeIntelliMousePage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizePage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeShortkeysPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeSplashWindowPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeTabbedMDIPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeToolbarsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeTrayIconPage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXCustomizeWorkspaceStatePage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDao.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDckPPg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDckPSh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDFlHdr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\Oxdib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDirDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDLLManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\oxdocktabctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDocTemplateSpy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDragDockContext.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXDragDropCommands.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXEditList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXFileChanger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXFileWatcher.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXFileWatchNotifier.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXFolderPickerDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXFontComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXFontPickerButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXFrameWndDock.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXFullScreenFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXGphFle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXGraphics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\oxgridedit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXGridHdr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXGridList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHashTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHistoryCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHistoryCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHookWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHTMLParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHyperbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHyperBarOverlay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHyperBarRender.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHyperFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXHyperLinkAction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXImageListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXImageViewer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXInstanceManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXInteger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXIntelliMouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXIPEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXItemTip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXItemTipWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXIteratorRegistryItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXIteratorService.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXIteratorUNC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXJPGCod.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXJPGCom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXJPGDom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXJPGExp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXJPGFle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXLayoutManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXListBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXListEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXMaskedEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXMDIFloatWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXMetaFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXMonthCalCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXMulticlipboardDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXMultiComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXNetBrowseTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXNotesEditView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXOptionTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXPhysicalEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXPhysicalEditEx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXPopupBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXPreviewDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXPropertiesWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXPropertyDescriptionPane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXPropertyTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXQuickString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRectTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRegExpression.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRegistryItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRegistryValFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRegistryWatcher.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRegistryWatchNotifier.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXResourceFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXResourceLibrary.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRollup.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRoundedButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXRulerOrganizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXScreenGrab.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXScrollTip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXScrollTipOwner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXScrollWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSeparator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXService.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXShape.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXShdWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXShellFolderTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXShellNamespaceNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXShellObjectList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXShortcut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXShortcutBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXShortkeysOrganizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSizeCtrlBar.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSizeDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSizeDockBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSizeToolBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSkinnedComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSkinnedSpinCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSkinnedTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSkins.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSoundCustomizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSoundEffectManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSoundWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSpinCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSplashWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSplashWndDIB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSplitterColRowState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSplitterRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXStatic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXStaticHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXStaticText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSysInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXSzMiniDockFrmWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTabClientWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTabView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTaskbarIcon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTleBar.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\OXToolTipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTrace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTreeCal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTreeCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTreeEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTreeHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXTreeItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXUNC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXUNCStandardActor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXURL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXUsertl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXUUDec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXUxTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXWABook.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXWatchedDir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\oxwebctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\OXWorkspaceState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\Oxzoomvw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\Path.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\PATHITER.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\PROGRESS.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\UTBStrOp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\WATCHBUF.CPP
# End Source File
# Begin Source File

SOURCE=..\..\source\Xstatus4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\XSTRING.CPP
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\include\OXMain.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UT.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\Include\OXMain.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\Include\RES\command_group.ico
# End Source File
# Begin Source File

SOURCE=..\..\include\res\command_group.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\custom_button.ico
# End Source File
# Begin Source File

SOURCE=..\..\include\res\custom_button.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\new_popup_menu.ico
# End Source File
# Begin Source File

SOURCE=..\..\include\res\new_popup_menu.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\noimage.ico
# End Source File
# Begin Source File

SOURCE=..\..\include\res\noimage.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\OXHandCursor.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\OXHandCursor.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\OXNoHandCursor.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\OXNoHandCursor.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\res\OXNotes.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\OXNotes.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\OXSpinHor.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\OXSpinHor.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\OXSpinVer.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\OXSpinVer.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\oxtbcopy.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\oxtbcopy.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\oxtbmove.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\oxtbmove.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\oxtbremove.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\oxtbremove.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\properti.bmp
# End Source File
# Begin Source File

SOURCE=..\..\include\res\properties_toolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_all.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_all.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_bottomleft.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_bottomleft.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_bottomright.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_bottomright.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_down.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_down.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_left.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_left.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_leftright.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_leftright.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_right.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_right.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_topbottom.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_topbottom.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_topleft.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_topleft.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_topright.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_topright.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\scrl_up.cur
# End Source File
# Begin Source File

SOURCE=..\..\include\res\scrl_up.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\RES\separator.ico
# End Source File
# Begin Source File

SOURCE=..\..\include\res\separator.ico
# End Source File
# End Group
# End Target
# End Project
