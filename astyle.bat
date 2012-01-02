@ECHO OFF
SETLOCAL

PUSHD %~dp0

AStyle.exe --version 1>&2 2>NUL

IF %ERRORLEVEL% NEQ 0 (
  ECHO.
  ECHO ERROR: Astyle wasn't found!
  ECHO Visit http://astyle.sourceforge.net/ for download and details.
  GOTO END
)

AStyle.exe^
 --indent=spaces=4 --indent-switches --indent-namespaces --indent-col1-comments^
 --pad-header --pad-oper --unpad-paren^
 --preserve-date^
 --lineend=windows^
 --keep-one-line-blocks --keep-one-line-statements^
 --suffix=none --ignore-exclude-errors --ignore-exclude-errors-x --recursive^
 --exclude=resource.h *.h *.hpp *.cpp

IF %ERRORLEVEL% NEQ 0 (
  ECHO.
  ECHO ERROR: Something went wrong!
  PAUSE
)

:END
POPD
PAUSE
ENDLOCAL
EXIT /B
