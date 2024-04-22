//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "wclBluetooth"
#pragma resource "*.dfm"
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btClearClick(TObject *Sender)
{
	lbLog->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::Trace(const String& Msg)
{
	lbLog->Items->Add(Msg);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::Trace(const String& Msg, const int Error)
{
	Trace(Msg + ": 0x" + IntToHex(Error, 8));
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormCreate(TObject *Sender)
{
	int Res = wclBluetoothManager->Open();
	if (Res != WCL_E_SUCCESS)
		Trace("Bluetooth Manager open failed", Res);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormDestroy(TObject *Sender)
{
	wclBluetoothManager->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclBluetoothManagerAfterOpen(TObject *Sender)
{
	Trace("Bluetooth Manager has been opened");
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclBluetoothManagerBeforeClose(TObject *Sender)
{
	Trace("Bluetooth Manager has been closed");
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btDiscoverClick(TObject *Sender)
{
	TwclBluetoothRadio* Radio;
	int Res = wclBluetoothManager->GetRadio(Radio);
	if (Res != WCL_E_SUCCESS)
		Trace("Get working radio failed", Res);
	else
	{
		Res = Radio->Discover(10, dkBle);
		if (Res != WCL_E_SUCCESS)
			Trace("Start discovering failed", Res);
		else
			btDiscover->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclBluetoothManagerDiscoveringStarted(TObject *Sender,
	TwclBluetoothRadio * const Radio)
{
	Trace("Discovering started on radio: " + Radio->ApiName);
	lvDevices->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclBluetoothManagerDiscoveringCompleted(
	TObject *Sender, TwclBluetoothRadio * const Radio, const int Error)
{
	Trace("Discovering completed with result", Error);
	if (lvDevices->Items->Count > 0)
	{
		for (int i = 0; i < lvDevices->Items->Count; i++)
		{
			TListItem* Item = lvDevices->Items->Item[i];
			__int64 Mac = StrToInt64("$" + Item->Caption);

			String Name;
			int Res = Radio->GetRemoteName(Mac, Name);
			if (Res != WCL_E_SUCCESS)
				Name = "Error: 0x" + IntToHex(Res, 8);
			Item->SubItems->Add(Name);

			bool Paired;
			Res = Radio->GetRemotePaired(Mac, Paired);
			if (Res != WCL_E_SUCCESS)
				Trace("Get paired status failed", Res);
			else
			{
				if (Paired)
				{
					Res = Radio->RemoteUnpair(Mac);
					if (Res != WCL_E_SUCCESS)
						Trace("Failed unpair device " + Item->Caption, Res);
					else
						Trace("Device " + Item->Caption + " unpaired");
				}
				else
					Trace("Device " + Item->Caption + " is not paired");
			}
		}
	}

	btDiscover->Enabled = true;
	btConnect->Enabled = (lvDevices->Selected != NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclBluetoothManagerDeviceFound(TObject *Sender,
	TwclBluetoothRadio * const Radio, const __int64 Address)
{
	TListItem* Item = lvDevices->Items->Add();
	Item->Caption = IntToHex(Address, 12);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btDisconnectClick(TObject *Sender)
{
	int Res = wclGattClient->Disconnect();
	if (Res != WCL_E_SUCCESS)
		Trace("Disconnect failed", Res);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclGattClientDisconnect(TObject *Sender,
	const int Reason)
{
	Trace("Disconnected with reason", Reason);
	Trace("Unpair with device");
	int Res = wclGattClient->Radio->RemoteUnpair(wclGattClient->Address);
	if (Res != WCL_E_SUCCESS)
		Trace("Unpair failed", Res);
	else
		Trace("Device unpaired");

	btDiscover->Enabled = true;
	btDisconnect->Enabled = false;
	btConnect->Enabled = (lvDevices->Selected != NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclGattClientConnect(TObject *Sender, const int Error)
{
	if (Error != WCL_E_SUCCESS)
	{
		Trace("Connect failed", Error);

		btDiscover->Enabled = true;
		btDisconnect->Enabled = false;
		btConnect->Enabled = (lvDevices->Selected != NULL);
	}
	else
	{
		Trace("Connected. Try to pair");
		int Res = wclGattClient->Radio->RemotePair(wclGattClient->Address);
		if (Res != WCL_E_SUCCESS)
		{
			Trace("Pair failed", Res);
			wclGattClient->Disconnect();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclBluetoothManagerConfirm(TObject *Sender,
	TwclBluetoothRadio * const Radio, const __int64 Address, bool& Confirm)
{
	Trace("On confirm pairing (Just Works)");
	Confirm = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclBluetoothManagerNumericComparison(TObject *Sender,
	TwclBluetoothRadio * const Radio, const __int64 Address, const DWORD Number,
	bool& Confirm)
{
	Trace("Numeric comparison pairing");
	Confirm = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::wclBluetoothManagerAuthenticationCompleted(
	TObject *Sender, TwclBluetoothRadio * const Radio, const __int64 Address,
	const int Error)
{
	if (Error != WCL_E_SUCCESS)
	{
		Trace("Authentication failed", Error);
		wclGattClient->Disconnect();
	}
	else
	{
		Trace("Paired. Read services");
		TwclGattServices Services;
		int Res = wclGattClient->ReadServices(goNone, Services);
		if (Res != WCL_E_SUCCESS)
			Trace("Read services failed", Res);
		else
		{
			if (Services.Length == 0)
				Trace("No services were found");
			else
			{
				for (int i = 0; i < Services.Length; i++)
				{
					TwclGattService Service = Services[i];
					Trace("Service[" + IntToStr(i) + "] handle 0x" +
						IntToHex(Service.Handle, 4));
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btConnectClick(TObject *Sender)
{
	if (lvDevices->Selected == NULL)
		Trace("Select device");
	else
	{
		TwclBluetoothRadio* Radio;
		int Res = wclBluetoothManager->GetRadio(Radio);
		if (Res != WCL_E_SUCCESS)
			Trace("Get working radio failed", Res);
		else
		{
			__int64 Mac = StrToInt64("$" + lvDevices->Selected->Caption);
			if (wclGattClient->State != csDisconnected)
				Trace("Already conencted");
			else
			{
				wclGattClient->Address = Mac;
				Res = wclGattClient->Connect(Radio);
				if (Res != WCL_E_SUCCESS)
					Trace("Connect failed", Res);
				else
				{
					Trace("Connecting");

					btConnect->Enabled = false;
					btDisconnect->Enabled = true;
					btDiscover->Enabled = false;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::lvDevicesSelectItem(TObject *Sender, TListItem *Item,
	bool Selected)
{
	btConnect->Enabled = (lvDevices->Selected != NULL && btDiscover->Enabled &&
		wclGattClient->State == csDisconnected);
}
//---------------------------------------------------------------------------

