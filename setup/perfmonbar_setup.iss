;
; Copyright (C) 2011-2021 XhmikosR
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.

; Requirements:
; Inno Setup: https://jrsoftware.org/isdl.php


#if VER < EncodeVer(6,1,2)
  #error Update your Inno Setup version (6.1.2 or newer)
#endif

#define bindir "..\bin"

#ifnexist bindir + "\Release_Win32\PerfmonBar.dll"
  #error Compile PerfmonBar x86 first
#endif

#ifnexist bindir + "\Release_x64\PerfmonBar.dll"
  #error Compile PerfmonBar x64 first
#endif

#define VerMajor
#define VerMinor
#define VerBuild
#define VerRevision

#expr ParseVersion(bindir + "\Release_Win32\PerfmonBar.dll", VerMajor, VerMinor, VerBuild, VerRevision)
#define app_version  str(VerMajor) + "." + str(VerMinor) + "." + str(VerBuild) + "." + str(VerRevision)
#define app_name     "PerfmonBar"


[Setup]
AppId={#app_name}
AppName={#app_name}
AppVersion={#app_version}
AppVerName={#app_name} {#app_version}
AppPublisher=XhmikosR
AppPublisherURL=https://xhmikosr.github.io/perfmonbar/
AppSupportURL=https://xhmikosr.github.io/perfmonbar/
AppUpdatesURL=https://xhmikosr.github.io/perfmonbar/
AppContact=https://xhmikosr.github.io/perfmonbar/
AppCopyright=Copyright © 2008, Danny Couture - 2010-2021 XhmikosR
VersionInfoCompany=XhmikosR
VersionInfoCopyright=Copyright © 2008, Danny Couture - 2010-2021 XhmikosR
VersionInfoDescription={#app_name} {#app_version} Setup
VersionInfoTextVersion={#app_version}
VersionInfoVersion={#app_version}
VersionInfoProductName={#app_name}
VersionInfoProductVersion={#app_version}
VersionInfoProductTextVersion={#app_version}
UninstallDisplayName={#app_name} {#app_version}
DefaultDirName={commonpf}\PerfmonBar
DefaultGroupName=PerfmonBar
LicenseFile=..\LICENSE.txt
OutputDir=.
OutputBaseFilename={#app_name}.{#app_version}
SolidCompression=yes
EnableDirDoesntExistWarning=no
ShowTasksTreeLines=yes
DisableDirPage=yes
DisableProgramGroupPage=yes
DisableReadyPage=yes
DisableWelcomePage=yes
AllowCancelDuringInstall=no
MinVersion=6.0
ArchitecturesAllowed=x86 x64
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=admin
;UsedUserAreasWarning=no
SetupMutex='{#app_name}' + '_setup_mutex'


[Languages]
Name: en; MessagesFile: compiler:Default.isl


[Messages]
BeveledLabel={#app_name} {#app_version}
SetupAppTitle=Setup - {#app_name}
SetupWindowTitle=Setup - {#app_name}
WinVersionTooLowError=This program only works on Windows Vista or newer.


[CustomMessages]
en.msg_DeleteSettings=Do you also want to delete {#app_name}'s settings?%n%nIf you plan on installing {#app_name} again then you do not have to delete them.
en.tsk_ResetSettings=Reset {#app_name}'s settings
en.run_ViewConfig=View PerfmonBar's configuration file


[Tasks]
Name: reset_settings; Description: {cm:tsk_ResetSettings}; Flags: checkedonce unchecked; Check: ConfigExists('{userappdata}')


[Files]
Source: ..\LICENSE.txt;                         DestDir: {app};                    Flags: ignoreversion
Source: {#bindir}\Release_Win32\PerfmonBar.dll; DestDir: {app};                    Flags: ignoreversion regserver restartreplace uninsrestartdelete; Check: not Is64BitInstallMode()
Source: {#bindir}\Release_x64\PerfmonBar.dll;   DestDir: {app};                    Flags: ignoreversion regserver restartreplace uninsrestartdelete; Check: Is64BitInstallMode()
Source: ..\src\config.xml;                      DestDir: {userappdata}\PerfmonBar; Flags: uninsneveruninstall;                                       Check: not ConfigExists('{userappdata}') and not ConfigExists('{userdocs}')
Source: {userdocs}\PerfmonBar\config.xml;       DestDir: {userappdata}\PerfmonBar; Flags: external uninsneveruninstall;                              Check: not ConfigExists('{userappdata}') and ConfigExists('{userdocs}')


[Icons]
Name: {group}\Shortcut to config.xml;            Filename: notepad.exe; Parameters: {userappdata}\PerfmonBar\config.xml; WorkingDir: {app}; Comment: Shortcut to config.xml [{#app_name} {#app_version}]; Flags: excludefromshowinnewinstall
Name: {group}\{cm:UninstallProgram,{#app_name}}; Filename: {uninstallexe};                                               WorkingDir: {app}; Comment: {cm:UninstallProgram,{#app_name}};                   Flags: excludefromshowinnewinstall


[Run]
Filename: notepad.exe; Description: {cm:run_ViewConfig}; Parameters: {userappdata}\PerfmonBar\config.xml; Flags: nowait postinstall skipifsilent unchecked


[UninstallDelete]
Type: dirifempty; Name: {app}


[Code]
// Check if PerfmonBar's settings exist
function ConfigExists(sPath: String): Boolean;
begin
  if FileExists(ExpandConstant(sPath + '\PerfmonBar\config.xml')) then begin
    Log('Custom Code: Settings are present in ' + sPath + '\PerfmonBar\config.xml');
    Result := True;
  end
  else begin
    Log('Custom Code: Settings are NOT present in ' + sPath + '\PerfmonBar\config.xml');
    Result := False;
  end;
end;


function IsOldBuildInstalled(): Boolean;
begin
  if RegKeyExists(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{3C201211-770A-4048-9889-CFABA8A3FF35}') then begin
    Log('Custom Code: The old build is installed');
    Result := True;
  end
  else begin
    Log('Custom Code: The old build is NOT installed');
    Result := False;
  end;
end;


function IsUpgrade(): Boolean;
var
  sPrevPath: String;
begin
  sPrevPath := WizardForm.PrevAppDir;
  Result := (sPrevPath <> '');
end;


// Function to retrieve the uninstall string and uninstall the old build
function UninstallOldVersion(): Integer;
var
  iResultCode: Integer;
begin
  // Return Values:
  // 0 - no idea
  // 1 - error executing the command
  // 2 - successfully executed the command

  // default return value
  Log('Custom Code: Will try to uninstall the old build');
  Result := 0;
    if Exec('msiexec.exe', '/x{3C201211-770A-4048-9889-CFABA8A3FF35} /qn REBOOT=ReallySuppress', '', SW_HIDE, ewWaitUntilTerminated, iResultCode) then begin
      Result := 2;
      Sleep(500);
      Log('Custom Code: The old build was successfully uninstalled');
    end
    else begin
      Result := 1;
      Log('Custom Code: Something went wrong when uninstalling the old build');
    end;
end;


function ShouldSkipPage(PageID: Integer): Boolean;
begin
  // Hide the License page if it's an upgrade
  if IsUpgrade() and (PageID = wpLicense) then
    Result := True;
end;


procedure CleanUpSettings(sPath: String);
begin
  DeleteFile(ExpandConstant(sPath + '\PerfmonBar\config.xml'));
  RemoveDir(ExpandConstant(sPath + '\PerfmonBar'));
end;


procedure CurPageChanged(CurPageID: Integer);
begin
  // TODO: find a better way to check if the wpSelectTasks exists?
  if (CurPageID = wpLicense) and not ConfigExists('{userappdata}') then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonInstall)
  else if CurPageID = wpSelectTasks then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonInstall)
  else if CurPageID = wpFinished then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonFinish);
end;


procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssInstall then begin
    if IsOldBuildInstalled() then
      UninstallOldVersion();
    if WizardIsTaskSelected('reset_settings') then
      CleanUpSettings('{userappdata}');
  end;
  if CurStep = ssPostInstall then begin
    if ConfigExists('{userappdata}') and ConfigExists('{userdocs}') then
      CleanUpSettings('{userdocs}');
  end;
end;


procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  // When uninstalling, ask the user to delete PerfmonBar's config file
  if (CurUninstallStep = usUninstall) and ConfigExists('{userappdata}') then begin
    if SuppressibleMsgBox(CustomMessage('msg_DeleteSettings'), mbConfirmation, MB_YESNO or MB_DEFBUTTON2, IDNO) = IDYES then
      CleanUpSettings('{userappdata}');
  end;
end;
