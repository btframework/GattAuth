// GattAuthDlg.h : header file
//

#pragma once

#pragma comment(lib, "..\\..\\..\\..\\WCL7\\CPP\\Lib\\2005\\x32\\WclBluetoothFramework.lib")

#include "..\\..\\..\\..\\WCL7\\CPP\\Source\\Bluetooth\\wclBluetooth.h"
#include "afxcmn.h"
#include "afxwin.h"

using namespace wclCommon;
using namespace wclBluetooth;

// CGattAuthDlg dialog
class CGattAuthDlg : public CDialog
{
// Construction
public:
	CGattAuthDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GATTAUTH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CwclBluetoothManager wclBluetoothManager;
	CwclGattClient wclGattClient;
	CListCtrl lvDevices;
	CListBox lbLog;

	CString IntToHex(int i);
	CString IntToHex(__int64 i);
	CString IntToStr(int i);
	CString IntToHex(unsigned short i);
	__int64 StrToInt64(CString s);

	void Trace(CString Msg);
	void Trace(CString Msg, int Error);

	void wclBluetoothManagerAfterOpen(void* Sender);
	void wclBluetoothManagerClosed(void* Sender);
	
	void wclBluetoothManagerDiscoveringStarted(void* Sender, CwclBluetoothRadio* Radio);
	void wclBluetoothManagerDiscoveringCompleted(void* Sender, CwclBluetoothRadio* Radio,
		int Error);
	void wclBluetoothManagerDeviceFound(void* Sender, CwclBluetoothRadio* Radio,
		__int64 Address);
	void wclBluetoothManagerConfirm(void* Sender, CwclBluetoothRadio* Radio,
		__int64 Address, bool& Confirm);
	void wclBluetoothManagerNumericComparison(void* Sender, CwclBluetoothRadio* Radio,
		__int64 Address, unsigned long Number, bool& Confirm);
	void wclBluetoothManagerAuthenticationCompleted(void* Sender, CwclBluetoothRadio* Radio,
		__int64 Address, int Error);

	void wclGattClientDisconnect(void* Sender, int Reason);
	void wclGattClientConnect(void* Sender, int Error);
    
public:
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonDiscover();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonConnect();
};
