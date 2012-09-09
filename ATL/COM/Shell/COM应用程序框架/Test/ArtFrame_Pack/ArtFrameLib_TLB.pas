unit ArtFrameLib_TLB;

// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// PASTLWTR : 1.2
// File generated on 2006-6-30 14:25:22 from Type Library described below.

// ************************************************************************  //
// Type Lib: f:\Test\ArtFrame\bin\ArtFrame.dll (1)
// LIBID: {28462839-FA53-4A4C-A776-676CD60D4238}
// LCID: 0
// Helpfile: 
// HelpString: ArtFrame 1.0 Type Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
// ************************************************************************ //
// *************************************************************************//
// NOTE:                                                                      
// Items guarded by $IFDEF_LIVE_SERVER_AT_DESIGN_TIME are used by properties  
// which return objects that may need to be explicitly created via a function 
// call prior to any access via the property. These items have been disabled  
// in order to prevent accidental use from within the object inspector. You   
// may enable them by defining LIVE_SERVER_AT_DESIGN_TIME or by selectively   
// removing them from the $IFDEF blocks. However, such items must still be    
// programmatically created via a method of the appropriate CoClass before    
// they can be used.                                                          
{$TYPEDADDRESS OFF} // Unit must be compiled without type-checked pointers. 
{$WARN SYMBOL_PLATFORM OFF}
{$WRITEABLECONST ON}
{$VARPROPSETTER ON}
interface

uses Windows, ActiveX, Classes, Graphics, OleServer, StdVCL, Variants;
  

// *********************************************************************//
// GUIDS declared in the TypeLibrary. Following prefixes are used:        
//   Type Libraries     : LIBID_xxxx                                      
//   CoClasses          : CLASS_xxxx                                      
//   DISPInterfaces     : DIID_xxxx                                       
//   Non-DISP interfaces: IID_xxxx                                        
// *********************************************************************//
const
  // TypeLibrary Major and minor versions
  ArtFrameLibMajorVersion = 1;
  ArtFrameLibMinorVersion = 0;

  LIBID_ArtFrameLib: TGUID = '{28462839-FA53-4A4C-A776-676CD60D4238}';

  DIID__IMDIFrameEvents: TGUID = '{866BD9AA-8EC6-45D1-B1D2-55B1E8967E37}';
  IID__COMFrameDispObj: TGUID = '{3B8C7933-4F37-4D83-9107-CF68586155E2}';
  IID_IMDIFrame: TGUID = '{D3137FF6-206B-4767-A905-5D252B3D9325}';
  CLASS_MDIFrame: TGUID = '{B092D48B-B0B7-4AFE-9010-986274A78673}';
  DIID__IMDIChildFrameEvents: TGUID = '{5BE13374-8203-4A09-8A34-CAE92D2747A0}';
  IID_IMDIChildFrame: TGUID = '{A61BB8F9-6141-427C-AA31-CC4888335E51}';
  CLASS_MDIChildFrame: TGUID = '{47B924E4-4E16-4DB9-AE43-3F0144BE0DD2}';
  IID_IDataView: TGUID = '{E2E319A4-D764-4DC1-95CD-C92D3754D95B}';
  IID_IMDIChildFrames: TGUID = '{DBDC4265-324B-4E01-9BBE-8AB76FBB8CF6}';
  CLASS_MDIChildFrames: TGUID = '{9462FE98-891A-463D-9B6D-CDEA80462BB2}';
type

// *********************************************************************//
// Forward declaration of types defined in TypeLibrary                    
// *********************************************************************//
  _IMDIFrameEvents = dispinterface;
  _COMFrameDispObj = interface;
  _COMFrameDispObjDisp = dispinterface;
  IMDIFrame = interface;
  IMDIFrameDisp = dispinterface;
  _IMDIChildFrameEvents = dispinterface;
  IMDIChildFrame = interface;
  IMDIChildFrameDisp = dispinterface;
  IDataView = interface;
  IDataViewDisp = dispinterface;
  IMDIChildFrames = interface;
  IMDIChildFramesDisp = dispinterface;

