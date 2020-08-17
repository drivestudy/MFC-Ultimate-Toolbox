# Microsoft Developer Studio Generated NMAKE File, Based on UTStaticLib.dsp
!IF "$(CFG)" == ""
CFG=UTStaticLib - Win32 UnicodeDebugMFCStatic
!MESSAGE No configuration specified. Defaulting to UTStaticLib - Win32 UnicodeDebugMFCStatic.
!ENDIF 

!IF "$(CFG)" != "UTStaticLib - Win32 DebugMFCShared" && "$(CFG)" != "UTStaticLib - Win32 ReleaseMFCShared" && "$(CFG)" != "UTStaticLib - Win32 DebugMFCStatic" && "$(CFG)" != "UTStaticLib - Win32 ReleaseMFCStatic" && "$(CFG)" != "UTStaticLib - Win32 UnicodeDebugMFCShared" && "$(CFG)" != "UTStaticLib - Win32 UnicodeReleaseMFCShared" && "$(CFG)" != "UTStaticLib - Win32 UnicodeDebugMFCStatic" && "$(CFG)" != "UTStaticLib - Win32 UnicodeReleaseMFCStatic"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"

OUTDIR=.\DebugMFCShared
INTDIR=.\DebugMFCShared

ALL : "..\libs\UTLibSharedD.lib"


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
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\UTStaticLib.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\libs\UTLibSharedD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\UTStaticLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UTStaticLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\libs\UTLibSharedD.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\OXUxTheme.obj"

"..\libs\UTLibSharedD.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"

OUTDIR=.\ReleaseMFCShared
INTDIR=.\ReleaseMFCShared

ALL : "..\libs\UTLibSharedR.lib"


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
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\UTStaticLib.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\libs\UTLibSharedR.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\UTStaticLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UTStaticLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\libs\UTLibSharedR.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\OXUxTheme.obj"

"..\libs\UTLibSharedR.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"

OUTDIR=.\DebugMFCStatic
INTDIR=.\DebugMFCStatic

ALL : "..\libs\UTLibStaticD.lib"


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
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\UTStaticLib.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\libs\UTLibStaticD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W4 /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\UTStaticLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UTStaticLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\libs\UTLibStaticD.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\OXUxTheme.obj"

"..\libs\UTLibStaticD.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"

OUTDIR=.\ReleaseMFCStatic
INTDIR=.\ReleaseMFCStatic

ALL : "..\libs\UTLibStaticR.lib"


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
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\UTStaticLib.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\libs\UTLibStaticR.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\UTStaticLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UTStaticLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\libs\UTLibStaticR.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\OXUxTheme.obj"

"..\libs\UTLibStaticR.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"

OUTDIR=.\UnicodeDebugMFCShared
INTDIR=.\UnicodeDebugMFCShared

ALL : "..\libs\UTLibSharedDU.lib"


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
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\UTStaticLib.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\libs\UTLibSharedDU.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)\UTStaticLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UTStaticLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\libs\UTLibSharedDU.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\OXUxTheme.obj"

"..\libs\UTLibSharedDU.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"

OUTDIR=.\UnicodeReleaseMFCShared
INTDIR=.\UnicodeReleaseMFCShared

ALL : "..\libs\UTLibSharedRU.lib"


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
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\UTStaticLib.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\libs\UTLibSharedRU.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)\UTStaticLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UTStaticLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\libs\UTLibSharedRU.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\OXUxTheme.obj"

"..\libs\UTLibSharedRU.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"

OUTDIR=.\UnicodeDebugMFCStatic
INTDIR=.\UnicodeDebugMFCStatic
# Begin Custom Macros
OutDir=.\UnicodeDebugMFCStatic
# End Custom Macros

ALL : "..\libs\UTLibStaticDU.lib" "$(OUTDIR)\UTStaticLib.bsc"


