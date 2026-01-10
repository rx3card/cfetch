; Script de instalación para CFetch
; Creado con Inno Setup

[Setup]
AppName=CFetch
AppVersion=1.0
DefaultDirName={autopf}\CFetch
DefaultGroupName=CFetch
UninstallDisplayIcon={app}\cfetch.exe
Compression=lzma2
SolidCompression=yes
OutputDir=.\dist
OutputBaseFilename=CFetch-Setup
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Files]
Source: "build\cfetch.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\CFetch"; Filename: "{app}\cfetch.exe"
Name: "{group}\Desinstalar CFetch"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\cfetch.exe"; Description: "Ejecutar CFetch"; Flags: nowait postinstall skipifsilent

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
var
  Path: string;
  Paths: string;
begin
  if CurStep = ssPostInstall then
  begin
    // Agregar al PATH del sistema
    RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', Paths);
    if Pos(';' + ExpandConstant('{app}'), Paths) = 0 then
    begin
      Paths := Paths + ';' + ExpandConstant('{app}');
      RegWriteStringValue(HKEY_LOCAL_MACHINE, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', Paths);
    end;
  end;
end;