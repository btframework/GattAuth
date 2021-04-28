unit main;

interface

uses
  Forms, Controls, ComCtrls, StdCtrls, wclBluetooth, Classes;

type
  TfmMain = class(TForm)
    wclBluetoothManager: TwclBluetoothManager;
    wclGattClient: TwclGattClient;
    btDiscover: TButton;
    lvDevices: TListView;
    btConnect: TButton;
    btDisconnect: TButton;
    lbLog: TListBox;
    btClear: TButton;
    procedure btClearClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure wclBluetoothManagerAfterOpen(Sender: TObject);
    procedure wclBluetoothManagerClosed(Sender: TObject);
    procedure btDiscoverClick(Sender: TObject);
    procedure wclBluetoothManagerDiscoveringStarted(Sender: TObject;
      const Radio: TwclBluetoothRadio);
    procedure wclBluetoothManagerDiscoveringCompleted(Sender: TObject;
      const Radio: TwclBluetoothRadio; const Error: Integer);
    procedure wclBluetoothManagerDeviceFound(Sender: TObject;
      const Radio: TwclBluetoothRadio; const Address: Int64);
    procedure btDisconnectClick(Sender: TObject);
    procedure wclGattClientDisconnect(Sender: TObject;
      const Reason: Integer);
    procedure wclGattClientConnect(Sender: TObject; const Error: Integer);
    procedure wclBluetoothManagerConfirm(Sender: TObject;
      const Radio: TwclBluetoothRadio; const Address: Int64;
      out Confirm: Boolean);
    procedure wclBluetoothManagerNumericComparison(Sender: TObject;
      const Radio: TwclBluetoothRadio; const Address: Int64;
      const Number: Cardinal; out Confirm: Boolean);
    procedure wclBluetoothManagerAuthenticationCompleted(Sender: TObject;
      const Radio: TwclBluetoothRadio; const Address: Int64;
      const Error: Integer);
    procedure btConnectClick(Sender: TObject);
    procedure lvDevicesSelectItem(Sender: TObject; Item: TListItem;
      Selected: Boolean);

  private
    procedure Trace(const Msg: string); overload;
    procedure Trace(const Msg: string; const Error: Integer); overload;
  end;

var
  fmMain: TfmMain;

implementation

uses
  SysUtils, wclErrors, wclConnections;

{$R *.dfm}

procedure TfmMain.btClearClick(Sender: TObject);
begin
  lbLog.Items.Clear;
end;

procedure TfmMain.Trace(const Msg: string);
begin
  lbLog.Items.Add(Msg);
end;

procedure TfmMain.Trace(const Msg: string; const Error: Integer);
begin
  Trace(Msg + ': 0x' + IntToHex(Error, 8));
end;

procedure TfmMain.FormCreate(Sender: TObject);
var
  Res: Integer;
begin
  Res := wclBluetoothManager.Open;
  if Res <> WCL_E_SUCCESS then
    Trace('Bluetooth Manager open failed', Res);
end;

procedure TfmMain.FormDestroy(Sender: TObject);
begin
  wclBluetoothManager.Close;
end;

procedure TfmMain.wclBluetoothManagerAfterOpen(Sender: TObject);
begin
  Trace('Bluetooth Manager has been opened');
end;

procedure TfmMain.wclBluetoothManagerClosed(Sender: TObject);
begin
  Trace('Bluetooth Manager has been closed');
end;

procedure TfmMain.btDiscoverClick(Sender: TObject);
var
  Res: Integer;
  Radio: TwclBluetoothRadio;
begin
  Res := wclBluetoothManager.GetRadio(Radio);
  if Res <> WCL_E_SUCCESS then
    Trace('Get working radio failed', Res)

  else begin
    Res := Radio.Discover(10, dkBle);
    if Res <> WCL_E_SUCCESS then
      Trace('Start discovering failed', Res)
    else
      btDiscover.Enabled := False;
  end;
end;

procedure TfmMain.wclBluetoothManagerDiscoveringStarted(Sender: TObject;
  const Radio: TwclBluetoothRadio);
begin
  Trace('Discovering started on radio: ' + Radio.ApiName);
  lvDevices.Items.Clear;
end;

procedure TfmMain.wclBluetoothManagerDiscoveringCompleted(Sender: TObject;
  const Radio: TwclBluetoothRadio; const Error: Integer);
var
  i: Integer;
  Item: TListItem;
  Mac: Int64;
  Res: Integer;
  Name: string;
  Paired: Boolean;
begin
  Trace('Discovering completed with result', Error);
  if lvDevices.Items.Count > 0 then begin
    for i := 0 to lvDevices.Items.Count - 1 do begin
      Item := lvDevices.Items[i];
      Mac := StrToInt64('$' + Item.Caption);

      Res := Radio.GetRemoteName(Mac, Name);
      if Res <> WCL_E_SUCCESS then
        Name := 'Error: 0x' + IntToHex(Res, 8);
      Item.SubItems.Add(Name);

      Res := Radio.GetRemotePaired(Mac, Paired);
      if Res <> WCL_E_SUCCESS then
        Trace('Get paired status failed', Res)

      else begin
        if Paired then begin
          Res := Radio.RemoteUnpair(Mac);
          if Res <> WCL_E_SUCCESS then
            Trace('Failed unpair device ' + Item.Caption, Res)
          else
            Trace('Device ' + Item.Caption + ' unpaired');

        end else
          Trace('Device ' + Item.Caption + ' is not paired');
      end;
    end;
  end;

  btDiscover.Enabled := True;
  btConnect.Enabled := lvDevices.Selected <> nil;
