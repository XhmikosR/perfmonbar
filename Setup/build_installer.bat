@ECHO OFF
REM  Copyright (C) 2011 XhmikosR
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

SETLOCAL ENABLEEXTENSIONS
CD /D %~dp0

REM You can set here the Inno Setup path if for example you have Inno Setup Unicode
REM installed and you want to use the ANSI Inno Setup which is in another location
SET "InnoSetupPath=H:\progs\thirdparty\isetup"


CALL :SubDetectInnoSetup

TITLE Building installer...
"%InnoSetupPath%\iscc.exe" /Q "perfmonbar_setup.iss"
IF %ERRORLEVEL% NEQ 0 GOTO EndWithError


:END
TITLE Finished!
ECHO.
ENDLOCAL
EXIT /B


:SubDetectInnoSetup
REM Detect if we are running on 64bit WIN and use Wow6432Node, and set the path
REM of Inno Setup accordingly
IF DEFINED PROGRAMFILES(x86) (
  SET "U_=HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall"
) ELSE (
  SET "U_=HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
)

IF NOT DEFINED InnoSetupPath (
  FOR /F "delims=" %%a IN (
    'REG QUERY "%U_%\Inno Setup 5_is1" /v "Inno Setup: App Path"2^>Nul^|FIND "REG_"') DO (
    SET "InnoSetupPath=%%a" & CALL :SubInnoSetupPath %%InnoSetupPath:*Z=%%)
)

IF NOT EXIST "%InnoSetupPath%" GOTO EndWithError
EXIT /B


:SubInnoSetupPath
SET InnoSetupPath=%*
EXIT /B


:EndWithError
Title Compiling PerfmonBar [ERROR]
ECHO. & ECHO.
ECHO **ERROR: Build failed and aborted!**
PAUSE
ENDLOCAL
EXIT
