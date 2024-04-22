//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "wclBluetooth.hpp"
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TButton *btDiscover;
	TListView *lvDevices;
	TButton *btConnect;
	TButton *btDisconnect;
	TListBox *lbLog;
	TButton *btClear;
	TwclBluetoothManager *wclBluetoothManager;
	TwclGattClient *wclGattClient;
	void __fastcall btClearClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall wclBluetoothManagerAfterOpen(TObject *Sender);
	void __fastcall wclBluetoothManagerBeforeClose(TObject *Sender);
	void __fastcall btDiscoverClick(TObject *Sender);
	void __fastcall wclBluetoothManagerDiscoveringStarted(TObject *Sender,
		TwclBluetoothRadio * const Radio);
	void __fastcall wclBluetoothManagerDiscoveringCompleted(TObject *Sender,
		TwclBluetoothRadio * const Radio, const int Error);
	void __fastcall wclBluetoothManagerDeviceFound(TObject *Sender,
		TwclBluetoothRadio * const Radio, const __int64 Address);
	void __fastcall btDisconnectClick(TObject *Sender);
	void __fastcall wclGattClientDisconnect(TObject *Sender, const int Reason);
	void __fastcall wclGattClientConnect(TObject *Sender, const int Error);
	void __fastcall wclBluetoothManagerConfirm(TObject *Sender,
		TwclBluetoothRadio * const Radio, const __int64 Address, bool& Confirm);
	void __fastcall wclBluetoothManagerNumericComparison(TObject *Sender,
		TwclBluetoothRadio * const Radio, const __int64 Address,
		const DWORD Number, bool& Confirm);
	void __fastcall wclBluetoothManagerAuthenticationCompleted(TObject *Sender,
		TwclBluetoothRadio * const Radio, const __int64 Address,
		const int Error);
	void __fastcall btConnectClick(TObject *Sender);
	void __fastcall lvDevicesSelectItem(TObject *Sender, TListItem *Item,
		bool Selected);

private:	// User declarations
	void __fastcall Trace(const String& Msg);
	void __fastcall Trace(const String& Msg, const int Error);

public:		// User declarations
	__fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
