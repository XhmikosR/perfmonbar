;
; Copyright (C) 2011 XhmikosR
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
; Inno Setup v5.4.2(+): http://www.jrsoftware.org/isdl.php

; $Id$


#if VER < 0x05040200
  #error Update your Inno Setup version
#endif

#define bindir       "..\bin\"

#ifnexist SourcePath + bindir + "\Release_Win32\PerfmonBar.dll"
  #error Compile PerfmonBar x86 first
#endif

#ifnexist SourcePath + bindir + "\Release_x64\PerfmonBar.dll"
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
AppPublisherURL=http://code.google.com/p/perfmonbar/
AppSupportURL=http://code.google.com/p/perfmonbar/
AppUpdatesURL=http://code.google.com/p/perfmonbar/
AppContact=http://code.google.com/p/perfmonbar/
AppCopyright=Copyright © 2008, Danny Couture - 2010-2011 XhmikosR
VersionInfoCompany=XhmikosR
VersionInfoCopyright=Copyright © 2008, Danny Couture - 2010-2011 XhmikosR
VersionInfoDescription={#app_name} {#app_version} Setup
VersionInfoTextVersion={#app_version}
VersionInfoVersion={#app_version}
VersionInfoProductName={#app_name}
VersionInfoProductVersion={#app_version}
VersionInfoProductTextVersion={#app_version}
UninstallDisplayName={#app_name} {#app_version}
DefaultDirName={pf}\PerfmonBar
DefaultGroupName=PerfmonBar
LicenseFile=..\PerfmonBar\license.txt
OutputDir=.
OutputBaseFilename={#app_name}.{#app_version}
SolidCompression=yes
EnableDirDoesntExistWarning=no
AllowNoIcons=yes
ShowTasksTreeLines=yes
DisableDirPage=yes
DisableProgramGroupPage=yes
DisableReadyPage=yes
DisableWelcomePage=yes
AllowCancelDuringInstall=no
MinVersion=0,5.1.2600sp3
ArchitecturesAllowed=x86 x64
ArchitecturesInstallIn64BitMode=x64


[Languages]
Name: en; MessagesFile: compiler:Default.isl


[Messages]
BeveledLabel     ={#app_name} {#app_version}
SetupAppTitle    =Setup - {#app_name}
SetupWindowTitle =Setup - {#app_name}


[CustomMessages]
en.msg_DeleteSettings        =Do you also want to delete {#app_name}'s settings?%n%nIf you plan on installing {#app_name} again then you do not have to delete them.
en.msg_SetupIsRunningWarning ={#app_name} setup is already running!
en.tsk_ResetSettings         =Reset {#app_name}'s settings
en.run_ViewConfig            =View PerfmonBar's configuration file


[Tasks]
Name: reset_settings; Description: {cm:tsk_ResetSettings}; Flags: checkedonce unchecked; Check: ConfigExistsCheck()


[Files]
Source: ..\PerfmonBar\license.txt;              DestDir: {app};                  Flags: ignoreversion
Source: {#bindir}\Release_Win32\PerfmonBar.dll; DestDir: {app};                  Flags: ignoreversion regserver restartreplace uninsrestartdelete; Check: not Is64BitInstallMode()
Source: {#bindir}\Release_x64\PerfmonBar.dll;   DestDir: {app};                  Flags: ignoreversion regserver restartreplace uninsrestartdelete; Check: Is64BitInstallMode()
Source: ..\PerfmonBar\config.xml;               DestDir: {userdocs}\PerfmonBar;  Flags: onlyifdoesntexist uninsneveruninstall


[Icons]
Name: {group}\Shortcut to config.xml;            Filename: notepad.exe; Parameters: "{userdocs}\PerfmonBar\config.xml"; Comment: Shortcut to config.xml [{#app_name} {#app_version}]
Name: {group}\{cm:UninstallProgram,{#app_name}}; Filename: {uninstallexe};                                              Comment: {cm:UninstallProgram,{#app_name}}; WorkingDir: {app}

[Run]
Filename: notepad.exe; Description: {cm:run_ViewConfig}; Parameters: "{userdocs}\PerfmonBar\config.xml"; Flags: nowait postinstall skipifsilent unchecked


[InstallDelete]
Type: files;      Name: {userdocs}\config.xml; Check: IsTaskSelected('reset_settings')


[UninstallDelete]
Type: dirifempty; Name: {app}


[Code]
// Global variables/constants and general functions
const installer_mutex_name = '{#app_name}' + '_setup_mutex';


////////////////////////////////////////
//  Custom functions and procedures   //
////////////////////////////////////////


// Check if PerfmonBar's settings exist
function ConfigExistsCheck(): Boolean;
begin
  if FileExists(ExpandConstant('{userdocs}\PerfmonBar\config.xml')) then begin
    Log('Custom Code: Settings are present');
    Result := True;
  end
  else begin
    Log('Custom Code: Settings are NOT present');
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
  // Hide the license page
  if IsUpgrade() and (PageID = wpLicense) then
    Result := True
  else
    Result := False;
end;


procedure CleanUpSettings();
begin
  DeleteFile(ExpandConstant('{userdocs}\PerfmonBar\config.xml'));
  RemoveDir(ExpandConstant('{userdocs}\PerfmonBar'));
end;


procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpSelectTasks then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonInstall)
  else if CurPageID = wpFinished then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonFinish);
end;


procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep = ssInstall) and IsOldBuildInstalled() then
    UninstallOldVersion();
  if (CurStep = ssPostInstall) and IsTaskSelected('reset_settings') then
    CleanUpSettings();
end;


procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  // When uninstalling, ask the user to delete PerfmonBar's config file
  if (CurUninstallStep = usUninstall) and ConfigExistsCheck() then begin
    if SuppressibleMsgBox(CustomMessage('msg_DeleteSettings'), mbConfirmation, MB_YESNO or MB_DEFBUTTON2, IDNO) = IDYES then begin
      CleanUpSettings();
    end;
  end;
end;


function InitializeSetup(): Boolean;
begin
  // Create a mutex for the installer and if it's already running then show a message and stop installation
  if CheckForMutexes(installer_mutex_name) and not WizardSilent() then begin
    SuppressibleMsgBox(CustomMessage('msg_SetupIsRunningWarning'), mbError, MB_OK, MB_OK);
    Result := False;
  end
  else begin
    Result := True;
    CreateMutex(installer_mutex_name);
  end;
end;


function InitializeUninstall(): Boolean;
begin
  if CheckForMutexes(installer_mutex_name) then begin
    SuppressibleMsgBox(CustomMessage('msg_SetupIsRunningWarning'), mbError, MB_OK, MB_OK);
    Result := False;
  end
  else begin
    Result := True;
    CreateMutex(installer_mutex_name);
  end;
end;
