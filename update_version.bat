@ECHO OFF
SETLOCAL

PUSHD %~dp0%

SET "SUBWCREV=SubWCRev.exe"

"%SUBWCREV%" . "src\Version.in.h" "src\Version.h" -f
IF %ERRORLEVEL% NEQ 0 GOTO Error

POPD
ENDLOCAL
EXIT /B


:Error
ECHO. & ECHO SubWCRev, which is part of TortoiseSVN, wasn't found!
ECHO You should (re)install TortoiseSVN.

ECHO #pragma once> "src\Version.h"
ECHO #define VER_NUM 1,0,2,9999 >> "src\Version.h"
ECHO #define VER_STR "1.0.2.9999" >> "src\Version.h"

POPD
ENDLOCAL
EXIT /B
