
  !include "MUI2.nsh"
  
  !include "WinVer.nsh"

  !define APPNAME "GubberennDataprogram"

  !define UNINSTKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"

  Name "${APPNAME}"
  
  OutFile "GubberennDataprogramInstaller.exe"

  InstallDir "$PROGRAMFILES\GubberennDataprogram"

  InstallDirRegKey HKCU "Software\GubberennDataprogram" ""

  RequestExecutionLevel admin

  !insertmacro MUI_LANGUAGE "English"



Function .onInit

  Push $R0
  Push $R1
  Push $R2

; get acount info into $R2
  userInfo::getAccountType
  pop $0
  StrCpy $R2 $0 5

${If} ${IsWin2000}
    strCmp $R2 "Admin" lbl_checkok
    messageBox MB_OK "I am sorry, this installer needs Admin priviledges, Please login as an administrator and install the software."
    Quit
${EndIf}

${If} ${IsWinXP}
    strCmp $R2 "Admin" lbl_checkok
    messageBox MB_OK "I am sorry, this installer needs Admin priviledges, Please login as an administrator and install the software."
    Quit
${EndIf}

  lbl_checkok:
  Pop $R2
  Pop $R1
  Pop $R0

FunctionEnd


!ifndef Qt_Dir
    !define Qt_Dir 	"E:\Qt\"
!endif
!ifndef Qt_Version
    !define Qt_Version 	"5.2.1"
!endif
!ifndef Mingw_Ver
    !define Mingw_Ver 	"mingw48_32"
!endif

!define MINGW_DIR 	"${Qt_Dir}\Tools\${Mingw_Ver}\bin"
!define QTCREATOR_DIR 	"${Qt_Dir}\Tools\QtCreator\bin"
!define QTMINGW_DIR 	"${Qt_Dir}\${Qt_Version}\${Mingw_Ver}"

!define QMAKE_DIR 	"${QTMINGW_DIR}\bin"
!define PLUGINS_DIR 	"${QTMINGW_DIR}\plugins"
!define TRANSLATIONS_DIR	"${QTMINGW_DIR}\translations"


Section "Install Section" SecInstall
  SetOutPath "$INSTDIR"  
  File "..\..\build-gdp-Desktop_Qt_5_2_1_MinGW_32bit-Release\release\gdp.exe"
  File "testinput.csv"
  File "${MINGW_DIR}\libgcc_s_dw2-1.dll"
  File "${QMAKE_DIR}\libstdc++-6.dll"
  File "${MINGW_DIR}\libwinpthread-1.dll"
   
  SetOutPath "$INSTDIR"
 
  WriteRegStr HKCU "Software\GubberennDataprogram" "" $INSTDIR

  WriteUninstaller "$INSTDIR\Uninstall.exe"

  createShortCut "$DESKTOP\GubberennDataprogram.lnk" "$INSTDIR\GubberennDataprogram.exe"

  ; Startmenu shortcuts
  createDirectory "$SMPROGRAMS\GubberennDataprogram\"
  createShortCut "$SMPROGRAMS\GubberennDataprogram\GubberennDataprogram.lnk" "$INSTDIR\GubberennDataprogram.exe"
  createShortCut "$SMPROGRAMS\GubberennDataprogram\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

  ; create add/remove software entries
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayName" "${APPNAME}"
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayIcon" "$INSTDIR\GubberennDataprogram.exe"
  WriteRegStr HKLM "${UNINSTKEY}" "DisplayVersion" "1.0.0"
  WriteRegStr HKLM "${UNINSTKEY}" "Publisher" "GubberennDataprogram Team"
  WriteRegStr HKLM "${UNINSTKEY}" "Version" "1.0"
  WriteRegStr HKLM "${UNINSTKEY}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "${UNINSTKEY}" "Comments" "GubberennDataprogram - din venn på Skjærtorsdag"
  WriteRegStr HKLM "${UNINSTKEY}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegDWORD HKLM "${UNINSTKEY}" "VersionMinor" "0"
  WriteRegDWORD HKLM "${UNINSTKEY}" "VersionMajor" "1"
  WriteRegDWORD HKLM "${UNINSTKEY}" "NoModify" "1"
  WriteRegDWORD HKLM "${UNINSTKEY}" "NoRepair" "1"


SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecInstall ${LANG_ENGLISH} "A test section."

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"
  Delete "$DESKTOP\GubberennDataprogram.lnk"
  RMDir /r "$SMPROGRAMS\GubberennDataprogram\"
  RMDir /r $INSTDIR

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\GubberennDataprogram"
  DeleteRegKey HKLM "${UNINSTKEY}"

SectionEnd