CLEAN :
	-@erase "$(INTDIR)\CONVFILE.OBJ"
	-@erase "$(INTDIR)\CONVFILE.SBR"
	-@erase "$(INTDIR)\COPYTREE.OBJ"
	-@erase "$(INTDIR)\COPYTREE.SBR"
	-@erase "$(INTDIR)\CPYSTDLG.OBJ"
	-@erase "$(INTDIR)\CPYSTDLG.SBR"
	-@erase "$(INTDIR)\DIR.OBJ"
	-@erase "$(INTDIR)\DIR.SBR"
	-@erase "$(INTDIR)\DIRLIST.OBJ"
	-@erase "$(INTDIR)\DIRLIST.SBR"
	-@erase "$(INTDIR)\DSTRLIST.OBJ"
	-@erase "$(INTDIR)\DSTRLIST.SBR"
	-@erase "$(INTDIR)\EVNTLOG.OBJ"
	-@erase "$(INTDIR)\EVNTLOG.SBR"
	-@erase "$(INTDIR)\FILE.OBJ"
	-@erase "$(INTDIR)\FILE.SBR"
	-@erase "$(INTDIR)\FILELIST.OBJ"
	-@erase "$(INTDIR)\FILELIST.SBR"
	-@erase "$(INTDIR)\MDELDLG.OBJ"
	-@erase "$(INTDIR)\MDELDLG.SBR"
	-@erase "$(INTDIR)\MFCToolBar.obj"
	-@erase "$(INTDIR)\MFCToolBar.sbr"
	-@erase "$(INTDIR)\OX3DTabView.obj"
	-@erase "$(INTDIR)\OX3DTabView.sbr"
	-@erase "$(INTDIR)\OXActiveDesktop.obj"
	-@erase "$(INTDIR)\OXActiveDesktop.sbr"
	-@erase "$(INTDIR)\OXAdvancedAssert.obj"
	-@erase "$(INTDIR)\OXAdvancedAssert.sbr"
	-@erase "$(INTDIR)\OXAdvancedAssertMail.obj"
	-@erase "$(INTDIR)\OXAdvancedAssertMail.sbr"
	-@erase "$(INTDIR)\OXAppBar.obj"
	-@erase "$(INTDIR)\OXAppBar.sbr"
	-@erase "$(INTDIR)\OXAutoComplete.obj"
	-@erase "$(INTDIR)\OXAutoComplete.sbr"
	-@erase "$(INTDIR)\OXAutoListBox.obj"
	-@erase "$(INTDIR)\OXAutoListBox.sbr"
	-@erase "$(INTDIR)\OXBackgroundPainter.obj"
	-@erase "$(INTDIR)\OXBackgroundPainter.sbr"
	-@erase "$(INTDIR)\OXBITBUF.OBJ"
	-@erase "$(INTDIR)\OXBITBUF.SBR"
	-@erase "$(INTDIR)\OXBitmapButton.obj"
	-@erase "$(INTDIR)\OXBitmapButton.sbr"
	-@erase "$(INTDIR)\OXBitmapMenu.obj"
	-@erase "$(INTDIR)\OXBitmapMenu.sbr"
	-@erase "$(INTDIR)\OXBitmapMenuOrganizer.obj"
	-@erase "$(INTDIR)\OXBitmapMenuOrganizer.sbr"
	-@erase "$(INTDIR)\OXBMPFilePickerCombo.obj"
	-@erase "$(INTDIR)\OXBMPFilePickerCombo.sbr"
	-@erase "$(INTDIR)\OXBMPFle.obj"
	-@erase "$(INTDIR)\OXBMPFle.sbr"
	-@erase "$(INTDIR)\OXBrowseColorEdit.obj"
	-@erase "$(INTDIR)\OXBrowseColorEdit.sbr"
	-@erase "$(INTDIR)\OXBrowseDirEdit.obj"
	-@erase "$(INTDIR)\OXBrowseDirEdit.sbr"
	-@erase "$(INTDIR)\OXBrowseDirEditList.obj"
	-@erase "$(INTDIR)\OXBrowseDirEditList.sbr"
	-@erase "$(INTDIR)\OXButtonAppearanceDlg.obj"
	-@erase "$(INTDIR)\OXButtonAppearanceDlg.sbr"
	-@erase "$(INTDIR)\OXCalculatorCtrl.obj"
	-@erase "$(INTDIR)\OXCalculatorCtrl.sbr"
	-@erase "$(INTDIR)\OXCalculatorEdit.obj"
	-@erase "$(INTDIR)\OXCalculatorEdit.sbr"
	-@erase "$(INTDIR)\OXCalendar.obj"
	-@erase "$(INTDIR)\OXCalendar.sbr"
	-@erase "$(INTDIR)\OXCalendarEdit.obj"
	-@erase "$(INTDIR)\OXCalendarEdit.sbr"
	-@erase "$(INTDIR)\OXCalendarPopup.obj"
	-@erase "$(INTDIR)\OXCalendarPopup.sbr"
	-@erase "$(INTDIR)\OXCaptionPainter.obj"
	-@erase "$(INTDIR)\OXCaptionPainter.sbr"
	-@erase "$(INTDIR)\OXChildFrameState.obj"
	-@erase "$(INTDIR)\OXChildFrameState.sbr"
	-@erase "$(INTDIR)\OXClipPocket.obj"
	-@erase "$(INTDIR)\OXClipPocket.sbr"
	-@erase "$(INTDIR)\OXCMPFL.OBJ"
	-@erase "$(INTDIR)\OXCMPFL.SBR"
	-@erase "$(INTDIR)\OXColorPickerButton.obj"
	-@erase "$(INTDIR)\OXColorPickerButton.sbr"
	-@erase "$(INTDIR)\OXColorPickerCtrl.obj"
	-@erase "$(INTDIR)\OXColorPickerCtrl.sbr"
	-@erase "$(INTDIR)\OXComboPickerCtrl.obj"
	-@erase "$(INTDIR)\OXComboPickerCtrl.sbr"
	-@erase "$(INTDIR)\OXCOMPR.OBJ"
	-@erase "$(INTDIR)\OXCOMPR.SBR"
	-@erase "$(INTDIR)\OXContextHelpDialog.obj"
	-@erase "$(INTDIR)\OXContextHelpDialog.sbr"
	-@erase "$(INTDIR)\OXContextHelpPropertyPage.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertyPage.sbr"
	-@erase "$(INTDIR)\OXContextHelpPropertySheet.obj"
	-@erase "$(INTDIR)\OXContextHelpPropertySheet.sbr"
	-@erase "$(INTDIR)\OXCoolBar.obj"
	-@erase "$(INTDIR)\OXCoolBar.sbr"
	-@erase "$(INTDIR)\OXCoolComboBox.obj"
	-@erase "$(INTDIR)\OXCoolComboBox.sbr"
	-@erase "$(INTDIR)\OXCoolControlOrganizer.obj"
	-@erase "$(INTDIR)\OXCoolControlOrganizer.sbr"
	-@erase "$(INTDIR)\OXCoolCtrl.obj"
	-@erase "$(INTDIR)\OXCoolCtrl.sbr"
	-@erase "$(INTDIR)\OXCoolToolBar.obj"
	-@erase "$(INTDIR)\OXCoolToolBar.sbr"
	-@erase "$(INTDIR)\OXCRCChk.obj"
	-@erase "$(INTDIR)\OXCRCChk.sbr"
	-@erase "$(INTDIR)\OXCsvFile.obj"
	-@erase "$(INTDIR)\OXCsvFile.sbr"
	-@erase "$(INTDIR)\OXCustomizeBackgroundPage.obj"
	-@erase "$(INTDIR)\OXCustomizeBackgroundPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeCaptionPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCaptionPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeCommandsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCommandsPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeCoolControlsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeCoolControlsPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeInstanceManagerPage.obj"
	-@erase "$(INTDIR)\OXCustomizeInstanceManagerPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeIntelliMousePage.obj"
	-@erase "$(INTDIR)\OXCustomizeIntelliMousePage.sbr"
	-@erase "$(INTDIR)\OXCustomizeManager.obj"
	-@erase "$(INTDIR)\OXCustomizeManager.sbr"
	-@erase "$(INTDIR)\OXCustomizePage.obj"
	-@erase "$(INTDIR)\OXCustomizePage.sbr"
	-@erase "$(INTDIR)\OXCustomizeShortkeysPage.obj"
	-@erase "$(INTDIR)\OXCustomizeShortkeysPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeSplashWindowPage.obj"
	-@erase "$(INTDIR)\OXCustomizeSplashWindowPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeTabbedMDIPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTabbedMDIPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeToolbarsPage.obj"
	-@erase "$(INTDIR)\OXCustomizeToolbarsPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeTrayIconPage.obj"
	-@erase "$(INTDIR)\OXCustomizeTrayIconPage.sbr"
	-@erase "$(INTDIR)\OXCustomizeWorkspaceStatePage.obj"
	-@erase "$(INTDIR)\OXCustomizeWorkspaceStatePage.sbr"
	-@erase "$(INTDIR)\OXDao.obj"
	-@erase "$(INTDIR)\OXDao.sbr"
	-@erase "$(INTDIR)\OXDateTimeCtrl.obj"
	-@erase "$(INTDIR)\OXDateTimeCtrl.sbr"
	-@erase "$(INTDIR)\OXDckPPg.obj"
	-@erase "$(INTDIR)\OXDckPPg.sbr"
	-@erase "$(INTDIR)\OXDckPSh.obj"
	-@erase "$(INTDIR)\OXDckPSh.sbr"
	-@erase "$(INTDIR)\OXDFlHdr.obj"
	-@erase "$(INTDIR)\OXDFlHdr.sbr"
	-@erase "$(INTDIR)\Oxdib.obj"
	-@erase "$(INTDIR)\Oxdib.sbr"
	-@erase "$(INTDIR)\OXDirDlg.obj"
	-@erase "$(INTDIR)\OXDirDlg.sbr"
	-@erase "$(INTDIR)\OXDlgBar.obj"
	-@erase "$(INTDIR)\OXDlgBar.sbr"
	-@erase "$(INTDIR)\OXDLLManager.obj"
	-@erase "$(INTDIR)\OXDLLManager.sbr"
	-@erase "$(INTDIR)\oxdocktabctrl.obj"
	-@erase "$(INTDIR)\oxdocktabctrl.sbr"
	-@erase "$(INTDIR)\OXDocTemplateSpy.obj"
	-@erase "$(INTDIR)\OXDocTemplateSpy.sbr"
	-@erase "$(INTDIR)\OXDragDockContext.obj"
	-@erase "$(INTDIR)\OXDragDockContext.sbr"
	-@erase "$(INTDIR)\OXDragDropCommands.obj"
	-@erase "$(INTDIR)\OXDragDropCommands.sbr"
	-@erase "$(INTDIR)\OXEdit.obj"
	-@erase "$(INTDIR)\OXEdit.sbr"
	-@erase "$(INTDIR)\OXEditList.obj"
	-@erase "$(INTDIR)\OXEditList.sbr"
	-@erase "$(INTDIR)\OXFileChanger.obj"
	-@erase "$(INTDIR)\OXFileChanger.sbr"
	-@erase "$(INTDIR)\OXFileWatcher.obj"
	-@erase "$(INTDIR)\OXFileWatcher.sbr"
	-@erase "$(INTDIR)\OXFileWatchNotifier.obj"
	-@erase "$(INTDIR)\OXFileWatchNotifier.sbr"
	-@erase "$(INTDIR)\OXFolderPickerDlg.obj"
	-@erase "$(INTDIR)\OXFolderPickerDlg.sbr"
	-@erase "$(INTDIR)\OXFontComboBox.obj"
	-@erase "$(INTDIR)\OXFontComboBox.sbr"
	-@erase "$(INTDIR)\OXFontPickerButton.obj"
	-@erase "$(INTDIR)\OXFontPickerButton.sbr"
	-@erase "$(INTDIR)\OXFrameWndDock.obj"
	-@erase "$(INTDIR)\OXFrameWndDock.sbr"
	-@erase "$(INTDIR)\OXFullScreenFrame.obj"
	-@erase "$(INTDIR)\OXFullScreenFrame.sbr"
	-@erase "$(INTDIR)\OXGphFle.obj"
	-@erase "$(INTDIR)\OXGphFle.sbr"
	-@erase "$(INTDIR)\OXGraphics.obj"
	-@erase "$(INTDIR)\OXGraphics.sbr"
	-@erase "$(INTDIR)\oxgridedit.obj"
	-@erase "$(INTDIR)\oxgridedit.sbr"
	-@erase "$(INTDIR)\OXGridHdr.obj"
	-@erase "$(INTDIR)\OXGridHdr.sbr"
	-@erase "$(INTDIR)\OXGridList.obj"
	-@erase "$(INTDIR)\OXGridList.sbr"
	-@erase "$(INTDIR)\OXHashTable.obj"
	-@erase "$(INTDIR)\OXHashTable.sbr"
	-@erase "$(INTDIR)\OXHistoryCombo.obj"
	-@erase "$(INTDIR)\OXHistoryCombo.sbr"
	-@erase "$(INTDIR)\OXHistoryCtrl.obj"
	-@erase "$(INTDIR)\OXHistoryCtrl.sbr"
	-@erase "$(INTDIR)\OXHookWnd.obj"
	-@erase "$(INTDIR)\OXHookWnd.sbr"
	-@erase "$(INTDIR)\OXHTMLParser.obj"
	-@erase "$(INTDIR)\OXHTMLParser.sbr"
	-@erase "$(INTDIR)\OXHyperLinkAction.obj"
	-@erase "$(INTDIR)\OXHyperLinkAction.sbr"
	-@erase "$(INTDIR)\OXImageListBox.obj"
	-@erase "$(INTDIR)\OXImageListBox.sbr"
	-@erase "$(INTDIR)\OXImageViewer.obj"
	-@erase "$(INTDIR)\OXImageViewer.sbr"
	-@erase "$(INTDIR)\OXInstanceManager.obj"
	-@erase "$(INTDIR)\OXInstanceManager.sbr"
	-@erase "$(INTDIR)\OXInteger.obj"
	-@erase "$(INTDIR)\OXInteger.sbr"
	-@erase "$(INTDIR)\OXIntelliMouse.obj"
	-@erase "$(INTDIR)\OXIntelliMouse.sbr"
	-@erase "$(INTDIR)\OXIPEdit.obj"
	-@erase "$(INTDIR)\OXIPEdit.sbr"
	-@erase "$(INTDIR)\OXItemTip.obj"
	-@erase "$(INTDIR)\OXItemTip.sbr"
	-@erase "$(INTDIR)\OXItemTipWnd.obj"
	-@erase "$(INTDIR)\OXItemTipWnd.sbr"
	-@erase "$(INTDIR)\OXIteratorRegistryItem.obj"
	-@erase "$(INTDIR)\OXIteratorRegistryItem.sbr"
	-@erase "$(INTDIR)\OXIteratorService.obj"
	-@erase "$(INTDIR)\OXIteratorService.sbr"
	-@erase "$(INTDIR)\OXIteratorUNC.obj"
	-@erase "$(INTDIR)\OXIteratorUNC.sbr"
	-@erase "$(INTDIR)\OXJPGCod.obj"
	-@erase "$(INTDIR)\OXJPGCod.sbr"
	-@erase "$(INTDIR)\OXJPGCom.obj"
	-@erase "$(INTDIR)\OXJPGCom.sbr"
	-@erase "$(INTDIR)\OXJPGDom.obj"
	-@erase "$(INTDIR)\OXJPGDom.sbr"
	-@erase "$(INTDIR)\OXJPGExp.obj"
	-@erase "$(INTDIR)\OXJPGExp.sbr"
	-@erase "$(INTDIR)\OXJPGFle.obj"
	-@erase "$(INTDIR)\OXJPGFle.sbr"
	-@erase "$(INTDIR)\OXLayoutManager.obj"
	-@erase "$(INTDIR)\OXLayoutManager.sbr"
	-@erase "$(INTDIR)\OXListBoxEx.obj"
	-@erase "$(INTDIR)\OXListBoxEx.sbr"
	-@erase "$(INTDIR)\OXListEdit.obj"
	-@erase "$(INTDIR)\OXListEdit.sbr"
	-@erase "$(INTDIR)\OXMaskedEdit.obj"
	-@erase "$(INTDIR)\OXMaskedEdit.sbr"
	-@erase "$(INTDIR)\OXMDIFloatWnd.obj"
	-@erase "$(INTDIR)\OXMDIFloatWnd.sbr"
	-@erase "$(INTDIR)\OXMenuBar.obj"
	-@erase "$(INTDIR)\OXMenuBar.sbr"
	-@erase "$(INTDIR)\OXMetaFile.obj"
	-@erase "$(INTDIR)\OXMetaFile.sbr"
	-@erase "$(INTDIR)\OXMonthCalCtrl.obj"
	-@erase "$(INTDIR)\OXMonthCalCtrl.sbr"
	-@erase "$(INTDIR)\OXMulticlipboardDlg.obj"
	-@erase "$(INTDIR)\OXMulticlipboardDlg.sbr"
	-@erase "$(INTDIR)\OXMultiComboBox.obj"
	-@erase "$(INTDIR)\OXMultiComboBox.sbr"
	-@erase "$(INTDIR)\OXNetBrowseTree.obj"
	-@erase "$(INTDIR)\OXNetBrowseTree.sbr"
	-@erase "$(INTDIR)\OXNotesEditView.obj"
	-@erase "$(INTDIR)\OXNotesEditView.sbr"
	-@erase "$(INTDIR)\OXOptionTreeCtrl.obj"
	-@erase "$(INTDIR)\OXOptionTreeCtrl.sbr"
	-@erase "$(INTDIR)\OXParser.obj"
	-@erase "$(INTDIR)\OXParser.sbr"
	-@erase "$(INTDIR)\OXPhysicalEdit.obj"
	-@erase "$(INTDIR)\OXPhysicalEdit.sbr"
	-@erase "$(INTDIR)\OXPhysicalEditEx.obj"
	-@erase "$(INTDIR)\OXPhysicalEditEx.sbr"
	-@erase "$(INTDIR)\OXPopupBarCtrl.obj"
	-@erase "$(INTDIR)\OXPopupBarCtrl.sbr"
	-@erase "$(INTDIR)\OXPreviewDialog.obj"
	-@erase "$(INTDIR)\OXPreviewDialog.sbr"
	-@erase "$(INTDIR)\OXProcess.obj"
	-@erase "$(INTDIR)\OXProcess.sbr"
	-@erase "$(INTDIR)\OXPropertiesWnd.obj"
	-@erase "$(INTDIR)\OXPropertiesWnd.sbr"
	-@erase "$(INTDIR)\OXPropertyDescriptionPane.obj"
	-@erase "$(INTDIR)\OXPropertyDescriptionPane.sbr"
	-@erase "$(INTDIR)\OXPropertyTree.obj"
	-@erase "$(INTDIR)\OXPropertyTree.sbr"
	-@erase "$(INTDIR)\OXQuickString.obj"
	-@erase "$(INTDIR)\OXQuickString.sbr"
	-@erase "$(INTDIR)\OXRectTracker.obj"
	-@erase "$(INTDIR)\OXRectTracker.sbr"
	-@erase "$(INTDIR)\OXRegExpression.obj"
	-@erase "$(INTDIR)\OXRegExpression.sbr"
	-@erase "$(INTDIR)\OXRegistryItem.obj"
	-@erase "$(INTDIR)\OXRegistryItem.sbr"
	-@erase "$(INTDIR)\OXRegistryValFile.obj"
	-@erase "$(INTDIR)\OXRegistryValFile.sbr"
	-@erase "$(INTDIR)\OXRegistryWatcher.obj"
	-@erase "$(INTDIR)\OXRegistryWatcher.sbr"
	-@erase "$(INTDIR)\OXRegistryWatchNotifier.obj"
	-@erase "$(INTDIR)\OXRegistryWatchNotifier.sbr"
	-@erase "$(INTDIR)\OXResourceFile.obj"
	-@erase "$(INTDIR)\OXResourceFile.sbr"
	-@erase "$(INTDIR)\OXResourceLibrary.obj"
	-@erase "$(INTDIR)\OXResourceLibrary.sbr"
	-@erase "$(INTDIR)\OXRollup.obj"
	-@erase "$(INTDIR)\OXRollup.sbr"
	-@erase "$(INTDIR)\OXRoundedButton.obj"
	-@erase "$(INTDIR)\OXRoundedButton.sbr"
	-@erase "$(INTDIR)\OXRulerOrganizer.obj"
	-@erase "$(INTDIR)\OXRulerOrganizer.sbr"
	-@erase "$(INTDIR)\OXScreenGrab.obj"
	-@erase "$(INTDIR)\OXScreenGrab.sbr"
	-@erase "$(INTDIR)\OXScript.obj"
	-@erase "$(INTDIR)\OXScript.sbr"
	-@erase "$(INTDIR)\OXScrollTip.obj"
	-@erase "$(INTDIR)\OXScrollTip.sbr"
	-@erase "$(INTDIR)\OXScrollTipOwner.obj"
	-@erase "$(INTDIR)\OXScrollTipOwner.sbr"
	-@erase "$(INTDIR)\OXScrollWnd.obj"
	-@erase "$(INTDIR)\OXScrollWnd.sbr"
	-@erase "$(INTDIR)\OXSeparator.obj"
	-@erase "$(INTDIR)\OXSeparator.sbr"
	-@erase "$(INTDIR)\OXService.obj"
	-@erase "$(INTDIR)\OXService.sbr"
	-@erase "$(INTDIR)\OXShape.obj"
	-@erase "$(INTDIR)\OXShape.sbr"
	-@erase "$(INTDIR)\OXShdWnd.obj"
	-@erase "$(INTDIR)\OXShdWnd.sbr"
	-@erase "$(INTDIR)\OXShellFolderTree.obj"
	-@erase "$(INTDIR)\OXShellFolderTree.sbr"
	-@erase "$(INTDIR)\OXShellNamespaceNavigator.obj"
	-@erase "$(INTDIR)\OXShellNamespaceNavigator.sbr"
	-@erase "$(INTDIR)\OXShellObjectList.obj"
	-@erase "$(INTDIR)\OXShellObjectList.sbr"
	-@erase "$(INTDIR)\OXShortcut.obj"
	-@erase "$(INTDIR)\OXShortcut.sbr"
	-@erase "$(INTDIR)\OXShortcutBar.obj"
	-@erase "$(INTDIR)\OXShortcutBar.sbr"
	-@erase "$(INTDIR)\OXShortkeysOrganizer.obj"
	-@erase "$(INTDIR)\OXShortkeysOrganizer.sbr"
	-@erase "$(INTDIR)\OXSizeCtrlBar.obj"
	-@erase "$(INTDIR)\OXSizeCtrlBar.sbr"
	-@erase "$(INTDIR)\OXSizeDlgBar.obj"
	-@erase "$(INTDIR)\OXSizeDlgBar.sbr"
	-@erase "$(INTDIR)\OXSizeDockBar.obj"
	-@erase "$(INTDIR)\OXSizeDockBar.sbr"
	-@erase "$(INTDIR)\OXSizeToolBar.obj"
	-@erase "$(INTDIR)\OXSizeToolBar.sbr"
	-@erase "$(INTDIR)\OXSkinnedComboBox.obj"
	-@erase "$(INTDIR)\OXSkinnedComboBox.sbr"
	-@erase "$(INTDIR)\OXSkinnedSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSkinnedSpinCtrl.sbr"
	-@erase "$(INTDIR)\OXSkinnedTabCtrl.obj"
	-@erase "$(INTDIR)\OXSkinnedTabCtrl.sbr"
	-@erase "$(INTDIR)\OXSkins.obj"
	-@erase "$(INTDIR)\OXSkins.sbr"
	-@erase "$(INTDIR)\OXSound.obj"
	-@erase "$(INTDIR)\OXSound.sbr"
	-@erase "$(INTDIR)\OXSoundCustomizeDlg.obj"
	-@erase "$(INTDIR)\OXSoundCustomizeDlg.sbr"
	-@erase "$(INTDIR)\OXSoundEffectManager.obj"
	-@erase "$(INTDIR)\OXSoundEffectManager.sbr"
	-@erase "$(INTDIR)\OXSoundWnd.obj"
	-@erase "$(INTDIR)\OXSoundWnd.sbr"
	-@erase "$(INTDIR)\OXSpinCtrl.obj"
	-@erase "$(INTDIR)\OXSpinCtrl.sbr"
	-@erase "$(INTDIR)\OXSplashWnd.obj"
	-@erase "$(INTDIR)\OXSplashWnd.sbr"
	-@erase "$(INTDIR)\OXSplashWndDIB.obj"
	-@erase "$(INTDIR)\OXSplashWndDIB.sbr"
	-@erase "$(INTDIR)\OXSplitterColRowState.obj"
	-@erase "$(INTDIR)\OXSplitterColRowState.sbr"
	-@erase "$(INTDIR)\OXSplitterRect.obj"
	-@erase "$(INTDIR)\OXSplitterRect.sbr"
	-@erase "$(INTDIR)\OXSplitterWnd.obj"
	-@erase "$(INTDIR)\OXSplitterWnd.sbr"
	-@erase "$(INTDIR)\OXStatic.obj"
	-@erase "$(INTDIR)\OXStatic.sbr"
	-@erase "$(INTDIR)\OXStaticHyperLink.obj"
	-@erase "$(INTDIR)\OXStaticHyperLink.sbr"
	-@erase "$(INTDIR)\OXStaticText.obj"
	-@erase "$(INTDIR)\OXStaticText.sbr"
	-@erase "$(INTDIR)\OXSysInfo.obj"
	-@erase "$(INTDIR)\OXSysInfo.sbr"
	-@erase "$(INTDIR)\OXSzMiniDockFrmWnd.obj"
	-@erase "$(INTDIR)\OXSzMiniDockFrmWnd.sbr"
	-@erase "$(INTDIR)\OXTabClientWnd.obj"
	-@erase "$(INTDIR)\OXTabClientWnd.sbr"
	-@erase "$(INTDIR)\OXTabView.obj"
	-@erase "$(INTDIR)\OXTabView.sbr"
	-@erase "$(INTDIR)\OXTaskbarIcon.obj"
	-@erase "$(INTDIR)\OXTaskbarIcon.sbr"
	-@erase "$(INTDIR)\OXTleBar.obj"
	-@erase "$(INTDIR)\OXTleBar.sbr"
	-@erase "$(INTDIR)\OXToolTipCtrl.obj"
	-@erase "$(INTDIR)\OXToolTipCtrl.sbr"
	-@erase "$(INTDIR)\OXTrace.obj"
	-@erase "$(INTDIR)\OXTrace.sbr"
	-@erase "$(INTDIR)\OXTreeCal.obj"
	-@erase "$(INTDIR)\OXTreeCal.sbr"
	-@erase "$(INTDIR)\OXTreeCombo.obj"
	-@erase "$(INTDIR)\OXTreeCombo.sbr"
	-@erase "$(INTDIR)\OXTreeCtrl.obj"
	-@erase "$(INTDIR)\OXTreeCtrl.sbr"
	-@erase "$(INTDIR)\OXTreeEdit.obj"
	-@erase "$(INTDIR)\OXTreeEdit.sbr"
	-@erase "$(INTDIR)\OXTreeHeader.obj"
	-@erase "$(INTDIR)\OXTreeHeader.sbr"
	-@erase "$(INTDIR)\OXTreeItem.obj"
	-@erase "$(INTDIR)\OXTreeItem.sbr"
	-@erase "$(INTDIR)\OXUNC.obj"
	-@erase "$(INTDIR)\OXUNC.sbr"
	-@erase "$(INTDIR)\OXUNCStandardActor.obj"
	-@erase "$(INTDIR)\OXUNCStandardActor.sbr"
	-@erase "$(INTDIR)\OXURL.obj"
	-@erase "$(INTDIR)\OXURL.sbr"
	-@erase "$(INTDIR)\OXUsertl.obj"
	-@erase "$(INTDIR)\OXUsertl.sbr"
	-@erase "$(INTDIR)\OXUUDec.obj"
	-@erase "$(INTDIR)\OXUUDec.sbr"
	-@erase "$(INTDIR)\OXUxTheme.obj"
	-@erase "$(INTDIR)\OXUxTheme.sbr"
	-@erase "$(INTDIR)\OXWABook.obj"
	-@erase "$(INTDIR)\OXWABook.sbr"
	-@erase "$(INTDIR)\OXWatchedDir.obj"
	-@erase "$(INTDIR)\OXWatchedDir.sbr"
	-@erase "$(INTDIR)\oxwebctrl.obj"
	-@erase "$(INTDIR)\oxwebctrl.sbr"
	-@erase "$(INTDIR)\OXWorkspaceState.obj"
	-@erase "$(INTDIR)\OXWorkspaceState.sbr"
	-@erase "$(INTDIR)\Oxzoomvw.obj"
	-@erase "$(INTDIR)\Oxzoomvw.sbr"
	-@erase "$(INTDIR)\Path.obj"
	-@erase "$(INTDIR)\Path.sbr"
	-@erase "$(INTDIR)\PATHITER.OBJ"
	-@erase "$(INTDIR)\PATHITER.SBR"
	-@erase "$(INTDIR)\PROGRESS.OBJ"
	-@erase "$(INTDIR)\PROGRESS.SBR"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\UTStaticLib.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\WATCHBUF.SBR"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\Xstatus4.sbr"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "$(INTDIR)\XSTRING.SBR"
	-@erase "$(OUTDIR)\UTStaticLib.bsc"
	-@erase "..\libs\UTLibStaticDU.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W4 /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\UTStaticLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UTStaticLib.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CONVFILE.SBR" \
	"$(INTDIR)\COPYTREE.SBR" \
	"$(INTDIR)\CPYSTDLG.SBR" \
	"$(INTDIR)\DIR.SBR" \
	"$(INTDIR)\DIRLIST.SBR" \
	"$(INTDIR)\DSTRLIST.SBR" \
	"$(INTDIR)\EVNTLOG.SBR" \
	"$(INTDIR)\FILE.SBR" \
	"$(INTDIR)\FILELIST.SBR" \
	"$(INTDIR)\MDELDLG.SBR" \
	"$(INTDIR)\MFCToolBar.sbr" \
	"$(INTDIR)\OX3DTabView.sbr" \
	"$(INTDIR)\OXActiveDesktop.sbr" \
	"$(INTDIR)\OXAdvancedAssert.sbr" \
	"$(INTDIR)\OXAdvancedAssertMail.sbr" \
	"$(INTDIR)\OXAppBar.sbr" \
	"$(INTDIR)\OXAutoComplete.sbr" \
	"$(INTDIR)\OXAutoListBox.sbr" \
	"$(INTDIR)\OXBackgroundPainter.sbr" \
	"$(INTDIR)\OXBITBUF.SBR" \
	"$(INTDIR)\OXBitmapButton.sbr" \
	"$(INTDIR)\OXBitmapMenu.sbr" \
	"$(INTDIR)\OXBitmapMenuOrganizer.sbr" \
	"$(INTDIR)\OXBMPFilePickerCombo.sbr" \
	"$(INTDIR)\OXBMPFle.sbr" \
	"$(INTDIR)\OXBrowseColorEdit.sbr" \
	"$(INTDIR)\OXBrowseDirEdit.sbr" \
	"$(INTDIR)\OXBrowseDirEditList.sbr" \
	"$(INTDIR)\OXButtonAppearanceDlg.sbr" \
	"$(INTDIR)\OXCalculatorCtrl.sbr" \
	"$(INTDIR)\OXCalculatorEdit.sbr" \
	"$(INTDIR)\OXCalendar.sbr" \
	"$(INTDIR)\OXCalendarEdit.sbr" \
	"$(INTDIR)\OXCalendarPopup.sbr" \
	"$(INTDIR)\OXCaptionPainter.sbr" \
	"$(INTDIR)\OXChildFrameState.sbr" \
	"$(INTDIR)\OXClipPocket.sbr" \
	"$(INTDIR)\OXCMPFL.SBR" \
	"$(INTDIR)\OXColorPickerButton.sbr" \
	"$(INTDIR)\OXColorPickerCtrl.sbr" \
	"$(INTDIR)\OXComboPickerCtrl.sbr" \
	"$(INTDIR)\OXCOMPR.SBR" \
	"$(INTDIR)\OXContextHelpDialog.sbr" \
	"$(INTDIR)\OXContextHelpPropertyPage.sbr" \
	"$(INTDIR)\OXContextHelpPropertySheet.sbr" \
	"$(INTDIR)\OXCoolBar.sbr" \
	"$(INTDIR)\OXCoolComboBox.sbr" \
	"$(INTDIR)\OXCoolControlOrganizer.sbr" \
	"$(INTDIR)\OXCoolCtrl.sbr" \
	"$(INTDIR)\OXCoolToolBar.sbr" \
	"$(INTDIR)\OXCRCChk.sbr" \
	"$(INTDIR)\OXCsvFile.sbr" \
	"$(INTDIR)\OXCustomizeBackgroundPage.sbr" \
	"$(INTDIR)\OXCustomizeCaptionPage.sbr" \
	"$(INTDIR)\OXCustomizeCommandsPage.sbr" \
	"$(INTDIR)\OXCustomizeCoolControlsPage.sbr" \
	"$(INTDIR)\OXCustomizeInstanceManagerPage.sbr" \
	"$(INTDIR)\OXCustomizeIntelliMousePage.sbr" \
	"$(INTDIR)\OXCustomizeManager.sbr" \
	"$(INTDIR)\OXCustomizePage.sbr" \
	"$(INTDIR)\OXCustomizeShortkeysPage.sbr" \
	"$(INTDIR)\OXCustomizeSplashWindowPage.sbr" \
	"$(INTDIR)\OXCustomizeTabbedMDIPage.sbr" \
	"$(INTDIR)\OXCustomizeToolbarsPage.sbr" \
	"$(INTDIR)\OXCustomizeTrayIconPage.sbr" \
	"$(INTDIR)\OXCustomizeWorkspaceStatePage.sbr" \
	"$(INTDIR)\OXDao.sbr" \
	"$(INTDIR)\OXDateTimeCtrl.sbr" \
	"$(INTDIR)\OXDckPPg.sbr" \
	"$(INTDIR)\OXDckPSh.sbr" \
	"$(INTDIR)\OXDFlHdr.sbr" \
	"$(INTDIR)\Oxdib.sbr" \
	"$(INTDIR)\OXDirDlg.sbr" \
	"$(INTDIR)\OXDlgBar.sbr" \
	"$(INTDIR)\OXDLLManager.sbr" \
	"$(INTDIR)\oxdocktabctrl.sbr" \
	"$(INTDIR)\OXDocTemplateSpy.sbr" \
	"$(INTDIR)\OXDragDockContext.sbr" \
	"$(INTDIR)\OXDragDropCommands.sbr" \
	"$(INTDIR)\OXEdit.sbr" \
	"$(INTDIR)\OXEditList.sbr" \
	"$(INTDIR)\OXFileChanger.sbr" \
	"$(INTDIR)\OXFileWatcher.sbr" \
	"$(INTDIR)\OXFileWatchNotifier.sbr" \
	"$(INTDIR)\OXFolderPickerDlg.sbr" \
	"$(INTDIR)\OXFontComboBox.sbr" \
	"$(INTDIR)\OXFontPickerButton.sbr" \
	"$(INTDIR)\OXFrameWndDock.sbr" \
	"$(INTDIR)\OXFullScreenFrame.sbr" \
	"$(INTDIR)\OXGphFle.sbr" \
	"$(INTDIR)\OXGraphics.sbr" \
	"$(INTDIR)\oxgridedit.sbr" \
	"$(INTDIR)\OXGridHdr.sbr" \
	"$(INTDIR)\OXGridList.sbr" \
	"$(INTDIR)\OXHashTable.sbr" \
	"$(INTDIR)\OXHistoryCombo.sbr" \
	"$(INTDIR)\OXHistoryCtrl.sbr" \
	"$(INTDIR)\OXHookWnd.sbr" \
	"$(INTDIR)\OXHTMLParser.sbr" \
	"$(INTDIR)\OXHyperLinkAction.sbr" \
	"$(INTDIR)\OXImageListBox.sbr" \
	"$(INTDIR)\OXImageViewer.sbr" \
	"$(INTDIR)\OXInstanceManager.sbr" \
	"$(INTDIR)\OXInteger.sbr" \
	"$(INTDIR)\OXIntelliMouse.sbr" \
	"$(INTDIR)\OXIPEdit.sbr" \
	"$(INTDIR)\OXItemTip.sbr" \
	"$(INTDIR)\OXItemTipWnd.sbr" \
	"$(INTDIR)\OXIteratorRegistryItem.sbr" \
	"$(INTDIR)\OXIteratorService.sbr" \
	"$(INTDIR)\OXIteratorUNC.sbr" \
	"$(INTDIR)\OXJPGCod.sbr" \
	"$(INTDIR)\OXJPGCom.sbr" \
	"$(INTDIR)\OXJPGDom.sbr" \
	"$(INTDIR)\OXJPGExp.sbr" \
	"$(INTDIR)\OXJPGFle.sbr" \
	"$(INTDIR)\OXLayoutManager.sbr" \
	"$(INTDIR)\OXListBoxEx.sbr" \
	"$(INTDIR)\OXListEdit.sbr" \
	"$(INTDIR)\OXMaskedEdit.sbr" \
	"$(INTDIR)\OXMDIFloatWnd.sbr" \
	"$(INTDIR)\OXMenuBar.sbr" \
	"$(INTDIR)\OXMetaFile.sbr" \
	"$(INTDIR)\OXMonthCalCtrl.sbr" \
	"$(INTDIR)\OXMulticlipboardDlg.sbr" \
	"$(INTDIR)\OXMultiComboBox.sbr" \
	"$(INTDIR)\OXNetBrowseTree.sbr" \
	"$(INTDIR)\OXNotesEditView.sbr" \
	"$(INTDIR)\OXOptionTreeCtrl.sbr" \
	"$(INTDIR)\OXParser.sbr" \
	"$(INTDIR)\OXPhysicalEdit.sbr" \
	"$(INTDIR)\OXPhysicalEditEx.sbr" \
	"$(INTDIR)\OXPopupBarCtrl.sbr" \
	"$(INTDIR)\OXPreviewDialog.sbr" \
	"$(INTDIR)\OXProcess.sbr" \
	"$(INTDIR)\OXPropertiesWnd.sbr" \
	"$(INTDIR)\OXPropertyDescriptionPane.sbr" \
	"$(INTDIR)\OXPropertyTree.sbr" \
	"$(INTDIR)\OXQuickString.sbr" \
	"$(INTDIR)\OXRectTracker.sbr" \
	"$(INTDIR)\OXRegExpression.sbr" \
	"$(INTDIR)\OXRegistryItem.sbr" \
	"$(INTDIR)\OXRegistryValFile.sbr" \
	"$(INTDIR)\OXRegistryWatcher.sbr" \
	"$(INTDIR)\OXRegistryWatchNotifier.sbr" \
	"$(INTDIR)\OXResourceFile.sbr" \
	"$(INTDIR)\OXResourceLibrary.sbr" \
	"$(INTDIR)\OXRollup.sbr" \
	"$(INTDIR)\OXRoundedButton.sbr" \
	"$(INTDIR)\OXRulerOrganizer.sbr" \
	"$(INTDIR)\OXScreenGrab.sbr" \
	"$(INTDIR)\OXScript.sbr" \
	"$(INTDIR)\OXScrollTip.sbr" \
	"$(INTDIR)\OXScrollTipOwner.sbr" \
	"$(INTDIR)\OXScrollWnd.sbr" \
	"$(INTDIR)\OXSeparator.sbr" \
	"$(INTDIR)\OXService.sbr" \
	"$(INTDIR)\OXShape.sbr" \
	"$(INTDIR)\OXShdWnd.sbr" \
	"$(INTDIR)\OXShellFolderTree.sbr" \
	"$(INTDIR)\OXShellNamespaceNavigator.sbr" \
	"$(INTDIR)\OXShellObjectList.sbr" \
	"$(INTDIR)\OXShortcut.sbr" \
	"$(INTDIR)\OXShortcutBar.sbr" \
	"$(INTDIR)\OXShortkeysOrganizer.sbr" \
	"$(INTDIR)\OXSizeCtrlBar.sbr" \
	"$(INTDIR)\OXSizeDlgBar.sbr" \
	"$(INTDIR)\OXSizeDockBar.sbr" \
	"$(INTDIR)\OXSizeToolBar.sbr" \
	"$(INTDIR)\OXSkinnedComboBox.sbr" \
	"$(INTDIR)\OXSkinnedSpinCtrl.sbr" \
	"$(INTDIR)\OXSkinnedTabCtrl.sbr" \
	"$(INTDIR)\OXSkins.sbr" \
	"$(INTDIR)\OXSound.sbr" \
	"$(INTDIR)\OXSoundCustomizeDlg.sbr" \
	"$(INTDIR)\OXSoundEffectManager.sbr" \
	"$(INTDIR)\OXSoundWnd.sbr" \
	"$(INTDIR)\OXSpinCtrl.sbr" \
	"$(INTDIR)\OXSplashWnd.sbr" \
	"$(INTDIR)\OXSplashWndDIB.sbr" \
	"$(INTDIR)\OXSplitterColRowState.sbr" \
	"$(INTDIR)\OXSplitterRect.sbr" \
	"$(INTDIR)\OXSplitterWnd.sbr" \
	"$(INTDIR)\OXStatic.sbr" \
	"$(INTDIR)\OXStaticHyperLink.sbr" \
	"$(INTDIR)\OXStaticText.sbr" \
	"$(INTDIR)\OXSysInfo.sbr" \
	"$(INTDIR)\OXSzMiniDockFrmWnd.sbr" \
	"$(INTDIR)\OXTabClientWnd.sbr" \
	"$(INTDIR)\OXTabView.sbr" \
	"$(INTDIR)\OXTaskbarIcon.sbr" \
	"$(INTDIR)\OXTleBar.sbr" \
	"$(INTDIR)\OXToolTipCtrl.sbr" \
	"$(INTDIR)\OXTrace.sbr" \
	"$(INTDIR)\OXTreeCal.sbr" \
	"$(INTDIR)\OXTreeCombo.sbr" \
	"$(INTDIR)\OXTreeCtrl.sbr" \
	"$(INTDIR)\OXTreeEdit.sbr" \
	"$(INTDIR)\OXTreeHeader.sbr" \
	"$(INTDIR)\OXTreeItem.sbr" \
	"$(INTDIR)\OXUNC.sbr" \
	"$(INTDIR)\OXUNCStandardActor.sbr" \
	"$(INTDIR)\OXURL.sbr" \
	"$(INTDIR)\OXUsertl.sbr" \
	"$(INTDIR)\OXUUDec.sbr" \
	"$(INTDIR)\OXWABook.sbr" \
	"$(INTDIR)\OXWatchedDir.sbr" \
	"$(INTDIR)\oxwebctrl.sbr" \
	"$(INTDIR)\OXWorkspaceState.sbr" \
	"$(INTDIR)\Oxzoomvw.sbr" \
	"$(INTDIR)\Path.sbr" \
	"$(INTDIR)\PATHITER.SBR" \
	"$(INTDIR)\PROGRESS.SBR" \
	"$(INTDIR)\WATCHBUF.SBR" \
	"$(INTDIR)\Xstatus4.sbr" \
	"$(INTDIR)\XSTRING.SBR" \
	"$(INTDIR)\stdafx.sbr" \
	"$(INTDIR)\OXUxTheme.sbr"

