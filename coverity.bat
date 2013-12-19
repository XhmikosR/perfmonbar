@ECHO OFF
REM
REM  Copyright (C) 2013 XhmikosR
REM
REM  This program is free software: you can redistribute it and/or modify
REM  it under the terms of the GNU General Public License as published by
REM  the Free Software Foundation, either version 3 of the License, or
REM  (at your option) any later version.
REM
REM  This program is distributed in the hope that it will be useful,
REM  but WITHOUT ANY WARRANTY; without even the implied warranty of
REM  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM  GNU General Public License for more details.
REM
REM  You should have received a copy of the GNU General Public License
REM  along with this program.  If not, see <http://www.gnu.org/licenses/>.


SETLOCAL

PUSHD %~dp0

IF NOT DEFINED COVDIR SET "COVDIR=H:\progs\thirdparty\cov-analysis-win64-6.6.1"
IF DEFINED COVDIR IF NOT EXIST "%COVDIR%" (
  ECHO.
  ECHO ERROR: Coverity not found in "%COVDIR%"
  GOTO End
)


CALL "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" x86
IF %ERRORLEVEL% NEQ 0 (
  ECHO vcvarsall.bat call failed.
  GOTO End
)

IF EXIST "cov-int" RD /q /s "cov-int"

"%COVDIR%\bin\cov-build.exe" --dir cov-int "build.bat" Rebuild All

IF EXIST "PerfmonBar.tar" DEL "PerfmonBar.tar"
IF EXIST "PerfmonBar.tgz" DEL "PerfmonBar.tgz"


:tar
tar --version 1>&2 2>NUL || (ECHO. & ECHO ERROR: tar not found & GOTO SevenZip)
tar czvf "PerfmonBar.tgz" "cov-int"
GOTO End


:SevenZip
IF NOT EXIST "%PROGRAMFILES%\7za.exe" (
  ECHO.
  ECHO ERROR: "%PROGRAMFILES%\7za.exe" not found
  GOTO End
)
"%PROGRAMFILES%\7za.exe" a -ttar "PerfmonBar.tar" "cov-int"
"%PROGRAMFILES%\7za.exe" a -tgzip "PerfmonBar.tgz" "PerfmonBar.tar"
IF EXIST "PerfmonBar.tar" DEL "PerfmonBar.tar"


:End
POPD
ECHO. & ECHO Press any key to close this window...
PAUSE >NUL
ENDLOCAL
EXIT /B
