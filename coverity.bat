@ECHO OFF
REM
REM  Copyright (C) 2013-2015, 2017, 2019-2021 XhmikosR
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

IF NOT DEFINED COVDIR SET "COVDIR=C:\cov-analysis-win64"
IF DEFINED COVDIR IF NOT EXIST "%COVDIR%" (
  ECHO.
  ECHO ERROR: Coverity not found in "%COVDIR%"
  GOTO End
)

CALL :SubVSPath
IF NOT EXIST "%VS_PATH%" CALL :SUBMSG "ERROR" "Visual Studio 2019 NOT FOUND!"


:Cleanup
IF EXIST "cov-int"         RD /q /s "cov-int"
IF EXIST "PerfmonBar.tgz"  DEL "PerfmonBar.tgz"


:Main
"%COVDIR%\bin\cov-build.exe" --dir cov-int "build.bat" Rebuild All


:tar
tar --version 1>&2 2>NUL || (ECHO. & ECHO ERROR: tar not found & GOTO End)
tar caf "PerfmonBar.tgz" "cov-int"
GOTO End


:SubVSPath
FOR /f "delims=" %%A IN ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -property installationPath -latest -requires Microsoft.Component.MSBuild Microsoft.VisualStudio.Component.VC.ATLMFC Microsoft.VisualStudio.Component.VC.Tools.x86.x64') DO SET "VS_PATH=%%A"
EXIT /B

:End
POPD
ECHO. & ECHO Press any key to close this window...
IF NOT DEFINED CI PAUSE >NUL
ENDLOCAL
EXIT /B
