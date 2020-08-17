# Microsoft Developer Studio Generated NMAKE File, Based on UT.dsp
!IF "$(CFG)" == ""
CFG=UT - Win32 Unicode Debug
!MESSAGE No configuration specified. Defaulting to UT - Win32 Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "UT - Win32 Debug" && "$(CFG)" != "UT - Win32 Release" && "$(CFG)" != "UT - Win32 Unicode Debug" && "$(CFG)" != "UT - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UT - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\DLLs\UTD.dll"


CLEAN :
	-@erase "$(INTDIR)\CONVFILE.OBJ"
	-@erase "$(INTDIR)\COPYTREE.OBJ"
	-@erase "$(INTDIR)\CPYSTDLG.OBJ"
	-@erase "$(INTDIR)\DIR.OBJ"
	-@erase "$(INTDIR)\DIRLIST.OBJ"
	-@erase "$(INTDIR)\DSTRLIST.OBJ"
	-@erase "$(INTDIR)\EVNTLOG.OBJ"
	-@erase "$(INTDIR)\FILE.OBJ"
	-@erase "$(INTDIR)\FILELIST.OBJ"
	-@erase "$(INTDIR)\MDELDLG.OBJ"
	-@erase "$(INTDIR)\MFCToolBar.obj"
	-@erase "$(INTDIR)\OX3DTabView.obj"
	-@erase "$(INTDIR)\OXActiveDesktop.obj"
	-@erase "$(INTDIR)\OXAdvancedAssert.obj"
	-@erase "$(INTDIR)\OXAdvancedAssertMail.obj"
	-@erase "$(INTDIR)\OXAppBar.obj"
	-@erase "$(INTDIR)\OXAutoComplete.obj"
	-@erase "$(INTDIR)\OXAutoListBox.obj"
	-@erase "$(INTDIR)\OXBackgroundPainter.obj"
	-@erase "$(INTDIR)\OXBITBUF.OBJ"
	-@erase "$(INTDIR)\OXBitmapButton.obj"
	-@erase "$(INTDIR)\OXBitmapMenu.obj"
	-@erase "$(INTDIR)\OXBitmapMenuOrganizer.obj"
	-@erase "$(INTDIR)\OXBMPFilePickerCombo.obj"
	-@erase "$(INTDIR)\OXBMPFle.obj"
	-@erase "$(INTDIR)\OXBrowseColorEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEditList.obj"
	-@erase "$(INTDIR)\OXButtonAppearanceDlg.obj"
	-@erase "$(INTDIR)\OXCalculatorCtrl.obj"
	-@erase "$(INTDIR)\OXCalculatorEdit.obj"
	-@erase "$(INTDIR)\OXCalendar.obj"
	-@erase "$(INTDIR)\OXCalendarEdit.obj"
	-@erase "$(INTDIR)\OXCalendarPopup.obj"
	-@erase "$(INTDIR)\OXCaptionPainter.obj"
	-@erase "$(INTDIR)\OXChildFrameState.obj"
	-@erase "$(INTDIR)\OXClipPocket.obj"
	-@erase "$(INTDIR)\OXCMPFL.OBJ"
	-@erase "$(INTDIR)\OXColorPickerButton.obj"
	-@erase "$(INTDIR)\OXColorPickerCtrl.obj"
	-@erase "$(INTDIR)\OXComboPickerCtrl.obj"
	-@erase "$(INTDIR)\OXCOMPR.OBJ"
	-@erase "$(INTDIR)\OXContextHelpDialog.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertyPage.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertySheet.obj"
	-@erase "$(INTDIR)\OXCoolBar.obj"
	-@erase "$(INTDIR)\OXCoolComboBox.obj"
	-@erase "$(INTDIR)\OXCoolControlOrganizer.obj"
	-@erase "$(INTDIR)\OXCoolCtrl.obj"
	-@erase "$(INTDIR)\OXCoolToolBar.obj"
	-@erase "$(INTDIR)\OXCRCChk.obj"
	-@erase "$(INTDIR)\OXCsvFile.obj"
	-@erase "$(INTDIR)\OXCustomizeBackgroundPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCaptionPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCommandsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCoolControlsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeInstanceManagerPage.obj"
	-@erase "$(INTDIR)\OXCustomizeIntelliMousePage.obj"
	-@erase "$(INTDIR)\OXCustomizeManager.obj"
	-@erase "$(INTDIR)\OXCustomizePage.obj"
	-@erase "$(INTDIR)\OXCustomizeShortkeysPage.obj"
	-@erase "$(INTDIR)\OXCustomizeSplashWindowPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTabbedMDIPage.obj"
	-@erase "$(INTDIR)\OXCustomizeToolbarsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTrayIconPage.obj"
	-@erase "$(INTDIR)\OXCustomizeWorkspaceStatePage.obj"
	-@erase "$(INTDIR)\OXDao.obj"
	-@erase "$(INTDIR)\OXDateTimeCtrl.obj"
	-@erase "$(INTDIR)\OXDckPPg.obj"
	-@erase "$(INTDIR)\OXDckPSh.obj"
	-@erase "$(INTDIR)\OXDFlHdr.obj"
	-@erase "$(INTDIR)\Oxdib.obj"
	-@erase "$(INTDIR)\OXDirDlg.obj"
	-@erase "$(INTDIR)\OXDlgBar.obj"
	-@erase "$(INTDIR)\OXDLLManager.obj"
	-@erase "$(INTDIR)\oxdocktabctrl.obj"
	-@erase "$(INTDIR)\OXDocTemplateSpy.obj"
	-@erase "$(INTDIR)\OXDragDockContext.obj"
	-@erase "$(INTDIR)\OXDragDropCommands.obj"
	-@erase "$(INTDIR)\OXEdit.obj"
	-@erase "$(INTDIR)\OXEditList.obj"
	-@erase "$(INTDIR)\OXFileChanger.obj"
	-@erase "$(INTDIR)\OXFileWatcher.obj"
	-@erase "$(INTDIR)\OXFileWatchNotifier.obj"
	-@erase "$(INTDIR)\OXFolderPickerDlg.obj"
	-@erase "$(INTDIR)\OXFontComboBox.obj"
	-@erase "$(INTDIR)\OXFontPickerButton.obj"
	-@erase "$(INTDIR)\OXFrameWndDock.obj"
	-@erase "$(INTDIR)\OXFullScreenFrame.obj"
	-@erase "$(INTDIR)\OXGphFle.obj"
	-@erase "$(INTDIR)\OXGraphics.obj"
	-@erase "$(INTDIR)\oxgridedit.obj"
	-@erase "$(INTDIR)\OXGridHdr.obj"
	-@erase "$(INTDIR)\OXGridList.obj"
	-@erase "$(INTDIR)\OXHashTable.obj"
	-@erase "$(INTDIR)\OXHistoryCombo.obj"
	-@erase "$(INTDIR)\OXHistoryCtrl.obj"
	-@erase "$(INTDIR)\OXHookWnd.obj"
	-@erase "$(INTDIR)\OXHTMLParser.obj"
	-@erase "$(INTDIR)\OXHyperLinkAction.obj"
	-@erase "$(INTDIR)\OXImageListBox.obj"
	-@erase "$(INTDIR)\OXImageViewer.obj"
	-@erase "$(INTDIR)\OXInstanceManager.obj"
	-@erase "$(INTDIR)\OXInteger.obj"
	-@erase "$(INTDIR)\OXIntelliMouse.obj"
	-@erase "$(INTDIR)\OXIPEdit.obj"
	-@erase "$(INTDIR)\OXItemTip.obj"
	-@erase "$(INTDIR)\OXItemTipWnd.obj"
	-@erase "$(INTDIR)\OXIteratorRegistryItem.obj"
	-@erase "$(INTDIR)\OXIteratorService.obj"
	-@erase "$(INTDIR)\OXIteratorUNC.obj"
	-@erase "$(INTDIR)\OXJPGCod.obj"
	-@erase "$(INTDIR)\OXJPGCom.obj"
	-@erase "$(INTDIR)\OXJPGDom.obj"
	-@erase "$(INTDIR)\OXJPGExp.obj"
	-@erase "$(INTDIR)\OXJPGFle.obj"
	-@erase "$(INTDIR)\OXLayoutManager.obj"
	-@erase "$(INTDIR)\OXListBoxEx.obj"
	-@erase "$(INTDIR)\OXListEdit.obj"
	-@erase "$(INTDIR)\OXMain.res"
	-@erase "$(INTDIR)\OXMaskedEdit.obj"
	-@erase "$(INTDIR)\OXMDIFloatWnd.obj"
	-@erase "$(INTDIR)\OXMenuBar.obj"
	-@erase "$(INTDIR)\OXMetaFile.obj"
	-@erase "$(INTDIR)\OXMonthCalCtrl.obj"
	-@erase "$(INTDIR)\OXMulticlipboardDlg.obj"
	-@erase "$(INTDIR)\OXMultiComboBox.obj"
	-@erase "$(INTDIR)\OXNetBrowseTree.obj"
	-@erase "$(INTDIR)\OXNotesEditView.obj"
	-@erase "$(INTDIR)\OXOptionTreeCtrl.obj"
	-@erase "$(INTDIR)\OXParser.obj"
	-@erase "$(INTDIR)\OXPhysicalEdit.obj"
	-@erase "$(INTDIR)\OXPhysicalEditEx.obj"
	-@erase "$(INTDIR)\OXPopupBarCtrl.obj"
	-@erase "$(INTDIR)\OXPreviewDialog.obj"
	-@erase "$(INTDIR)\OXProcess.obj"
	-@erase "$(INTDIR)\OXPropertiesWnd.obj"
	-@erase "$(INTDIR)\OXPropertyDescriptionPane.obj"
	-@erase "$(INTDIR)\OXPropertyTree.obj"
	-@erase "$(INTDIR)\OXQuickString.obj"
	-@erase "$(INTDIR)\OXRectTracker.obj"
	-@erase "$(INTDIR)\OXRegExpression.obj"
	-@erase "$(INTDIR)\OXRegistryItem.obj"
	-@erase "$(INTDIR)\OXRegistryValFile.obj"
	-@erase "$(INTDIR)\OXRegistryWatcher.obj"
	-@erase "$(INTDIR)\OXRegistryWatchNotifier.obj"
	-@erase "$(INTDIR)\OXResourceFile.obj"
	-@erase "$(INTDIR)\OXResourceLibrary.obj"
	-@erase "$(INTDIR)\OXRollup.obj"
	-@erase "$(INTDIR)\OXRoundedButton.obj"
	-@erase "$(INTDIR)\OXRulerOrganizer.obj"
	-@erase "$(INTDIR)\OXScreenGrab.obj"
	-@erase "$(INTDIR)\OXScript.obj"
	-@erase "$(INTDIR)\OXScrollTip.obj"
	-@erase "$(INTDIR)\OXScrollTipOwner.obj"
	-@erase "$(INTDIR)\OXScrollWnd.obj"
	-@erase "$(INTDIR)\OXSeparator.obj"
	-@erase "$(INTDIR)\OXService.obj"
	-@erase "$(INTDIR)\OXShape.obj"
	-@erase "$(INTDIR)\OXShdWnd.obj"
	-@erase "$(INTDIR)\OXShellFolderTree.obj"
	-@erase "$(INTDIR)\OXShellNamespaceNavigator.obj"
	-@erase "$(INTDIR)\OXShellObjectList.obj"
	-@erase "$(INTDIR)\OXShortcut.obj"
	-@erase "$(INTDIR)\OXShortcutBar.obj"
	-@erase "$(INTDIR)\OXShortkeysOrganizer.obj"
	-@erase "$(INTDIR)\OXSizeCtrlBar.obj"
	-@erase "$(INTDIR)\OXSizeDlgBar.obj"
	-@erase "$(INTDIR)\OXSizeDockBar.obj"
	-@erase "$(INTDIR)\OXSizeToolBar.obj"
	-@erase "$(INTDIR)\OXSkinnedComboBox.obj"
	-@erase "$(INTDIR)\OXSkinnedSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSkinnedTabCtrl.obj"
	-@erase "$(INTDIR)\OXSkins.obj"
	-@erase "$(INTDIR)\OXSound.obj"
	-@erase "$(INTDIR)\OXSoundCustomizeDlg.obj"
	-@erase "$(INTDIR)\OXSoundEffectManager.obj"
	-@erase "$(INTDIR)\OXSoundWnd.obj"
	-@erase "$(INTDIR)\OXSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSplashWnd.obj"
	-@erase "$(INTDIR)\OXSplashWndDIB.obj"
	-@erase "$(INTDIR)\OXSplitterColRowState.obj"
	-@erase "$(INTDIR)\OXSplitterRect.obj"
	-@erase "$(INTDIR)\OXSplitterWnd.obj"
	-@erase "$(INTDIR)\OXStatic.obj"
	-@erase "$(INTDIR)\OXStaticHyperLink.obj"
	-@erase "$(INTDIR)\OXStaticText.obj"
	-@erase "$(INTDIR)\OXSysInfo.obj"
	-@erase "$(INTDIR)\OXSzMiniDockFrmWnd.obj"
	-@erase "$(INTDIR)\OXTabClientWnd.obj"
	-@erase "$(INTDIR)\OXTabView.obj"
	-@erase "$(INTDIR)\OXTaskbarIcon.obj"
	-@erase "$(INTDIR)\OXTleBar.obj"
	-@erase "$(INTDIR)\OXToolTipCtrl.obj"
	-@erase "$(INTDIR)\OXTrace.obj"
	-@erase "$(INTDIR)\OXTreeCal.obj"
	-@erase "$(INTDIR)\OXTreeCombo.obj"
	-@erase "$(INTDIR)\OXTreeCtrl.obj"
	-@erase "$(INTDIR)\OXTreeEdit.obj"
	-@erase "$(INTDIR)\OXTreeHeader.obj"
	-@erase "$(INTDIR)\OXTreeItem.obj"
	-@erase "$(INTDIR)\OXUNC.obj"
	-@erase "$(INTDIR)\OXUNCStandardActor.obj"
	-@erase "$(INTDIR)\OXURL.obj"
	-@erase "$(INTDIR)\OXUsertl.obj"
	-@erase "$(INTDIR)\OXUUDec.obj"
	-@erase "$(INTDIR)\OXUxTheme.obj"
	-@erase "$(INTDIR)\OXWABook.obj"
	-@erase "$(INTDIR)\OXWatchedDir.obj"
	-@erase "$(INTDIR)\oxwebctrl.obj"
	-@erase "$(INTDIR)\OXWorkspaceState.obj"
	-@erase "$(INTDIR)\Oxzoomvw.obj"
	-@erase "$(INTDIR)\Path.obj"
	-@erase "$(INTDIR)\PATHITER.OBJ"
	-@erase "$(INTDIR)\PROGRESS.OBJ"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\UT.obj"
	-@erase "$(INTDIR)\UT.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "$(OUTDIR)\UTD.pdb"
	-@erase "..\DLLs\UTD.dll"
	-@erase "..\DLLs\UTD.exp"
	-@erase "..\DLLs\UTD.ilk"
	-@erase "..\DLLs\UTD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_WINDLL" /D "_AFXDLL" /Fp"$(INTDIR)\UT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x1009 /fo"$(INTDIR)\OXMain.res" /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=..\..\Lib\libs\jpgd.lib ..\..\Lib\libs\cpuinfd.lib version.lib mapi32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\UTD.pdb" /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\DLLs/UTD.dll" /implib:"..\DLLs/UTD.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\CONVFILE.OBJ" \
	"$(INTDIR)\COPYTREE.OBJ" \
	"$(INTDIR)\CPYSTDLG.OBJ" \
	"$(INTDIR)\DIR.OBJ" \
	"$(INTDIR)\DIRLIST.OBJ" \
	"$(INTDIR)\DSTRLIST.OBJ" \
	"$(INTDIR)\EVNTLOG.OBJ" \
	"$(INTDIR)\FILE.OBJ" \
	"$(INTDIR)\FILELIST.OBJ" \
	"$(INTDIR)\MDELDLG.OBJ" \
	"$(INTDIR)\MFCToolBar.obj" \
	"$(INTDIR)\OX3DTabView.obj" \
	"$(INTDIR)\OXActiveDesktop.obj" \
	"$(INTDIR)\OXAdvancedAssert.obj" \
	"$(INTDIR)\OXAdvancedAssertMail.obj" \
	"$(INTDIR)\OXAppBar.obj" \
	"$(INTDIR)\OXAutoComplete.obj" \
	"$(INTDIR)\OXAutoListBox.obj" \
	"$(INTDIR)\OXBackgroundPainter.obj" \
	"$(INTDIR)\OXBITBUF.OBJ" \
	"$(INTDIR)\OXBitmapButton.obj" \
	"$(INTDIR)\OXBitmapMenu.obj" \
	"$(INTDIR)\OXBitmapMenuOrganizer.obj" \
	"$(INTDIR)\OXBMPFilePickerCombo.obj" \
	"$(INTDIR)\OXBMPFle.obj" \
	"$(INTDIR)\OXBrowseColorEdit.obj" \
	"$(INTDIR)\OXBrowseDirEdit.obj" \
	"$(INTDIR)\OXBrowseDirEditList.obj" \
	"$(INTDIR)\OXButtonAppearanceDlg.obj" \
	"$(INTDIR)\OXCalculatorCtrl.obj" \
	"$(INTDIR)\OXCalculatorEdit.obj" \
	"$(INTDIR)\OXCalendar.obj" \
	"$(INTDIR)\OXCalendarEdit.obj" \
	"$(INTDIR)\OXCalendarPopup.obj" \
	"$(INTDIR)\OXCaptionPainter.obj" \
	"$(INTDIR)\OXChildFrameState.obj" \
	"$(INTDIR)\OXClipPocket.obj" \
	"$(INTDIR)\OXCMPFL.OBJ" \
	"$(INTDIR)\OXColorPickerButton.obj" \
	"$(INTDIR)\OXColorPickerCtrl.obj" \
	"$(INTDIR)\OXComboPickerCtrl.obj" \
	"$(INTDIR)\OXCOMPR.OBJ" \
	"$(INTDIR)\OXContextHelpDialog.obj" \
	"$(INTDIR)\OXContextHelpPropertyPage.obj" \
	"$(INTDIR)\OXContextHelpPropertySheet.obj" \
	"$(INTDIR)\OXCoolBar.obj" \
	"$(INTDIR)\OXCoolComboBox.obj" \
	"$(INTDIR)\OXCoolControlOrganizer.obj" \
	"$(INTDIR)\OXCoolCtrl.obj" \
	"$(INTDIR)\OXCoolToolBar.obj" \
	"$(INTDIR)\OXCRCChk.obj" \
	"$(INTDIR)\OXCsvFile.obj" \
	"$(INTDIR)\OXCustomizeBackgroundPage.obj" \
	"$(INTDIR)\OXCustomizeCaptionPage.obj" \
	"$(INTDIR)\OXCustomizeCommandsPage.obj" \
	"$(INTDIR)\OXCustomizeCoolControlsPage.obj" \
	"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" \
	"$(INTDIR)\OXCustomizeIntelliMousePage.obj" \
	"$(INTDIR)\OXCustomizeManager.obj" \
	"$(INTDIR)\OXCustomizePage.obj" \
	"$(INTDIR)\OXCustomizeShortkeysPage.obj" \
	"$(INTDIR)\OXCustomizeSplashWindowPage.obj" \
	"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" \
	"$(INTDIR)\OXCustomizeToolbarsPage.obj" \
	"$(INTDIR)\OXCustomizeTrayIconPage.obj" \
	"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" \
	"$(INTDIR)\OXDao.obj" \
	"$(INTDIR)\OXDateTimeCtrl.obj" \
	"$(INTDIR)\OXDckPPg.obj" \
	"$(INTDIR)\OXDckPSh.obj" \
	"$(INTDIR)\OXDFlHdr.obj" \
	"$(INTDIR)\Oxdib.obj" \
	"$(INTDIR)\OXDirDlg.obj" \
	"$(INTDIR)\OXDlgBar.obj" \
	"$(INTDIR)\OXDLLManager.obj" \
	"$(INTDIR)\oxdocktabctrl.obj" \
	"$(INTDIR)\OXDocTemplateSpy.obj" \
	"$(INTDIR)\OXDragDockContext.obj" \
	"$(INTDIR)\OXDragDropCommands.obj" \
	"$(INTDIR)\OXEdit.obj" \
	"$(INTDIR)\OXEditList.obj" \
	"$(INTDIR)\OXFileChanger.obj" \
	"$(INTDIR)\OXFileWatcher.obj" \
	"$(INTDIR)\OXFileWatchNotifier.obj" \
	"$(INTDIR)\OXFolderPickerDlg.obj" \
	"$(INTDIR)\OXFontComboBox.obj" \
	"$(INTDIR)\OXFontPickerButton.obj" \
	"$(INTDIR)\OXFrameWndDock.obj" \
	"$(INTDIR)\OXFullScreenFrame.obj" \
	"$(INTDIR)\OXGphFle.obj" \
	"$(INTDIR)\OXGraphics.obj" \
	"$(INTDIR)\oxgridedit.obj" \
	"$(INTDIR)\OXGridHdr.obj" \
	"$(INTDIR)\OXGridList.obj" \
	"$(INTDIR)\OXHashTable.obj" \
	"$(INTDIR)\OXHistoryCombo.obj" \
	"$(INTDIR)\OXHistoryCtrl.obj" \
	"$(INTDIR)\OXHookWnd.obj" \
	"$(INTDIR)\OXHTMLParser.obj" \
	"$(INTDIR)\OXHyperLinkAction.obj" \
	"$(INTDIR)\OXImageListBox.obj" \
	"$(INTDIR)\OXImageViewer.obj" \
	"$(INTDIR)\OXInstanceManager.obj" \
	"$(INTDIR)\OXInteger.obj" \
	"$(INTDIR)\OXIntelliMouse.obj" \
	"$(INTDIR)\OXIPEdit.obj" \
	"$(INTDIR)\OXItemTip.obj" \
	"$(INTDIR)\OXItemTipWnd.obj" \
	"$(INTDIR)\OXIteratorRegistryItem.obj" \
	"$(INTDIR)\OXIteratorService.obj" \
	"$(INTDIR)\OXIteratorUNC.obj" \
	"$(INTDIR)\OXJPGCod.obj" \
	"$(INTDIR)\OXJPGCom.obj" \
	"$(INTDIR)\OXJPGDom.obj" \
	"$(INTDIR)\OXJPGExp.obj" \
	"$(INTDIR)\OXJPGFle.obj" \
	"$(INTDIR)\OXLayoutManager.obj" \
	"$(INTDIR)\OXListBoxEx.obj" \
	"$(INTDIR)\OXListEdit.obj" \
	"$(INTDIR)\OXMaskedEdit.obj" \
	"$(INTDIR)\OXMDIFloatWnd.obj" \
	"$(INTDIR)\OXMenuBar.obj" \
	"$(INTDIR)\OXMetaFile.obj" \
	"$(INTDIR)\OXMonthCalCtrl.obj" \
	"$(INTDIR)\OXMulticlipboardDlg.obj" \
	"$(INTDIR)\OXMultiComboBox.obj" \
	"$(INTDIR)\OXNetBrowseTree.obj" \
	"$(INTDIR)\OXNotesEditView.obj" \
	"$(INTDIR)\OXOptionTreeCtrl.obj" \
	"$(INTDIR)\OXParser.obj" \
	"$(INTDIR)\OXPhysicalEdit.obj" \
	"$(INTDIR)\OXPhysicalEditEx.obj" \
	"$(INTDIR)\OXPopupBarCtrl.obj" \
	"$(INTDIR)\OXPreviewDialog.obj" \
	"$(INTDIR)\OXProcess.obj" \
	"$(INTDIR)\OXPropertiesWnd.obj" \
	"$(INTDIR)\OXPropertyDescriptionPane.obj" \
	"$(INTDIR)\OXPropertyTree.obj" \
	"$(INTDIR)\OXQuickString.obj" \
	"$(INTDIR)\OXRectTracker.obj" \
	"$(INTDIR)\OXRegExpression.obj" \
	"$(INTDIR)\OXRegistryItem.obj" \
	"$(INTDIR)\OXRegistryValFile.obj" \
	"$(INTDIR)\OXRegistryWatcher.obj" \
	"$(INTDIR)\OXRegistryWatchNotifier.obj" \
	"$(INTDIR)\OXResourceFile.obj" \
	"$(INTDIR)\OXResourceLibrary.obj" \
	"$(INTDIR)\OXRollup.obj" \
	"$(INTDIR)\OXRoundedButton.obj" \
	"$(INTDIR)\OXRulerOrganizer.obj" \
	"$(INTDIR)\OXScreenGrab.obj" \
	"$(INTDIR)\OXScript.obj" \
	"$(INTDIR)\OXScrollTip.obj" \
	"$(INTDIR)\OXScrollTipOwner.obj" \
	"$(INTDIR)\OXScrollWnd.obj" \
	"$(INTDIR)\OXSeparator.obj" \
	"$(INTDIR)\OXService.obj" \
	"$(INTDIR)\OXShape.obj" \
	"$(INTDIR)\OXShdWnd.obj" \
	"$(INTDIR)\OXShellFolderTree.obj" \
	"$(INTDIR)\OXShellNamespaceNavigator.obj" \
	"$(INTDIR)\OXShellObjectList.obj" \
	"$(INTDIR)\OXShortcut.obj" \
	"$(INTDIR)\OXShortcutBar.obj" \
	"$(INTDIR)\OXShortkeysOrganizer.obj" \
	"$(INTDIR)\OXSizeCtrlBar.obj" \
	"$(INTDIR)\OXSizeDlgBar.obj" \
	"$(INTDIR)\OXSizeDockBar.obj" \
	"$(INTDIR)\OXSizeToolBar.obj" \
	"$(INTDIR)\OXSkinnedComboBox.obj" \
	"$(INTDIR)\OXSkinnedSpinCtrl.obj" \
	"$(INTDIR)\OXSkinnedTabCtrl.obj" \
	"$(INTDIR)\OXSkins.obj" \
	"$(INTDIR)\OXSound.obj" \
	"$(INTDIR)\OXSoundCustomizeDlg.obj" \
	"$(INTDIR)\OXSoundEffectManager.obj" \
	"$(INTDIR)\OXSoundWnd.obj" \
	"$(INTDIR)\OXSpinCtrl.obj" \
	"$(INTDIR)\OXSplashWnd.obj" \
	"$(INTDIR)\OXSplashWndDIB.obj" \
	"$(INTDIR)\OXSplitterColRowState.obj" \
	"$(INTDIR)\OXSplitterRect.obj" \
	"$(INTDIR)\OXSplitterWnd.obj" \
	"$(INTDIR)\OXStatic.obj" \
	"$(INTDIR)\OXStaticHyperLink.obj" \
	"$(INTDIR)\OXStaticText.obj" \
	"$(INTDIR)\OXSysInfo.obj" \
	"$(INTDIR)\OXSzMiniDockFrmWnd.obj" \
	"$(INTDIR)\OXTabClientWnd.obj" \
	"$(INTDIR)\OXTabView.obj" \
	"$(INTDIR)\OXTaskbarIcon.obj" \
	"$(INTDIR)\OXTleBar.obj" \
	"$(INTDIR)\OXToolTipCtrl.obj" \
	"$(INTDIR)\OXTrace.obj" \
	"$(INTDIR)\OXTreeCal.obj" \
	"$(INTDIR)\OXTreeCombo.obj" \
	"$(INTDIR)\OXTreeCtrl.obj" \
	"$(INTDIR)\OXTreeEdit.obj" \
	"$(INTDIR)\OXTreeHeader.obj" \
	"$(INTDIR)\OXTreeItem.obj" \
	"$(INTDIR)\OXUNC.obj" \
	"$(INTDIR)\OXUNCStandardActor.obj" \
	"$(INTDIR)\OXURL.obj" \
	"$(INTDIR)\OXUsertl.obj" \
	"$(INTDIR)\OXUUDec.obj" \
	"$(INTDIR)\OXUxTheme.obj" \
	"$(INTDIR)\OXWABook.obj" \
	"$(INTDIR)\OXWatchedDir.obj" \
	"$(INTDIR)\oxwebctrl.obj" \
	"$(INTDIR)\OXWorkspaceState.obj" \
	"$(INTDIR)\Oxzoomvw.obj" \
	"$(INTDIR)\Path.obj" \
	"$(INTDIR)\PATHITER.OBJ" \
	"$(INTDIR)\PROGRESS.OBJ" \
	"$(INTDIR)\WATCHBUF.OBJ" \
	"$(INTDIR)\Xstatus4.obj" \
	"$(INTDIR)\XSTRING.OBJ" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UT.obj" \
	"$(INTDIR)\OXMain.res"

