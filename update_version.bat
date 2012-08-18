@ECHO OFF
SETLOCAL

PUSHD %~dp0

IF EXIST "SubWCRev.exe" SET "SUBWCREV=SubWCRev.exe"
FOR %%A IN (SubWCRev.exe) DO (SET SUBWCREV=%%~$PATH:A)
IF NOT DEFINED SUBWCREV GOTO SubNoSubWCRev

"%SUBWCREV%" . "src\Version.in.h" "src\Version.h" -f
IF %ERRORLEVEL% NEQ 0 GOTO SubError

:END
POPD
ENDLOCAL
EXIT /B


:SubNoSubWCRev
ECHO. & ECHO SubWCRev, which is part of TortoiseSVN, wasn't found!
ECHO You should (re)install TortoiseSVN.
GOTO SubCommon

:SubError
ECHO Something went wrong when generating the revision number.

:SubCommon
ECHO I'll use VER_NUM=9999 for now.

ECHO #pragma once> "src\Version.h"
ECHO #define VER_NUM 1,0,2,9999>> "src\Version.h"
ECHO #define VER_STR "1.0.2.9999">> "src\Version.h"
GOTO END