// *********************************************************************//
// Declaration of CoClasses defined in Type Library                       
// (NOTE: Here we map each CoClass to its Default Interface)              
// *********************************************************************//
  MDIFrame = IMDIFrame;
  MDIChildFrame = IMDIChildFrame;
  MDIChildFrames = IMDIChildFrames;


// *********************************************************************//
// Declaration of structures, unions and aliases.                         
// *********************************************************************//
  wireHMENU = ^_RemotableHandle; 
  wireHWND = ^_RemotableHandle; 
  PWordBool1 = ^WordBool; {*}
  PUserType1 = ^CreateMDIStruct; {*}
  PPUserType1 = ^wireHWND; {*}


  CreateMDIStruct = packed record
    cbSize: SYSUINT;
    mask: LongWord;
    lpszWindowName: PWideChar;
    hMenu: wireHMENU;
    lParam: Integer;
  end;

  __MIDL_IWinTypes_0009 = record
    case Integer of
      0: (hInproc: Integer);
      1: (hRemote: Integer);
  end;

  _RemotableHandle = packed record
    fContext: Integer;
    u: __MIDL_IWinTypes_0009;
  end;


// *********************************************************************//
// DispIntf:  _IMDIFrameEvents
// Flags:     (4096) Dispatchable
// GUID:      {866BD9AA-8EC6-45D1-B1D2-55B1E8967E37}
// *********************************************************************//
  _IMDIFrameEvents = dispinterface
    ['{866BD9AA-8EC6-45D1-B1D2-55B1E8967E37}']
    procedure OnQuit(var vbQuit: WordBool); dispid 1;
    procedure NotifyCommand(codeNotify: Integer; cmdID: Integer; var bHandle: WordBool); dispid 2;
  end;

// *********************************************************************//
// Interface: _COMFrameDispObj
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {3B8C7933-4F37-4D83-9107-CF68586155E2}
// *********************************************************************//
  _COMFrameDispObj = interface(IDispatch)
    ['{3B8C7933-4F37-4D83-9107-CF68586155E2}']
    function Get_COMMDIFrame: IDispatch; safecall;
    property COMMDIFrame: IDispatch read Get_COMMDIFrame;
  end;

// *********************************************************************//
// DispIntf:  _COMFrameDispObjDisp
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {3B8C7933-4F37-4D83-9107-CF68586155E2}
// *********************************************************************//
  _COMFrameDispObjDisp = dispinterface
    ['{3B8C7933-4F37-4D83-9107-CF68586155E2}']
    property COMMDIFrame: IDispatch readonly dispid 50;
  end;

// *********************************************************************//
// Interface: IMDIFrame
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {D3137FF6-206B-4767-A905-5D252B3D9325}
// *********************************************************************//
  IMDIFrame = interface(_COMFrameDispObj)
    ['{D3137FF6-206B-4767-A905-5D252B3D9325}']
    procedure Run; safecall;
    procedure CreateWnd(var lpMDIStruct: CreateMDIStruct); safecall;
    procedure Quit; safecall;
    procedure ShowMe(vbShow: WordBool); safecall;
    function Get_MDIChildFrames: IDispatch; safecall;
    procedure AboutCOMFrame; safecall;
    property MDIChildFrames: IDispatch read Get_MDIChildFrames;
  end;

// *********************************************************************//
// DispIntf:  IMDIFrameDisp
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {D3137FF6-206B-4767-A905-5D252B3D9325}
// *********************************************************************//
  IMDIFrameDisp = dispinterface
    ['{D3137FF6-206B-4767-A905-5D252B3D9325}']
    procedure Run; dispid 1;
    procedure CreateWnd(var lpMDIStruct: {??CreateMDIStruct}OleVariant); dispid 2;
    procedure Quit; dispid 3;
    procedure ShowMe(vbShow: WordBool); dispid 4;
    property MDIChildFrames: IDispatch readonly dispid 5;
    procedure AboutCOMFrame; dispid 52;
    property COMMDIFrame: IDispatch readonly dispid 50;
  end;

// *********************************************************************//
// DispIntf:  _IMDIChildFrameEvents
// Flags:     (4096) Dispatchable
// GUID:      {5BE13374-8203-4A09-8A34-CAE92D2747A0}
// *********************************************************************//
  _IMDIChildFrameEvents = dispinterface
    ['{5BE13374-8203-4A09-8A34-CAE92D2747A0}']
    procedure NotifyCommand(codeNotify: SYSUINT; cmdID: SYSUINT; var bHandle: WordBool); dispid 1;
  end;