"..\DLLs\UTD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UT - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\DLLs\UT.dll"


CLEAN :
	-@erase "$(INTDIR)\CONVFILE.OBJ"
	-@erase "$(INTDIR)\COPYTREE.OBJ"
	-@erase "$(INTDIR)\CPYSTDLG.OBJ"
	-@erase "$(INTDIR)\DIR.OBJ"
	-@erase "$(INTDIR)\DIRLIST.OBJ"
	-@erase "$(INTDIR)\DSTRLIST.OBJ"
	-@erase "$(INTDIR)\EVNTLOG.OBJ"
	-@erase "$(INTDIR)\FILE.OBJ"
	-@erase "$(INTDIR)\FILELIST.OBJ"
	-@erase "$(INTDIR)\MDELDLG.OBJ"
	-@erase "$(INTDIR)\MFCToolBar.obj"
	-@erase "$(INTDIR)\OX3DTabView.obj"
	-@erase "$(INTDIR)\OXActiveDesktop.obj"
	-@erase "$(INTDIR)\OXAdvancedAssert.obj"
	-@erase "$(INTDIR)\OXAdvancedAssertMail.obj"
	-@erase "$(INTDIR)\OXAppBar.obj"
	-@erase "$(INTDIR)\OXAutoComplete.obj"
	-@erase "$(INTDIR)\OXAutoListBox.obj"
	-@erase "$(INTDIR)\OXBackgroundPainter.obj"
	-@erase "$(INTDIR)\OXBITBUF.OBJ"
	-@erase "$(INTDIR)\OXBitmapButton.obj"
	-@erase "$(INTDIR)\OXBitmapMenu.obj"
	-@erase "$(INTDIR)\OXBitmapMenuOrganizer.obj"
	-@erase "$(INTDIR)\OXBMPFilePickerCombo.obj"
	-@erase "$(INTDIR)\OXBMPFle.obj"
	-@erase "$(INTDIR)\OXBrowseColorEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEditList.obj"
	-@erase "$(INTDIR)\OXButtonAppearanceDlg.obj"
	-@erase "$(INTDIR)\OXCalculatorCtrl.obj"
	-@erase "$(INTDIR)\OXCalculatorEdit.obj"
	-@erase "$(INTDIR)\OXCalendar.obj"
	-@erase "$(INTDIR)\OXCalendarEdit.obj"
	-@erase "$(INTDIR)\OXCalendarPopup.obj"
	-@erase "$(INTDIR)\OXCaptionPainter.obj"
	-@erase "$(INTDIR)\OXChildFrameState.obj"
	-@erase "$(INTDIR)\OXClipPocket.obj"
	-@erase "$(INTDIR)\OXCMPFL.OBJ"
	-@erase "$(INTDIR)\OXColorPickerButton.obj"
	-@erase "$(INTDIR)\OXColorPickerCtrl.obj"
	-@erase "$(INTDIR)\OXComboPickerCtrl.obj"
	-@erase "$(INTDIR)\OXCOMPR.OBJ"
	-@erase "$(INTDIR)\OXContextHelpDialog.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertyPage.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertySheet.obj"
	-@erase "$(INTDIR)\OXCoolBar.obj"
	-@erase "$(INTDIR)\OXCoolComboBox.obj"
	-@erase "$(INTDIR)\OXCoolControlOrganizer.obj"
	-@erase "$(INTDIR)\OXCoolCtrl.obj"
	-@erase "$(INTDIR)\OXCoolToolBar.obj"
	-@erase "$(INTDIR)\OXCRCChk.obj"
	-@erase "$(INTDIR)\OXCsvFile.obj"
	-@erase "$(INTDIR)\OXCustomizeBackgroundPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCaptionPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCommandsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCoolControlsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeInstanceManagerPage.obj"
	-@erase "$(INTDIR)\OXCustomizeIntelliMousePage.obj"
	-@erase "$(INTDIR)\OXCustomizeManager.obj"
	-@erase "$(INTDIR)\OXCustomizePage.obj"
	-@erase "$(INTDIR)\OXCustomizeShortkeysPage.obj"
	-@erase "$(INTDIR)\OXCustomizeSplashWindowPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTabbedMDIPage.obj"
	-@erase "$(INTDIR)\OXCustomizeToolbarsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTrayIconPage.obj"
	-@erase "$(INTDIR)\OXCustomizeWorkspaceStatePage.obj"
	-@erase "$(INTDIR)\OXDao.obj"
	-@erase "$(INTDIR)\OXDateTimeCtrl.obj"
	-@erase "$(INTDIR)\OXDckPPg.obj"
	-@erase "$(INTDIR)\OXDckPSh.obj"
	-@erase "$(INTDIR)\OXDFlHdr.obj"
	-@erase "$(INTDIR)\Oxdib.obj"
	-@erase "$(INTDIR)\OXDirDlg.obj"
	-@erase "$(INTDIR)\OXDlgBar.obj"
	-@erase "$(INTDIR)\OXDLLManager.obj"
	-@erase "$(INTDIR)\oxdocktabctrl.obj"
	-@erase "$(INTDIR)\OXDocTemplateSpy.obj"
	-@erase "$(INTDIR)\OXDragDockContext.obj"
	-@erase "$(INTDIR)\OXDragDropCommands.obj"
	-@erase "$(INTDIR)\OXEdit.obj"
	-@erase "$(INTDIR)\OXEditList.obj"
	-@erase "$(INTDIR)\OXFileChanger.obj"
	-@erase "$(INTDIR)\OXFileWatcher.obj"
	-@erase "$(INTDIR)\OXFileWatchNotifier.obj"
	-@erase "$(INTDIR)\OXFolderPickerDlg.obj"
	-@erase "$(INTDIR)\OXFontComboBox.obj"
	-@erase "$(INTDIR)\OXFontPickerButton.obj"
	-@erase "$(INTDIR)\OXFrameWndDock.obj"
	-@erase "$(INTDIR)\OXFullScreenFrame.obj"
	-@erase "$(INTDIR)\OXGphFle.obj"
	-@erase "$(INTDIR)\OXGraphics.obj"
	-@erase "$(INTDIR)\oxgridedit.obj"
	-@erase "$(INTDIR)\OXGridHdr.obj"
	-@erase "$(INTDIR)\OXGridList.obj"
	-@erase "$(INTDIR)\OXHashTable.obj"
	-@erase "$(INTDIR)\OXHistoryCombo.obj"
	-@erase "$(INTDIR)\OXHistoryCtrl.obj"
	-@erase "$(INTDIR)\OXHookWnd.obj"
	-@erase "$(INTDIR)\OXHTMLParser.obj"
	-@erase "$(INTDIR)\OXHyperLinkAction.obj"
	-@erase "$(INTDIR)\OXImageListBox.obj"
	-@erase "$(INTDIR)\OXImageViewer.obj"
	-@erase "$(INTDIR)\OXInstanceManager.obj"
	-@erase "$(INTDIR)\OXInteger.obj"
	-@erase "$(INTDIR)\OXIntelliMouse.obj"
	-@erase "$(INTDIR)\OXIPEdit.obj"
	-@erase "$(INTDIR)\OXItemTip.obj"
	-@erase "$(INTDIR)\OXItemTipWnd.obj"
	-@erase "$(INTDIR)\OXIteratorRegistryItem.obj"
	-@erase "$(INTDIR)\OXIteratorService.obj"
	-@erase "$(INTDIR)\OXIteratorUNC.obj"
	-@erase "$(INTDIR)\OXJPGCod.obj"
	-@erase "$(INTDIR)\OXJPGCom.obj"
	-@erase "$(INTDIR)\OXJPGDom.obj"
	-@erase "$(INTDIR)\OXJPGExp.obj"
	-@erase "$(INTDIR)\OXJPGFle.obj"
	-@erase "$(INTDIR)\OXLayoutManager.obj"
	-@erase "$(INTDIR)\OXListBoxEx.obj"
	-@erase "$(INTDIR)\OXListEdit.obj"
	-@erase "$(INTDIR)\OXMain.res"
	-@erase "$(INTDIR)\OXMaskedEdit.obj"
	-@erase "$(INTDIR)\OXMDIFloatWnd.obj"
	-@erase "$(INTDIR)\OXMenuBar.obj"
	-@erase "$(INTDIR)\OXMetaFile.obj"
	-@erase "$(INTDIR)\OXMonthCalCtrl.obj"
	-@erase "$(INTDIR)\OXMulticlipboardDlg.obj"
	-@erase "$(INTDIR)\OXMultiComboBox.obj"
	-@erase "$(INTDIR)\OXNetBrowseTree.obj"
	-@erase "$(INTDIR)\OXNotesEditView.obj"
	-@erase "$(INTDIR)\OXOptionTreeCtrl.obj"
	-@erase "$(INTDIR)\OXParser.obj"
	-@erase "$(INTDIR)\OXPhysicalEdit.obj"
	-@erase "$(INTDIR)\OXPhysicalEditEx.obj"
	-@erase "$(INTDIR)\OXPopupBarCtrl.obj"
	-@erase "$(INTDIR)\OXPreviewDialog.obj"
	-@erase "$(INTDIR)\OXProcess.obj"
	-@erase "$(INTDIR)\OXPropertiesWnd.obj"
	-@erase "$(INTDIR)\OXPropertyDescriptionPane.obj"
	-@erase "$(INTDIR)\OXPropertyTree.obj"
	-@erase "$(INTDIR)\OXQuickString.obj"
	-@erase "$(INTDIR)\OXRectTracker.obj"
	-@erase "$(INTDIR)\OXRegExpression.obj"
	-@erase "$(INTDIR)\OXRegistryItem.obj"
	-@erase "$(INTDIR)\OXRegistryValFile.obj"
	-@erase "$(INTDIR)\OXRegistryWatcher.obj"
	-@erase "$(INTDIR)\OXRegistryWatchNotifier.obj"
	-@erase "$(INTDIR)\OXResourceFile.obj"
	-@erase "$(INTDIR)\OXResourceLibrary.obj"
	-@erase "$(INTDIR)\OXRollup.obj"
	-@erase "$(INTDIR)\OXRoundedButton.obj"
	-@erase "$(INTDIR)\OXRulerOrganizer.obj"
	-@erase "$(INTDIR)\OXScreenGrab.obj"
	-@erase "$(INTDIR)\OXScript.obj"
	-@erase "$(INTDIR)\OXScrollTip.obj"
	-@erase "$(INTDIR)\OXScrollTipOwner.obj"
	-@erase "$(INTDIR)\OXScrollWnd.obj"
	-@erase "$(INTDIR)\OXSeparator.obj"
	-@erase "$(INTDIR)\OXService.obj"
	-@erase "$(INTDIR)\OXShape.obj"
	-@erase "$(INTDIR)\OXShdWnd.obj"
	-@erase "$(INTDIR)\OXShellFolderTree.obj"
	-@erase "$(INTDIR)\OXShellNamespaceNavigator.obj"
	-@erase "$(INTDIR)\OXShellObjectList.obj"
	-@erase "$(INTDIR)\OXShortcut.obj"
	-@erase "$(INTDIR)\OXShortcutBar.obj"
	-@erase "$(INTDIR)\OXShortkeysOrganizer.obj"
	-@erase "$(INTDIR)\OXSizeCtrlBar.obj"
	-@erase "$(INTDIR)\OXSizeDlgBar.obj"
	-@erase "$(INTDIR)\OXSizeDockBar.obj"
	-@erase "$(INTDIR)\OXSizeToolBar.obj"
	-@erase "$(INTDIR)\OXSkinnedComboBox.obj"
	-@erase "$(INTDIR)\OXSkinnedSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSkinnedTabCtrl.obj"
	-@erase "$(INTDIR)\OXSkins.obj"
	-@erase "$(INTDIR)\OXSound.obj"
	-@erase "$(INTDIR)\OXSoundCustomizeDlg.obj"
	-@erase "$(INTDIR)\OXSoundEffectManager.obj"
	-@erase "$(INTDIR)\OXSoundWnd.obj"
	-@erase "$(INTDIR)\OXSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSplashWnd.obj"
	-@erase "$(INTDIR)\OXSplashWndDIB.obj"
	-@erase "$(INTDIR)\OXSplitterColRowState.obj"
	-@erase "$(INTDIR)\OXSplitterRect.obj"
	-@erase "$(INTDIR)\OXSplitterWnd.obj"
	-@erase "$(INTDIR)\OXStatic.obj"
	-@erase "$(INTDIR)\OXStaticHyperLink.obj"
	-@erase "$(INTDIR)\OXStaticText.obj"
	-@erase "$(INTDIR)\OXSysInfo.obj"
	-@erase "$(INTDIR)\OXSzMiniDockFrmWnd.obj"
	-@erase "$(INTDIR)\OXTabClientWnd.obj"
	-@erase "$(INTDIR)\OXTabView.obj"
	-@erase "$(INTDIR)\OXTaskbarIcon.obj"
	-@erase "$(INTDIR)\OXTleBar.obj"
	-@erase "$(INTDIR)\OXToolTipCtrl.obj"
	-@erase "$(INTDIR)\OXTrace.obj"
	-@erase "$(INTDIR)\OXTreeCal.obj"
	-@erase "$(INTDIR)\OXTreeCombo.obj"
	-@erase "$(INTDIR)\OXTreeCtrl.obj"
	-@erase "$(INTDIR)\OXTreeEdit.obj"
	-@erase "$(INTDIR)\OXTreeHeader.obj"
	-@erase "$(INTDIR)\OXTreeItem.obj"
	-@erase "$(INTDIR)\OXUNC.obj"
	-@erase "$(INTDIR)\OXUNCStandardActor.obj"
	-@erase "$(INTDIR)\OXURL.obj"
	-@erase "$(INTDIR)\OXUsertl.obj"
	-@erase "$(INTDIR)\OXUUDec.obj"
	-@erase "$(INTDIR)\OXUxTheme.obj"
	-@erase "$(INTDIR)\OXWABook.obj"
	-@erase "$(INTDIR)\OXWatchedDir.obj"
	-@erase "$(INTDIR)\oxwebctrl.obj"
	-@erase "$(INTDIR)\OXWorkspaceState.obj"
	-@erase "$(INTDIR)\Oxzoomvw.obj"
	-@erase "$(INTDIR)\Path.obj"
	-@erase "$(INTDIR)\PATHITER.OBJ"
	-@erase "$(INTDIR)\PROGRESS.OBJ"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\UT.obj"
	-@erase "$(INTDIR)\UT.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\DLLs\UT.dll"
	-@erase "..\DLLs\UT.exp"
	-@erase "..\DLLs\UT.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_WINDLL" /D "_AFXDLL" /Fp"$(INTDIR)\UT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x1009 /fo"$(INTDIR)\OXMain.res" /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=..\..\Lib\libs\jpgr.lib ..\..\Lib\libs\cpuinfr.lib version.lib mapi32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\UT.pdb" /machine:I386 /nodefaultlib:"LIBC" /out:"..\DLLs/UT.dll" /implib:"..\DLLs/UT.lib" 
