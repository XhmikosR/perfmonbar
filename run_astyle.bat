@ECHO OFF
SETLOCAL

PUSHD %~dp0

SET "AStyleVerMin=2.03"
astyle --version 2>NUL || (ECHO. & ECHO ERROR: AStyle not found & GOTO End)
CALL :SubCheckVer || GOTO End

rem  http://astyle.sourceforge.net/astyle.html
SET ASTYLE_OPTIONS=--indent=spaces=4 --style=kr^
 --indent-switches --indent-namespaces --indent-col1-comments^
 --pad-header --pad-oper --unpad-paren^
 --align-pointer=type --align-reference=type^
 --close-templates^
 --add-brackets^
 --keep-one-line-blocks^
 --convert-tabs^
 --lineend=windows --preserve-date^
 --suffix=none^
 --recursive^
 --ignore-exclude-errors --ignore-exclude-errors-x^
 --formatted^
 --exclude=bin --exclude=bin


:Start
astyle %ASTYLE_OPTIONS% --exclude=resource.h --exclude=Version.h src\*.h src\*.cpp

IF %ERRORLEVEL% NEQ 0 (ECHO. & ECHO ERROR: Something went wrong!)


:END
POPD
ECHO. & ECHO Press any key to close this window...
PAUSE >NUL
ENDLOCAL
EXIT /B


:SubCheckVer
TITLE Checking astyle version
FOR /F "tokens=4 delims= " %%A IN ('astyle --version 2^>^&1 NUL') DO (
  SET "AStyleVer=%%A"
)

IF %AStyleVer% LSS %AStyleVerMin% (
  ECHO. & ECHO ERROR: AStyle v%AStyleVer% is too old, please update AStyle to v%AStyleVerMin% or newer.
  EXIT /B 1
)
EXIT /B
