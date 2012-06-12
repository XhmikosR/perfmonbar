@ECHO OFF
SETLOCAL

PUSHD %~dp0

AStyle.exe --version 1>&2 2>NUL

IF %ERRORLEVEL% NEQ 0 (
  ECHO ERROR: Astyle wasn't found!
  ECHO Visit http://astyle.sourceforge.net/ for download and details.
  GOTO END
)

AStyle.exe^
 --indent=spaces=4 --style=kr --indent-switches --indent-namespaces --indent-col1-comments^
 --add-brackets --pad-header --pad-oper --unpad-paren^
 --align-pointer=type --align-reference=type^
 --preserve-date --lineend=windows --convert-tabs^
 --keep-one-line-blocks --keep-one-line-statements^
 --suffix=none --ignore-exclude-errors --ignore-exclude-errors-x --recursive^
 --exclude=resource.h *.h *.hpp *.cpp

IF %ERRORLEVEL% NEQ 0 (
  ECHO.
  ECHO ERROR: Something went wrong!
)

:END
POPD
PAUSE
ENDLOCAL
EXIT /B
