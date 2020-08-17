# Microsoft Developer Studio Project File - Name="ExtendedTreeControl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ExtendedTreeControl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ExtendedTreeControl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ExtendedTreeControl.mak" CFG="ExtendedTreeControl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ExtendedTreeControl - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ExtendedTreeControl - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ExtendedTreeControl - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\..\..\include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ExtendedTreeControl - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\..\..\include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"ExtendedTreeControl.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ExtendedTreeControl - Win32 Release"
# Name "ExtendedTreeControl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Ultimate ToolBox"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\..\..\source\UTSampleAbout\HtmlCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXCalendar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXCalendarPopup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXHookWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXItemTip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXItemTipWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXLayoutManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXSeparator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXTreeCal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXTreeCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXTreeEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXTreeHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXTreeItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXUxTheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\UTBStrOp.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ExtendedTreeControl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtendedTreeControl.rc
# End Source File
# Begin Source File

SOURCE=.\ExtendedTreeControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Ultimate ToolBox Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\..\include\OXTreeCal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\OXTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\OXTreeEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\OXTreeHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\OXTreeItem.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ExtendedTreeControl.h
# End Source File
# Begin Source File

SOURCE=.\ExtendedTreeControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ExtendedTreeControl.ico
# End Source File
# Begin Source File

SOURCE=.\res\ExtendedTreeControl.rc2
# End Source File
# Begin Source File

SOURCE=..\..\..\One\Two\DemoBrowser\Demos\Controls\ExtendedTree\il_tree.bmp
# End Source File
# Begin Source File

SOURCE=.\il_tree.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ExtendedTreeControl.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
