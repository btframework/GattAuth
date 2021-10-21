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

	CString IntToHex(const int i) const;
	CString IntToHex(const __int64 i) const;
	CString IntToStr(const int i) const;
	CString IntToHex(const unsigned short i) const;
	__int64 StrToInt64(const CString& s) const;

	void Trace(const CString& Msg);
	void Trace(const CString& Msg, const int Error);

	void wclBluetoothManagerAfterOpen(void* Sender);
	void wclBluetoothManagerClosed(void* Sender);
	
	void wclBluetoothManagerDiscoveringStarted(void* Sender,
		CwclBluetoothRadio* const Radio);
	void wclBluetoothManagerDiscoveringCompleted(void* Sender,
		CwclBluetoothRadio* const Radio, const int Error);
	void wclBluetoothManagerDeviceFound(void* Sender,
		CwclBluetoothRadio* const Radio, const __int64 Address);
	void wclBluetoothManagerConfirm(void* Sender,
		CwclBluetoothRadio* const Radio, const __int64 Address,
		bool& Confirm);
	void wclBluetoothManagerNumericComparison(void* Sender,
		CwclBluetoothRadio* const Radio, const __int64 Address, 
		const unsigned long Number, bool& Confirm);
	void wclBluetoothManagerAuthenticationCompleted(void* Sender,
		CwclBluetoothRadio* const Radio, const __int64 Address,
		const int Error);

	void wclGattClientDisconnect(void* Sender, const int Reason);
	void wclGattClientConnect(void* Sender, const int Error);
    
public:
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonDiscover();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonConnect();
private:
	CButton btDiscover;
	CButton btConnect;
	CButton btDisconnect;
public:
	afx_msg void OnLvnItemchangedListDevices(NMHDR *pNMHDR, LRESULT *pResult);
};
