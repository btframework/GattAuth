// GattAuthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GattAuth.h"
#include "GattAuthDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGattAuthDlg dialog




CGattAuthDlg::CGattAuthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGattAuthDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGattAuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICES, lvDevices);
	DDX_Control(pDX, IDC_LIST_LOG, lbLog);
	DDX_Control(pDX, IDC_BUTTON_DISCOVER, btDiscover);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, btConnect);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT, btDisconnect);
}

BEGIN_MESSAGE_MAP(CGattAuthDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CGattAuthDlg::OnBnClickedButtonClear)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_DISCOVER, &CGattAuthDlg::OnBnClickedButtonDiscover)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CGattAuthDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CGattAuthDlg::OnBnClickedButtonConnect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DEVICES, &CGattAuthDlg::OnLvnItemchangedListDevices)
END_MESSAGE_MAP()


// CGattAuthDlg message handlers

BOOL CGattAuthDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	lvDevices.InsertColumn(0, _T("Address"), 0, 150);
	lvDevices.InsertColumn(1, _T("Name"), 0, 150);

	btConnect.EnableWindow(FALSE);
	btDisconnect.EnableWindow(FALSE);

	__hook(&CwclBluetoothManager::AfterOpen, &wclBluetoothManager,
		&CGattAuthDlg::wclBluetoothManagerAfterOpen);
	__hook(&CwclBluetoothManager::OnClosed, &wclBluetoothManager,
		&CGattAuthDlg::wclBluetoothManagerClosed);
	__hook(&CwclBluetoothManager::OnDiscoveringStarted, &wclBluetoothManager,
		&CGattAuthDlg::wclBluetoothManagerDiscoveringStarted);
	__hook(&CwclBluetoothManager::OnDiscoveringCompleted, &wclBluetoothManager,
		&CGattAuthDlg::wclBluetoothManagerDiscoveringCompleted);
	__hook(&CwclBluetoothManager::OnDeviceFound, &wclBluetoothManager,
		&CGattAuthDlg::wclBluetoothManagerDeviceFound);
	__hook(&CwclBluetoothManager::OnConfirm, &wclBluetoothManager,
		&CGattAuthDlg::wclBluetoothManagerConfirm);
	__hook(&CwclBluetoothManager::OnNumericComparison, &wclBluetoothManager,
		&CGattAuthDlg::wclBluetoothManagerNumericComparison);
	__hook(&CwclBluetoothManager::OnAuthenticationCompleted, &wclBluetoothManager,
		&CGattAuthDlg::wclBluetoothManagerAuthenticationCompleted);

	__hook(&CwclGattClient::OnDisconnect, &wclGattClient,
		&CGattAuthDlg::wclGattClientDisconnect);
	__hook(&CwclGattClient::OnConnect, &wclGattClient,
		&CGattAuthDlg::wclGattClientConnect);

	int Res = wclBluetoothManager.Open();
	if (Res != WCL_E_SUCCESS)
		Trace(_T("Bluetooth Manager open failed"), Res);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGattAuthDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGattAuthDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CGattAuthDlg::IntToHex(const int i) const
{
	CString s;
	s.Format(_T("%.8X"), i);
	return s;
}

CString CGattAuthDlg::IntToHex(const __int64 i) const
{
	CString s;
	s.Format(_T("%.4X%.8X"), static_cast<INT32>((i >> 32) & 0x00000FFFF),
		static_cast<INT32>(i) & 0xFFFFFFFF);
	return s;
}

CString CGattAuthDlg::IntToStr(int i) const
{
	CString s;
	s.Format(_T("%d"), i);
	return s;
}

CString CGattAuthDlg::IntToHex(const unsigned short i) const
{
	CString s;
	s.Format(_T("%.4X"), i);
	return s;
}

__int64 CGattAuthDlg::StrToInt64(const CString& s) const
{
	return _tcstoi64(s, NULL, 16);
}

void CGattAuthDlg::OnBnClickedButtonClear()
{
	lbLog.ResetContent();
}

void CGattAuthDlg::Trace(const CString& Msg)
{
	lbLog.AddString(Msg);
}

void CGattAuthDlg::Trace(const CString& Msg, const int Error)
{
	Trace(Msg + _T(": 0x") + IntToHex(Error));
}

void CGattAuthDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	wclBluetoothManager.Close();

	__unhook(&wclBluetoothManager);
	__unhook(&wclGattClient);

	CDialog::OnClose();
}

