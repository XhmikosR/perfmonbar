@ECHO OFF
SETLOCAL

PUSHD %~dp0

rem check powershell installed and at least v5.1
powershell -Command "if ([Version]'5.1' -gt $PSVersionTable.PSVersion){exit 1;}" > NUL 2>&1
if ERRORLEVEL 1 GOTO :MissingPowershell

powershell -ExecutionPolicy Bypass -File .\version.ps1

:END
POPD
ENDLOCAL
EXIT /B


:MissingPowershell
COLOR 0C
TITLE ERROR
ECHO Powershell 5.1 or later is not installed
ECHO. & ECHO.
ECHO Press any key to exit...
PAUSE >NUL
ENDLOCAL
EXIT /B