end;

procedure TfmMain.wclBluetoothManagerDeviceFound(Sender: TObject;
  const Radio: TwclBluetoothRadio; const Address: Int64);
var
  Item: TListItem;
begin
  Item := lvDevices.Items.Add;
  Item.Caption := IntToHex(Address, 12);
end;

procedure TfmMain.btDisconnectClick(Sender: TObject);
var
  Res: Integer;
begin
  Res := wclGattClient.Disconnect;
  if Res <> WCL_E_SUCCESS then
    Trace('Disconnect failed', Res);
end;

procedure TfmMain.wclGattClientDisconnect(Sender: TObject;
  const Reason: Integer);
var
  Res: Integer;
begin
  Trace('Disconnected with reason', Reason);
  Trace('Unpair with device');
  Res := wclGattClient.Radio.RemoteUnpair(wclGattClient.Address);
  if Res <> WCL_E_SUCCESS then
    Trace('Unpair failed', Res)
  else
    Trace('Device unpaired');

  btDiscover.Enabled := True;
  btDisconnect.Enabled := False;
  btConnect.Enabled := lvDevices.Selected <> nil;
end;

procedure TfmMain.wclGattClientConnect(Sender: TObject;
  const Error: Integer);
var
  Res: Integer;
begin
  if Error <> WCL_E_SUCCESS then begin
    Trace('Connect failed', Error);

    btDiscover.Enabled := True;
    btDisconnect.Enabled := False;
    btConnect.Enabled := lvDevices.Selected <> nil;

  end else begin
    Trace('Connected. Try to pair');
    Res := wclGattClient.Radio.RemotePair(wclGattClient.Address);
    if Res <> WCL_E_SUCCESS then begin
      Trace('Pair failed', Res);
      wclGattClient.Disconnect;
    end;
  end;
end;

procedure TfmMain.wclBluetoothManagerConfirm(Sender: TObject;
  const Radio: TwclBluetoothRadio; const Address: Int64;
  out Confirm: Boolean);
begin
  Trace('On confirm pairing (Just Works)');
  Confirm := True;
end;

procedure TfmMain.wclBluetoothManagerNumericComparison(Sender: TObject;
  const Radio: TwclBluetoothRadio; const Address: Int64;
  const Number: Cardinal; out Confirm: Boolean);
begin
  Trace('Numeric comparison pairing');
  Confirm := True;
end;

procedure TfmMain.wclBluetoothManagerAuthenticationCompleted(
  Sender: TObject; const Radio: TwclBluetoothRadio; const Address: Int64;
  const Error: Integer);
var
  Res: Integer;
  Services: TwclGattServices;
  i: Integer;
  Service: TwclGattService;
begin
  if Error <> WCL_E_SUCCESS then begin
    Trace('Authentication failed', Error);
    wclGattClient.Disconnect;

  end else begin
    Trace('Paired. Read services');
    Res := wclGattClient.ReadServices(goNone, Services);
    if Res <> WCL_E_SUCCESS then
      Trace('Read services failed', Res)

    else begin
      if Length(Services) = 0 then
        Trace('No services were found')

      else begin
        for i := 0 to Length(Services) - 1 do begin
          Service := Services[i];
          Trace('Service[' + IntToStr(i) + '] handle 0x' + IntToHex(Service.Handle, 4)); 
        end;
      end;
    end;
  end;
end;

procedure TfmMain.btConnectClick(Sender: TObject);
var
  Res: Integer;
  Radio: TwclBluetoothRadio;
  Mac: Int64;
begin
  if lvDevices.Selected = nil then
    Trace('Select device')

  else begin
    Res := wclBluetoothManager.GetRadio(Radio);
    if Res <> WCL_E_SUCCESS then
      Trace('Get working radio failed', Res)

    else begin
      Mac := StrToInt64('$' + lvDevices.Selected.Caption);
      if wclGattClient.State <> csDisconnected then
        Trace('Already conencted')

      else begin
        wclGattClient.Address := Mac;
        Res := wclGattClient.Connect(Radio);
        if Res <> WCL_E_SUCCESS then
          Trace('Connect failed', Res)

        else begin
          Trace('Connecting');

          btConnect.Enabled := False;
          btDisconnect.Enabled := True;
          btDiscover.Enabled := False;
        end;
      end;
    end;
  end;
end;

procedure TfmMain.lvDevicesSelectItem(Sender: TObject; Item: TListItem;
  Selected: Boolean);
begin
  btConnect.Enabled := (lvDevices.Selected <> nil) and btDiscover.Enabled
    and (wclGattClient.State = csDisconnected);
end;

end.
