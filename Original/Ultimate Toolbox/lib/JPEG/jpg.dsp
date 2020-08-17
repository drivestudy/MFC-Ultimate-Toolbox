# Microsoft Developer Studio Project File - Name="jpg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=jpg - Win32 DebugUnicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jpg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jpg.mak" CFG="jpg - Win32 DebugUnicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jpg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jpg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "jpg - Win32 DebugUnicode" (based on "Win32 (x86) Static Library")
!MESSAGE "jpg - Win32 ReleaseUnicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jpg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\\" /I ".\Jpegsrc6" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libs\jpgr.lib"

!ELSEIF  "$(CFG)" == "jpg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I ".\\" /I ".\Jpegsrc6" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libs\jpgd.lib"

!ELSEIF  "$(CFG)" == "jpg - Win32 DebugUnicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "jpg___Win32_DebugUnicode"
# PROP BASE Intermediate_Dir "jpg___Win32_DebugUnicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "jpg___Win32_DebugUnicode"
# PROP Intermediate_Dir "jpg___Win32_DebugUnicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /I ".\\" /I ".\Jpegsrc6" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I ".\\" /I ".\Jpegsrc6" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\jpgd.lib"
# ADD LIB32 /nologo /out:"..\libs\jpgdu.lib"

!ELSEIF  "$(CFG)" == "jpg - Win32 ReleaseUnicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "jpg___Win32_ReleaseUnicode"
# PROP BASE Intermediate_Dir "jpg___Win32_ReleaseUnicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "jpg___Win32_ReleaseUnicode"
# PROP Intermediate_Dir "jpg___Win32_ReleaseUnicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".\\" /I ".\Jpegsrc6" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\\" /I ".\Jpegsrc6" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\libs\jpgr.lib"
# ADD LIB32 /nologo /out:"..\libs\jpgru.lib"

!ENDIF 

# Begin Target

# Name "jpg - Win32 Release"
# Name "jpg - Win32 Debug"
# Name "jpg - Win32 DebugUnicode"
# Name "jpg - Win32 ReleaseUnicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\JPEGSRC6\JCAPIMIN.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCAPISTD.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCCOEFCT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCCOLOR.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCDCTMGR.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCHUFF.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCINIT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCMAINCT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCMARKER.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCMASTER.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCOMAPI.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCPARAM.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCPHUFF.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCPREPCT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCSAMPLE.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JCTRANS.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDAPIMIN.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDAPISTD.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDATADST.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDATASRC.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDCOEFCT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDCOLOR.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDDCTMGR.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDHUFF.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDINPUT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDMAINCT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDMARKER.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDMASTER.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDMERGE.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDPHUFF.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDPOSTCT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDSAMPLE.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JDTRANS.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JERROR.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JFDCTFLT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JFDCTFST.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JFDCTINT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JIDCTFLT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JIDCTFST.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JIDCTINT.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JIDCTRED.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JMEMMGR.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JMEMNOBS.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JQUANT1.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JQUANT2.C
# End Source File
# Begin Source File

SOURCE=.\JPEGSRC6\JUTILS.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\JMORECFG.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