LINK32_OBJS= \
	"$(INTDIR)\CONVFILE.OBJ" \
	"$(INTDIR)\COPYTREE.OBJ" \
	"$(INTDIR)\CPYSTDLG.OBJ" \
	"$(INTDIR)\DIR.OBJ" \
	"$(INTDIR)\DIRLIST.OBJ" \
	"$(INTDIR)\DSTRLIST.OBJ" \
	"$(INTDIR)\EVNTLOG.OBJ" \
	"$(INTDIR)\FILE.OBJ" \
	"$(INTDIR)\FILELIST.OBJ" \
	"$(INTDIR)\MDELDLG.OBJ" \
	"$(INTDIR)\MFCToolBar.obj" \
	"$(INTDIR)\OX3DTabView.obj" \
	"$(INTDIR)\OXActiveDesktop.obj" \
	"$(INTDIR)\OXAdvancedAssert.obj" \
	"$(INTDIR)\OXAdvancedAssertMail.obj" \
	"$(INTDIR)\OXAppBar.obj" \
	"$(INTDIR)\OXAutoComplete.obj" \
	"$(INTDIR)\OXAutoListBox.obj" \
	"$(INTDIR)\OXBackgroundPainter.obj" \
	"$(INTDIR)\OXBITBUF.OBJ" \
	"$(INTDIR)\OXBitmapButton.obj" \
	"$(INTDIR)\OXBitmapMenu.obj" \
	"$(INTDIR)\OXBitmapMenuOrganizer.obj" \
	"$(INTDIR)\OXBMPFilePickerCombo.obj" \
	"$(INTDIR)\OXBMPFle.obj" \
	"$(INTDIR)\OXBrowseColorEdit.obj" \
	"$(INTDIR)\OXBrowseDirEdit.obj" \
	"$(INTDIR)\OXBrowseDirEditList.obj" \
	"$(INTDIR)\OXButtonAppearanceDlg.obj" \
	"$(INTDIR)\OXCalculatorCtrl.obj" \
	"$(INTDIR)\OXCalculatorEdit.obj" \
	"$(INTDIR)\OXCalendar.obj" \
	"$(INTDIR)\OXCalendarEdit.obj" \
	"$(INTDIR)\OXCalendarPopup.obj" \
	"$(INTDIR)\OXCaptionPainter.obj" \
	"$(INTDIR)\OXChildFrameState.obj" \
	"$(INTDIR)\OXClipPocket.obj" \
	"$(INTDIR)\OXCMPFL.OBJ" \
	"$(INTDIR)\OXColorPickerButton.obj" \
	"$(INTDIR)\OXColorPickerCtrl.obj" \
	"$(INTDIR)\OXComboPickerCtrl.obj" \
	"$(INTDIR)\OXCOMPR.OBJ" \
	"$(INTDIR)\OXContextHelpDialog.obj" \
	"$(INTDIR)\OXContextHelpPropertyPage.obj" \
	"$(INTDIR)\OXContextHelpPropertySheet.obj" \
	"$(INTDIR)\OXCoolBar.obj" \
	"$(INTDIR)\OXCoolComboBox.obj" \
	"$(INTDIR)\OXCoolControlOrganizer.obj" \
	"$(INTDIR)\OXCoolCtrl.obj" \
	"$(INTDIR)\OXCoolToolBar.obj" \
	"$(INTDIR)\OXCRCChk.obj" \
	"$(INTDIR)\OXCsvFile.obj" \
	"$(INTDIR)\OXCustomizeBackgroundPage.obj" \
	"$(INTDIR)\OXCustomizeCaptionPage.obj" \
	"$(INTDIR)\OXCustomizeCommandsPage.obj" \
	"$(INTDIR)\OXCustomizeCoolControlsPage.obj" \
	"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" \
	"$(INTDIR)\OXCustomizeIntelliMousePage.obj" \
	"$(INTDIR)\OXCustomizeManager.obj" \
	"$(INTDIR)\OXCustomizePage.obj" \
	"$(INTDIR)\OXCustomizeShortkeysPage.obj" \
	"$(INTDIR)\OXCustomizeSplashWindowPage.obj" \
	"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" \
	"$(INTDIR)\OXCustomizeToolbarsPage.obj" \
	"$(INTDIR)\OXCustomizeTrayIconPage.obj" \
	"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" \
	"$(INTDIR)\OXDao.obj" \
	"$(INTDIR)\OXDateTimeCtrl.obj" \
	"$(INTDIR)\OXDckPPg.obj" \
	"$(INTDIR)\OXDckPSh.obj" \
	"$(INTDIR)\OXDFlHdr.obj" \
	"$(INTDIR)\Oxdib.obj" \
	"$(INTDIR)\OXDirDlg.obj" \
	"$(INTDIR)\OXDlgBar.obj" \
	"$(INTDIR)\OXDLLManager.obj" \
	"$(INTDIR)\oxdocktabctrl.obj" \
	"$(INTDIR)\OXDocTemplateSpy.obj" \
	"$(INTDIR)\OXDragDockContext.obj" \
	"$(INTDIR)\OXDragDropCommands.obj" \
	"$(INTDIR)\OXEdit.obj" \
	"$(INTDIR)\OXEditList.obj" \
	"$(INTDIR)\OXFileChanger.obj" \
	"$(INTDIR)\OXFileWatcher.obj" \
	"$(INTDIR)\OXFileWatchNotifier.obj" \
	"$(INTDIR)\OXFolderPickerDlg.obj" \
	"$(INTDIR)\OXFontComboBox.obj" \
	"$(INTDIR)\OXFontPickerButton.obj" \
	"$(INTDIR)\OXFrameWndDock.obj" \
	"$(INTDIR)\OXFullScreenFrame.obj" \
	"$(INTDIR)\OXGphFle.obj" \
	"$(INTDIR)\OXGraphics.obj" \
	"$(INTDIR)\oxgridedit.obj" \
	"$(INTDIR)\OXGridHdr.obj" \
	"$(INTDIR)\OXGridList.obj" \
	"$(INTDIR)\OXHashTable.obj" \
	"$(INTDIR)\OXHistoryCombo.obj" \
	"$(INTDIR)\OXHistoryCtrl.obj" \
	"$(INTDIR)\OXHookWnd.obj" \
	"$(INTDIR)\OXHTMLParser.obj" \
	"$(INTDIR)\OXHyperLinkAction.obj" \
	"$(INTDIR)\OXImageListBox.obj" \
	"$(INTDIR)\OXImageViewer.obj" \
	"$(INTDIR)\OXInstanceManager.obj" \
	"$(INTDIR)\OXInteger.obj" \
	"$(INTDIR)\OXIntelliMouse.obj" \
	"$(INTDIR)\OXIPEdit.obj" \
	"$(INTDIR)\OXItemTip.obj" \
	"$(INTDIR)\OXItemTipWnd.obj" \
	"$(INTDIR)\OXIteratorRegistryItem.obj" \
	"$(INTDIR)\OXIteratorService.obj" \
	"$(INTDIR)\OXIteratorUNC.obj" \
	"$(INTDIR)\OXJPGCod.obj" \
	"$(INTDIR)\OXJPGCom.obj" \
	"$(INTDIR)\OXJPGDom.obj" \
	"$(INTDIR)\OXJPGExp.obj" \
	"$(INTDIR)\OXJPGFle.obj" \
	"$(INTDIR)\OXLayoutManager.obj" \
	"$(INTDIR)\OXListBoxEx.obj" \
	"$(INTDIR)\OXListEdit.obj" \
	"$(INTDIR)\OXMaskedEdit.obj" \
	"$(INTDIR)\OXMDIFloatWnd.obj" \
	"$(INTDIR)\OXMenuBar.obj" \
	"$(INTDIR)\OXMetaFile.obj" \
	"$(INTDIR)\OXMonthCalCtrl.obj" \
	"$(INTDIR)\OXMulticlipboardDlg.obj" \
	"$(INTDIR)\OXMultiComboBox.obj" \
	"$(INTDIR)\OXNetBrowseTree.obj" \
	"$(INTDIR)\OXNotesEditView.obj" \
	"$(INTDIR)\OXOptionTreeCtrl.obj" \
	"$(INTDIR)\OXParser.obj" \
	"$(INTDIR)\OXPhysicalEdit.obj" \
	"$(INTDIR)\OXPhysicalEditEx.obj" \
	"$(INTDIR)\OXPopupBarCtrl.obj" \
	"$(INTDIR)\OXPreviewDialog.obj" \
	"$(INTDIR)\OXProcess.obj" \
	"$(INTDIR)\OXPropertiesWnd.obj" \
	"$(INTDIR)\OXPropertyDescriptionPane.obj" \
	"$(INTDIR)\OXPropertyTree.obj" \
	"$(INTDIR)\OXQuickString.obj" \
	"$(INTDIR)\OXRectTracker.obj" \
	"$(INTDIR)\OXRegExpression.obj" \
	"$(INTDIR)\OXRegistryItem.obj" \
	"$(INTDIR)\OXRegistryValFile.obj" \
	"$(INTDIR)\OXRegistryWatcher.obj" \
	"$(INTDIR)\OXRegistryWatchNotifier.obj" \
	"$(INTDIR)\OXResourceFile.obj" \
	"$(INTDIR)\OXResourceLibrary.obj" \
	"$(INTDIR)\OXRollup.obj" \
	"$(INTDIR)\OXRoundedButton.obj" \
	"$(INTDIR)\OXRulerOrganizer.obj" \
	"$(INTDIR)\OXScreenGrab.obj" \
	"$(INTDIR)\OXScript.obj" \
	"$(INTDIR)\OXScrollTip.obj" \
	"$(INTDIR)\OXScrollTipOwner.obj" \
	"$(INTDIR)\OXScrollWnd.obj" \
	"$(INTDIR)\OXSeparator.obj" \
	"$(INTDIR)\OXService.obj" \
	"$(INTDIR)\OXShape.obj" \
	"$(INTDIR)\OXShdWnd.obj" \
	"$(INTDIR)\OXShellFolderTree.obj" \
	"$(INTDIR)\OXShellNamespaceNavigator.obj" \
	"$(INTDIR)\OXShellObjectList.obj" \
	"$(INTDIR)\OXShortcut.obj" \
	"$(INTDIR)\OXShortcutBar.obj" \
	"$(INTDIR)\OXShortkeysOrganizer.obj" \
	"$(INTDIR)\OXSizeCtrlBar.obj" \
	"$(INTDIR)\OXSizeDlgBar.obj" \
	"$(INTDIR)\OXSizeDockBar.obj" \
	"$(INTDIR)\OXSizeToolBar.obj" \
	"$(INTDIR)\OXSkinnedComboBox.obj" \
	"$(INTDIR)\OXSkinnedSpinCtrl.obj" \
	"$(INTDIR)\OXSkinnedTabCtrl.obj" \
	"$(INTDIR)\OXSkins.obj" \
	"$(INTDIR)\OXSound.obj" \
	"$(INTDIR)\OXSoundCustomizeDlg.obj" \
	"$(INTDIR)\OXSoundEffectManager.obj" \
	"$(INTDIR)\OXSoundWnd.obj" \
	"$(INTDIR)\OXSpinCtrl.obj" \
	"$(INTDIR)\OXSplashWnd.obj" \
	"$(INTDIR)\OXSplashWndDIB.obj" \
	"$(INTDIR)\OXSplitterColRowState.obj" \
	"$(INTDIR)\OXSplitterRect.obj" \
	"$(INTDIR)\OXSplitterWnd.obj" \
	"$(INTDIR)\OXStatic.obj" \
	"$(INTDIR)\OXStaticHyperLink.obj" \
	"$(INTDIR)\OXStaticText.obj" \
	"$(INTDIR)\OXSysInfo.obj" \
	"$(INTDIR)\OXSzMiniDockFrmWnd.obj" \
	"$(INTDIR)\OXTabClientWnd.obj" \
	"$(INTDIR)\OXTabView.obj" \
	"$(INTDIR)\OXTaskbarIcon.obj" \
	"$(INTDIR)\OXTleBar.obj" \
	"$(INTDIR)\OXToolTipCtrl.obj" \
	"$(INTDIR)\OXTrace.obj" \
	"$(INTDIR)\OXTreeCal.obj" \
	"$(INTDIR)\OXTreeCombo.obj" \
	"$(INTDIR)\OXTreeCtrl.obj" \
	"$(INTDIR)\OXTreeEdit.obj" \
	"$(INTDIR)\OXTreeHeader.obj" \
	"$(INTDIR)\OXTreeItem.obj" \
	"$(INTDIR)\OXUNC.obj" \
	"$(INTDIR)\OXUNCStandardActor.obj" \
	"$(INTDIR)\OXURL.obj" \
	"$(INTDIR)\OXUsertl.obj" \
	"$(INTDIR)\OXUUDec.obj" \
	"$(INTDIR)\OXUxTheme.obj" \
	"$(INTDIR)\OXWABook.obj" \
	"$(INTDIR)\OXWatchedDir.obj" \
	"$(INTDIR)\oxwebctrl.obj" \
	"$(INTDIR)\OXWorkspaceState.obj" \
	"$(INTDIR)\Oxzoomvw.obj" \
	"$(INTDIR)\Path.obj" \
	"$(INTDIR)\PATHITER.OBJ" \
	"$(INTDIR)\PROGRESS.OBJ" \
	"$(INTDIR)\WATCHBUF.OBJ" \
	"$(INTDIR)\Xstatus4.obj" \
	"$(INTDIR)\XSTRING.OBJ" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UT.obj" \
	"$(INTDIR)\OXMain.res"

