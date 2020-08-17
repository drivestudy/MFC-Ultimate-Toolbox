# Microsoft Developer Studio Project File - Name="HyperbarDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HyperbarDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HyperbarDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HyperbarDemo.mak" CFG="HyperbarDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HyperbarDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HyperbarDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HyperbarDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\include" /I "..\..\..\source" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 msimg32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "HyperbarDemo - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\include" /I "..\..\..\source" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msimg32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HyperbarDemo - Win32 Release"
# Name "HyperbarDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomHyperbar.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperbarDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperbarDemo.rc
# End Source File
# Begin Source File

SOURCE=.\HyperbarDemoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperbarDemoView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\CustomHyperbar.h
# End Source File
# Begin Source File

SOURCE=.\HyperbarDemo.h
# End Source File
# Begin Source File

SOURCE=.\HyperbarDemoDoc.h
# End Source File
# Begin Source File

SOURCE=.\HyperbarDemoView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
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

SOURCE=.\res\book.bmp
# End Source File
# Begin Source File

SOURCE=.\res\code.bmp
# End Source File
# Begin Source File

SOURCE=.\res\disk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\document.bmp
# End Source File
# Begin Source File

SOURCE=.\res\folders.bmp
# End Source File
# Begin Source File

SOURCE=.\res\globe.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HyperbarDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\HyperbarDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\HyperbarDemoDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\pc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\plug.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pocketpc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools.bmp
# End Source File
# Begin Source File

SOURCE=.\res\user.bmp
# End Source File
# End Group
# Begin Group "Hyperbar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\OXHyperbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXHyperBarOverlay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXHyperBarRender.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\OXHyperFrameWnd.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