"$(OUTDIR)\UTStaticLib.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\libs\UTLibStaticDU.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\OXUxTheme.obj"

"..\libs\UTLibStaticDU.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"

OUTDIR=.\UnicodeReleaseMFCStatic
INTDIR=.\UnicodeReleaseMFCStatic

ALL : "..\libs\UTLibStaticRU.lib"


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
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\UTStaticLib.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WATCHBUF.OBJ"
	-@erase "$(INTDIR)\Xstatus4.obj"
	-@erase "$(INTDIR)\XSTRING.OBJ"
	-@erase "..\libs\UTLibStaticRU.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\UTStaticLib.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UTStaticLib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\libs\UTLibStaticRU.lib" 
LIB32_OBJS= \
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
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\OXUxTheme.obj"

"..\libs\UTLibStaticRU.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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
!IF EXISTS("UTStaticLib.dep")
!INCLUDE "UTStaticLib.dep"
!ELSE 
!MESSAGE Warning: cannot find "UTStaticLib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared" || "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared" || "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic" || "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic" || "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared" || "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared" || "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic" || "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"
SOURCE=..\..\source\CONVFILE.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\CONVFILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\CONVFILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\CONVFILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\CONVFILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\CONVFILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\CONVFILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\CONVFILE.OBJ"	"$(INTDIR)\CONVFILE.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\CONVFILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\COPYTREE.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\COPYTREE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\COPYTREE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\COPYTREE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\COPYTREE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\COPYTREE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\COPYTREE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\COPYTREE.OBJ"	"$(INTDIR)\COPYTREE.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\COPYTREE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\CPYSTDLG.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\CPYSTDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\CPYSTDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\CPYSTDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\CPYSTDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\CPYSTDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\CPYSTDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\CPYSTDLG.OBJ"	"$(INTDIR)\CPYSTDLG.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\CPYSTDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\DIR.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\DIR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\DIR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\DIR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\DIR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\DIR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\DIR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\DIR.OBJ"	"$(INTDIR)\DIR.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\DIR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\DIRLIST.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\DIRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\DIRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\DIRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\DIRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\DIRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\DIRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\DIRLIST.OBJ"	"$(INTDIR)\DIRLIST.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\DIRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\DSTRLIST.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\DSTRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\DSTRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\DSTRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\DSTRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\DSTRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\DSTRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\DSTRLIST.OBJ"	"$(INTDIR)\DSTRLIST.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\DSTRLIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\EVNTLOG.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\EVNTLOG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\EVNTLOG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\EVNTLOG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\EVNTLOG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\EVNTLOG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\EVNTLOG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\EVNTLOG.OBJ"	"$(INTDIR)\EVNTLOG.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\EVNTLOG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\FILE.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\FILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\FILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\FILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\FILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\FILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\FILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\FILE.OBJ"	"$(INTDIR)\FILE.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\FILE.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\FILELIST.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\FILELIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\FILELIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\FILELIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\FILELIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\FILELIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\FILELIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\FILELIST.OBJ"	"$(INTDIR)\FILELIST.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\FILELIST.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\MDELDLG.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\MDELDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\MDELDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\MDELDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\MDELDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\MDELDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\MDELDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\MDELDLG.OBJ"	"$(INTDIR)\MDELDLG.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\MDELDLG.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\MFCToolBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\MFCToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\MFCToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\MFCToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\MFCToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\MFCToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\MFCToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\MFCToolBar.obj"	"$(INTDIR)\MFCToolBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\MFCToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OX3DTabView.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OX3DTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OX3DTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OX3DTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OX3DTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OX3DTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OX3DTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OX3DTabView.obj"	"$(INTDIR)\OX3DTabView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OX3DTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXActiveDesktop.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXActiveDesktop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXActiveDesktop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXActiveDesktop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXActiveDesktop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXActiveDesktop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXActiveDesktop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXActiveDesktop.obj"	"$(INTDIR)\OXActiveDesktop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXActiveDesktop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXAdvancedAssert.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXAdvancedAssert.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXAdvancedAssert.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXAdvancedAssert.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXAdvancedAssert.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXAdvancedAssert.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXAdvancedAssert.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXAdvancedAssert.obj"	"$(INTDIR)\OXAdvancedAssert.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXAdvancedAssert.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXAdvancedAssertMail.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXAdvancedAssertMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXAdvancedAssertMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXAdvancedAssertMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXAdvancedAssertMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXAdvancedAssertMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXAdvancedAssertMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXAdvancedAssertMail.obj"	"$(INTDIR)\OXAdvancedAssertMail.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXAdvancedAssertMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXAppBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXAppBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXAppBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXAppBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXAppBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXAppBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXAppBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXAppBar.obj"	"$(INTDIR)\OXAppBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXAppBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXAutoComplete.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXAutoComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXAutoComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXAutoComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXAutoComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXAutoComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXAutoComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXAutoComplete.obj"	"$(INTDIR)\OXAutoComplete.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXAutoComplete.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXAutoListBox.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXAutoListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXAutoListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXAutoListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXAutoListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXAutoListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXAutoListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXAutoListBox.obj"	"$(INTDIR)\OXAutoListBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXAutoListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBackgroundPainter.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBackgroundPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBackgroundPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBackgroundPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBackgroundPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBackgroundPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBackgroundPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBackgroundPainter.obj"	"$(INTDIR)\OXBackgroundPainter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBackgroundPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBITBUF.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBITBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBITBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBITBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBITBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBITBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBITBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBITBUF.OBJ"	"$(INTDIR)\OXBITBUF.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBITBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBitmapButton.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBitmapButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBitmapButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBitmapButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBitmapButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBitmapButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBitmapButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBitmapButton.obj"	"$(INTDIR)\OXBitmapButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBitmapButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBitmapMenu.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBitmapMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBitmapMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBitmapMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBitmapMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBitmapMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBitmapMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBitmapMenu.obj"	"$(INTDIR)\OXBitmapMenu.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBitmapMenu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBitmapMenuOrganizer.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBitmapMenuOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBitmapMenuOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBitmapMenuOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBitmapMenuOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBitmapMenuOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBitmapMenuOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBitmapMenuOrganizer.obj"	"$(INTDIR)\OXBitmapMenuOrganizer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBitmapMenuOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBMPFilePickerCombo.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBMPFilePickerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBMPFilePickerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBMPFilePickerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBMPFilePickerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBMPFilePickerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBMPFilePickerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBMPFilePickerCombo.obj"	"$(INTDIR)\OXBMPFilePickerCombo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBMPFilePickerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBMPFle.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBMPFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBMPFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBMPFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBMPFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBMPFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBMPFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBMPFle.obj"	"$(INTDIR)\OXBMPFle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBMPFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBrowseColorEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBrowseColorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBrowseColorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBrowseColorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBrowseColorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBrowseColorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBrowseColorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBrowseColorEdit.obj"	"$(INTDIR)\OXBrowseColorEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBrowseColorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBrowseDirEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBrowseDirEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBrowseDirEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBrowseDirEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBrowseDirEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBrowseDirEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBrowseDirEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBrowseDirEdit.obj"	"$(INTDIR)\OXBrowseDirEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBrowseDirEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXBrowseDirEditList.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXBrowseDirEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXBrowseDirEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXBrowseDirEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXBrowseDirEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXBrowseDirEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXBrowseDirEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXBrowseDirEditList.obj"	"$(INTDIR)\OXBrowseDirEditList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXBrowseDirEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXButtonAppearanceDlg.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXButtonAppearanceDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXButtonAppearanceDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXButtonAppearanceDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXButtonAppearanceDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXButtonAppearanceDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXButtonAppearanceDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXButtonAppearanceDlg.obj"	"$(INTDIR)\OXButtonAppearanceDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXButtonAppearanceDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCalculatorCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCalculatorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCalculatorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCalculatorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCalculatorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCalculatorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCalculatorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCalculatorCtrl.obj"	"$(INTDIR)\OXCalculatorCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCalculatorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCalculatorEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCalculatorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCalculatorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCalculatorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCalculatorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCalculatorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCalculatorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCalculatorEdit.obj"	"$(INTDIR)\OXCalculatorEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCalculatorEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCalendar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCalendar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCalendar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCalendar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCalendar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCalendar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCalendar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCalendar.obj"	"$(INTDIR)\OXCalendar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCalendar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCalendarEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCalendarEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCalendarEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCalendarEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCalendarEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCalendarEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCalendarEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCalendarEdit.obj"	"$(INTDIR)\OXCalendarEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCalendarEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCalendarPopup.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCalendarPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCalendarPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCalendarPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCalendarPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCalendarPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCalendarPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCalendarPopup.obj"	"$(INTDIR)\OXCalendarPopup.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCalendarPopup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCaptionPainter.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCaptionPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCaptionPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCaptionPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCaptionPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCaptionPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCaptionPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCaptionPainter.obj"	"$(INTDIR)\OXCaptionPainter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCaptionPainter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXChildFrameState.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXChildFrameState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXChildFrameState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXChildFrameState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXChildFrameState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXChildFrameState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXChildFrameState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXChildFrameState.obj"	"$(INTDIR)\OXChildFrameState.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXChildFrameState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXClipPocket.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXClipPocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXClipPocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXClipPocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXClipPocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXClipPocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXClipPocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXClipPocket.obj"	"$(INTDIR)\OXClipPocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXClipPocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCMPFL.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCMPFL.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCMPFL.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCMPFL.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCMPFL.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCMPFL.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCMPFL.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCMPFL.OBJ"	"$(INTDIR)\OXCMPFL.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCMPFL.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXColorPickerButton.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXColorPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXColorPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXColorPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXColorPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXColorPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXColorPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXColorPickerButton.obj"	"$(INTDIR)\OXColorPickerButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXColorPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXColorPickerCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXColorPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXColorPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXColorPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXColorPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXColorPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXColorPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXColorPickerCtrl.obj"	"$(INTDIR)\OXColorPickerCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXColorPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXComboPickerCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXComboPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXComboPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXComboPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXComboPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXComboPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXComboPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXComboPickerCtrl.obj"	"$(INTDIR)\OXComboPickerCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXComboPickerCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCOMPR.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCOMPR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCOMPR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCOMPR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCOMPR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCOMPR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCOMPR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCOMPR.OBJ"	"$(INTDIR)\OXCOMPR.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCOMPR.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXContextHelpDialog.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXContextHelpDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXContextHelpDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXContextHelpDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXContextHelpDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXContextHelpDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXContextHelpDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXContextHelpDialog.obj"	"$(INTDIR)\OXContextHelpDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXContextHelpDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXContextHelpPropertyPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXContextHelpPropertyPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXContextHelpPropertyPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXContextHelpPropertyPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXContextHelpPropertyPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXContextHelpPropertyPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXContextHelpPropertyPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXContextHelpPropertyPage.obj"	"$(INTDIR)\OXContextHelpPropertyPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXContextHelpPropertyPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXContextHelpPropertySheet.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXContextHelpPropertySheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXContextHelpPropertySheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXContextHelpPropertySheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXContextHelpPropertySheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXContextHelpPropertySheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXContextHelpPropertySheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXContextHelpPropertySheet.obj"	"$(INTDIR)\OXContextHelpPropertySheet.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXContextHelpPropertySheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCoolBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCoolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCoolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCoolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCoolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCoolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCoolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCoolBar.obj"	"$(INTDIR)\OXCoolBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCoolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCoolComboBox.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCoolComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCoolComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCoolComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCoolComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCoolComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCoolComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCoolComboBox.obj"	"$(INTDIR)\OXCoolComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCoolComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCoolControlOrganizer.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCoolControlOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCoolControlOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCoolControlOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCoolControlOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCoolControlOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCoolControlOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCoolControlOrganizer.obj"	"$(INTDIR)\OXCoolControlOrganizer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCoolControlOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCoolCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCoolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCoolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCoolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCoolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCoolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCoolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCoolCtrl.obj"	"$(INTDIR)\OXCoolCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCoolCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCoolToolBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCoolToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCoolToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCoolToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCoolToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCoolToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCoolToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCoolToolBar.obj"	"$(INTDIR)\OXCoolToolBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCoolToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCRCChk.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCRCChk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCRCChk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCRCChk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCRCChk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCRCChk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCRCChk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCRCChk.obj"	"$(INTDIR)\OXCRCChk.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCRCChk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCsvFile.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCsvFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCsvFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCsvFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCsvFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCsvFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCsvFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCsvFile.obj"	"$(INTDIR)\OXCsvFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCsvFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeBackgroundPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeBackgroundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeBackgroundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeBackgroundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeBackgroundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeBackgroundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeBackgroundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeBackgroundPage.obj"	"$(INTDIR)\OXCustomizeBackgroundPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeBackgroundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeCaptionPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeCaptionPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeCaptionPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeCaptionPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeCaptionPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeCaptionPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeCaptionPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeCaptionPage.obj"	"$(INTDIR)\OXCustomizeCaptionPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeCaptionPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeCommandsPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeCommandsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeCommandsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeCommandsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeCommandsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeCommandsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeCommandsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeCommandsPage.obj"	"$(INTDIR)\OXCustomizeCommandsPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeCommandsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeCoolControlsPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeCoolControlsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeCoolControlsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeCoolControlsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeCoolControlsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeCoolControlsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeCoolControlsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeCoolControlsPage.obj"	"$(INTDIR)\OXCustomizeCoolControlsPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeCoolControlsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeInstanceManagerPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeInstanceManagerPage.obj"	"$(INTDIR)\OXCustomizeInstanceManagerPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeInstanceManagerPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeIntelliMousePage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeIntelliMousePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeIntelliMousePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeIntelliMousePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeIntelliMousePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeIntelliMousePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeIntelliMousePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeIntelliMousePage.obj"	"$(INTDIR)\OXCustomizeIntelliMousePage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeIntelliMousePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeManager.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeManager.obj"	"$(INTDIR)\OXCustomizeManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizePage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizePage.obj"	"$(INTDIR)\OXCustomizePage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeShortkeysPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeShortkeysPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeShortkeysPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeShortkeysPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeShortkeysPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeShortkeysPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeShortkeysPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeShortkeysPage.obj"	"$(INTDIR)\OXCustomizeShortkeysPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeShortkeysPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeSplashWindowPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeSplashWindowPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeSplashWindowPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeSplashWindowPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeSplashWindowPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeSplashWindowPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeSplashWindowPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeSplashWindowPage.obj"	"$(INTDIR)\OXCustomizeSplashWindowPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeSplashWindowPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeTabbedMDIPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeTabbedMDIPage.obj"	"$(INTDIR)\OXCustomizeTabbedMDIPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeTabbedMDIPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeToolbarsPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeToolbarsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeToolbarsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeToolbarsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeToolbarsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeToolbarsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeToolbarsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeToolbarsPage.obj"	"$(INTDIR)\OXCustomizeToolbarsPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeToolbarsPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeTrayIconPage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeTrayIconPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeTrayIconPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeTrayIconPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeTrayIconPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeTrayIconPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeTrayIconPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeTrayIconPage.obj"	"$(INTDIR)\OXCustomizeTrayIconPage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeTrayIconPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXCustomizeWorkspaceStatePage.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj"	"$(INTDIR)\OXCustomizeWorkspaceStatePage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXCustomizeWorkspaceStatePage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDao.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDao.obj"	"$(INTDIR)\OXDao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDateTimeCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDateTimeCtrl.obj"	"$(INTDIR)\OXDateTimeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDateTimeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDckPPg.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDckPPg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDckPPg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDckPPg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDckPPg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDckPPg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDckPPg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDckPPg.obj"	"$(INTDIR)\OXDckPPg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDckPPg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDckPSh.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDckPSh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDckPSh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDckPSh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDckPSh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDckPSh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDckPSh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDckPSh.obj"	"$(INTDIR)\OXDckPSh.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDckPSh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDFlHdr.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDFlHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDFlHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDFlHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDFlHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDFlHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDFlHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDFlHdr.obj"	"$(INTDIR)\OXDFlHdr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDFlHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\Oxdib.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\Oxdib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\Oxdib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\Oxdib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\Oxdib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\Oxdib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\Oxdib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\Oxdib.obj"	"$(INTDIR)\Oxdib.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\Oxdib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDirDlg.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDirDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDirDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDirDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDirDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDirDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDirDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDirDlg.obj"	"$(INTDIR)\OXDirDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDirDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDlgBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDlgBar.obj"	"$(INTDIR)\OXDlgBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDLLManager.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDLLManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDLLManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDLLManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDLLManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDLLManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDLLManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDLLManager.obj"	"$(INTDIR)\OXDLLManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDLLManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\oxdocktabctrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\oxdocktabctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\oxdocktabctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\oxdocktabctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\oxdocktabctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\oxdocktabctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\oxdocktabctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\oxdocktabctrl.obj"	"$(INTDIR)\oxdocktabctrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\oxdocktabctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDocTemplateSpy.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDocTemplateSpy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDocTemplateSpy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDocTemplateSpy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDocTemplateSpy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDocTemplateSpy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDocTemplateSpy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDocTemplateSpy.obj"	"$(INTDIR)\OXDocTemplateSpy.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDocTemplateSpy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDragDockContext.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDragDockContext.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDragDockContext.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDragDockContext.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDragDockContext.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDragDockContext.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDragDockContext.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDragDockContext.obj"	"$(INTDIR)\OXDragDockContext.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDragDockContext.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXDragDropCommands.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXDragDropCommands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXDragDropCommands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXDragDropCommands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXDragDropCommands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXDragDropCommands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXDragDropCommands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXDragDropCommands.obj"	"$(INTDIR)\OXDragDropCommands.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXDragDropCommands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXEdit.obj"	"$(INTDIR)\OXEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXEditList.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXEditList.obj"	"$(INTDIR)\OXEditList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXEditList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXFileChanger.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXFileChanger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXFileChanger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXFileChanger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXFileChanger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXFileChanger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXFileChanger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXFileChanger.obj"	"$(INTDIR)\OXFileChanger.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXFileChanger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXFileWatcher.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXFileWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXFileWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXFileWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXFileWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXFileWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXFileWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXFileWatcher.obj"	"$(INTDIR)\OXFileWatcher.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXFileWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXFileWatchNotifier.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXFileWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXFileWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXFileWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXFileWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXFileWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXFileWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXFileWatchNotifier.obj"	"$(INTDIR)\OXFileWatchNotifier.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXFileWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXFolderPickerDlg.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXFolderPickerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXFolderPickerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXFolderPickerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXFolderPickerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXFolderPickerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXFolderPickerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXFolderPickerDlg.obj"	"$(INTDIR)\OXFolderPickerDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXFolderPickerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXFontComboBox.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXFontComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXFontComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXFontComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXFontComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXFontComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXFontComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXFontComboBox.obj"	"$(INTDIR)\OXFontComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXFontComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXFontPickerButton.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXFontPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXFontPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXFontPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXFontPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXFontPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXFontPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXFontPickerButton.obj"	"$(INTDIR)\OXFontPickerButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXFontPickerButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXFrameWndDock.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXFrameWndDock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXFrameWndDock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXFrameWndDock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXFrameWndDock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXFrameWndDock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXFrameWndDock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXFrameWndDock.obj"	"$(INTDIR)\OXFrameWndDock.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXFrameWndDock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXFullScreenFrame.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXFullScreenFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXFullScreenFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXFullScreenFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXFullScreenFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXFullScreenFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXFullScreenFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXFullScreenFrame.obj"	"$(INTDIR)\OXFullScreenFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXFullScreenFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXGphFle.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXGphFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXGphFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXGphFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXGphFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXGphFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXGphFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXGphFle.obj"	"$(INTDIR)\OXGphFle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXGphFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXGraphics.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXGraphics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXGraphics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXGraphics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXGraphics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXGraphics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXGraphics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXGraphics.obj"	"$(INTDIR)\OXGraphics.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXGraphics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\oxgridedit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\oxgridedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\oxgridedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\oxgridedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\oxgridedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\oxgridedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\oxgridedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\oxgridedit.obj"	"$(INTDIR)\oxgridedit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\oxgridedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXGridHdr.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXGridHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXGridHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXGridHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXGridHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXGridHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXGridHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXGridHdr.obj"	"$(INTDIR)\OXGridHdr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXGridHdr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXGridList.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXGridList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXGridList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXGridList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXGridList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXGridList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXGridList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXGridList.obj"	"$(INTDIR)\OXGridList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXGridList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXHashTable.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXHashTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXHashTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXHashTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXHashTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXHashTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXHashTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXHashTable.obj"	"$(INTDIR)\OXHashTable.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXHashTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXHistoryCombo.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXHistoryCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXHistoryCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXHistoryCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXHistoryCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXHistoryCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXHistoryCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXHistoryCombo.obj"	"$(INTDIR)\OXHistoryCombo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXHistoryCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXHistoryCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXHistoryCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXHistoryCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXHistoryCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXHistoryCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXHistoryCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXHistoryCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXHistoryCtrl.obj"	"$(INTDIR)\OXHistoryCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXHistoryCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXHookWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXHookWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXHookWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXHookWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXHookWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXHookWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXHookWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXHookWnd.obj"	"$(INTDIR)\OXHookWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXHookWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXHTMLParser.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXHTMLParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXHTMLParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXHTMLParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXHTMLParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXHTMLParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXHTMLParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXHTMLParser.obj"	"$(INTDIR)\OXHTMLParser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXHTMLParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXHyperLinkAction.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXHyperLinkAction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXHyperLinkAction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXHyperLinkAction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXHyperLinkAction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXHyperLinkAction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXHyperLinkAction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXHyperLinkAction.obj"	"$(INTDIR)\OXHyperLinkAction.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXHyperLinkAction.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXImageListBox.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXImageListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXImageListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXImageListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXImageListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXImageListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXImageListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXImageListBox.obj"	"$(INTDIR)\OXImageListBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXImageListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXImageViewer.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXImageViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXImageViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXImageViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXImageViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXImageViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXImageViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXImageViewer.obj"	"$(INTDIR)\OXImageViewer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXImageViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXInstanceManager.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXInstanceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXInstanceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXInstanceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXInstanceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXInstanceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXInstanceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXInstanceManager.obj"	"$(INTDIR)\OXInstanceManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXInstanceManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXInteger.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXInteger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXInteger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXInteger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXInteger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXInteger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXInteger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXInteger.obj"	"$(INTDIR)\OXInteger.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXInteger.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXIntelliMouse.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXIntelliMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXIntelliMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXIntelliMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXIntelliMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXIntelliMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXIntelliMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXIntelliMouse.obj"	"$(INTDIR)\OXIntelliMouse.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXIntelliMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXIPEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXIPEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXIPEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXIPEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXIPEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXIPEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXIPEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXIPEdit.obj"	"$(INTDIR)\OXIPEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXIPEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXItemTip.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXItemTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXItemTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXItemTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXItemTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXItemTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXItemTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXItemTip.obj"	"$(INTDIR)\OXItemTip.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXItemTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXItemTipWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXItemTipWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXItemTipWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXItemTipWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXItemTipWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXItemTipWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXItemTipWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXItemTipWnd.obj"	"$(INTDIR)\OXItemTipWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXItemTipWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXIteratorRegistryItem.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXIteratorRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXIteratorRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXIteratorRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXIteratorRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXIteratorRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXIteratorRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXIteratorRegistryItem.obj"	"$(INTDIR)\OXIteratorRegistryItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXIteratorRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXIteratorService.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXIteratorService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXIteratorService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXIteratorService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXIteratorService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXIteratorService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXIteratorService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXIteratorService.obj"	"$(INTDIR)\OXIteratorService.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXIteratorService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXIteratorUNC.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXIteratorUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXIteratorUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXIteratorUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXIteratorUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXIteratorUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXIteratorUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXIteratorUNC.obj"	"$(INTDIR)\OXIteratorUNC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXIteratorUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXJPGCod.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXJPGCod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXJPGCod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXJPGCod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXJPGCod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXJPGCod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXJPGCod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXJPGCod.obj"	"$(INTDIR)\OXJPGCod.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXJPGCod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXJPGCom.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXJPGCom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXJPGCom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXJPGCom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXJPGCom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXJPGCom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXJPGCom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXJPGCom.obj"	"$(INTDIR)\OXJPGCom.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXJPGCom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXJPGDom.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXJPGDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXJPGDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXJPGDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXJPGDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXJPGDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXJPGDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXJPGDom.obj"	"$(INTDIR)\OXJPGDom.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXJPGDom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXJPGExp.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXJPGExp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXJPGExp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXJPGExp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXJPGExp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXJPGExp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXJPGExp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXJPGExp.obj"	"$(INTDIR)\OXJPGExp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXJPGExp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXJPGFle.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXJPGFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXJPGFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXJPGFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXJPGFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXJPGFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXJPGFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXJPGFle.obj"	"$(INTDIR)\OXJPGFle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXJPGFle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXLayoutManager.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXLayoutManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXLayoutManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXLayoutManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXLayoutManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXLayoutManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXLayoutManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXLayoutManager.obj"	"$(INTDIR)\OXLayoutManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXLayoutManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXListBoxEx.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXListBoxEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXListBoxEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXListBoxEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXListBoxEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXListBoxEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXListBoxEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXListBoxEx.obj"	"$(INTDIR)\OXListBoxEx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXListBoxEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXListEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXListEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXListEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXListEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXListEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXListEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXListEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXListEdit.obj"	"$(INTDIR)\OXListEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXListEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXMaskedEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXMaskedEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXMaskedEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXMaskedEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXMaskedEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXMaskedEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXMaskedEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXMaskedEdit.obj"	"$(INTDIR)\OXMaskedEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXMaskedEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXMDIFloatWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXMDIFloatWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXMDIFloatWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXMDIFloatWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXMDIFloatWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXMDIFloatWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXMDIFloatWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXMDIFloatWnd.obj"	"$(INTDIR)\OXMDIFloatWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXMDIFloatWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXMenuBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXMenuBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXMenuBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXMenuBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXMenuBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXMenuBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXMenuBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXMenuBar.obj"	"$(INTDIR)\OXMenuBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXMenuBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXMetaFile.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXMetaFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXMetaFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXMetaFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXMetaFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXMetaFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXMetaFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXMetaFile.obj"	"$(INTDIR)\OXMetaFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXMetaFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXMonthCalCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXMonthCalCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXMonthCalCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXMonthCalCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXMonthCalCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXMonthCalCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXMonthCalCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXMonthCalCtrl.obj"	"$(INTDIR)\OXMonthCalCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXMonthCalCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXMulticlipboardDlg.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXMulticlipboardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXMulticlipboardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXMulticlipboardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXMulticlipboardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXMulticlipboardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXMulticlipboardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXMulticlipboardDlg.obj"	"$(INTDIR)\OXMulticlipboardDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXMulticlipboardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXMultiComboBox.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXMultiComboBox.obj"	"$(INTDIR)\OXMultiComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXMultiComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXNetBrowseTree.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXNetBrowseTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXNetBrowseTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXNetBrowseTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXNetBrowseTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXNetBrowseTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXNetBrowseTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXNetBrowseTree.obj"	"$(INTDIR)\OXNetBrowseTree.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXNetBrowseTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXNotesEditView.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXNotesEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXNotesEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXNotesEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXNotesEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXNotesEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXNotesEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXNotesEditView.obj"	"$(INTDIR)\OXNotesEditView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXNotesEditView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXOptionTreeCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXOptionTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXOptionTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXOptionTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXOptionTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXOptionTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXOptionTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXOptionTreeCtrl.obj"	"$(INTDIR)\OXOptionTreeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXOptionTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXParser.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXParser.obj"	"$(INTDIR)\OXParser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXParser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXPhysicalEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXPhysicalEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXPhysicalEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXPhysicalEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXPhysicalEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXPhysicalEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXPhysicalEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXPhysicalEdit.obj"	"$(INTDIR)\OXPhysicalEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXPhysicalEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXPhysicalEditEx.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXPhysicalEditEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXPhysicalEditEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXPhysicalEditEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXPhysicalEditEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXPhysicalEditEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXPhysicalEditEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXPhysicalEditEx.obj"	"$(INTDIR)\OXPhysicalEditEx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXPhysicalEditEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXPopupBarCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXPopupBarCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXPopupBarCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXPopupBarCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXPopupBarCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXPopupBarCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXPopupBarCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXPopupBarCtrl.obj"	"$(INTDIR)\OXPopupBarCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXPopupBarCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXPreviewDialog.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXPreviewDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXPreviewDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXPreviewDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXPreviewDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXPreviewDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXPreviewDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXPreviewDialog.obj"	"$(INTDIR)\OXPreviewDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXPreviewDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXProcess.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXProcess.obj"	"$(INTDIR)\OXProcess.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXPropertiesWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXPropertiesWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXPropertiesWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXPropertiesWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXPropertiesWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXPropertiesWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXPropertiesWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXPropertiesWnd.obj"	"$(INTDIR)\OXPropertiesWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXPropertiesWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXPropertyDescriptionPane.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXPropertyDescriptionPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXPropertyDescriptionPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXPropertyDescriptionPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXPropertyDescriptionPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXPropertyDescriptionPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXPropertyDescriptionPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXPropertyDescriptionPane.obj"	"$(INTDIR)\OXPropertyDescriptionPane.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXPropertyDescriptionPane.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXPropertyTree.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXPropertyTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXPropertyTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXPropertyTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXPropertyTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXPropertyTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXPropertyTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXPropertyTree.obj"	"$(INTDIR)\OXPropertyTree.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXPropertyTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXQuickString.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXQuickString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXQuickString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXQuickString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXQuickString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXQuickString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXQuickString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXQuickString.obj"	"$(INTDIR)\OXQuickString.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXQuickString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRectTracker.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRectTracker.obj"	"$(INTDIR)\OXRectTracker.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRectTracker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRegExpression.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRegExpression.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRegExpression.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRegExpression.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRegExpression.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRegExpression.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRegExpression.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRegExpression.obj"	"$(INTDIR)\OXRegExpression.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRegExpression.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRegistryItem.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRegistryItem.obj"	"$(INTDIR)\OXRegistryItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRegistryItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRegistryValFile.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRegistryValFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRegistryValFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRegistryValFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRegistryValFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRegistryValFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRegistryValFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRegistryValFile.obj"	"$(INTDIR)\OXRegistryValFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRegistryValFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRegistryWatcher.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRegistryWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRegistryWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRegistryWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRegistryWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRegistryWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRegistryWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRegistryWatcher.obj"	"$(INTDIR)\OXRegistryWatcher.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRegistryWatcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRegistryWatchNotifier.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRegistryWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRegistryWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRegistryWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRegistryWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRegistryWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRegistryWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRegistryWatchNotifier.obj"	"$(INTDIR)\OXRegistryWatchNotifier.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRegistryWatchNotifier.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXResourceFile.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXResourceFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXResourceFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXResourceFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXResourceFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXResourceFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXResourceFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXResourceFile.obj"	"$(INTDIR)\OXResourceFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXResourceFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXResourceLibrary.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXResourceLibrary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXResourceLibrary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXResourceLibrary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXResourceLibrary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXResourceLibrary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXResourceLibrary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXResourceLibrary.obj"	"$(INTDIR)\OXResourceLibrary.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXResourceLibrary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRollup.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRollup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRollup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRollup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRollup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRollup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRollup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRollup.obj"	"$(INTDIR)\OXRollup.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRollup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRoundedButton.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRoundedButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRoundedButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRoundedButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRoundedButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRoundedButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRoundedButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRoundedButton.obj"	"$(INTDIR)\OXRoundedButton.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRoundedButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXRulerOrganizer.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXRulerOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXRulerOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXRulerOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXRulerOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXRulerOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXRulerOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXRulerOrganizer.obj"	"$(INTDIR)\OXRulerOrganizer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXRulerOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXScreenGrab.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXScreenGrab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXScreenGrab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXScreenGrab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXScreenGrab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXScreenGrab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXScreenGrab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXScreenGrab.obj"	"$(INTDIR)\OXScreenGrab.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXScreenGrab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXScript.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXScript.obj"	"$(INTDIR)\OXScript.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXScrollTip.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXScrollTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXScrollTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXScrollTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXScrollTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXScrollTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXScrollTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXScrollTip.obj"	"$(INTDIR)\OXScrollTip.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXScrollTip.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXScrollTipOwner.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXScrollTipOwner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXScrollTipOwner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXScrollTipOwner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXScrollTipOwner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXScrollTipOwner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXScrollTipOwner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXScrollTipOwner.obj"	"$(INTDIR)\OXScrollTipOwner.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXScrollTipOwner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXScrollWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXScrollWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXScrollWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXScrollWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXScrollWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXScrollWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXScrollWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXScrollWnd.obj"	"$(INTDIR)\OXScrollWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXScrollWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSeparator.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSeparator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSeparator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSeparator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSeparator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSeparator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSeparator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSeparator.obj"	"$(INTDIR)\OXSeparator.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSeparator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXService.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXService.obj"	"$(INTDIR)\OXService.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXService.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXShape.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXShape.obj"	"$(INTDIR)\OXShape.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXShdWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXShdWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXShdWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXShdWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXShdWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXShdWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXShdWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXShdWnd.obj"	"$(INTDIR)\OXShdWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXShdWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXShellFolderTree.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXShellFolderTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXShellFolderTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXShellFolderTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXShellFolderTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXShellFolderTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXShellFolderTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXShellFolderTree.obj"	"$(INTDIR)\OXShellFolderTree.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXShellFolderTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXShellNamespaceNavigator.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXShellNamespaceNavigator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXShellNamespaceNavigator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXShellNamespaceNavigator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXShellNamespaceNavigator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXShellNamespaceNavigator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXShellNamespaceNavigator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXShellNamespaceNavigator.obj"	"$(INTDIR)\OXShellNamespaceNavigator.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXShellNamespaceNavigator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXShellObjectList.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXShellObjectList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXShellObjectList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXShellObjectList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXShellObjectList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXShellObjectList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXShellObjectList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXShellObjectList.obj"	"$(INTDIR)\OXShellObjectList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXShellObjectList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXShortcut.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXShortcut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXShortcut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXShortcut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXShortcut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXShortcut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXShortcut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXShortcut.obj"	"$(INTDIR)\OXShortcut.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXShortcut.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXShortcutBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXShortcutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXShortcutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXShortcutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXShortcutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXShortcutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXShortcutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXShortcutBar.obj"	"$(INTDIR)\OXShortcutBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXShortcutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXShortkeysOrganizer.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXShortkeysOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXShortkeysOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXShortkeysOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXShortkeysOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXShortkeysOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXShortkeysOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXShortkeysOrganizer.obj"	"$(INTDIR)\OXShortkeysOrganizer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXShortkeysOrganizer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSizeCtrlBar.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSizeCtrlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSizeCtrlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSizeCtrlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSizeCtrlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSizeCtrlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSizeCtrlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSizeCtrlBar.obj"	"$(INTDIR)\OXSizeCtrlBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSizeCtrlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSizeDlgBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSizeDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSizeDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSizeDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSizeDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSizeDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSizeDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSizeDlgBar.obj"	"$(INTDIR)\OXSizeDlgBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSizeDlgBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSizeDockBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSizeDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSizeDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSizeDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSizeDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSizeDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSizeDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSizeDockBar.obj"	"$(INTDIR)\OXSizeDockBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSizeDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSizeToolBar.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSizeToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSizeToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSizeToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSizeToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSizeToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSizeToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSizeToolBar.obj"	"$(INTDIR)\OXSizeToolBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSizeToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSkinnedComboBox.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSkinnedComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSkinnedComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSkinnedComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSkinnedComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSkinnedComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSkinnedComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSkinnedComboBox.obj"	"$(INTDIR)\OXSkinnedComboBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSkinnedComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSkinnedSpinCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSkinnedSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSkinnedSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSkinnedSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSkinnedSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSkinnedSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSkinnedSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSkinnedSpinCtrl.obj"	"$(INTDIR)\OXSkinnedSpinCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSkinnedSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSkinnedTabCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSkinnedTabCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSkinnedTabCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSkinnedTabCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSkinnedTabCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSkinnedTabCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSkinnedTabCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSkinnedTabCtrl.obj"	"$(INTDIR)\OXSkinnedTabCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSkinnedTabCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSkins.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSkins.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSkins.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSkins.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSkins.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSkins.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSkins.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSkins.obj"	"$(INTDIR)\OXSkins.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSkins.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSound.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSound.obj"	"$(INTDIR)\OXSound.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSound.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSoundCustomizeDlg.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSoundCustomizeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSoundCustomizeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSoundCustomizeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSoundCustomizeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSoundCustomizeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSoundCustomizeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSoundCustomizeDlg.obj"	"$(INTDIR)\OXSoundCustomizeDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSoundCustomizeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSoundEffectManager.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSoundEffectManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSoundEffectManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSoundEffectManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSoundEffectManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSoundEffectManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSoundEffectManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSoundEffectManager.obj"	"$(INTDIR)\OXSoundEffectManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSoundEffectManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSoundWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSoundWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSoundWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSoundWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSoundWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSoundWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSoundWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSoundWnd.obj"	"$(INTDIR)\OXSoundWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSoundWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSpinCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSpinCtrl.obj"	"$(INTDIR)\OXSpinCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSpinCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSplashWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSplashWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSplashWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSplashWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSplashWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSplashWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSplashWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSplashWnd.obj"	"$(INTDIR)\OXSplashWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSplashWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSplashWndDIB.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSplashWndDIB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSplashWndDIB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSplashWndDIB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSplashWndDIB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSplashWndDIB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSplashWndDIB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSplashWndDIB.obj"	"$(INTDIR)\OXSplashWndDIB.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSplashWndDIB.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSplitterColRowState.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSplitterColRowState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSplitterColRowState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSplitterColRowState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSplitterColRowState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSplitterColRowState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSplitterColRowState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSplitterColRowState.obj"	"$(INTDIR)\OXSplitterColRowState.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSplitterColRowState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSplitterRect.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSplitterRect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSplitterRect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSplitterRect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSplitterRect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSplitterRect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSplitterRect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSplitterRect.obj"	"$(INTDIR)\OXSplitterRect.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSplitterRect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSplitterWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSplitterWnd.obj"	"$(INTDIR)\OXSplitterWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXStatic.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXStatic.obj"	"$(INTDIR)\OXStatic.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXStaticHyperLink.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXStaticHyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXStaticHyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXStaticHyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXStaticHyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXStaticHyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXStaticHyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXStaticHyperLink.obj"	"$(INTDIR)\OXStaticHyperLink.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXStaticHyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXStaticText.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXStaticText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXStaticText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXStaticText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXStaticText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXStaticText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXStaticText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXStaticText.obj"	"$(INTDIR)\OXStaticText.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXStaticText.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSysInfo.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSysInfo.obj"	"$(INTDIR)\OXSysInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSysInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXSzMiniDockFrmWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXSzMiniDockFrmWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXSzMiniDockFrmWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXSzMiniDockFrmWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXSzMiniDockFrmWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXSzMiniDockFrmWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXSzMiniDockFrmWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXSzMiniDockFrmWnd.obj"	"$(INTDIR)\OXSzMiniDockFrmWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXSzMiniDockFrmWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTabClientWnd.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTabClientWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTabClientWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTabClientWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTabClientWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTabClientWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTabClientWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTabClientWnd.obj"	"$(INTDIR)\OXTabClientWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTabClientWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTabView.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTabView.obj"	"$(INTDIR)\OXTabView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTabView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTaskbarIcon.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTaskbarIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTaskbarIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTaskbarIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTaskbarIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTaskbarIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTaskbarIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTaskbarIcon.obj"	"$(INTDIR)\OXTaskbarIcon.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTaskbarIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTleBar.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTleBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTleBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTleBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTleBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTleBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTleBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTleBar.obj"	"$(INTDIR)\OXTleBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTleBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXToolTipCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXToolTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXToolTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXToolTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXToolTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXToolTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXToolTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXToolTipCtrl.obj"	"$(INTDIR)\OXToolTipCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXToolTipCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTrace.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTrace.obj"	"$(INTDIR)\OXTrace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTreeCal.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTreeCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTreeCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTreeCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTreeCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTreeCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTreeCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTreeCal.obj"	"$(INTDIR)\OXTreeCal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTreeCal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTreeCombo.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTreeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTreeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTreeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTreeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTreeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTreeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTreeCombo.obj"	"$(INTDIR)\OXTreeCombo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTreeCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTreeCtrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTreeCtrl.obj"	"$(INTDIR)\OXTreeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTreeEdit.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTreeEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTreeEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTreeEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTreeEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTreeEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTreeEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTreeEdit.obj"	"$(INTDIR)\OXTreeEdit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTreeEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTreeHeader.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTreeHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTreeHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTreeHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTreeHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTreeHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTreeHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTreeHeader.obj"	"$(INTDIR)\OXTreeHeader.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTreeHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXTreeItem.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXTreeItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXTreeItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXTreeItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXTreeItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXTreeItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXTreeItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXTreeItem.obj"	"$(INTDIR)\OXTreeItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXTreeItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXUNC.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXUNC.obj"	"$(INTDIR)\OXUNC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXUNC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXUNCStandardActor.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXUNCStandardActor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXUNCStandardActor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXUNCStandardActor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXUNCStandardActor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXUNCStandardActor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXUNCStandardActor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXUNCStandardActor.obj"	"$(INTDIR)\OXUNCStandardActor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXUNCStandardActor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXURL.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXURL.obj"	"$(INTDIR)\OXURL.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXURL.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXUsertl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXUsertl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXUsertl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXUsertl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXUsertl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXUsertl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXUsertl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXUsertl.obj"	"$(INTDIR)\OXUsertl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXUsertl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXUUDec.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXUUDec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXUUDec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXUUDec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXUUDec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXUUDec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXUUDec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXUUDec.obj"	"$(INTDIR)\OXUUDec.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXUUDec.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXUxTheme.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXUxTheme.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXUxTheme.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXUxTheme.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXUxTheme.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXUxTheme.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXUxTheme.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXUxTheme.obj"	"$(INTDIR)\OXUxTheme.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXUxTheme.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXWABook.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXWABook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXWABook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXWABook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXWABook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXWABook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXWABook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXWABook.obj"	"$(INTDIR)\OXWABook.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXWABook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXWatchedDir.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXWatchedDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXWatchedDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXWatchedDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXWatchedDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXWatchedDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXWatchedDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXWatchedDir.obj"	"$(INTDIR)\OXWatchedDir.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXWatchedDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\oxwebctrl.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\oxwebctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\oxwebctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\oxwebctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\oxwebctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\oxwebctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\oxwebctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\oxwebctrl.obj"	"$(INTDIR)\oxwebctrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\oxwebctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\OXWorkspaceState.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\OXWorkspaceState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\OXWorkspaceState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\OXWorkspaceState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\OXWorkspaceState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\OXWorkspaceState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\OXWorkspaceState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\OXWorkspaceState.obj"	"$(INTDIR)\OXWorkspaceState.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\OXWorkspaceState.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\Oxzoomvw.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\Oxzoomvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\Oxzoomvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\Oxzoomvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\Oxzoomvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\Oxzoomvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\Oxzoomvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\Oxzoomvw.obj"	"$(INTDIR)\Oxzoomvw.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\Oxzoomvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\Path.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\Path.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\Path.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\Path.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\Path.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\Path.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\Path.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\Path.obj"	"$(INTDIR)\Path.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\Path.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\PATHITER.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\PATHITER.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\PATHITER.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\PATHITER.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\PATHITER.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\PATHITER.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\PATHITER.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\PATHITER.OBJ"	"$(INTDIR)\PATHITER.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\PATHITER.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\PROGRESS.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\PROGRESS.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\PROGRESS.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\PROGRESS.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\PROGRESS.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\PROGRESS.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\PROGRESS.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\PROGRESS.OBJ"	"$(INTDIR)\PROGRESS.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\PROGRESS.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\WATCHBUF.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\WATCHBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\WATCHBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\WATCHBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\WATCHBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\WATCHBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\WATCHBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\WATCHBUF.OBJ"	"$(INTDIR)\WATCHBUF.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\WATCHBUF.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\Xstatus4.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\Xstatus4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\Xstatus4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\Xstatus4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\Xstatus4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\Xstatus4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\Xstatus4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\Xstatus4.obj"	"$(INTDIR)\Xstatus4.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\Xstatus4.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\source\XSTRING.CPP

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"