// *********************************************************************//
// Interface: IMDIChildFrame
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {A61BB8F9-6141-427C-AA31-CC4888335E51}
// *********************************************************************//
  IMDIChildFrame = interface(_COMFrameDispObj)
    ['{A61BB8F9-6141-427C-AA31-CC4888335E51}']
    procedure PushDataView(const lpDataView: IDataView); safecall;
    procedure PopDataView(const lpCW: IDataView); safecall;
    function Get_ActiveChildWindow: IDataView; safecall;
    procedure Set_ActiveChildWindow(const pVal: IDataView); safecall;
    function Get_Title: WideString; safecall;
    procedure Set_Title(const pVal: WideString); safecall;
    property ActiveChildWindow: IDataView read Get_ActiveChildWindow write Set_ActiveChildWindow;
    property Title: WideString read Get_Title write Set_Title;
  end;

// *********************************************************************//
// DispIntf:  IMDIChildFrameDisp
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {A61BB8F9-6141-427C-AA31-CC4888335E51}
// *********************************************************************//
  IMDIChildFrameDisp = dispinterface
    ['{A61BB8F9-6141-427C-AA31-CC4888335E51}']
    procedure PushDataView(const lpDataView: IDataView); dispid 4;
    procedure PopDataView(const lpCW: IDataView); dispid 6;
    property ActiveChildWindow: IDataView dispid 7;
    property Title: WideString dispid 51;
    property COMMDIFrame: IDispatch readonly dispid 50;
  end;

// *********************************************************************//
// Interface: IDataView
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {E2E319A4-D764-4DC1-95CD-C92D3754D95B}
// *********************************************************************//
  IDataView = interface(IDispatch)
    ['{E2E319A4-D764-4DC1-95CD-C92D3754D95B}']
    procedure NotifyCommand(codeNotify: SYSUINT; cmdID: SYSUINT; var bHandle: WordBool); safecall;
    procedure GetWnd(hWnd: PPUserType1); safecall;
    procedure OnActiveWindow(nType: SYSUINT); safecall;
    procedure OnCloseWindow(var vbClose: WordBool); safecall;
    procedure CreateWnd(var hWndParent: _RemotableHandle); safecall;
  end;

// *********************************************************************//
// DispIntf:  IDataViewDisp
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {E2E319A4-D764-4DC1-95CD-C92D3754D95B}
// *********************************************************************//
  IDataViewDisp = dispinterface
    ['{E2E319A4-D764-4DC1-95CD-C92D3754D95B}']
    procedure NotifyCommand(codeNotify: SYSUINT; cmdID: SYSUINT; var bHandle: WordBool); dispid 1;
    procedure GetWnd(hWnd: {??PPUserType1}OleVariant); dispid 2;
    procedure OnActiveWindow(nType: SYSUINT); dispid 3;
    procedure OnCloseWindow(var vbClose: WordBool); dispid 4;
    procedure CreateWnd(var hWndParent: {??_RemotableHandle}OleVariant); dispid 5;
  end;

// *********************************************************************//
// Interface: IMDIChildFrames
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {DBDC4265-324B-4E01-9BBE-8AB76FBB8CF6}
// *********************************************************************//
  IMDIChildFrames = interface(_COMFrameDispObj)
    ['{DBDC4265-324B-4E01-9BBE-8AB76FBB8CF6}']
    procedure CreateChildFrame(const lpdv: IDataView; out lpcf: IMDIChildFrame); safecall;
  end;

// *********************************************************************//
// DispIntf:  IMDIChildFramesDisp
// Flags:     (4544) Dual NonExtensible OleAutomation Dispatchable
// GUID:      {DBDC4265-324B-4E01-9BBE-8AB76FBB8CF6}
// *********************************************************************//
  IMDIChildFramesDisp = dispinterface
    ['{DBDC4265-324B-4E01-9BBE-8AB76FBB8CF6}']
    procedure CreateChildFrame(const lpdv: IDataView; out lpcf: IMDIChildFrame); dispid 1;
    property COMMDIFrame: IDispatch readonly dispid 50;
  end;

