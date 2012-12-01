@ECHO OFF
SETLOCAL

PUSHD %~dp0

rem  http://astyle.sourceforge.net/astyle.html
SET ASTYLE_OPTIONS=--indent=spaces=4 --style=kr^
 --indent-switches --indent-namespaces --indent-col1-comments^
 --pad-header --pad-oper --unpad-paren^
 --align-pointer=type --align-reference=type^
 --add-brackets^
 --keep-one-line-blocks^
 --convert-tabs^
 --lineend=windows --preserve-date^
 --suffix=none^
 --recursive^
 --ignore-exclude-errors --ignore-exclude-errors-x^
 --formatted

astyle %ASTYLE_OPTIONS% --exclude=resource.h --exclude=Version.h src\*.h src\*.cpp


IF %ERRORLEVEL% NEQ 0 (ECHO. & ECHO ERROR: Something went wrong!)

:END
POPD
PAUSE
ENDLOCAL
EXIT /B