"..\DLLs\UT.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UT - Win32 Unicode Debug"

OUTDIR=.\DebugU
INTDIR=.\DebugU

ALL : "..\DLLs\UTDU.dll"


CLEAN :
	-@erase "$(INTDIR)\CONVFILE.OBJ"
	-@erase "$(INTDIR)\COPYTREE.OBJ"
	-@erase "$(INTDIR)\CPYSTDLG.OBJ"
	-@erase "$(INTDIR)\DIR.OBJ"
	-@erase "$(INTDIR)\DIRLIST.OBJ"
	-@erase "$(INTDIR)\DSTRLIST.OBJ"
	-@erase "$(INTDIR)\EVNTLOG.OBJ"
	-@erase "$(INTDIR)\FILE.OBJ"
	-@erase "$(INTDIR)\FILELIST.OBJ"
	-@erase "$(INTDIR)\MDELDLG.OBJ"
	-@erase "$(INTDIR)\MFCToolBar.obj"
	-@erase "$(INTDIR)\OX3DTabView.obj"
	-@erase "$(INTDIR)\OXActiveDesktop.obj"
	-@erase "$(INTDIR)\OXAdvancedAssert.obj"
	-@erase "$(INTDIR)\OXAdvancedAssertMail.obj"
	-@erase "$(INTDIR)\OXAppBar.obj"
	-@erase "$(INTDIR)\OXAutoComplete.obj"
	-@erase "$(INTDIR)\OXAutoListBox.obj"
	-@erase "$(INTDIR)\OXBackgroundPainter.obj"
	-@erase "$(INTDIR)\OXBITBUF.OBJ"
	-@erase "$(INTDIR)\OXBitmapButton.obj"
	-@erase "$(INTDIR)\OXBitmapMenu.obj"
	-@erase "$(INTDIR)\OXBitmapMenuOrganizer.obj"
	-@erase "$(INTDIR)\OXBMPFilePickerCombo.obj"
	-@erase "$(INTDIR)\OXBMPFle.obj"
	-@erase "$(INTDIR)\OXBrowseColorEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEditList.obj"
	-@erase "$(INTDIR)\OXButtonAppearanceDlg.obj"
	-@erase "$(INTDIR)\OXCalculatorCtrl.obj"
	-@erase "$(INTDIR)\OXCalculatorEdit.obj"
	-@erase "$(INTDIR)\OXCalendar.obj"
	-@erase "$(INTDIR)\OXCalendarEdit.obj"
	-@erase "$(INTDIR)\OXCalendarPopup.obj"
	-@erase "$(INTDIR)\OXCaptionPainter.obj"
	-@erase "$(INTDIR)\OXChildFrameState.obj"
	-@erase "$(INTDIR)\OXClipPocket.obj"
	-@erase "$(INTDIR)\OXCMPFL.OBJ"
	-@erase "$(INTDIR)\OXColorPickerButton.obj"
	-@erase "$(INTDIR)\OXColorPickerCtrl.obj"
	-@erase "$(INTDIR)\OXComboPickerCtrl.obj"
	-@erase "$(INTDIR)\OXCOMPR.OBJ"
	-@erase "$(INTDIR)\OXContextHelpDialog.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertyPage.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertySheet.obj"
	-@erase "$(INTDIR)\OXCoolBar.obj"
	-@erase "$(INTDIR)\OXCoolComboBox.obj"
	-@erase "$(INTDIR)\OXCoolControlOrganizer.obj"
	-@erase "$(INTDIR)\OXCoolCtrl.obj"
	-@erase "$(INTDIR)\OXCoolToolBar.obj"
	-@erase "$(INTDIR)\OXCRCChk.obj"
	-@erase "$(INTDIR)\OXCsvFile.obj"
	-@erase "$(INTDIR)\OXCustomizeBackgroundPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCaptionPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCommandsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCoolControlsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeInstanceManagerPage.obj"
	-@erase "$(INTDIR)\OXCustomizeIntelliMousePage.obj"
	-@erase "$(INTDIR)\OXCustomizeManager.obj"
	-@erase "$(INTDIR)\OXCustomizePage.obj"
	-@erase "$(INTDIR)\OXCustomizeShortkeysPage.obj"
	-@erase "$(INTDIR)\OXCustomizeSplashWindowPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTabbedMDIPage.obj"
	-@erase "$(INTDIR)\OXCustomizeToolbarsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTrayIconPage.obj"
	-@erase "$(INTDIR)\OXCustomizeWorkspaceStatePage.obj"
	-@erase "$(INTDIR)\OXDao.obj"
	-@erase "$(INTDIR)\OXDateTimeCtrl.obj"
	-@erase "$(INTDIR)\OXDckPPg.obj"
	-@erase "$(INTDIR)\OXDckPSh.obj"
	-@erase "$(INTDIR)\OXDFlHdr.obj"
	-@erase "$(INTDIR)\Oxdib.obj"
	-@erase "$(INTDIR)\OXDirDlg.obj"
	-@erase "$(INTDIR)\OXDlgBar.obj"
	-@erase "$(INTDIR)\OXDLLManager.obj"
	-@erase "$(INTDIR)\oxdocktabctrl.obj"
	-@erase "$(INTDIR)\OXDocTemplateSpy.obj"
	-@erase "$(INTDIR)\OXDragDockContext.obj"
	-@erase "$(INTDIR)\OXDragDropCommands.obj"
	-@erase "$(INTDIR)\OXEdit.obj"
	-@erase "$(INTDIR)\OXEditList.obj"
	-@erase "$(INTDIR)\OXFileChanger.obj"
	-@erase "$(INTDIR)\OXFileWatcher.obj"
	-@erase "$(INTDIR)\OXFileWatchNotifier.obj"
	-@erase "$(INTDIR)\OXFolderPickerDlg.obj"
	-@erase "$(INTDIR)\OXFontComboBox.obj"
	-@erase "$(INTDIR)\OXFontPickerButton.obj"
	-@erase "$(INTDIR)\OXFrameWndDock.obj"
	-@erase "$(INTDIR)\OXFullScreenFrame.obj"
	-@erase "$(INTDIR)\OXGphFle.obj"
	-@erase "$(INTDIR)\OXGraphics.obj"
	-@erase "$(INTDIR)\oxgridedit.obj"
	-@erase "$(INTDIR)\OXGridHdr.obj"
	-@erase "$(INTDIR)\OXGridList.obj"
	-@erase "$(INTDIR)\OXHashTable.obj"
	-@erase "$(INTDIR)\OXHistoryCombo.obj"
	-@erase "$(INTDIR)\OXHistoryCtrl.obj"
	-@erase "$(INTDIR)\OXHookWnd.obj"
	-@erase "$(INTDIR)\OXHTMLParser.obj"
	-@erase "$(INTDIR)\OXHyperLinkAction.obj"
	-@erase "$(INTDIR)\OXImageListBox.obj"
	-@erase "$(INTDIR)\OXImageViewer.obj"
	-@erase "$(INTDIR)\OXInstanceManager.obj"
	-@erase "$(INTDIR)\OXInteger.obj"
	-@erase "$(INTDIR)\OXIntelliMouse.obj"
	-@erase "$(INTDIR)\OXIPEdit.obj"
	-@erase "$(INTDIR)\OXItemTip.obj"
	-@erase "$(INTDIR)\OXItemTipWnd.obj"
	-@erase "$(INTDIR)\OXIteratorRegistryItem.obj"
	-@erase "$(INTDIR)\OXIteratorService.obj"
	-@erase "$(INTDIR)\OXIteratorUNC.obj"
	-@erase "$(INTDIR)\OXJPGCod.obj"
	-@erase "$(INTDIR)\OXJPGCom.obj"
	-@erase "$(INTDIR)\OXJPGDom.obj"
	-@erase "$(INTDIR)\OXJPGExp.obj"
	-@erase "$(INTDIR)\OXJPGFle.obj"
	-@erase "$(INTDIR)\OXLayoutManager.obj"
	-@erase "$(INTDIR)\OXListBoxEx.obj"
	-@erase "$(INTDIR)\OXListEdit.obj"
	-@erase "$(INTDIR)\OXMain.res"
	-@erase "$(INTDIR)\OXMaskedEdit.obj"
	-@erase "$(INTDIR)\OXMDIFloatWnd.obj"
	-@erase "$(INTDIR)\OXMenuBar.obj"
	-@erase "$(INTDIR)\OXMetaFile.obj"
	-@erase "$(INTDIR)\OXMonthCalCtrl.obj"
	-@erase "$(INTDIR)\OXMulticlipboardDlg.obj"
	-@erase "$(INTDIR)\OXMultiComboBox.obj"
	-@erase "$(INTDIR)\OXNetBrowseTree.obj"
	-@erase "$(INTDIR)\OXNotesEditView.obj"
	-@erase "$(INTDIR)\OXOptionTreeCtrl.obj"
	-@erase "$(INTDIR)\OXParser.obj"
	-@erase "$(INTDIR)\OXPhysicalEdit.obj"
	-@erase "$(INTDIR)\OXPhysicalEditEx.obj"
	-@erase "$(INTDIR)\OXPopupBarCtrl.obj"
	-@erase "$(INTDIR)\OXPreviewDialog.obj"
	-@erase "$(INTDIR)\OXProcess.obj"
	-@erase "$(INTDIR)\OXPropertiesWnd.obj"
	-@erase "$(INTDIR)\OXPropertyDescriptionPane.obj"
	-@erase "$(INTDIR)\OXPropertyTree.obj"
	-@erase "$(INTDIR)\OXQuickString.obj"
	-@erase "$(INTDIR)\OXRectTracker.obj"
	-@erase "$(INTDIR)\OXRegExpression.obj"
	-@erase "$(INTDIR)\OXRegistryItem.obj"
	-@erase "$(INTDIR)\OXRegistryValFile.obj"
	-@erase "$(INTDIR)\OXRegistryWatcher.obj"
	-@erase "$(INTDIR)\OXRegistryWatchNotifier.obj"
	-@erase "$(INTDIR)\OXResourceFile.obj"
	-@erase "$(INTDIR)\OXResourceLibrary.obj"
	-@erase "$(INTDIR)\OXRollup.obj"
	-@erase "$(INTDIR)\OXRoundedButton.obj"
	-@erase "$(INTDIR)\OXRulerOrganizer.obj"
	-@erase "$(INTDIR)\OXScreenGrab.obj"
	-@erase "$(INTDIR)\OXScript.obj"
	-@erase "$(INTDIR)\OXScrollTip.obj"
	-@erase "$(INTDIR)\OXScrollTipOwner.obj"
	-@erase "$(INTDIR)\OXScrollWnd.obj"
	-@erase "$(INTDIR)\OXSeparator.obj"
	-@erase "$(INTDIR)\OXService.obj"
	-@erase "$(INTDIR)\OXShape.obj"
	-@erase "$(INTDIR)\OXShdWnd.obj"
	-@erase "$(INTDIR)\OXShellFolderTree.obj"
	-@erase "$(INTDIR)\OXShellNamespaceNavigator.obj"
	-@erase "$(INTDIR)\OXShellObjectList.obj"
	-@erase "$(INTDIR)\OXShortcut.obj"
	-@erase "$(INTDIR)\OXShortcutBar.obj"
	-@erase "$(INTDIR)\OXShortkeysOrganizer.obj"
	-@erase "$(INTDIR)\OXSizeCtrlBar.obj"
	-@erase "$(INTDIR)\OXSizeDlgBar.obj"
	-@erase "$(INTDIR)\OXSizeDockBar.obj"
	-@erase "$(INTDIR)\OXSizeToolBar.obj"
	-@erase "$(INTDIR)\OXSkinnedComboBox.obj"
	-@erase "$(INTDIR)\OXSkinnedSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSkinnedTabCtrl.obj"
	-@erase "$(INTDIR)\OXSkins.obj"
	-@erase "$(INTDIR)\OXSound.obj"
	-@erase "$(INTDIR)\OXSoundCustomizeDlg.obj"
	-@erase "$(INTDIR)\OXSoundEffectManager.obj"
	-@erase "$(INTDIR)\OXSoundWnd.obj"
	-@erase "$(INTDIR)\OXSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSplashWnd.obj"
	-@erase "$(INTDIR)\OXSplashWndDIB.obj"
	-@erase "$(INTDIR)\OXSplitterColRowState.obj"
	-@erase "$(INTDIR)\OXSplitterRect.obj"
	-@erase "$(INTDIR)\OXSplitterWnd.obj"
	-@erase "$(INTDIR)\OXStatic.obj"
	-@erase "$(INTDIR)\OXStaticHyperLink.obj"
	-@erase "$(INTDIR)\OXStaticText.obj"
	-@erase "$(INTDIR)\OXSysInfo.obj"
	-@erase "$(INTDIR)\OXSzMiniDockFrmWnd.obj"
	-@erase "$(INTDIR)\OXTabClientWnd.obj"
	-@erase "$(INTDIR)\OXTabView.obj"
	-@erase "$(INTDIR)\OXTaskbarIcon.obj"
	-@erase "$(INTDIR)\OXTleBar.obj"
	-@erase "$(INTDIR)\OXToolTipCtrl.obj"
	-@erase "$(INTDIR)\OXTrace.obj"
	-@erase "$(INTDIR)\OXTreeCal.obj"
	-@erase "$(INTDIR)\OXTreeCombo.obj"
	-@erase "$(INTDIR)\OXTreeCtrl.obj"
	-@erase "$(INTDIR)\OXTreeEdit.obj"
	-@erase "$(INTDIR)\OXTreeHeader.obj"
	-@erase "$(INTDIR)\OXTreeItem.obj"
	-@erase "$(INTDIR)\OXUNC.obj"
	-@erase "$(INTDIR)\OXUNCStandardActor.obj"
	-@erase "$(INTDIR)\OXURL.obj"
	-@erase "$(INTDIR)\OXUsertl.obj"
	-@erase "$(INTDIR)\OXUUDec.obj"
	-@erase "$(INTDIR)\OXUxTheme.obj"
	-@erase "$(INTDIR)\OXWABook.obj"
	-@erase "$(INTDIR)\OXWatchedDir.obj"
	-@erase "$(INTDIR)\oxwebctrl.obj"
	-@erase "$(INTDIR)\OXWorkspaceState.obj"
	-@erase "$(INTDIR)\Oxzoomvw.obj"
	-@erase "$(INTDIR)\Path.obj"
	-@erase "$(INTDIR)\PATHITER.OBJ"
	-@erase "$(INTDIR)\PROGRESS.OBJ"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\UT.obj"
	-@erase "$(INTDIR)\UT.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "$(OUTDIR)\UTDU.pdb"
	-@erase "..\DLLs\UTDU.dll"
	-@erase "..\DLLs\UTDU.exp"
	-@erase "..\DLLs\UTDU.ilk"
	-@erase "..\DLLs\UTDU.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /Fp"$(INTDIR)\UT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x1009 /fo"$(INTDIR)\OXMain.res" /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=..\..\Lib\libs\jpgd.lib ..\..\Lib\libs\cpuinfd.lib version.lib mapi32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\UTDU.pdb" /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\DLLs/UTDU.dll" /implib:"..\DLLs/UTDU.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\CONVFILE.OBJ" \
	"$(INTDIR)\COPYTREE.OBJ" \
	"$(INTDIR)\CPYSTDLG.OBJ" \
	"$(INTDIR)\DIR.OBJ" \
	"$(INTDIR)\DIRLIST.OBJ" \
	"$(INTDIR)\DSTRLIST.OBJ" \
	"$(INTDIR)\EVNTLOG.OBJ" \
	"$(INTDIR)\FILE.OBJ" \
	"$(INTDIR)\FILELIST.OBJ" \
	"$(INTDIR)\MDELDLG.OBJ" \
	"$(INTDIR)\MFCToolBar.obj" \
	"$(INTDIR)\OX3DTabView.obj" \
	"$(INTDIR)\OXActiveDesktop.obj" \
	"$(INTDIR)\OXAdvancedAssert.obj" \
	"$(INTDIR)\OXAdvancedAssertMail.obj" \
	"$(INTDIR)\OXAppBar.obj" \
	"$(INTDIR)\OXAutoComplete.obj" \
	"$(INTDIR)\OXAutoListBox.obj" \
	"$(INTDIR)\OXBackgroundPainter.obj" \
	"$(INTDIR)\OXBITBUF.OBJ" \
	"$(INTDIR)\OXBitmapButton.obj" \
	"$(INTDIR)\OXBitmapMenu.obj" \
	"$(INTDIR)\OXBitmapMenuOrganizer.obj" \
	"$(INTDIR)\OXBMPFilePickerCombo.obj" \
	"$(INTDIR)\OXBMPFle.obj" \
	"$(INTDIR)\OXBrowseColorEdit.obj" \
	"$(INTDIR)\OXBrowseDirEdit.obj" \
	"$(INTDIR)\OXBrowseDirEditList.obj" \
	"$(INTDIR)\OXButtonAppearanceDlg.obj" \
	"$(INTDIR)\OXCalculatorCtrl.obj" \
	"$(INTDIR)\OXCalculatorEdit.obj" \
	"$(INTDIR)\OXCalendar.obj" \
	"$(INTDIR)\OXCalendarEdit.obj" \
	"$(INTDIR)\OXCalendarPopup.obj" \
	"$(INTDIR)\OXCaptionPainter.obj" \
	"$(INTDIR)\OXChildFrameState.obj" \
	"$(INTDIR)\OXClipPocket.obj" \
	"$(INTDIR)\OXCMPFL.OBJ" \
	"$(INTDIR)\OXColorPickerButton.obj" \
	"$(INTDIR)\OXColorPickerCtrl.obj" \
	"$(INTDIR)\OXComboPickerCtrl.obj" \
	"$(INTDIR)\OXCOMPR.OBJ" \
	"$(INTDIR)\OXContextHelpDialog.obj" \
	"$(INTDIR)\OXContextHelpPropertyPage.obj" \
	"$(INTDIR)\OXContextHelpPropertySheet.obj" \
	"$(INTDIR)\OXCoolBar.obj" \
	"$(INTDIR)\OXCoolComboBox.obj" \
	"$(INTDIR)\OXCoolControlOrganizer.obj" \
	"$(INTDIR)\OXCoolCtrl.obj" \
	"$(INTDIR)\OXCoolToolBar.obj" \
	"$(INTDIR)\OXCRCChk.obj" \
	"$(INTDIR)\OXCsvFile.obj" \
	"$(INTDIR)\OXCustomizeBackgroundPage.obj" \
	"$(INTDIR)\OXCustomizeCaptionPage.obj" \
	"$(INTDIR)\OXCustomizeCommandsPage.obj" \
	"$(INTDIR)\OXCustomizeCoolControlsPage.obj" \
	"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" \
	"$(INTDIR)\OXCustomizeIntelliMousePage.obj" \
	"$(INTDIR)\OXCustomizeManager.obj" \
	"$(INTDIR)\OXCustomizePage.obj" \
	"$(INTDIR)\OXCustomizeShortkeysPage.obj" \
	"$(INTDIR)\OXCustomizeSplashWindowPage.obj" \
	"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" \
	"$(INTDIR)\OXCustomizeToolbarsPage.obj" \
	"$(INTDIR)\OXCustomizeTrayIconPage.obj" \
	"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" \
	"$(INTDIR)\OXDao.obj" \
	"$(INTDIR)\OXDateTimeCtrl.obj" \
	"$(INTDIR)\OXDckPPg.obj" \
	"$(INTDIR)\OXDckPSh.obj" \
	"$(INTDIR)\OXDFlHdr.obj" \
	"$(INTDIR)\Oxdib.obj" \
	"$(INTDIR)\OXDirDlg.obj" \
	"$(INTDIR)\OXDlgBar.obj" \
	"$(INTDIR)\OXDLLManager.obj" \
	"$(INTDIR)\oxdocktabctrl.obj" \
	"$(INTDIR)\OXDocTemplateSpy.obj" \
	"$(INTDIR)\OXDragDockContext.obj" \
	"$(INTDIR)\OXDragDropCommands.obj" \
	"$(INTDIR)\OXEdit.obj" \
	"$(INTDIR)\OXEditList.obj" \
	"$(INTDIR)\OXFileChanger.obj" \
	"$(INTDIR)\OXFileWatcher.obj" \
	"$(INTDIR)\OXFileWatchNotifier.obj" \
	"$(INTDIR)\OXFolderPickerDlg.obj" \
	"$(INTDIR)\OXFontComboBox.obj" \
	"$(INTDIR)\OXFontPickerButton.obj" \
	"$(INTDIR)\OXFrameWndDock.obj" \
	"$(INTDIR)\OXFullScreenFrame.obj" \
	"$(INTDIR)\OXGphFle.obj" \
	"$(INTDIR)\OXGraphics.obj" \
	"$(INTDIR)\oxgridedit.obj" \
	"$(INTDIR)\OXGridHdr.obj" \
	"$(INTDIR)\OXGridList.obj" \
	"$(INTDIR)\OXHashTable.obj" \
	"$(INTDIR)\OXHistoryCombo.obj" \
	"$(INTDIR)\OXHistoryCtrl.obj" \
	"$(INTDIR)\OXHookWnd.obj" \
	"$(INTDIR)\OXHTMLParser.obj" \
	"$(INTDIR)\OXHyperLinkAction.obj" \
	"$(INTDIR)\OXImageListBox.obj" \
	"$(INTDIR)\OXImageViewer.obj" \
	"$(INTDIR)\OXInstanceManager.obj" \
	"$(INTDIR)\OXInteger.obj" \
	"$(INTDIR)\OXIntelliMouse.obj" \
	"$(INTDIR)\OXIPEdit.obj" \
	"$(INTDIR)\OXItemTip.obj" \
	"$(INTDIR)\OXItemTipWnd.obj" \
	"$(INTDIR)\OXIteratorRegistryItem.obj" \
	"$(INTDIR)\OXIteratorService.obj" \
	"$(INTDIR)\OXIteratorUNC.obj" \
	"$(INTDIR)\OXJPGCod.obj" \
	"$(INTDIR)\OXJPGCom.obj" \
	"$(INTDIR)\OXJPGDom.obj" \
	"$(INTDIR)\OXJPGExp.obj" \
	"$(INTDIR)\OXJPGFle.obj" \
	"$(INTDIR)\OXLayoutManager.obj" \
	"$(INTDIR)\OXListBoxEx.obj" \
	"$(INTDIR)\OXListEdit.obj" \
	"$(INTDIR)\OXMaskedEdit.obj" \
	"$(INTDIR)\OXMDIFloatWnd.obj" \
	"$(INTDIR)\OXMenuBar.obj" \
	"$(INTDIR)\OXMetaFile.obj" \
	"$(INTDIR)\OXMonthCalCtrl.obj" \
	"$(INTDIR)\OXMulticlipboardDlg.obj" \
	"$(INTDIR)\OXMultiComboBox.obj" \
	"$(INTDIR)\OXNetBrowseTree.obj" \
	"$(INTDIR)\OXNotesEditView.obj" \
	"$(INTDIR)\OXOptionTreeCtrl.obj" \
	"$(INTDIR)\OXParser.obj" \
	"$(INTDIR)\OXPhysicalEdit.obj" \
	"$(INTDIR)\OXPhysicalEditEx.obj" \
	"$(INTDIR)\OXPopupBarCtrl.obj" \
	"$(INTDIR)\OXPreviewDialog.obj" \
	"$(INTDIR)\OXProcess.obj" \
	"$(INTDIR)\OXPropertiesWnd.obj" \
	"$(INTDIR)\OXPropertyDescriptionPane.obj" \
	"$(INTDIR)\OXPropertyTree.obj" \
	"$(INTDIR)\OXQuickString.obj" \
	"$(INTDIR)\OXRectTracker.obj" \
	"$(INTDIR)\OXRegExpression.obj" \
	"$(INTDIR)\OXRegistryItem.obj" \
	"$(INTDIR)\OXRegistryValFile.obj" \
	"$(INTDIR)\OXRegistryWatcher.obj" \
	"$(INTDIR)\OXRegistryWatchNotifier.obj" \
	"$(INTDIR)\OXResourceFile.obj" \
	"$(INTDIR)\OXResourceLibrary.obj" \
	"$(INTDIR)\OXRollup.obj" \
	"$(INTDIR)\OXRoundedButton.obj" \
	"$(INTDIR)\OXRulerOrganizer.obj" \
	"$(INTDIR)\OXScreenGrab.obj" \
	"$(INTDIR)\OXScript.obj" \
	"$(INTDIR)\OXScrollTip.obj" \
	"$(INTDIR)\OXScrollTipOwner.obj" \
	"$(INTDIR)\OXScrollWnd.obj" \
	"$(INTDIR)\OXSeparator.obj" \
	"$(INTDIR)\OXService.obj" \
	"$(INTDIR)\OXShape.obj" \
	"$(INTDIR)\OXShdWnd.obj" \
	"$(INTDIR)\OXShellFolderTree.obj" \
	"$(INTDIR)\OXShellNamespaceNavigator.obj" \
	"$(INTDIR)\OXShellObjectList.obj" \
	"$(INTDIR)\OXShortcut.obj" \
	"$(INTDIR)\OXShortcutBar.obj" \
	"$(INTDIR)\OXShortkeysOrganizer.obj" \
	"$(INTDIR)\OXSizeCtrlBar.obj" \
	"$(INTDIR)\OXSizeDlgBar.obj" \
	"$(INTDIR)\OXSizeDockBar.obj" \
	"$(INTDIR)\OXSizeToolBar.obj" \
	"$(INTDIR)\OXSkinnedComboBox.obj" \
	"$(INTDIR)\OXSkinnedSpinCtrl.obj" \
	"$(INTDIR)\OXSkinnedTabCtrl.obj" \
	"$(INTDIR)\OXSkins.obj" \
	"$(INTDIR)\OXSound.obj" \
	"$(INTDIR)\OXSoundCustomizeDlg.obj" \
	"$(INTDIR)\OXSoundEffectManager.obj" \
	"$(INTDIR)\OXSoundWnd.obj" \
	"$(INTDIR)\OXSpinCtrl.obj" \
	"$(INTDIR)\OXSplashWnd.obj" \
	"$(INTDIR)\OXSplashWndDIB.obj" \
	"$(INTDIR)\OXSplitterColRowState.obj" \
	"$(INTDIR)\OXSplitterRect.obj" \
	"$(INTDIR)\OXSplitterWnd.obj" \
	"$(INTDIR)\OXStatic.obj" \
	"$(INTDIR)\OXStaticHyperLink.obj" \
	"$(INTDIR)\OXStaticText.obj" \
	"$(INTDIR)\OXSysInfo.obj" \
	"$(INTDIR)\OXSzMiniDockFrmWnd.obj" \
	"$(INTDIR)\OXTabClientWnd.obj" \
	"$(INTDIR)\OXTabView.obj" \
	"$(INTDIR)\OXTaskbarIcon.obj" \
	"$(INTDIR)\OXTleBar.obj" \
	"$(INTDIR)\OXToolTipCtrl.obj" \
	"$(INTDIR)\OXTrace.obj" \
	"$(INTDIR)\OXTreeCal.obj" \
	"$(INTDIR)\OXTreeCombo.obj" \
	"$(INTDIR)\OXTreeCtrl.obj" \
	"$(INTDIR)\OXTreeEdit.obj" \
	"$(INTDIR)\OXTreeHeader.obj" \
	"$(INTDIR)\OXTreeItem.obj" \
	"$(INTDIR)\OXUNC.obj" \
	"$(INTDIR)\OXUNCStandardActor.obj" \
	"$(INTDIR)\OXURL.obj" \
	"$(INTDIR)\OXUsertl.obj" \
	"$(INTDIR)\OXUUDec.obj" \
	"$(INTDIR)\OXUxTheme.obj" \
	"$(INTDIR)\OXWABook.obj" \
	"$(INTDIR)\OXWatchedDir.obj" \
	"$(INTDIR)\oxwebctrl.obj" \
	"$(INTDIR)\OXWorkspaceState.obj" \
	"$(INTDIR)\Oxzoomvw.obj" \
	"$(INTDIR)\Path.obj" \
	"$(INTDIR)\PATHITER.OBJ" \
	"$(INTDIR)\PROGRESS.OBJ" \
	"$(INTDIR)\WATCHBUF.OBJ" \
	"$(INTDIR)\Xstatus4.obj" \
	"$(INTDIR)\XSTRING.OBJ" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UT.obj" \
	"$(INTDIR)\OXMain.res"

