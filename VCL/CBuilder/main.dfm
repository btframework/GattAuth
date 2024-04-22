object fmMain: TfmMain
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'GATT Authentication Demo'
  ClientHeight = 450
  ClientWidth = 639
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object btDiscover: TButton
    Left = 8
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Discover'
    TabOrder = 0
    OnClick = btDiscoverClick
  end
  object lvDevices: TListView
    Left = 8
    Top = 39
    Width = 617
    Height = 113
    Columns = <
      item
        Caption = 'Address'
        Width = 150
      end
      item
        Caption = 'Name'
        Width = 150
      end>
    ColumnClick = False
    GridLines = True
    HideSelection = False
    ReadOnly = True
    RowSelect = True
    TabOrder = 1
    ViewStyle = vsReport
    OnSelectItem = lvDevicesSelectItem
  end
  object btConnect: TButton
    Left = 8
    Top = 161
    Width = 75
    Height = 25
    Caption = 'Connect'
    Enabled = False
    TabOrder = 2
    OnClick = btConnectClick
  end
  object btDisconnect: TButton
    Left = 89
    Top = 159
    Width = 75
    Height = 25
    Caption = 'Disconnect'
    Enabled = False
    TabOrder = 3
    OnClick = btDisconnectClick
  end
  object lbLog: TListBox
    Left = 8
    Top = 192
    Width = 617
    Height = 249
    ItemHeight = 13
    TabOrder = 4
  end
  object btClear: TButton
    Left = 544
    Top = 160
    Width = 75
    Height = 25
    Caption = 'Clear'
    TabOrder = 5
    OnClick = btClearClick
  end
  object wclBluetoothManager: TwclBluetoothManager
    AfterOpen = wclBluetoothManagerAfterOpen
    BeforeClose = wclBluetoothManagerBeforeClose
    OnAuthenticationCompleted = wclBluetoothManagerAuthenticationCompleted
    OnConfirm = wclBluetoothManagerConfirm
    OnDeviceFound = wclBluetoothManagerDeviceFound
    OnDiscoveringCompleted = wclBluetoothManagerDiscoveringCompleted
    OnDiscoveringStarted = wclBluetoothManagerDiscoveringStarted
    OnNumericComparison = wclBluetoothManagerNumericComparison
    Left = 104
    Top = 88
  end
  object wclGattClient: TwclGattClient
    OnConnect = wclGattClientConnect
    OnDisconnect = wclGattClientDisconnect
    Left = 208
    Top = 80
  end
end