// *********************************************************************//
// The Class CoMDIFrame provides a Create and CreateRemote method to          
// create instances of the default interface IMDIFrame exposed by              
// the CoClass MDIFrame. The functions are intended to be used by             
// clients wishing to automate the CoClass objects exposed by the         
// server of this typelibrary.                                            
// *********************************************************************//
  CoMDIFrame = class
    class function Create: IMDIFrame;
    class function CreateRemote(const MachineName: string): IMDIFrame;
  end;

  TMDIFrameOnQuit = procedure(ASender: TObject; var vbQuit: WordBool) of object;
  TMDIFrameNotifyCommand = procedure(ASender: TObject; codeNotify: Integer; cmdID: Integer; 
                                                       var bHandle: WordBool) of object;


// *********************************************************************//
// OLE Server Proxy class declaration
// Server Object    : TMDIFrame
// Help String      : MDIFrame Class
// Default Interface: IMDIFrame
// Def. Intf. DISP? : No
// Event   Interface: _IMDIFrameEvents
// TypeFlags        : (2) CanCreate
// *********************************************************************//
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  TMDIFrameProperties= class;
{$ENDIF}
  TMDIFrame = class(TOleServer)
  private
    FOnQuit: TMDIFrameOnQuit;
    FOnNotifyCommand: TMDIFrameNotifyCommand;
    FAutoQuit: Boolean;
    FIntf: IMDIFrame;
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
    FProps: TMDIFrameProperties;
    function GetServerProperties: TMDIFrameProperties;
{$ENDIF}
    function GetDefaultInterface: IMDIFrame;
  protected
    procedure InitServerData; override;
    procedure InvokeEvent(DispID: TDispID; var Params: TVariantArray); override;
    function Get_MDIChildFrames: IDispatch;
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;
    procedure Connect; override;
    procedure ConnectTo(svrIntf: IMDIFrame);
    procedure Disconnect; override;
    procedure Run;
    procedure CreateWnd(var lpMDIStruct: CreateMDIStruct);
    procedure Quit;
    procedure ShowMe(vbShow: WordBool);
    procedure AboutCOMFrame;
    property DefaultInterface: IMDIFrame read GetDefaultInterface;
    property MDIChildFrames: IDispatch read Get_MDIChildFrames;
  published
    property AutoQuit: Boolean read FAutoQuit write FAutoQuit; 
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
    property Server: TMDIFrameProperties read GetServerProperties;
{$ENDIF}
    property OnQuit: TMDIFrameOnQuit read FOnQuit write FOnQuit;
    property OnNotifyCommand: TMDIFrameNotifyCommand read FOnNotifyCommand write FOnNotifyCommand;
  end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
// *********************************************************************//
// OLE Server Properties Proxy Class
// Server Object    : TMDIFrame
// (This object is used by the IDE's Property Inspector to allow editing
//  of the properties of this server)
// *********************************************************************//
 TMDIFrameProperties = class(TPersistent)
  private
    FServer:    TMDIFrame;
    function    GetDefaultInterface: IMDIFrame;
    constructor Create(AServer: TMDIFrame);
  protected
    function Get_MDIChildFrames: IDispatch;
  public
    property DefaultInterface: IMDIFrame read GetDefaultInterface;
  published
  end;
{$ENDIF}


// *********************************************************************//
// The Class CoMDIChildFrame provides a Create and CreateRemote method to          
// create instances of the default interface IMDIChildFrame exposed by              
// the CoClass MDIChildFrame. The functions are intended to be used by             
// clients wishing to automate the CoClass objects exposed by the         
// server of this typelibrary.                                            
// *********************************************************************//
  CoMDIChildFrame = class
    class function Create: IMDIChildFrame;
    class function CreateRemote(const MachineName: string): IMDIChildFrame;
  end;

  TMDIChildFrameNotifyCommand = procedure(ASender: TObject; codeNotify: SYSUINT; cmdID: SYSUINT; 
                                                            var bHandle: WordBool) of object;


