unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, OleServer, ArtFrameLib_TLB, OleCtrls, SHDocVw, ActiveX;

type
  TForm1 = class(TForm, IDataView)
    //WebBrowser1: TWebBrowser;
    procedure FormCreate(Sender: TObject);
  private
    m_MDIFrame:TMDIFrame;
    { Private declarations }
  public
    { Public declarations }
    procedure NotifyCommand(codeNotify: SYSUINT; cmdID: SYSUINT; var bHandle: WordBool); safecall;
    procedure GetWnd(hWnd: PPUserType1); safecall;
    procedure OnActiveWindow(nType: SYSUINT); safecall;
    procedure OnCloseWindow(var vbClose: WordBool); safecall;
    procedure CreateWnd(var hWndParent: _RemotableHandle); safecall;
  end;

var
  Form1: TForm1;

implementation



{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
var
  cs :CreateMDIStruct;
begin
    cs.cbSize := sizeof(cs);
    cs.mask := 0;
    cs.lpszWindowName := WideString('Delphi Test artFrame');
    cs.hMenu := 0;
    cs.lParam := 0;
    m_MDIFrame := TMDIFrame.Create(nil);
    m_MDIFrame.CreateWnd(cs);
    m_MDIFrame.ShowMe(true);
end;

    procedure TForm1.NotifyCommand(codeNotify: SYSUINT; cmdID: SYSUINT; var bHandle: WordBool); safecall;
    begin
    end;
    procedure TForm1.GetWnd(hWnd: PPUserType1); safecall;
    begin

    end;
    procedure TForm1.OnActiveWindow(nType: SYSUINT); safecall;
    begin

    end;
    procedure TForm1.OnCloseWindow(var vbClose: WordBool); safecall;
    begin

    end;
    procedure TForm1.CreateWnd(var hWndParent: _RemotableHandle); safecall;
    begin

    end;


end.
