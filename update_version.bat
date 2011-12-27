@ECHO OFF
SETLOCAL

PUSHD %~dp0%

SET "SUBWCREV=SubWCRev.exe"

"%SUBWCREV%" . "PerfmonBar\Version.in.h" "PerfmonBar\Version.h" -f
IF %ERRORLEVEL% NEQ 0 GOTO Error

POPD
ENDLOCAL
EXIT /B


:Error
ECHO. & ECHO SubWCRev, which is part of TortoiseSVN, wasn't found!
ECHO You should (re)install TortoiseSVN.

ECHO #pragma once> "PerfmonBar\Version.h"
ECHO #define VER_NUM 1,0,2,9999 >> "PerfmonBar\Version.h"
ECHO #define VER_STR "1.0.2.9999" >> "PerfmonBar\Version.h"

POPD
ENDLOCAL
EXIT /B