void CGattAuthDlg::wclBluetoothManagerAfterOpen(void* Sender)
{
	UNREFERENCED_PARAMETER(Sender);

	Trace(_T("Bluetooth Manager has been opened"));
}

void CGattAuthDlg::wclBluetoothManagerClosed(void* Sender)
{
	UNREFERENCED_PARAMETER(Sender);

	Trace(_T("Bluetooth Manager has been closed"));
}

void CGattAuthDlg::OnBnClickedButtonDiscover()
{
	CwclBluetoothRadio* Radio;
	int Res = wclBluetoothManager.GetRadio(Radio);
	if (Res != WCL_E_SUCCESS)
		Trace(_T("Get working radio failed"), Res);
	else
	{
		Res = Radio->Discover(10, dkBle);
		if (Res != WCL_E_SUCCESS)
			Trace(_T("Start discovering failed"), Res);
		else
			btDiscover.EnableWindow(FALSE);
	}
}

void CGattAuthDlg::wclBluetoothManagerDiscoveringStarted(void* Sender,
	CwclBluetoothRadio* const Radio)
{
	UNREFERENCED_PARAMETER(Sender);
	
	Trace(_T("Discovering started on radio: ") + CString(Radio->GetApiName().c_str()));
	lvDevices.DeleteAllItems();
}

void CGattAuthDlg::wclBluetoothManagerDiscoveringCompleted(void* Sender,
	CwclBluetoothRadio* const Radio, const int Error)
{
	UNREFERENCED_PARAMETER(Sender);

	Trace(_T("Discovering completed with result"), Error);
	if (lvDevices.GetItemCount() > 0)
	{
		for (int i = 0; i < lvDevices.GetItemCount(); i++)
		{
			CString Addr = lvDevices.GetItemText(i, 0);
			__int64 Mac = StrToInt64(Addr);
			tstring Name;
			int Res = Radio->GetRemoteName(Mac, Name);
			if (Res != WCL_E_SUCCESS)
				Name = _T("Error: 0x") + IntToHex(Res);
			lvDevices.SetItemText(i, 1, Name.c_str());
			bool Paired;
			Res = Radio->GetRemotePaired(Mac, Paired);
			if (Res != WCL_E_SUCCESS)
				Trace(_T("Get paired status failed"), Res);
			else
			{
				if (Paired)
				{
					Res = Radio->RemoteUnpair(Mac);
					if (Res != WCL_E_SUCCESS)
						Trace(_T("Failed unpair device ") + Addr, Res);
					else
						Trace(_T("Device ") + Addr + _T(" unpaired"));
				}
				else
					Trace(_T("Device ") + Addr + _T(" is not paired"));
			}
		}
	}

	btDiscover.EnableWindow(TRUE);
	btConnect.EnableWindow(lvDevices.GetFirstSelectedItemPosition() != NULL);
}

void CGattAuthDlg::wclBluetoothManagerDeviceFound(void* Sender,
	CwclBluetoothRadio* const Radio, const __int64 Address)
{
	UNREFERENCED_PARAMETER(Sender);
	UNREFERENCED_PARAMETER(Radio);

	int Item = lvDevices.GetItemCount();
	lvDevices.InsertItem(Item, IntToHex(Address));
	lvDevices.SetItemText(Item, 1, _T(""));
}

void CGattAuthDlg::wclBluetoothManagerConfirm(void* Sender,
	CwclBluetoothRadio* const Radio, const __int64 Address, bool& Confirm)
{
	UNREFERENCED_PARAMETER(Sender);
	UNREFERENCED_PARAMETER(Radio);
	UNREFERENCED_PARAMETER(Address);

	Trace(_T("On confirm pairing (Just Works)"));
	Confirm = true;
}

void CGattAuthDlg::wclBluetoothManagerNumericComparison(void* Sender,
	CwclBluetoothRadio* const Radio, const __int64 Address, 
	const unsigned long Number, bool& Confirm)
{
	UNREFERENCED_PARAMETER(Sender);
	UNREFERENCED_PARAMETER(Radio);
	UNREFERENCED_PARAMETER(Address);
	UNREFERENCED_PARAMETER(Number);

	Trace(_T("Numeric comparison pairing"));
	Confirm = true;
}