"..\DLLs\UTDU.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UT - Win32 Unicode Release"

OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU

ALL : "..\DLLs\UTU.dll"


CLEAN :
	-@erase "$(INTDIR)\CONVFILE.OBJ"
	-@erase "$(INTDIR)\COPYTREE.OBJ"
	-@erase "$(INTDIR)\CPYSTDLG.OBJ"
	-@erase "$(INTDIR)\DIR.OBJ"
	-@erase "$(INTDIR)\DIRLIST.OBJ"
	-@erase "$(INTDIR)\DSTRLIST.OBJ"
	-@erase "$(INTDIR)\EVNTLOG.OBJ"
	-@erase "$(INTDIR)\FILE.OBJ"
	-@erase "$(INTDIR)\FILELIST.OBJ"
	-@erase "$(INTDIR)\MDELDLG.OBJ"
	-@erase "$(INTDIR)\MFCToolBar.obj"
	-@erase "$(INTDIR)\OX3DTabView.obj"
	-@erase "$(INTDIR)\OXActiveDesktop.obj"
	-@erase "$(INTDIR)\OXAdvancedAssert.obj"
	-@erase "$(INTDIR)\OXAdvancedAssertMail.obj"
	-@erase "$(INTDIR)\OXAppBar.obj"
	-@erase "$(INTDIR)\OXAutoComplete.obj"
	-@erase "$(INTDIR)\OXAutoListBox.obj"
	-@erase "$(INTDIR)\OXBackgroundPainter.obj"
	-@erase "$(INTDIR)\OXBITBUF.OBJ"
	-@erase "$(INTDIR)\OXBitmapButton.obj"
	-@erase "$(INTDIR)\OXBitmapMenu.obj"
	-@erase "$(INTDIR)\OXBitmapMenuOrganizer.obj"
	-@erase "$(INTDIR)\OXBMPFilePickerCombo.obj"
	-@erase "$(INTDIR)\OXBMPFle.obj"
	-@erase "$(INTDIR)\OXBrowseColorEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEditList.obj"
	-@erase "$(INTDIR)\OXButtonAppearanceDlg.obj"
	-@erase "$(INTDIR)\OXCalculatorCtrl.obj"
	-@erase "$(INTDIR)\OXCalculatorEdit.obj"
	-@erase "$(INTDIR)\OXCalendar.obj"
	-@erase "$(INTDIR)\OXCalendarEdit.obj"
	-@erase "$(INTDIR)\OXCalendarPopup.obj"
	-@erase "$(INTDIR)\OXCaptionPainter.obj"
	-@erase "$(INTDIR)\OXChildFrameState.obj"
	-@erase "$(INTDIR)\OXClipPocket.obj"
	-@erase "$(INTDIR)\OXCMPFL.OBJ"
	-@erase "$(INTDIR)\OXColorPickerButton.obj"
	-@erase "$(INTDIR)\OXColorPickerCtrl.obj"
	-@erase "$(INTDIR)\OXComboPickerCtrl.obj"
	-@erase "$(INTDIR)\OXCOMPR.OBJ"
	-@erase "$(INTDIR)\OXContextHelpDialog.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertyPage.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertySheet.obj"
	-@erase "$(INTDIR)\OXCoolBar.obj"
	-@erase "$(INTDIR)\OXCoolComboBox.obj"
	-@erase "$(INTDIR)\OXCoolControlOrganizer.obj"
	-@erase "$(INTDIR)\OXCoolCtrl.obj"
	-@erase "$(INTDIR)\OXCoolToolBar.obj"
	-@erase "$(INTDIR)\OXCRCChk.obj"
	-@erase "$(INTDIR)\OXCsvFile.obj"
	-@erase "$(INTDIR)\OXCustomizeBackgroundPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCaptionPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCommandsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCoolControlsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeInstanceManagerPage.obj"
	-@erase "$(INTDIR)\OXCustomizeIntelliMousePage.obj"
	-@erase "$(INTDIR)\OXCustomizeManager.obj"
	-@erase "$(INTDIR)\OXCustomizePage.obj"
	-@erase "$(INTDIR)\OXCustomizeShortkeysPage.obj"
	-@erase "$(INTDIR)\OXCustomizeSplashWindowPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTabbedMDIPage.obj"
	-@erase "$(INTDIR)\OXCustomizeToolbarsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTrayIconPage.obj"
	-@erase "$(INTDIR)\OXCustomizeWorkspaceStatePage.obj"
	-@erase "$(INTDIR)\OXDao.obj"
	-@erase "$(INTDIR)\OXDateTimeCtrl.obj"
	-@erase "$(INTDIR)\OXDckPPg.obj"
	-@erase "$(INTDIR)\OXDckPSh.obj"
	-@erase "$(INTDIR)\OXDFlHdr.obj"
	-@erase "$(INTDIR)\Oxdib.obj"
	-@erase "$(INTDIR)\OXDirDlg.obj"
	-@erase "$(INTDIR)\OXDlgBar.obj"
	-@erase "$(INTDIR)\OXDLLManager.obj"
	-@erase "$(INTDIR)\oxdocktabctrl.obj"
	-@erase "$(INTDIR)\OXDocTemplateSpy.obj"
	-@erase "$(INTDIR)\OXDragDockContext.obj"
	-@erase "$(INTDIR)\OXDragDropCommands.obj"
	-@erase "$(INTDIR)\OXEdit.obj"
	-@erase "$(INTDIR)\OXEditList.obj"
	-@erase "$(INTDIR)\OXFileChanger.obj"
	-@erase "$(INTDIR)\OXFileWatcher.obj"
	-@erase "$(INTDIR)\OXFileWatchNotifier.obj"
	-@erase "$(INTDIR)\OXFolderPickerDlg.obj"
	-@erase "$(INTDIR)\OXFontComboBox.obj"
	-@erase "$(INTDIR)\OXFontPickerButton.obj"
	-@erase "$(INTDIR)\OXFrameWndDock.obj"
	-@erase "$(INTDIR)\OXFullScreenFrame.obj"
	-@erase "$(INTDIR)\OXGphFle.obj"
	-@erase "$(INTDIR)\OXGraphics.obj"
	-@erase "$(INTDIR)\oxgridedit.obj"
	-@erase "$(INTDIR)\OXGridHdr.obj"
	-@erase "$(INTDIR)\OXGridList.obj"
	-@erase "$(INTDIR)\OXHashTable.obj"
	-@erase "$(INTDIR)\OXHistoryCombo.obj"
	-@erase "$(INTDIR)\OXHistoryCtrl.obj"
	-@erase "$(INTDIR)\OXHookWnd.obj"
	-@erase "$(INTDIR)\OXHTMLParser.obj"
	-@erase "$(INTDIR)\OXHyperLinkAction.obj"
	-@erase "$(INTDIR)\OXImageListBox.obj"
	-@erase "$(INTDIR)\OXImageViewer.obj"
	-@erase "$(INTDIR)\OXInstanceManager.obj"
	-@erase "$(INTDIR)\OXInteger.obj"
	-@erase "$(INTDIR)\OXIntelliMouse.obj"
	-@erase "$(INTDIR)\OXIPEdit.obj"
	-@erase "$(INTDIR)\OXItemTip.obj"
	-@erase "$(INTDIR)\OXItemTipWnd.obj"
	-@erase "$(INTDIR)\OXIteratorRegistryItem.obj"
	-@erase "$(INTDIR)\OXIteratorService.obj"
	-@erase "$(INTDIR)\OXIteratorUNC.obj"
	-@erase "$(INTDIR)\OXJPGCod.obj"
	-@erase "$(INTDIR)\OXJPGCom.obj"
	-@erase "$(INTDIR)\OXJPGDom.obj"
	-@erase "$(INTDIR)\OXJPGExp.obj"
	-@erase "$(INTDIR)\OXJPGFle.obj"
	-@erase "$(INTDIR)\OXLayoutManager.obj"
	-@erase "$(INTDIR)\OXListBoxEx.obj"
	-@erase "$(INTDIR)\OXListEdit.obj"
	-@erase "$(INTDIR)\OXMain.res"
	-@erase "$(INTDIR)\OXMaskedEdit.obj"
	-@erase "$(INTDIR)\OXMDIFloatWnd.obj"
	-@erase "$(INTDIR)\OXMenuBar.obj"
	-@erase "$(INTDIR)\OXMetaFile.obj"
	-@erase "$(INTDIR)\OXMonthCalCtrl.obj"
	-@erase "$(INTDIR)\OXMulticlipboardDlg.obj"
	-@erase "$(INTDIR)\OXMultiComboBox.obj"
	-@erase "$(INTDIR)\OXNetBrowseTree.obj"
	-@erase "$(INTDIR)\OXNotesEditView.obj"
	-@erase "$(INTDIR)\OXOptionTreeCtrl.obj"
	-@erase "$(INTDIR)\OXParser.obj"
	-@erase "$(INTDIR)\OXPhysicalEdit.obj"
	-@erase "$(INTDIR)\OXPhysicalEditEx.obj"
	-@erase "$(INTDIR)\OXPopupBarCtrl.obj"
	-@erase "$(INTDIR)\OXPreviewDialog.obj"
	-@erase "$(INTDIR)\OXProcess.obj"
	-@erase "$(INTDIR)\OXPropertiesWnd.obj"
	-@erase "$(INTDIR)\OXPropertyDescriptionPane.obj"
	-@erase "$(INTDIR)\OXPropertyTree.obj"
	-@erase "$(INTDIR)\OXQuickString.obj"
	-@erase "$(INTDIR)\OXRectTracker.obj"
	-@erase "$(INTDIR)\OXRegExpression.obj"
	-@erase "$(INTDIR)\OXRegistryItem.obj"
	-@erase "$(INTDIR)\OXRegistryValFile.obj"
	-@erase "$(INTDIR)\OXRegistryWatcher.obj"
	-@erase "$(INTDIR)\OXRegistryWatchNotifier.obj"
	-@erase "$(INTDIR)\OXResourceFile.obj"
	-@erase "$(INTDIR)\OXResourceLibrary.obj"
	-@erase "$(INTDIR)\OXRollup.obj"
	-@erase "$(INTDIR)\OXRoundedButton.obj"
	-@erase "$(INTDIR)\OXRulerOrganizer.obj"
	-@erase "$(INTDIR)\OXScreenGrab.obj"
	-@erase "$(INTDIR)\OXScript.obj"
	-@erase "$(INTDIR)\OXScrollTip.obj"
	-@erase "$(INTDIR)\OXScrollTipOwner.obj"
	-@erase "$(INTDIR)\OXScrollWnd.obj"
	-@erase "$(INTDIR)\OXSeparator.obj"
	-@erase "$(INTDIR)\OXService.obj"
	-@erase "$(INTDIR)\OXShape.obj"
	-@erase "$(INTDIR)\OXShdWnd.obj"
	-@erase "$(INTDIR)\OXShellFolderTree.obj"
	-@erase "$(INTDIR)\OXShellNamespaceNavigator.obj"
	-@erase "$(INTDIR)\OXShellObjectList.obj"
	-@erase "$(INTDIR)\OXShortcut.obj"
	-@erase "$(INTDIR)\OXShortcutBar.obj"
	-@erase "$(INTDIR)\OXShortkeysOrganizer.obj"
	-@erase "$(INTDIR)\OXSizeCtrlBar.obj"
	-@erase "$(INTDIR)\OXSizeDlgBar.obj"
	-@erase "$(INTDIR)\OXSizeDockBar.obj"
	-@erase "$(INTDIR)\OXSizeToolBar.obj"
	-@erase "$(INTDIR)\OXSkinnedComboBox.obj"
	-@erase "$(INTDIR)\OXSkinnedSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSkinnedTabCtrl.obj"
	-@erase "$(INTDIR)\OXSkins.obj"
	-@erase "$(INTDIR)\OXSound.obj"
	-@erase "$(INTDIR)\OXSoundCustomizeDlg.obj"
	-@erase "$(INTDIR)\OXSoundEffectManager.obj"
	-@erase "$(INTDIR)\OXSoundWnd.obj"
	-@erase "$(INTDIR)\OXSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSplashWnd.obj"
	-@erase "$(INTDIR)\OXSplashWndDIB.obj"
	-@erase "$(INTDIR)\OXSplitterColRowState.obj"
	-@erase "$(INTDIR)\OXSplitterRect.obj"
	-@erase "$(INTDIR)\OXSplitterWnd.obj"
	-@erase "$(INTDIR)\OXStatic.obj"
	-@erase "$(INTDIR)\OXStaticHyperLink.obj"
	-@erase "$(INTDIR)\OXStaticText.obj"
	-@erase "$(INTDIR)\OXSysInfo.obj"
	-@erase "$(INTDIR)\OXSzMiniDockFrmWnd.obj"
	-@erase "$(INTDIR)\OXTabClientWnd.obj"
	-@erase "$(INTDIR)\OXTabView.obj"
	-@erase "$(INTDIR)\OXTaskbarIcon.obj"
	-@erase "$(INTDIR)\OXTleBar.obj"
	-@erase "$(INTDIR)\OXToolTipCtrl.obj"
	-@erase "$(INTDIR)\OXTrace.obj"
	-@erase "$(INTDIR)\OXTreeCal.obj"
	-@erase "$(INTDIR)\OXTreeCombo.obj"
	-@erase "$(INTDIR)\OXTreeCtrl.obj"
	-@erase "$(INTDIR)\OXTreeEdit.obj"
	-@erase "$(INTDIR)\OXTreeHeader.obj"
	-@erase "$(INTDIR)\OXTreeItem.obj"
	-@erase "$(INTDIR)\OXUNC.obj"
	-@erase "$(INTDIR)\OXUNCStandardActor.obj"
	-@erase "$(INTDIR)\OXURL.obj"
	-@erase "$(INTDIR)\OXUsertl.obj"
	-@erase "$(INTDIR)\OXUUDec.obj"
	-@erase "$(INTDIR)\OXUxTheme.obj"
	-@erase "$(INTDIR)\OXWABook.obj"
	-@erase "$(INTDIR)\OXWatchedDir.obj"
	-@erase "$(INTDIR)\oxwebctrl.obj"
	-@erase "$(INTDIR)\OXWorkspaceState.obj"
	-@erase "$(INTDIR)\Oxzoomvw.obj"
	-@erase "$(INTDIR)\Path.obj"
	-@erase "$(INTDIR)\PATHITER.OBJ"
	-@erase "$(INTDIR)\PROGRESS.OBJ"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\UT.obj"
	-@erase "$(INTDIR)\UT.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\DLLs\UTU.dll"
	-@erase "..\DLLs\UTU.exp"
	-@erase "..\DLLs\UTU.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /Fp"$(INTDIR)\UT.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC_PROJ=/l 0x1009 /fo"$(INTDIR)\OXMain.res" /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=..\..\Lib\libs\jpgr.lib ..\..\Lib\libs\cpuinfr.lib version.lib mapi32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\UTU.pdb" /machine:I386 /nodefaultlib:"LIBC" /out:"..\DLLs/UTU.dll" /implib:"..\DLLs/UTU.lib" 