"$(INTDIR)\XSTRING.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"


"$(INTDIR)\XSTRING.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"


"$(INTDIR)\XSTRING.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"


"$(INTDIR)\XSTRING.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"


"$(INTDIR)\XSTRING.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"


"$(INTDIR)\XSTRING.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"


"$(INTDIR)\XSTRING.OBJ"	"$(INTDIR)\XSTRING.SBR" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"


"$(INTDIR)\XSTRING.OBJ" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\UTStaticLib.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCShared"

CPP_SWITCHES=/nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\UTStaticLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\UTStaticLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCShared"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\UTStaticLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\UTStaticLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 DebugMFCStatic"

CPP_SWITCHES=/nologo /MTd /W4 /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\UTStaticLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\UTStaticLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 ReleaseMFCStatic"

CPP_SWITCHES=/nologo /MT /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\UTStaticLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\UTStaticLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCShared"

CPP_SWITCHES=/nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)\UTStaticLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\UTStaticLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCShared"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)\UTStaticLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\UTStaticLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeDebugMFCStatic"

CPP_SWITCHES=/nologo /MTd /W4 /GX /ZI /Od /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\UTStaticLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\stdafx.sbr"	"$(INTDIR)\UTStaticLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "UTStaticLib - Win32 UnicodeReleaseMFCStatic"

CPP_SWITCHES=/nologo /MT /W4 /GX /O2 /I "..\..\Include" /I "..\..\Lib\Jpeg" /I "..\..\Lib\Jpeg\Jpegsrc6" /I "..\..\Lib\Processor\Intel\CPUINF32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /Fp"$(INTDIR)\UTStaticLib.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\UTStaticLib.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