// *********************************************************************//
// OLE Server Proxy class declaration
// Server Object    : TMDIChildFrame
// Help String      : MDIChildFrame Class
// Default Interface: IMDIChildFrame
// Def. Intf. DISP? : No
// Event   Interface: _IMDIChildFrameEvents
// TypeFlags        : (2) CanCreate
// *********************************************************************//
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  TMDIChildFrameProperties= class;
{$ENDIF}
  TMDIChildFrame = class(TOleServer)
  private
    FOnNotifyCommand: TMDIChildFrameNotifyCommand;
    FIntf: IMDIChildFrame;
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
    FProps: TMDIChildFrameProperties;
    function GetServerProperties: TMDIChildFrameProperties;
{$ENDIF}
    function GetDefaultInterface: IMDIChildFrame;
  protected
    procedure InitServerData; override;
    procedure InvokeEvent(DispID: TDispID; var Params: TVariantArray); override;
    function Get_ActiveChildWindow: IDataView;
    procedure Set_ActiveChildWindow(const pVal: IDataView);
    function Get_Title: WideString;
    procedure Set_Title(const pVal: WideString);
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;
    procedure Connect; override;
    procedure ConnectTo(svrIntf: IMDIChildFrame);
    procedure Disconnect; override;
    procedure PushDataView(const lpDataView: IDataView);
    procedure PopDataView(const lpCW: IDataView);
    property DefaultInterface: IMDIChildFrame read GetDefaultInterface;
    property ActiveChildWindow: IDataView read Get_ActiveChildWindow write Set_ActiveChildWindow;
    property Title: WideString read Get_Title write Set_Title;
  published
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
    property Server: TMDIChildFrameProperties read GetServerProperties;
{$ENDIF}
    property OnNotifyCommand: TMDIChildFrameNotifyCommand read FOnNotifyCommand write FOnNotifyCommand;
  end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
// *********************************************************************//
// OLE Server Properties Proxy Class
// Server Object    : TMDIChildFrame
// (This object is used by the IDE's Property Inspector to allow editing
//  of the properties of this server)
// *********************************************************************//
 TMDIChildFrameProperties = class(TPersistent)
  private
    FServer:    TMDIChildFrame;
    function    GetDefaultInterface: IMDIChildFrame;
    constructor Create(AServer: TMDIChildFrame);
  protected
    function Get_ActiveChildWindow: IDataView;
    procedure Set_ActiveChildWindow(const pVal: IDataView);
    function Get_Title: WideString;
    procedure Set_Title(const pVal: WideString);
  public
    property DefaultInterface: IMDIChildFrame read GetDefaultInterface;
  published
    property ActiveChildWindow: IDataView read Get_ActiveChildWindow write Set_ActiveChildWindow;
    property Title: WideString read Get_Title write Set_Title;
  end;
{$ENDIF}


// *********************************************************************//
// The Class CoMDIChildFrames provides a Create and CreateRemote method to          
// create instances of the default interface IMDIChildFrames exposed by              
// the CoClass MDIChildFrames. The functions are intended to be used by             
// clients wishing to automate the CoClass objects exposed by the         
// server of this typelibrary.                                            
// *********************************************************************//
  CoMDIChildFrames = class
    class function Create: IMDIChildFrames;
    class function CreateRemote(const MachineName: string): IMDIChildFrames;
  end;


// *********************************************************************//
// OLE Server Proxy class declaration
// Server Object    : TMDIChildFrames
// Help String      : MDIChildFrames Class
// Default Interface: IMDIChildFrames
// Def. Intf. DISP? : No
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  TMDIChildFramesProperties= class;
{$ENDIF}
  TMDIChildFrames = class(TOleServer)
  private
    FIntf: IMDIChildFrames;
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
    FProps: TMDIChildFramesProperties;
    function GetServerProperties: TMDIChildFramesProperties;
{$ENDIF}
    function GetDefaultInterface: IMDIChildFrames;
  protected
    procedure InitServerData; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;
    procedure Connect; override;
    procedure ConnectTo(svrIntf: IMDIChildFrames);
    procedure Disconnect; override;
    procedure CreateChildFrame(const lpdv: IDataView; out lpcf: IMDIChildFrame);
    property DefaultInterface: IMDIChildFrames read GetDefaultInterface;
  published
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
    property Server: TMDIChildFramesProperties read GetServerProperties;
{$ENDIF}
  end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
