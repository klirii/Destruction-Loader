; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{523C8FFD-CB64-4FC3-9C42-A2E247DC0941}
AppName=Destruction Loader
AppVersion=1.9.2
;AppVerName=Destruction Loader 1.8.0
AppPublisher=Destruction LLC
AppPublisherURL=https://vk.com/destructiqn
AppSupportURL=https://vk.com/destructiqn
AppUpdatesURL=https://vk.com/destructiqn
DefaultDirName={autopf}\Destruction
DefaultGroupName=Destruction Loader
DisableProgramGroupPage=yes
PrivilegesRequired=admin
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=C:\Users\ksafa\OneDrive\������� ����
;{#SetupSetting("AppVersion")}
OutputBaseFilename=Loader-Setup
SetupIconFile=C:\Projects\Destruction\Destruction Loader\resources\logo.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce

[Files]
Source: "C:\Users\ksafa\OneDrive\������� ����\Loader.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\ksafa\Documents\VC_redist.x64.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Destruction Loader"; Filename: "{app}\Loader.exe"
Name: "{autodesktop}\Destruction Loader"; Filename: "{app}\Loader.exe"; Tasks: desktopicon

[Run]
Filename: {tmp}\VC_redist.x64.exe; Parameters: "/quiet /norestart"; Flags: waituntilterminated; StatusMsg: "Installing Microsoft Visual C++ 2015-2022 Redistributable Package... Please do not turn off your computer." 
Filename: "{app}\Loader.exe"; Description: "{cm:LaunchProgram,Destruction Loader}"; Flags: nowait postinstall skipifsilent runascurrentuser

