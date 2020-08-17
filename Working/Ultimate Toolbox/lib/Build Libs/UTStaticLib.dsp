# Microsoft Developer Studio Project File - Name="UTStaticLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=UTStaticLib - Win32 UnicodeDebugMFCStatic
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UTStaticLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UTStaticLib.mak" CFG="UTStaticLib - Win32 UnicodeDebugMFCStatic"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UTStaticLib - Win32 DebugMFCShared" (based on "Win32 (x86) Static Library")
!MESSAGE "UTStaticLib - Win32 ReleaseMFCShared" (based on "Win32 (x86) Static Library")
!MESSAGE "UTStaticLib - Win32 DebugMFCStatic" (based on "Win32 (x86) Static Library")
!MESSAGE "UTStaticLib - Win32 ReleaseMFCStatic" (based on "Win32 (x86) Static Library")
!MESSAGE "UTStaticLib - Win32 UnicodeDebugMFCShared" (based on "Win32 (x86) Static Library")
!MESSAGE "UTStaticLib - Win32 UnicodeReleaseMFCShared" (based on "Win32 (x86) Static Library")
!MESSAGE "UTStaticLib - Win32 UnicodeDebugMFCStatic" (based on "Win32 (x86) Static Library")
!MESSAGE "UTStaticLib - Win32 UnicodeReleaseMFCStatic" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugMFCShared"
# PROP BASE Intermediate_Dir "DebugMFCShared"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugMFCShared"
# PROP Intermediate_Dir "DebugMFCShared"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
# ADD RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\oxlibad.lib"
# ADD LIB32 /nologo /out:"..\libs\UTLibSharedD.lib"

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMFCShared"
# PROP BASE Intermediate_Dir "ReleaseMFCShared"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMFCShared"
# PROP Intermediate_Dir "ReleaseMFCShared"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\oxlibar.lib"
# ADD LIB32 /nologo /out:"..\libs\UTLibSharedR.lib"

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugMFCStatic"
# PROP BASE Intermediate_Dir "DebugMFCStatic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugMFCStatic"
# PROP Intermediate_Dir "DebugMFCStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W4 /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
# ADD RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\oxlibd.lib"
# ADD LIB32 /nologo /out:"..\libs\UTLibStaticD.lib"

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMFCStatic"
# PROP BASE Intermediate_Dir "ReleaseMFCStatic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMFCStatic"
# PROP Intermediate_Dir "ReleaseMFCStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
# ADD RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\oxlibr.lib"
# ADD LIB32 /nologo /out:"..\libs\UTLibStaticR.lib"

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UnicodeDebugMFCShared"
# PROP BASE Intermediate_Dir "UnicodeDebugMFCShared"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UnicodeDebugMFCShared"
# PROP Intermediate_Dir "UnicodeDebugMFCShared"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
# ADD RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\oxlibadu.lib"
# ADD LIB32 /nologo /out:"..\libs\UTLibSharedDU.lib"

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UnicodeReleaseMFCShared"
# PROP BASE Intermediate_Dir "UnicodeReleaseMFCShared"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UnicodeReleaseMFCShared"
# PROP Intermediate_Dir "UnicodeReleaseMFCShared"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\oxlibaru.lib"
# ADD LIB32 /nologo /out:"..\libs\UTLibSharedRU.lib"

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UnicodeDebugMFCStatic"
# PROP BASE Intermediate_Dir "UnicodeDebugMFCStatic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UnicodeDebugMFCStatic"
# PROP Intermediate_Dir "UnicodeDebugMFCStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W4 /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
# ADD RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\oxlibdu.lib"
# ADD LIB32 /nologo /out:"..\libs\UTLibStaticDU.lib"

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UnicodeReleaseMFCStatic"
# PROP BASE Intermediate_Dir "UnicodeReleaseMFCStatic"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UnicodeReleaseMFCStatic"
# PROP Intermediate_Dir "UnicodeReleaseMFCStatic"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
# ADD RSC /l 0x409 /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\oxlibru.lib"
# ADD LIB32 /nologo /out:"..\libs\UTLibStaticRU.lib"

!ENDIF 

# Begin Target

# Name "UTStaticLib - Win32 DebugMFCShared"
# Name "UTStaticLib - Win32 ReleaseMFCShared"
# Name "UTStaticLib - Win32 DebugMFCStatic"
# Name "UTStaticLib - Win32 ReleaseMFCStatic"
# Name "UTStaticLib - Win32 UnicodeDebugMFCShared"
# Name "UTStaticLib - Win32 UnicodeReleaseMFCShared"
# Name "UTStaticLib - Win32 UnicodeDebugMFCStatic"
# Name "UTStaticLib - Win32 UnicodeReleaseMFCStatic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Group "UTB"

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

SOURCE=.\stdafx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# End Target
# End Project
