
  !include "MUI2.nsh"
  
  !include "WinVer.nsh"

  !define APPNAME "Gubb"

  !define UNINSTKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"

  Name "${APPNAME}" "1.0"
  
  OutFile "GubbInstaller.exe"

  InstallDir "$DESKTOP\Gubb"

  InstallDirRegKey HKCU "Software\Gubb" ""

  ShowInstDetails show
  ShowUnInstDetails show 
  
  RequestExecutionLevel user

  !insertmacro MUI_LANGUAGE "English"

Function .onInit

 MessageBox MB_YESNO "Dette vil installere 'Gubb' på datamaskinen din. Ønsker du å fortsette?" IDYES gogogo
    Abort
  gogogo:

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


Section "Install Section" SecInstall
  SetOutPath "$INSTDIR"  
  File "..\gds.exe"
  File "testinput.csv"
  File "manual.txt"
  File "README.TXT"
  File "C:\cygwin\bin\cygwin1.dll"
   
  SetOutPath "$INSTDIR" 
  WriteRegStr HKCU "Software\Gubb" "" $INSTDIR

  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
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
  RMDir /r "$SMPROGRAMS\Gubb\"
  RMDir /r $INSTDIR

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Gubb"
  DeleteRegKey HKLM "${UNINSTKEY}"

SectionEnd


