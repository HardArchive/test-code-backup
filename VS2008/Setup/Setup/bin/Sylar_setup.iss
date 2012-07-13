; 脚本用 Inno Setup 脚本向导 生成。
; 查阅文档获取创建 INNO SETUP 脚本文件的详细资料！

#define MyAppName "双硬盘隔离卡切换软件"
#define MyAppVersion "3.0.0"
#define MyAppPublisher "深圳市利谱信息技术有限公司"
#define MyAppURL "http://www.tiptop.com.cn/"
#define MyAppExeName "NetCtlService.exe"

[Setup]
; 注意: AppId 的值是唯一识别这个程序的标志。
; 不要在其他程序中使用相同的 AppId 值。
; (在编译器中点击菜单“工具 -> 产生 GUID”可以产生一个新的 GUID)
AppId={{B188FB09-0039-49C8-A115-A21485202DD5}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\双硬盘隔离卡切换软件
DefaultGroupName={#MyAppName}
OutputDir=D:\360data\重要数据\桌面\工作相关项目\隔离卡\安装包\Output
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes


[code]
// GLOBAL
var
g_bUseMySQL: Boolean;
g_bDBTypeSelected : Boolean;
g_bIsUpgrade : Boolean;

// The NT-service specific parts of the scrit below is taken
// from the innosetup extension knowledgebase.
// Author: Silvio Iaccarino silvio.iaccarino(at)de.adp.com
// Article created: 6 November 2002
// Article updated: 6 November 2002
// http://www13.brinkster.com/vincenzog/isxart.asp?idart=31
const WM_CLOSE=$0010;

type
SERVICE_STATUS = record
    dwServiceType     : cardinal;
    dwCurrentState     : cardinal;
    dwControlsAccepted     : cardinal;
    dwWin32ExitCode     : cardinal;
    dwServiceSpecificExitCode : cardinal;
    dwCheckPoint     : cardinal;
    dwWaitHint       : cardinal;
end;

ENUM_SERVICE_STATUS = record
  strServiceName:string;
  strDisplayName:string;
  Status:     SERVICE_STATUS;
end;
HANDLE = cardinal;
const
ENUM_SERVICE_STATUS_SIZE   = 36;

const SERVICE_ERROR_NORMAL   =1;
const
SERVICE_QUERY_CONFIG   = $1;
SERVICE_CHANGE_CONFIG   = $2;
SERVICE_QUERY_STATUS   = $4;
SERVICE_START     = $10;
SERVICE_STOP     = $20;
SERVICE_ALL_ACCESS     = $f01ff;
SC_MANAGER_ALL_ACCESS   = $f003f;
SERVICE_KERNEL_DRIVER   =$1;
SERVICE_WIN32_OWN_PROCESS = $10;
SERVICE_WIN32_SHARE_PROCESS = $20;
SERVICE_WIN32     = $30;
SERVICE_INTERACTIVE_PROCESS = $100;
SERVICE_BOOT_START       = $0;
SERVICE_SYSTEM_START     = $1;
SERVICE_AUTO_START       = $2;
SERVICE_DEMAND_START     = $3;
SERVICE_DISABLED         = $4;
SERVICE_DELETE         = $10000;
SERVICE_CONTROL_STOP   = $1;
SERVICE_CONTROL_PAUSE   = $2;
SERVICE_CONTROL_CONTINUE = $3;
SERVICE_CONTROL_INTERROGATE = $4;
SERVICE_STOPPED     = $1;
SERVICE_START_PENDING     = $2;
SERVICE_STOP_PENDING     = $3;
SERVICE_RUNNING         = $4;
SERVICE_CONTINUE_PENDING   = $5;
SERVICE_PAUSE_PENDING     = $6;
SERVICE_PAUSED         = $7;

function ControlService(hService :HANDLE; dwControl :cardinal;var ServiceStatus :SERVICE_STATUS) : boolean;
external 'ControlService@advapi32.dll stdcall';

function CloseServiceHandle(hSCObject :HANDLE): boolean;
external 'CloseServiceHandle@advapi32.dll stdcall';

function DeleteService(hService :HANDLE): boolean;
external 'DeleteService@advapi32.dll stdcall';

function CreateService(hSCManager :HANDLE;lpServiceName, lpDisplayName: string;dwDesiredAccess,dwServiceType,dwStartType,dwErrorControl: cardinal;lpBinaryPathName,lpLoadOrderGroup: String; lpdwTagId : cardinal;lpDependencies,lpServiceStartName,lpPassword :string): cardinal;
external 'CreateServiceA@advapi32.dll stdcall';

function OpenService(hSCManager :HANDLE;lpServiceName: string; dwDesiredAccess :cardinal): HANDLE;
external 'OpenServiceA@advapi32.dll stdcall';

function OpenSCManager(lpMachineName, lpDatabaseName: string; dwDesiredAccess :cardinal): HANDLE;
external 'OpenSCManagerA@advapi32.dll stdcall';

function QueryServiceStatus(hService :HANDLE;var ServiceStatus :SERVICE_STATUS) : boolean;
external 'QueryServiceStatus@advapi32.dll stdcall';

function StartService(hService:HANDLE;nNumOfpara:DWORD;strParam:String):boolean;
external 'StartServiceA@advapi32.dll stdcall';

function EnumDependentServices(hService:HANDLE;dwServiceState:DWORD;var Service_Status : Array of ENUM_SERVICE_STATUS; cbBufSize:DWORD; var pcbByteNeeded, lpServicesReturned:DWORD):boolean;
external 'EnumDependentServicesA@advapi32.dll stdcall';

{
function GetLastError():DWORD;
external 'GetLastError@Kernel32.dll stdcall';
}

// function IsModuleLoaded to call at install time
// added also setuponly flag
function IsModuleLoaded(modulename: String ):  Boolean;
external 'IsModuleLoaded@files:psvince.dll stdcall setuponly';

// function IsModuleLoadedU to call at uninstall time
// added also uninstallonly flag
function IsModuleLoadedU(modulename: String ):  Boolean;
external 'IsModuleLoaded@{app}\psvince.dll stdcall uninstallonly' ;


function OpenServiceManager() : HANDLE;
begin
if UsingWinNT() = true then begin
  Result := OpenSCManager('','ServicesActive',SC_MANAGER_ALL_ACCESS);
  if Result = 0 then
    MsgBox('the servicemanager is not available', mbError, MB_OK)
end
else begin
    MsgBox('only nt based systems support services', mbError, MB_OK)
    Result := 0;
end
end;

function IsServiceInstalled(ServiceName: string) : boolean;
var
hSCM : HANDLE;
hService: HANDLE;
begin
  hSCM := OpenServiceManager();
  Result := false;
  if hSCM <> 0 then
    begin
      hService := OpenService(hSCM,ServiceName,SERVICE_QUERY_CONFIG);
      if hService <> 0 then
      begin
        Result := true;
        CloseServiceHandle(hService)
      end;
      CloseServiceHandle(hSCM)
    end
end;

function IsServiceRunning(ServiceName: string) : boolean;
var
hSCM : HANDLE;
hService: HANDLE;
Status : SERVICE_STATUS;
begin
  hSCM := OpenServiceManager();
  Result := false;
  if hSCM <> 0 then
  begin
    hService := OpenService(hSCM,ServiceName,SERVICE_QUERY_STATUS);
    if hService <> 0 then
      begin
        if QueryServiceStatus(hService,Status) then
          begin
          Result :=(Status.dwCurrentState = SERVICE_RUNNING)
          end;
        CloseServiceHandle(hService)
      end;
    CloseServiceHandle(hSCM)
  end
end;

function StopService(ServiceName: string) : boolean;
var
hSCM : HANDLE;
hService: HANDLE;
Status : SERVICE_STATUS;
i   : integer;
begin
  hSCM := OpenServiceManager();
  Result := false;
  if hSCM <> 0 then
    begin
      hService := OpenService(hSCM,ServiceName,SERVICE_STOP);
      if hService <> 0 then
        begin
          Result := ControlService(hService,SERVICE_CONTROL_STOP,Status);
          CloseServiceHandle(hService)
        end;
      CloseServiceHandle(hSCM)

      // 延尽检查3次
      for i:=1 to 3 do
        begin
          if( IsServiceRunning(ServiceName)) then
            begin
              Sleep(3000);
            end
          else
            begin
              exit;
            end;
        end;
    end;
end;

function SartService(ServiceName: string) : boolean;
var
hSCM : HANDLE;
hService: HANDLE;
Status : SERVICE_STATUS;

begin
hSCM := OpenServiceManager();
Result := false;
if hSCM <> 0 then begin
  hService := OpenService(hSCM,ServiceName,SERVICE_START);
    if hService <> 0 then begin
      Result := StartService(hService,0,'');
        CloseServiceHandle(hService)
  end;
    CloseServiceHandle(hSCM)


end;
end;




function RemoveService(ServiceName: string) : boolean;
var
hSCM : HANDLE;
hService: HANDLE;
Status : SERVICE_STATUS;

begin
hSCM := OpenServiceManager();
Result := false;
if hSCM <> 0 then begin
  hService := OpenService(hSCM,ServiceName,SERVICE_DELETE);
    if hService <> 0 then begin
      Result := DeleteService(hService);
        //CloseServiceHandle(hService)

        RegDeleteKeyIncludingSubkeys(HKEY_LOCAL_MACHINE,'SYSTEM\CurrentControlSet\Services\'+ServiceName);
        sleep(100);
        RegDeleteKeyIncludingSubkeys(HKEY_LOCAL_MACHINE,'SYSTEM\CurrentControlSet\Services\'+ServiceName);
  end;
    CloseServiceHandle(hSCM)
end;
end;


function UninsService(ServiceName:string):boolean;
var
pStatus: array of ENUM_SERVICE_STATUS;
  hSCM : HANDLE;
hService: HANDLE;
Status : SERVICE_STATUS;
cbSizeNeed,cbSizeReturned:cardinal;
nServiceNum:integer;
i:integer;
begin
  cbSizeReturned:=0;
  nServiceNum:=0;

  while(IsServiceRunning(ServiceName)) do
    begin
      StopService(ServiceName);
    end;

  result:= RemoveService(ServiceName);
end;


function SGEnumService(strServicName:string):boolean;
var
pStatus: array of ENUM_SERVICE_STATUS;
  hSCM : HANDLE;
hService: HANDLE;
Status : SERVICE_STATUS;
cbSizeNeed,cbSizeReturned:DWORD;
nServiceNum:integer;
i:integer;
begin

hSCM := OpenServiceManager();
Result := false;
if hSCM <> 0 then begin
hService := OpenService(hSCM,strServicName,SERVICE_ALL_ACCESS);

  if hService <> 0 then begin
    SetArrayLength(pStatus,5);


    if EnumDependentServices(hService,$1,pStatus,ENUM_SERVICE_STATUS_SIZE,cbSizeNeed,cbSizeReturned) then begin

    nServiceNum:= cbSizeReturned/ENUM_SERVICE_STATUS_SIZE;
   // for i:=0 to (nServiceNum-1) do
   // begin

      //Msgbox(pStatus.strServiceName,mbInformation,MB_OK);
      //StopService(pStatus.strServiceName);
    //end;
    end ;

    //Msgbox(IntToStr(cbSizeNeed),mbInformation,MB_OK);

  CloseServiceHandle(hService)
  end;
  CloseServiceHandle(hSCM)
end;




end;

function InstallService(FileName, ServiceName, DisplayName, Description : string;ServiceType,StartType :cardinal;szDepends:string) : boolean;
var
hSCM : HANDLE;
hService: HANDLE;
begin
hSCM := OpenServiceManager();
Result := false;
if hSCM <> 0 then begin
  hService := CreateService(hSCM,ServiceName,DisplayName,SERVICE_ALL_ACCESS,ServiceType,StartType,SERVICE_ERROR_NORMAL,FileName,'',0,szDepends,'','');
  if hService <> 0 then begin
    Result := true;
    // Win2K & WinXP supports aditional description text for services
    if Description<> '' then
    RegWriteStringValue(HKLM,'System\CurrentControlSet\Services\' + ServiceName,'Description',Description);
    CloseServiceHandle(hService)
  end;
    CloseServiceHandle(hSCM)
end
end;

//var
//   is_value: integer;
//function InitializeSetup(): Boolean;
//begin
//   Result :=true;
//   is_value:=FindWindowByClassName('ReplacePicture');
//   while is_value<>0 do
//    begin
//       if Msgbox('安装程序检测到您的迅雷软件当前正在运行。'   #13#13 '您必须先关闭它然后单击“是”继续安装，或按“否”退出！', mbConfirmation, MB_YESNO) = idNO then
//           begin
//            Result :=false; //安装程序退出
//            is_value :=0;
//           end else begin
//            Result :=true;   //安装程序继续
//            is_value:=FindWindowByClassName('ReplacePicture');
//           end;
//    end;
//end;

function InitializeUninstall(): Boolean;
var
  IsAppRunning: boolean;
var
  IsCheck :  boolean;
begin
  if IsServiceInstalled('NetCtlCard Service') then
    begin
      StopService('NetCtlCard Service');
      Result :=true;
    end
  else
    Result :=true;
  IsCheck := true;
  IsAppRunning := IsModuleLoadedU('NETCTL.exe');
  IsCheck :=  IsAppRunning;
  while IsCheck do
  begin
    if MsgBox('双硬盘隔离卡切换软件正在运行,请先关闭它!', mbConfirmation, MB_OKCANCEL) = IDOK then
      begin
        IsAppRunning:= IsModuleLoadedU('NETCTL.exe') ;
        IsCheck := IsAppRunning;
      end
    else
      begin
        IsAppRunning:= true;
        IsCheck := false;
        Result:= false;
      end;
  end;
    
  if IsAppRunning then
    begin
      SartService('NetCtlCard Service');
    end
  else
    begin
      UninsService('NetCtlCard Service');
    end;

    UnloadDLL(ExpandConstant('{app}\psvince.dll'));
    Sleep(3000);
    DeleteFile('{app}\NetCtlService.exe');
end;

[UninstallDelete]
Name: {app}; Type: filesandordirs
Name: {sd}\Netctl; Type: filesandordirs


[Languages]
Name: "default"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: NetCtlService.exe; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: PrjCheckMediaZH.dll; DestDir: {app}; Flags: comparetimestamp regserver onlyifdoesntexist ignoreversion uninsnosharedfileprompt
Source: Resource_zh.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: CH364PGMDLL.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: CH364PGMDLL.H; DestDir: "{app}"; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: CH364PGMDLL.lib; DestDir: "{app}"; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: ISSkin.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: Resource_en.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: Resource_tw.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: Hibernate.DLL; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: CH364PGM.sys; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt; Tasks:
Source: NETCTL.exe; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt touch
Source: TP901.DLL; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: sendpack.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: wsa.exe; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: AdapterCtl.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: arpctl.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: CH366.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: Libnet.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: AboutSn.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: TPDeviceForbid.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: PrjCheckInterNet.dll; DestDir: {app}; Flags: comparetimestamp regserver onlyifdoesntexist ignoreversion uninsnosharedfileprompt
Source: EraseFile.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: PrjChangeNetIP.dll; DestDir: {app}; Flags: comparetimestamp regserver onlyifdoesntexist ignoreversion uninsnosharedfileprompt
Source: Suspend.exe; DestDir: {app}; Flags: comparetimestamp onlyifdoesntexist ignoreversion uninsnosharedfileprompt
Source: UnloadProcess.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: WatchNet.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: PowerOff.dll; DestDir: {app}; Flags: comparetimestamp onlyifdoesntexist ignoreversion uninsnosharedfileprompt
Source: GetLocalNet.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: WinPcap_4_1_1.exe; DestDir: {tmp}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: vcredist_x86.exe; DestDir: "{app}"; Flags: ignoreversion
Source: psvince.dll; DestDir: {app}; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: netctl.data; DestDir: {sd}\Netctl; Flags: comparetimestamp ignoreversion uninsnosharedfileprompt
Source: netctlCard.data; DestDir: {sd}\Netctl; Flags: comparetimestamp uninsnosharedfileprompt
; 注意: 不要在任何共享的系统文件使用 "Flags: ignoreversion"





[Icons]
Name: {group}\双硬盘隔离卡切换软件; Filename: {app}\netctl.exe; WorkingDir: {app}; IconIndex: 0
Name: {userdesktop}\双硬盘隔离卡切换软件; Filename: {app}\netctl.exe; Tasks: desktopicon; WorkingDir: {app}; IconIndex: 0
Name: {group}\卸载程序; Filename: {uninstallexe}; WorkingDir: {app}
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\双硬盘隔离卡切换软件"; Filename: "{app}\netctl.exe"; Tasks: quicklaunchicon

[Run]
Filename: {app}\vcredist_x86.exe; Parameters: "/q:a /r:n"; Flags: hidewizard
Filename: {app}\NetCtlService.exe; Parameters: -i; 

