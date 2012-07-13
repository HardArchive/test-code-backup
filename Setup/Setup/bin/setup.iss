; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "双硬盘隔离卡切换软件"
#define MyAppVersion "3.0.0"
#define MyAppPublisher "深圳利谱信息技术有限公司"
#define MyAppExeName "NETCTL.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{85578740-6146-4C2C-BBED-68AC70F2420F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=D:\360data\重要数据\桌面\
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "NETCTL.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "AboutSn.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "AdapterCtl.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "arpctl.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "CH364PGM.sys"; DestDir: "{app}"; Flags: ignoreversion
Source: "CH364PGMDLL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "CH364PGMDLL.H"; DestDir: "{app}"; Flags: ignoreversion
Source: "CH364PGMDLL.lib"; DestDir: "{app}"; Flags: ignoreversion
Source: "CH366.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "PowerOff.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "EraseFile.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "GetLocalNet.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Hibernate.DLL"; DestDir: "{app}"; Flags: ignoreversion
Source: "ISSkin.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Libnet.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "netctl.data"; DestDir: "{app}"; Flags: ignoreversion
Source: "NETCTL.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "netctlCard.data"; DestDir: "{app}"; Flags: ignoreversion
Source: "PrjChangeNetIP.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "PrjCheckInterNet.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "PrjCheckMediaZH.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Resource_en.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Resource_tw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Resource_zh.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "sendpack.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Suspend.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "TP901.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "TPDeviceForbid.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "UnloadProcess.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "WatchNet.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "WinPcap_4_1_1.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "wsa.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "NetCtlService.exe"; DestDir: "{app}"; Flags: ignoreversion
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, "&", "&&")}}"; Flags: nowait postinstall skipifsilent

