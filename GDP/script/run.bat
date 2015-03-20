
if "%NSIS_DIR%"=="" goto SetEnv
goto Exit

:SetEnv
set NSIS_DIR=C:\Program Files (x86)\NSIS
set PATH=%NSIS_DIR%;%PATH%

:Exit

makensis.exe nsiscript.nsi