void CGattAuthDlg::wclBluetoothManagerAuthenticationCompleted(void* Sender,
	CwclBluetoothRadio* const Radio, const __int64 Address, const int Error)
{
	UNREFERENCED_PARAMETER(Sender);
	UNREFERENCED_PARAMETER(Radio);
	UNREFERENCED_PARAMETER(Address);

	if (Error != WCL_E_SUCCESS)
	{
		Trace(_T("Authentication failed"), Error);
		wclGattClient.Disconnect();
	}
	else
	{
		Trace(_T("Paired. Read services"));
		wclGattServices Services;
		int Res = wclGattClient.ReadServices(goNone, Services);
		if (Res != WCL_E_SUCCESS)
			Trace(_T("Read services failed"), Res);
		else
		{
			if (Services.size() == 0)
				Trace(_T("No services were found"));
			else
			{
				int c = 0;
				for (wclGattServices::iterator i = Services.begin(); i != Services.end(); i++)
				{
					Trace(_T("Service[") + IntToStr(c) + _T("] handle 0x") + IntToHex((*i).Handle)); 
					c++;
				}
			}
		}
	}
}

void CGattAuthDlg::wclGattClientDisconnect(void* Sender, const int Reason)
{
	UNREFERENCED_PARAMETER(Sender);

	Trace(_T("Disconnected with reason"), Reason);
	Trace(_T("Unpair with device"));
	int Res = wclGattClient.GetRadio()->RemoteUnpair(wclGattClient.GetAddress());
	if (Res != WCL_E_SUCCESS)
		Trace(_T("Unpair failed"), Res);
	else
		Trace(_T("Device unpaired"));

	btDiscover.EnableWindow(TRUE);
	btDisconnect.EnableWindow(FALSE);
	btConnect.EnableWindow(lvDevices.GetFirstSelectedItemPosition() != NULL);
}

void CGattAuthDlg::wclGattClientConnect(void* Sender, const int Error)
{
	UNREFERENCED_PARAMETER(Sender);

	if (Error != WCL_E_SUCCESS)
	{
		Trace(_T("Connect failed"), Error);

		btDiscover.EnableWindow(TRUE);
		btDisconnect.EnableWindow(FALSE);
		btConnect.EnableWindow(lvDevices.GetFirstSelectedItemPosition() != NULL);
	}
	else
	{
		Trace(_T("Connected. Try to pair"));
		int Res = wclGattClient.GetRadio()->RemotePair(wclGattClient.GetAddress());
		if (Res != WCL_E_SUCCESS)
		{
			Trace(_T("Pair failed"), Res);
			wclGattClient.Disconnect();
		}
	}
}

void CGattAuthDlg::OnBnClickedButtonDisconnect()
{
	int Res = wclGattClient.Disconnect();
	if (Res != WCL_E_SUCCESS)
		Trace(_T("Disconnect failed"), Res);
}

void CGattAuthDlg::OnBnClickedButtonConnect()
{
	POSITION Pos = lvDevices.GetFirstSelectedItemPosition();
	if (Pos == NULL)
		Trace(_T("Select device"));
	else
	{
		int Item = lvDevices.GetNextSelectedItem(Pos);
		__int64 Mac = StrToInt64(lvDevices.GetItemText(Item, 0));
		CwclBluetoothRadio* Radio;
		int Res = wclBluetoothManager.GetRadio(Radio);
		if (Res != WCL_E_SUCCESS)
			Trace(_T("Get working radio failed"), Res);
		else
		{
			if (wclGattClient.GetState() != csDisconnected)
				Trace(_T("Already conencted"));
			else
			{
				wclGattClient.SetAddress(Mac);
				Res = wclGattClient.Connect(Radio);
				if (Res != WCL_E_SUCCESS)
					Trace(_T("Connect failed"), Res);
				else
				{
					Trace(_T("Connecting"));

					btConnect.EnableWindow(FALSE);
					btDisconnect.EnableWindow(TRUE);
					btDiscover.EnableWindow(FALSE);
				}
			}
		}
	}
}

void CGattAuthDlg::OnLvnItemchangedListDevices(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	BOOL Enabled = !btDiscover.EnableWindow(FALSE);
	btDiscover.EnableWindow(Enabled); 
	
	btConnect.EnableWindow(lvDevices.GetFirstSelectedItemPosition() != NULL &&
		Enabled && wclGattClient.GetState() == csDisconnected);
}