// *********************************************************************//
// OLE Server Properties Proxy Class
// Server Object    : TMDIChildFrames
// (This object is used by the IDE's Property Inspector to allow editing
//  of the properties of this server)
// *********************************************************************//
 TMDIChildFramesProperties = class(TPersistent)
  private
    FServer:    TMDIChildFrames;
    function    GetDefaultInterface: IMDIChildFrames;
    constructor Create(AServer: TMDIChildFrames);
  protected
  public
    property DefaultInterface: IMDIChildFrames read GetDefaultInterface;
  published
  end;
{$ENDIF}


procedure Register;

resourcestring
  dtlServerPage = 'Art Frmae';

  dtlOcxPage = 'Art Frmae';

implementation

uses ComObj;

class function CoMDIFrame.Create: IMDIFrame;
begin
  Result := CreateComObject(CLASS_MDIFrame) as IMDIFrame;
end;

class function CoMDIFrame.CreateRemote(const MachineName: string): IMDIFrame;
begin
  Result := CreateRemoteComObject(MachineName, CLASS_MDIFrame) as IMDIFrame;
end;

procedure TMDIFrame.InitServerData;
const
  CServerData: TServerData = (
    ClassID:   '{B092D48B-B0B7-4AFE-9010-986274A78673}';
    IntfIID:   '{D3137FF6-206B-4767-A905-5D252B3D9325}';
    EventIID:  '{866BD9AA-8EC6-45D1-B1D2-55B1E8967E37}';
    LicenseKey: nil;
    Version: 500);
begin
  ServerData := @CServerData;
end;

procedure TMDIFrame.Connect;
var
  punk: IUnknown;
begin
  if FIntf = nil then
  begin
    punk := GetServer;
    ConnectEvents(punk);
    Fintf:= punk as IMDIFrame;
  end;
end;

procedure TMDIFrame.ConnectTo(svrIntf: IMDIFrame);
begin
  Disconnect;
  FIntf := svrIntf;
  ConnectEvents(FIntf);
end;

procedure TMDIFrame.DisConnect;
begin
  if Fintf <> nil then
  begin
    DisconnectEvents(FIntf);
    if FAutoQuit then
      Quit();
    FIntf := nil;
  end;
end;

function TMDIFrame.GetDefaultInterface: IMDIFrame;
begin
  if FIntf = nil then
    Connect;
  Assert(FIntf <> nil, 'DefaultInterface is NULL. Component is not connected to Server. You must call ''Connect'' or ''ConnectTo'' before this operation');
  Result := FIntf;
end;

constructor TMDIFrame.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  FProps := TMDIFrameProperties.Create(Self);
{$ENDIF}
end;

destructor TMDIFrame.Destroy;
begin
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  FProps.Free;
{$ENDIF}
  inherited Destroy;
end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
function TMDIFrame.GetServerProperties: TMDIFrameProperties;
begin
  Result := FProps;
end;
{$ENDIF}

procedure TMDIFrame.InvokeEvent(DispID: TDispID; var Params: TVariantArray);
begin
  case DispID of
    -1: Exit;  // DISPID_UNKNOWN
    1: if Assigned(FOnQuit) then
         FOnQuit(Self, WordBool((TVarData(Params[0]).VPointer)^) {var WordBool});
    2: if Assigned(FOnNotifyCommand) then
         FOnNotifyCommand(Self,
                          Params[0] {Integer},
                          Params[1] {Integer},
                          WordBool((TVarData(Params[2]).VPointer)^) {var WordBool});
  end; {case DispID}
end;

function TMDIFrame.Get_MDIChildFrames: IDispatch;
begin
    Result := DefaultInterface.MDIChildFrames;
end;

procedure TMDIFrame.Run;
begin
  DefaultInterface.Run;
end;

procedure TMDIFrame.CreateWnd(var lpMDIStruct: CreateMDIStruct);
begin
  DefaultInterface.CreateWnd(lpMDIStruct);
end;

procedure TMDIFrame.Quit;
begin
  DefaultInterface.Quit;
end;

procedure TMDIFrame.ShowMe(vbShow: WordBool);
begin
  DefaultInterface.ShowMe(vbShow);
end;

