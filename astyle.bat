@ECHO OFF
SETLOCAL

PUSHD %~dp0%

SET "ASTYLE=AStyle.exe"

IF NOT EXIST %ASTYLE% (
  ECHO.
  ECHO ERROR: Astyle wasn't found!
  PAUSE
  EXIT /B
)

%ASTYLE% -s4 --indent-switches --indent-namespaces --indent-col1-comments --pad-header^
 --lineend=windows --preserve-date --keep-one-line-blocks --keep-one-line-statements^
 --suffix=none --ignore-exclude-errors --ignore-exclude-errors-x --recursive^
 --exclude=resource.h *.h *.hpp *.cpp

IF %ERRORLEVEL% NEQ 0 (
  ECHO.
  ECHO ERROR: Something went wrong!
  PAUSE
)

POPD
PAUSE
ENDLOCAL
EXIT /B