LINK32_OBJS= \
	"$(INTDIR)\CONVFILE.OBJ" \
	"$(INTDIR)\COPYTREE.OBJ" \
	"$(INTDIR)\CPYSTDLG.OBJ" \
	"$(INTDIR)\DIR.OBJ" \
	"$(INTDIR)\DIRLIST.OBJ" \
	"$(INTDIR)\DSTRLIST.OBJ" \
	"$(INTDIR)\EVNTLOG.OBJ" \
	"$(INTDIR)\FILE.OBJ" \
	"$(INTDIR)\FILELIST.OBJ" \
	"$(INTDIR)\MDELDLG.OBJ" \
	"$(INTDIR)\MFCToolBar.obj" \
	"$(INTDIR)\OX3DTabView.obj" \
	"$(INTDIR)\OXActiveDesktop.obj" \
	"$(INTDIR)\OXAdvancedAssert.obj" \
	"$(INTDIR)\OXAdvancedAssertMail.obj" \
	"$(INTDIR)\OXAppBar.obj" \
	"$(INTDIR)\OXAutoComplete.obj" \
	"$(INTDIR)\OXAutoListBox.obj" \
	"$(INTDIR)\OXBackgroundPainter.obj" \
	"$(INTDIR)\OXBITBUF.OBJ" \
	"$(INTDIR)\OXBitmapButton.obj" \
	"$(INTDIR)\OXBitmapMenu.obj" \
	"$(INTDIR)\OXBitmapMenuOrganizer.obj" \
	"$(INTDIR)\OXBMPFilePickerCombo.obj" \
	"$(INTDIR)\OXBMPFle.obj" \
	"$(INTDIR)\OXBrowseColorEdit.obj" \
	"$(INTDIR)\OXBrowseDirEdit.obj" \
	"$(INTDIR)\OXBrowseDirEditList.obj" \
	"$(INTDIR)\OXButtonAppearanceDlg.obj" \
	"$(INTDIR)\OXCalculatorCtrl.obj" \
	"$(INTDIR)\OXCalculatorEdit.obj" \
	"$(INTDIR)\OXCalendar.obj" \
	"$(INTDIR)\OXCalendarEdit.obj" \
	"$(INTDIR)\OXCalendarPopup.obj" \
	"$(INTDIR)\OXCaptionPainter.obj" \
	"$(INTDIR)\OXChildFrameState.obj" \
	"$(INTDIR)\OXClipPocket.obj" \
	"$(INTDIR)\OXCMPFL.OBJ" \
	"$(INTDIR)\OXColorPickerButton.obj" \
	"$(INTDIR)\OXColorPickerCtrl.obj" \
	"$(INTDIR)\OXComboPickerCtrl.obj" \
	"$(INTDIR)\OXCOMPR.OBJ" \
	"$(INTDIR)\OXContextHelpDialog.obj" \
	"$(INTDIR)\OXContextHelpPropertyPage.obj" \
	"$(INTDIR)\OXContextHelpPropertySheet.obj" \
	"$(INTDIR)\OXCoolBar.obj" \
	"$(INTDIR)\OXCoolComboBox.obj" \
	"$(INTDIR)\OXCoolControlOrganizer.obj" \
	"$(INTDIR)\OXCoolCtrl.obj" \
	"$(INTDIR)\OXCoolToolBar.obj" \
	"$(INTDIR)\OXCRCChk.obj" \
	"$(INTDIR)\OXCsvFile.obj" \
	"$(INTDIR)\OXCustomizeBackgroundPage.obj" \
	"$(INTDIR)\OXCustomizeCaptionPage.obj" \
	"$(INTDIR)\OXCustomizeCommandsPage.obj" \
	"$(INTDIR)\OXCustomizeCoolControlsPage.obj" \
	"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" \
	"$(INTDIR)\OXCustomizeIntelliMousePage.obj" \
	"$(INTDIR)\OXCustomizeManager.obj" \
	"$(INTDIR)\OXCustomizePage.obj" \
	"$(INTDIR)\OXCustomizeShortkeysPage.obj" \
	"$(INTDIR)\OXCustomizeSplashWindowPage.obj" \
	"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" \
	"$(INTDIR)\OXCustomizeToolbarsPage.obj" \
	"$(INTDIR)\OXCustomizeTrayIconPage.obj" \
	"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" \
	"$(INTDIR)\OXDao.obj" \
	"$(INTDIR)\OXDateTimeCtrl.obj" \
	"$(INTDIR)\OXDckPPg.obj" \
	"$(INTDIR)\OXDckPSh.obj" \
	"$(INTDIR)\OXDFlHdr.obj" \
	"$(INTDIR)\Oxdib.obj" \
	"$(INTDIR)\OXDirDlg.obj" \
	"$(INTDIR)\OXDlgBar.obj" \
	"$(INTDIR)\OXDLLManager.obj" \
	"$(INTDIR)\oxdocktabctrl.obj" \
	"$(INTDIR)\OXDocTemplateSpy.obj" \
	"$(INTDIR)\OXDragDockContext.obj" \
	"$(INTDIR)\OXDragDropCommands.obj" \
	"$(INTDIR)\OXEdit.obj" \
	"$(INTDIR)\OXEditList.obj" \
	"$(INTDIR)\OXFileChanger.obj" \
	"$(INTDIR)\OXFileWatcher.obj" \
	"$(INTDIR)\OXFileWatchNotifier.obj" \
	"$(INTDIR)\OXFolderPickerDlg.obj" \
	"$(INTDIR)\OXFontComboBox.obj" \
	"$(INTDIR)\OXFontPickerButton.obj" \
	"$(INTDIR)\OXFrameWndDock.obj" \
	"$(INTDIR)\OXFullScreenFrame.obj" \
	"$(INTDIR)\OXGphFle.obj" \
	"$(INTDIR)\OXGraphics.obj" \
	"$(INTDIR)\oxgridedit.obj" \
	"$(INTDIR)\OXGridHdr.obj" \
	"$(INTDIR)\OXGridList.obj" \
	"$(INTDIR)\OXHashTable.obj" \
	"$(INTDIR)\OXHistoryCombo.obj" \
	"$(INTDIR)\OXHistoryCtrl.obj" \
	"$(INTDIR)\OXHookWnd.obj" \
	"$(INTDIR)\OXHTMLParser.obj" \
	"$(INTDIR)\OXHyperLinkAction.obj" \
	"$(INTDIR)\OXImageListBox.obj" \
	"$(INTDIR)\OXImageViewer.obj" \
	"$(INTDIR)\OXInstanceManager.obj" \
	"$(INTDIR)\OXInteger.obj" \
	"$(INTDIR)\OXIntelliMouse.obj" \
	"$(INTDIR)\OXIPEdit.obj" \
	"$(INTDIR)\OXItemTip.obj" \
	"$(INTDIR)\OXItemTipWnd.obj" \
	"$(INTDIR)\OXIteratorRegistryItem.obj" \
	"$(INTDIR)\OXIteratorService.obj" \
	"$(INTDIR)\OXIteratorUNC.obj" \
	"$(INTDIR)\OXJPGCod.obj" \
	"$(INTDIR)\OXJPGCom.obj" \
	"$(INTDIR)\OXJPGDom.obj" \
	"$(INTDIR)\OXJPGExp.obj" \
	"$(INTDIR)\OXJPGFle.obj" \
	"$(INTDIR)\OXLayoutManager.obj" \
	"$(INTDIR)\OXListBoxEx.obj" \
	"$(INTDIR)\OXListEdit.obj" \
	"$(INTDIR)\OXMaskedEdit.obj" \
	"$(INTDIR)\OXMDIFloatWnd.obj" \
	"$(INTDIR)\OXMenuBar.obj" \
	"$(INTDIR)\OXMetaFile.obj" \
	"$(INTDIR)\OXMonthCalCtrl.obj" \
	"$(INTDIR)\OXMulticlipboardDlg.obj" \
	"$(INTDIR)\OXMultiComboBox.obj" \
	"$(INTDIR)\OXNetBrowseTree.obj" \
	"$(INTDIR)\OXNotesEditView.obj" \
	"$(INTDIR)\OXOptionTreeCtrl.obj" \
	"$(INTDIR)\OXParser.obj" \
	"$(INTDIR)\OXPhysicalEdit.obj" \
	"$(INTDIR)\OXPhysicalEditEx.obj" \
	"$(INTDIR)\OXPopupBarCtrl.obj" \
	"$(INTDIR)\OXPreviewDialog.obj" \
	"$(INTDIR)\OXProcess.obj" \
	"$(INTDIR)\OXPropertiesWnd.obj" \
	"$(INTDIR)\OXPropertyDescriptionPane.obj" \
	"$(INTDIR)\OXPropertyTree.obj" \
	"$(INTDIR)\OXQuickString.obj" \
	"$(INTDIR)\OXRectTracker.obj" \
	"$(INTDIR)\OXRegExpression.obj" \
	"$(INTDIR)\OXRegistryItem.obj" \
	"$(INTDIR)\OXRegistryValFile.obj" \
	"$(INTDIR)\OXRegistryWatcher.obj" \
	"$(INTDIR)\OXRegistryWatchNotifier.obj" \
	"$(INTDIR)\OXResourceFile.obj" \
	"$(INTDIR)\OXResourceLibrary.obj" \
	"$(INTDIR)\OXRollup.obj" \
	"$(INTDIR)\OXRoundedButton.obj" \
	"$(INTDIR)\OXRulerOrganizer.obj" \
	"$(INTDIR)\OXScreenGrab.obj" \
	"$(INTDIR)\OXScript.obj" \
	"$(INTDIR)\OXScrollTip.obj" \
	"$(INTDIR)\OXScrollTipOwner.obj" \
	"$(INTDIR)\OXScrollWnd.obj" \
	"$(INTDIR)\OXSeparator.obj" \
	"$(INTDIR)\OXService.obj" \
	"$(INTDIR)\OXShape.obj" \
	"$(INTDIR)\OXShdWnd.obj" \
	"$(INTDIR)\OXShellFolderTree.obj" \
	"$(INTDIR)\OXShellNamespaceNavigator.obj" \
	"$(INTDIR)\OXShellObjectList.obj" \
	"$(INTDIR)\OXShortcut.obj" \
	"$(INTDIR)\OXShortcutBar.obj" \
	"$(INTDIR)\OXShortkeysOrganizer.obj" \
	"$(INTDIR)\OXSizeCtrlBar.obj" \
	"$(INTDIR)\OXSizeDlgBar.obj" \
	"$(INTDIR)\OXSizeDockBar.obj" \
	"$(INTDIR)\OXSizeToolBar.obj" \
	"$(INTDIR)\OXSkinnedComboBox.obj" \
	"$(INTDIR)\OXSkinnedSpinCtrl.obj" \
	"$(INTDIR)\OXSkinnedTabCtrl.obj" \
	"$(INTDIR)\OXSkins.obj" \
	"$(INTDIR)\OXSound.obj" \
	"$(INTDIR)\OXSoundCustomizeDlg.obj" \
	"$(INTDIR)\OXSoundEffectManager.obj" \
	"$(INTDIR)\OXSoundWnd.obj" \
	"$(INTDIR)\OXSpinCtrl.obj" \
	"$(INTDIR)\OXSplashWnd.obj" \
	"$(INTDIR)\OXSplashWndDIB.obj" \
	"$(INTDIR)\OXSplitterColRowState.obj" \
	"$(INTDIR)\OXSplitterRect.obj" \
	"$(INTDIR)\OXSplitterWnd.obj" \
	"$(INTDIR)\OXStatic.obj" \
	"$(INTDIR)\OXStaticHyperLink.obj" \
	"$(INTDIR)\OXStaticText.obj" \
	"$(INTDIR)\OXSysInfo.obj" \
	"$(INTDIR)\OXSzMiniDockFrmWnd.obj" \
	"$(INTDIR)\OXTabClientWnd.obj" \
	"$(INTDIR)\OXTabView.obj" \
	"$(INTDIR)\OXTaskbarIcon.obj" \
	"$(INTDIR)\OXTleBar.obj" \
	"$(INTDIR)\OXToolTipCtrl.obj" \
	"$(INTDIR)\OXTrace.obj" \
	"$(INTDIR)\OXTreeCal.obj" \
	"$(INTDIR)\OXTreeCombo.obj" \
	"$(INTDIR)\OXTreeCtrl.obj" \
	"$(INTDIR)\OXTreeEdit.obj" \
	"$(INTDIR)\OXTreeHeader.obj" \
	"$(INTDIR)\OXTreeItem.obj" \
	"$(INTDIR)\OXUNC.obj" \
	"$(INTDIR)\OXUNCStandardActor.obj" \
	"$(INTDIR)\OXURL.obj" \
	"$(INTDIR)\OXUsertl.obj" \
	"$(INTDIR)\OXUUDec.obj" \
	"$(INTDIR)\OXUxTheme.obj" \
	"$(INTDIR)\OXWABook.obj" \
	"$(INTDIR)\OXWatchedDir.obj" \
	"$(INTDIR)\oxwebctrl.obj" \
	"$(INTDIR)\OXWorkspaceState.obj" \
	"$(INTDIR)\Oxzoomvw.obj" \
	"$(INTDIR)\Path.obj" \
	"$(INTDIR)\PATHITER.OBJ" \
	"$(INTDIR)\PROGRESS.OBJ" \
	"$(INTDIR)\WATCHBUF.OBJ" \
	"$(INTDIR)\Xstatus4.obj" \
	"$(INTDIR)\XSTRING.OBJ" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UT.obj" \
	"$(INTDIR)\OXMain.res"