procedure TMDIFrame.AboutCOMFrame;
begin
  DefaultInterface.AboutCOMFrame;
end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
constructor TMDIFrameProperties.Create(AServer: TMDIFrame);
begin
  inherited Create;
  FServer := AServer;
end;

function TMDIFrameProperties.GetDefaultInterface: IMDIFrame;
begin
  Result := FServer.DefaultInterface;
end;

function TMDIFrameProperties.Get_MDIChildFrames: IDispatch;
begin
    Result := DefaultInterface.MDIChildFrames;
end;

{$ENDIF}

class function CoMDIChildFrame.Create: IMDIChildFrame;
begin
  Result := CreateComObject(CLASS_MDIChildFrame) as IMDIChildFrame;
end;

class function CoMDIChildFrame.CreateRemote(const MachineName: string): IMDIChildFrame;
begin
  Result := CreateRemoteComObject(MachineName, CLASS_MDIChildFrame) as IMDIChildFrame;
end;

procedure TMDIChildFrame.InitServerData;
const
  CServerData: TServerData = (
    ClassID:   '{47B924E4-4E16-4DB9-AE43-3F0144BE0DD2}';
    IntfIID:   '{A61BB8F9-6141-427C-AA31-CC4888335E51}';
    EventIID:  '{5BE13374-8203-4A09-8A34-CAE92D2747A0}';
    LicenseKey: nil;
    Version: 500);
begin
  ServerData := @CServerData;
end;

procedure TMDIChildFrame.Connect;
var
  punk: IUnknown;
begin
  if FIntf = nil then
  begin
    punk := GetServer;
    ConnectEvents(punk);
    Fintf:= punk as IMDIChildFrame;
  end;
end;

procedure TMDIChildFrame.ConnectTo(svrIntf: IMDIChildFrame);
begin
  Disconnect;
  FIntf := svrIntf;
  ConnectEvents(FIntf);
end;

procedure TMDIChildFrame.DisConnect;
begin
  if Fintf <> nil then
  begin
    DisconnectEvents(FIntf);
    FIntf := nil;
  end;
end;

function TMDIChildFrame.GetDefaultInterface: IMDIChildFrame;
begin
  if FIntf = nil then
    Connect;
  Assert(FIntf <> nil, 'DefaultInterface is NULL. Component is not connected to Server. You must call ''Connect'' or ''ConnectTo'' before this operation');
  Result := FIntf;
end;

constructor TMDIChildFrame.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  FProps := TMDIChildFrameProperties.Create(Self);
{$ENDIF}
end;

destructor TMDIChildFrame.Destroy;
begin
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  FProps.Free;
{$ENDIF}
  inherited Destroy;
end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
function TMDIChildFrame.GetServerProperties: TMDIChildFrameProperties;
begin
  Result := FProps;
end;
{$ENDIF}

procedure TMDIChildFrame.InvokeEvent(DispID: TDispID; var Params: TVariantArray);
begin
  case DispID of
    -1: Exit;  // DISPID_UNKNOWN
    1: if Assigned(FOnNotifyCommand) then
         FOnNotifyCommand(Self,
                          Params[0] {SYSUINT},
                          Params[1] {SYSUINT},
                          WordBool((TVarData(Params[2]).VPointer)^) {var WordBool});
  end; {case DispID}
end;

function TMDIChildFrame.Get_ActiveChildWindow: IDataView;
begin
    Result := DefaultInterface.ActiveChildWindow;
end;

procedure TMDIChildFrame.Set_ActiveChildWindow(const pVal: IDataView);
begin
  DefaultInterface.Set_ActiveChildWindow(pVal);
end;

function TMDIChildFrame.Get_Title: WideString;
begin
    Result := DefaultInterface.Title;
end;

procedure TMDIChildFrame.Set_Title(const pVal: WideString);
  { Warning: The property Title has a setter and a getter whose
    types do not match. Delphi was unable to generate a property of
    this sort and so is using a Variant as a passthrough. }
var
  InterfaceVariant: OleVariant;
begin
  InterfaceVariant := DefaultInterface;
  InterfaceVariant.Title := pVal;
end;

procedure TMDIChildFrame.PushDataView(const lpDataView: IDataView);
begin
  DefaultInterface.PushDataView(lpDataView);
