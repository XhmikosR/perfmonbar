@ECHO OFF
SETLOCAL
SET "BUILD_TYPE=Build"
TITLE Compiling PerfmonBar...

IF NOT DEFINED VS100COMNTOOLS ECHO Visual Studio 2010 wasn't found & GOTO EndWithError

CD /D %~dp0

CALL "%VS100COMNTOOLS%vsvars32.bat" >NUL

TITLE Compiling PerfmonBar - Release^|Win32...
devenv PerfmonBar.sln /%BUILD_TYPE% "Release|Win32"
IF %ERRORLEVEL% NEQ 0 GOTO EndWithError

TITLE Compiling PerfmonBar - Release^|x64...
devenv PerfmonBar.sln /%BUILD_TYPE% "Release|x64"
IF %ERRORLEVEL% NEQ 0 GOTO EndWithError
ECHO.

CALL "Setup\build_installer.bat"


:END
ECHO.
ENDLOCAL
PAUSE
EXIT /B


:EndWithError
Title Compiling PerfmonBar [ERROR]
ECHO. & ECHO.
ECHO **ERROR: Build failed and aborted!**
PAUSE
ENDLOCAL
EXIT