"..\DLLs\UTU.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("UT.dep")
!INCLUDE "UT.dep"
!ELSE 
!MESSAGE Warning: cannot find "UT.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "UT - Win32 Debug" || "$(CFG)" == "UT - Win32 Release" || "$(CFG)" == "UT - Win32 Unicode Debug" || "$(CFG)" == "UT - Win32 Unicode Release"
SOURCE=..\..\source\CONVFILE.CPP

"$(INTDIR)\CONVFILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\COPYTREE.CPP

"$(INTDIR)\COPYTREE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\CPYSTDLG.CPP

"$(INTDIR)\CPYSTDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\DIR.CPP

"$(INTDIR)\DIR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\DIRLIST.CPP

"$(INTDIR)\DIRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\DSTRLIST.CPP

"$(INTDIR)\DSTRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\EVNTLOG.CPP

"$(INTDIR)\EVNTLOG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\FILE.CPP

"$(INTDIR)\FILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\FILELIST.CPP

"$(INTDIR)\FILELIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\MDELDLG.CPP

"$(INTDIR)\MDELDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\MFCToolBar.cpp

"$(INTDIR)\MFCToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OX3DTabView.cpp

"$(INTDIR)\OX3DTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXActiveDesktop.cpp

"$(INTDIR)\OXActiveDesktop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXAdvancedAssert.cpp