end;

procedure TMDIChildFrame.PopDataView(const lpCW: IDataView);
begin
  DefaultInterface.PopDataView(lpCW);
end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
constructor TMDIChildFrameProperties.Create(AServer: TMDIChildFrame);
begin
  inherited Create;
  FServer := AServer;
end;

function TMDIChildFrameProperties.GetDefaultInterface: IMDIChildFrame;
begin
  Result := FServer.DefaultInterface;
end;

function TMDIChildFrameProperties.Get_ActiveChildWindow: IDataView;
begin
    Result := DefaultInterface.ActiveChildWindow;
end;

procedure TMDIChildFrameProperties.Set_ActiveChildWindow(const pVal: IDataView);
begin
  DefaultInterface.Set_ActiveChildWindow(pVal);
end;

function TMDIChildFrameProperties.Get_Title: WideString;
begin
    Result := DefaultInterface.Title;
end;

procedure TMDIChildFrameProperties.Set_Title(const pVal: WideString);
  { Warning: The property Title has a setter and a getter whose
    types do not match. Delphi was unable to generate a property of
    this sort and so is using a Variant as a passthrough. }
var
  InterfaceVariant: OleVariant;
begin
  InterfaceVariant := DefaultInterface;
  InterfaceVariant.Title := pVal;
end;

{$ENDIF}

class function CoMDIChildFrames.Create: IMDIChildFrames;
begin
  Result := CreateComObject(CLASS_MDIChildFrames) as IMDIChildFrames;
end;

class function CoMDIChildFrames.CreateRemote(const MachineName: string): IMDIChildFrames;
begin
  Result := CreateRemoteComObject(MachineName, CLASS_MDIChildFrames) as IMDIChildFrames;
end;

procedure TMDIChildFrames.InitServerData;
const
  CServerData: TServerData = (
    ClassID:   '{9462FE98-891A-463D-9B6D-CDEA80462BB2}';
    IntfIID:   '{DBDC4265-324B-4E01-9BBE-8AB76FBB8CF6}';
    EventIID:  '';
    LicenseKey: nil;
    Version: 500);
begin
  ServerData := @CServerData;
end;

procedure TMDIChildFrames.Connect;
var
  punk: IUnknown;
begin
  if FIntf = nil then
  begin
    punk := GetServer;
    Fintf:= punk as IMDIChildFrames;
  end;
end;

procedure TMDIChildFrames.ConnectTo(svrIntf: IMDIChildFrames);
begin
  Disconnect;
  FIntf := svrIntf;
end;

procedure TMDIChildFrames.DisConnect;
begin
  if Fintf <> nil then
  begin
    FIntf := nil;
  end;
end;

function TMDIChildFrames.GetDefaultInterface: IMDIChildFrames;
begin
  if FIntf = nil then
    Connect;
  Assert(FIntf <> nil, 'DefaultInterface is NULL. Component is not connected to Server. You must call ''Connect'' or ''ConnectTo'' before this operation');
  Result := FIntf;
end;

constructor TMDIChildFrames.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  FProps := TMDIChildFramesProperties.Create(Self);
{$ENDIF}
end;

destructor TMDIChildFrames.Destroy;
begin
{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
  FProps.Free;
{$ENDIF}
  inherited Destroy;
end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
function TMDIChildFrames.GetServerProperties: TMDIChildFramesProperties;
begin
  Result := FProps;
end;
{$ENDIF}

procedure TMDIChildFrames.CreateChildFrame(const lpdv: IDataView; out lpcf: IMDIChildFrame);
begin
  DefaultInterface.CreateChildFrame(lpdv, lpcf);
end;

{$IFDEF LIVE_SERVER_AT_DESIGN_TIME}
constructor TMDIChildFramesProperties.Create(AServer: TMDIChildFrames);
begin
  inherited Create;
  FServer := AServer;
end;

function TMDIChildFramesProperties.GetDefaultInterface: IMDIChildFrames;
begin
  Result := FServer.DefaultInterface;
end;

{$ENDIF}

procedure Register;
begin
  RegisterComponents(dtlServerPage, [TMDIFrame, TMDIChildFrame, TMDIChildFrames]);
end;

end.