"$(INTDIR)\OXAdvancedAssert.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXAdvancedAssertMail.cpp

"$(INTDIR)\OXAdvancedAssertMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXAppBar.cpp

"$(INTDIR)\OXAppBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXAutoComplete.cpp

"$(INTDIR)\OXAutoComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXAutoListBox.cpp

"$(INTDIR)\OXAutoListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBackgroundPainter.cpp

"$(INTDIR)\OXBackgroundPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBITBUF.CPP

"$(INTDIR)\OXBITBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBitmapButton.cpp

"$(INTDIR)\OXBitmapButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBitmapMenu.cpp

"$(INTDIR)\OXBitmapMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBitmapMenuOrganizer.cpp

"$(INTDIR)\OXBitmapMenuOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBMPFilePickerCombo.cpp

"$(INTDIR)\OXBMPFilePickerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBMPFle.cpp

"$(INTDIR)\OXBMPFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBrowseColorEdit.cpp

"$(INTDIR)\OXBrowseColorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBrowseDirEdit.cpp

"$(INTDIR)\OXBrowseDirEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXBrowseDirEditList.cpp

"$(INTDIR)\OXBrowseDirEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXButtonAppearanceDlg.cpp

"$(INTDIR)\OXButtonAppearanceDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCalculatorCtrl.cpp

"$(INTDIR)\OXCalculatorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCalculatorEdit.cpp

"$(INTDIR)\OXCalculatorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCalendar.cpp

"$(INTDIR)\OXCalendar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCalendarEdit.cpp

"$(INTDIR)\OXCalendarEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCalendarPopup.cpp

"$(INTDIR)\OXCalendarPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCaptionPainter.cpp

"$(INTDIR)\OXCaptionPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXChildFrameState.cpp

"$(INTDIR)\OXChildFrameState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXClipPocket.cpp

"$(INTDIR)\OXClipPocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCMPFL.CPP

"$(INTDIR)\OXCMPFL.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXColorPickerButton.cpp

"$(INTDIR)\OXColorPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXColorPickerCtrl.cpp

"$(INTDIR)\OXColorPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXComboPickerCtrl.cpp

"$(INTDIR)\OXComboPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCOMPR.CPP

"$(INTDIR)\OXCOMPR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXContextHelpDialog.cpp

"$(INTDIR)\OXContextHelpDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXContextHelpPropertyPage.cpp

"$(INTDIR)\OXContextHelpPropertyPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXContextHelpPropertySheet.cpp

"$(INTDIR)\OXContextHelpPropertySheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCoolBar.cpp

"$(INTDIR)\OXCoolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCoolComboBox.cpp

"$(INTDIR)\OXCoolComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCoolControlOrganizer.cpp

"$(INTDIR)\OXCoolControlOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCoolCtrl.cpp

"$(INTDIR)\OXCoolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCoolToolBar.cpp

"$(INTDIR)\OXCoolToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCRCChk.CPP

"$(INTDIR)\OXCRCChk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCsvFile.cpp

"$(INTDIR)\OXCsvFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeBackgroundPage.cpp

"$(INTDIR)\OXCustomizeBackgroundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeCaptionPage.cpp

"$(INTDIR)\OXCustomizeCaptionPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeCommandsPage.cpp

"$(INTDIR)\OXCustomizeCommandsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeCoolControlsPage.cpp

"$(INTDIR)\OXCustomizeCoolControlsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeInstanceManagerPage.cpp

"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeIntelliMousePage.cpp

"$(INTDIR)\OXCustomizeIntelliMousePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeManager.cpp

"$(INTDIR)\OXCustomizeManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizePage.cpp

"$(INTDIR)\OXCustomizePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeShortkeysPage.cpp

"$(INTDIR)\OXCustomizeShortkeysPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeSplashWindowPage.cpp

"$(INTDIR)\OXCustomizeSplashWindowPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeTabbedMDIPage.cpp

"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeToolbarsPage.cpp

"$(INTDIR)\OXCustomizeToolbarsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeTrayIconPage.cpp

"$(INTDIR)\OXCustomizeTrayIconPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXCustomizeWorkspaceStatePage.cpp

"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDao.cpp

"$(INTDIR)\OXDao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDateTimeCtrl.cpp

"$(INTDIR)\OXDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDckPPg.cpp

"$(INTDIR)\OXDckPPg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDckPSh.cpp

"$(INTDIR)\OXDckPSh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDFlHdr.cpp

"$(INTDIR)\OXDFlHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\Oxdib.cpp

"$(INTDIR)\Oxdib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDirDlg.cpp

"$(INTDIR)\OXDirDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDlgBar.cpp

"$(INTDIR)\OXDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDLLManager.cpp

"$(INTDIR)\OXDLLManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\oxdocktabctrl.cpp

"$(INTDIR)\oxdocktabctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDocTemplateSpy.cpp

"$(INTDIR)\OXDocTemplateSpy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDragDockContext.CPP

"$(INTDIR)\OXDragDockContext.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXDragDropCommands.cpp

"$(INTDIR)\OXDragDropCommands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXEdit.cpp

"$(INTDIR)\OXEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXEditList.cpp

"$(INTDIR)\OXEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXFileChanger.cpp

"$(INTDIR)\OXFileChanger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXFileWatcher.cpp

"$(INTDIR)\OXFileWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXFileWatchNotifier.cpp

"$(INTDIR)\OXFileWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXFolderPickerDlg.cpp

"$(INTDIR)\OXFolderPickerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXFontComboBox.cpp

"$(INTDIR)\OXFontComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXFontPickerButton.cpp

"$(INTDIR)\OXFontPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXFrameWndDock.CPP

"$(INTDIR)\OXFrameWndDock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXFullScreenFrame.cpp

"$(INTDIR)\OXFullScreenFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXGphFle.cpp

"$(INTDIR)\OXGphFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXGraphics.cpp

"$(INTDIR)\OXGraphics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\oxgridedit.cpp

"$(INTDIR)\oxgridedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXGridHdr.cpp

"$(INTDIR)\OXGridHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXGridList.cpp

"$(INTDIR)\OXGridList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXHashTable.cpp

"$(INTDIR)\OXHashTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXHistoryCombo.cpp

"$(INTDIR)\OXHistoryCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXHistoryCtrl.cpp

"$(INTDIR)\OXHistoryCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXHookWnd.cpp

"$(INTDIR)\OXHookWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXHTMLParser.cpp

"$(INTDIR)\OXHTMLParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXHyperLinkAction.cpp

"$(INTDIR)\OXHyperLinkAction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXImageListBox.cpp

"$(INTDIR)\OXImageListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXImageViewer.cpp

"$(INTDIR)\OXImageViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXInstanceManager.cpp

"$(INTDIR)\OXInstanceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXInteger.cpp

"$(INTDIR)\OXInteger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXIntelliMouse.cpp

"$(INTDIR)\OXIntelliMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXIPEdit.cpp

"$(INTDIR)\OXIPEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXItemTip.cpp

"$(INTDIR)\OXItemTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXItemTipWnd.cpp

"$(INTDIR)\OXItemTipWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXIteratorRegistryItem.cpp

"$(INTDIR)\OXIteratorRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXIteratorService.cpp

"$(INTDIR)\OXIteratorService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXIteratorUNC.cpp

"$(INTDIR)\OXIteratorUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXJPGCod.cpp

"$(INTDIR)\OXJPGCod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXJPGCom.cpp

"$(INTDIR)\OXJPGCom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXJPGDom.cpp

"$(INTDIR)\OXJPGDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXJPGExp.cpp

"$(INTDIR)\OXJPGExp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXJPGFle.cpp

"$(INTDIR)\OXJPGFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXLayoutManager.cpp

"$(INTDIR)\OXLayoutManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXListBoxEx.cpp

"$(INTDIR)\OXListBoxEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXListEdit.cpp

"$(INTDIR)\OXListEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXMaskedEdit.cpp

"$(INTDIR)\OXMaskedEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXMDIFloatWnd.cpp

"$(INTDIR)\OXMDIFloatWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXMenuBar.cpp

"$(INTDIR)\OXMenuBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXMetaFile.cpp

"$(INTDIR)\OXMetaFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXMonthCalCtrl.cpp

"$(INTDIR)\OXMonthCalCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXMulticlipboardDlg.cpp

"$(INTDIR)\OXMulticlipboardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXMultiComboBox.cpp

"$(INTDIR)\OXMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXNetBrowseTree.cpp

"$(INTDIR)\OXNetBrowseTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXNotesEditView.cpp

"$(INTDIR)\OXNotesEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXOptionTreeCtrl.cpp

"$(INTDIR)\OXOptionTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXParser.cpp

"$(INTDIR)\OXParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXPhysicalEdit.cpp

"$(INTDIR)\OXPhysicalEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXPhysicalEditEx.cpp

"$(INTDIR)\OXPhysicalEditEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXPopupBarCtrl.cpp

"$(INTDIR)\OXPopupBarCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXPreviewDialog.cpp

"$(INTDIR)\OXPreviewDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXProcess.cpp

"$(INTDIR)\OXProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXPropertiesWnd.cpp

"$(INTDIR)\OXPropertiesWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXPropertyDescriptionPane.cpp

"$(INTDIR)\OXPropertyDescriptionPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXPropertyTree.cpp

"$(INTDIR)\OXPropertyTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXQuickString.cpp

"$(INTDIR)\OXQuickString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRectTracker.cpp

"$(INTDIR)\OXRectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRegExpression.cpp

"$(INTDIR)\OXRegExpression.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRegistryItem.cpp

"$(INTDIR)\OXRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRegistryValFile.cpp

"$(INTDIR)\OXRegistryValFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRegistryWatcher.cpp

"$(INTDIR)\OXRegistryWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRegistryWatchNotifier.cpp

"$(INTDIR)\OXRegistryWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXResourceFile.cpp

"$(INTDIR)\OXResourceFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXResourceLibrary.cpp

"$(INTDIR)\OXResourceLibrary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRollup.CPP

"$(INTDIR)\OXRollup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRoundedButton.cpp

"$(INTDIR)\OXRoundedButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXRulerOrganizer.cpp

"$(INTDIR)\OXRulerOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXScreenGrab.cpp

"$(INTDIR)\OXScreenGrab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXScript.cpp

"$(INTDIR)\OXScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXScrollTip.cpp

"$(INTDIR)\OXScrollTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXScrollTipOwner.cpp

"$(INTDIR)\OXScrollTipOwner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXScrollWnd.cpp

"$(INTDIR)\OXScrollWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSeparator.cpp

"$(INTDIR)\OXSeparator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXService.cpp

"$(INTDIR)\OXService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXShape.cpp

"$(INTDIR)\OXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXShdWnd.cpp

"$(INTDIR)\OXShdWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXShellFolderTree.cpp

"$(INTDIR)\OXShellFolderTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXShellNamespaceNavigator.cpp

"$(INTDIR)\OXShellNamespaceNavigator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXShellObjectList.cpp

"$(INTDIR)\OXShellObjectList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXShortcut.cpp

"$(INTDIR)\OXShortcut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXShortcutBar.cpp

"$(INTDIR)\OXShortcutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXShortkeysOrganizer.cpp

"$(INTDIR)\OXShortkeysOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSizeCtrlBar.CPP

"$(INTDIR)\OXSizeCtrlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSizeDlgBar.cpp

"$(INTDIR)\OXSizeDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSizeDockBar.cpp

"$(INTDIR)\OXSizeDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSizeToolBar.cpp

"$(INTDIR)\OXSizeToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSkinnedComboBox.cpp

"$(INTDIR)\OXSkinnedComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSkinnedSpinCtrl.cpp

"$(INTDIR)\OXSkinnedSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSkinnedTabCtrl.cpp

"$(INTDIR)\OXSkinnedTabCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSkins.cpp

"$(INTDIR)\OXSkins.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSound.cpp

"$(INTDIR)\OXSound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSoundCustomizeDlg.cpp

"$(INTDIR)\OXSoundCustomizeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSoundEffectManager.cpp

"$(INTDIR)\OXSoundEffectManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSoundWnd.cpp

"$(INTDIR)\OXSoundWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSpinCtrl.cpp

"$(INTDIR)\OXSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSplashWnd.cpp

"$(INTDIR)\OXSplashWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSplashWndDIB.cpp

"$(INTDIR)\OXSplashWndDIB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSplitterColRowState.cpp

"$(INTDIR)\OXSplitterColRowState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSplitterRect.cpp

"$(INTDIR)\OXSplitterRect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSplitterWnd.cpp

"$(INTDIR)\OXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXStatic.cpp

"$(INTDIR)\OXStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXStaticHyperLink.cpp

"$(INTDIR)\OXStaticHyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXStaticText.cpp

"$(INTDIR)\OXStaticText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSysInfo.cpp

"$(INTDIR)\OXSysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXSzMiniDockFrmWnd.cpp

"$(INTDIR)\OXSzMiniDockFrmWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTabClientWnd.cpp

"$(INTDIR)\OXTabClientWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTabView.cpp

"$(INTDIR)\OXTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTaskbarIcon.cpp

"$(INTDIR)\OXTaskbarIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTleBar.CPP

"$(INTDIR)\OXTleBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXToolTipCtrl.cpp

"$(INTDIR)\OXToolTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTrace.cpp

"$(INTDIR)\OXTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTreeCal.cpp

"$(INTDIR)\OXTreeCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTreeCombo.cpp

"$(INTDIR)\OXTreeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTreeCtrl.cpp

"$(INTDIR)\OXTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTreeEdit.cpp

"$(INTDIR)\OXTreeEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTreeHeader.cpp

"$(INTDIR)\OXTreeHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXTreeItem.cpp

"$(INTDIR)\OXTreeItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXUNC.cpp

"$(INTDIR)\OXUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXUNCStandardActor.cpp

"$(INTDIR)\OXUNCStandardActor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXURL.cpp

"$(INTDIR)\OXURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXUsertl.cpp

"$(INTDIR)\OXUsertl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXUUDec.cpp

"$(INTDIR)\OXUUDec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXUxTheme.cpp

"$(INTDIR)\OXUxTheme.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXWABook.cpp

"$(INTDIR)\OXWABook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXWatchedDir.cpp

"$(INTDIR)\OXWatchedDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\oxwebctrl.cpp

"$(INTDIR)\oxwebctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\OXWorkspaceState.cpp

"$(INTDIR)\OXWorkspaceState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\Oxzoomvw.cpp

"$(INTDIR)\Oxzoomvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\Path.cpp

"$(INTDIR)\Path.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\PATHITER.CPP

"$(INTDIR)\PATHITER.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\PROGRESS.CPP

"$(INTDIR)\PROGRESS.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\WATCHBUF.CPP

"$(INTDIR)\WATCHBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\Xstatus4.cpp

"$(INTDIR)\Xstatus4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\source\XSTRING.CPP

"$(INTDIR)\XSTRING.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\include\OXMain.rc

!IF  "$(CFG)" == "UT - Win32 Debug"


"$(INTDIR)\OXMain.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\OXMain.res" /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /i "\Ultimate ToolBox\Working\include" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "UT - Win32 Release"


"$(INTDIR)\OXMain.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\OXMain.res" /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /i "\Ultimate ToolBox\Working\include" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "UT - Win32 Unicode Debug"


"$(INTDIR)\OXMain.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\OXMain.res" /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /i "\Ultimate ToolBox\Working\include" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "UT - Win32 Unicode Release"


"$(INTDIR)\OXMain.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\OXMain.res" /i "..\..\Include" /i "..\..\Lib\Jpeg" /i "..\..\Lib\Jpeg\Jpegsrc6" /i "\Ultimate ToolBox\Working\include" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "UT - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_WINDLL" /D "_AFXDLL" /Fp"$(INTDIR)\UT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\UT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UT - Win32 Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_WINDLL" /D "_AFXDLL" /Fp"$(INTDIR)\UT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\UT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UT - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /Fp"$(INTDIR)\UT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\UT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UT - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /I "..\..\include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_BUILD_UTB_INTO_EXTDLL" /D "_UNICODE" /D "_WINDLL" /D "_AFXDLL" /Fp"$(INTDIR)\UT.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\UT.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\UT.cpp

"$(INTDIR)\UT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UT.pch"



!ENDIF 

