using System;
using System.Windows.Forms;

using wclCommon;
using wclCommunication;
using wclBluetooth;

namespace GattAuth
{
    public partial class fmMain : Form
    {
        private wclBluetoothManager wclBluetoothManager;
        private wclGattClient wclGattClient;

        public fmMain()
        {
            InitializeComponent();
        }

        private void fmMain_Load(object sender, System.EventArgs e)
        {
            wclBluetoothManager = new wclBluetoothManager();
            wclBluetoothManager.AfterOpen += WclBluetoothManager_AfterOpen;
            wclBluetoothManager.OnClosed += WclBluetoothManager_OnClosed;
            wclBluetoothManager.OnDiscoveringStarted += WclBluetoothManager_OnDiscoveringStarted;
            wclBluetoothManager.OnDiscoveringCompleted += WclBluetoothManager_OnDiscoveringCompleted;
            wclBluetoothManager.OnDeviceFound += WclBluetoothManager_OnDeviceFound;
            wclBluetoothManager.OnConfirm += WclBluetoothManager_OnConfirm;
            wclBluetoothManager.OnNumericComparison += WclBluetoothManager_OnNumericComparison;
            wclBluetoothManager.OnAuthenticationCompleted += WclBluetoothManager_OnAuthenticationCompleted;

            wclGattClient = new wclGattClient();
            wclGattClient.OnDisconnect += WclGattClient_OnDisconnect;
            wclGattClient.OnConnect += WclGattClient_OnConnect;

            Int32 Res = wclBluetoothManager.Open();
            if (Res != wclErrors.WCL_E_SUCCESS)
                Trace("Bluetooth Manager open failed", Res);
        }

        private void WclBluetoothManager_OnAuthenticationCompleted(object Sender, wclBluetoothRadio Radio, long Address, int Error)
        {
            if (Error != wclErrors.WCL_E_SUCCESS)
            {
                Trace("Authentication failed", Error);
                wclGattClient.Disconnect();
            }
            else
            {
                Trace("Paired. Read services");
                wclGattService[] Services;
                Int32 Res = wclGattClient.ReadServices(wclGattOperationFlag.goNone, out Services);
                if (Res != wclErrors.WCL_E_SUCCESS)
                    Trace("Read services failed", Res);
                else
                {
                    if (Services == null || Services.Length == 0)
                        Trace("No services were found");
                    else
                    {
                        Int32 i = 0;
                        foreach (wclGattService Service in Services)
                        {
                            Trace("Service[" + i.ToString() + "] handle 0x" + Service.Handle.ToString("X4"));
                            i++;
                        }
                    }
                }
            }
        }

        private void WclBluetoothManager_OnNumericComparison(object Sender, wclBluetoothRadio Radio, long Address, uint Number, out bool Confirm)
        {
            Trace("Numeric comparison pairing");
            Confirm = true;
        }

        private void WclBluetoothManager_OnConfirm(object Sender, wclBluetoothRadio Radio, long Address, out bool Confirm)
        {
            Trace("On confirm pairing (Just Works)");
            Confirm = true;
        }

        private void WclGattClient_OnConnect(object Sender, int Error)
        {
            if (Error != wclErrors.WCL_E_SUCCESS)
                Trace("Connect failed", Error);
            else
            {
                Trace("Connected. Try to pair");
                Int32 Res = wclGattClient.Radio.RemotePair(wclGattClient.Address);
                if (Res != wclErrors.WCL_E_SUCCESS)
                {
                    Trace("Pair failed", Res);
                    wclGattClient.Disconnect();
                }
            }
        }

        private void WclGattClient_OnDisconnect(object Sender, int Reason)
        {
            Trace("Disconnected with reason", Reason);
            Trace("Unpair with device");
            Int32 Res = wclGattClient.Radio.RemoteUnpair(wclGattClient.Address);
            if (Res != wclErrors.WCL_E_SUCCESS)
                Trace("Unpair failed", Res);
            else
                Trace("Device unpaired");
        }

        private void WclBluetoothManager_OnDeviceFound(object Sender, wclBluetoothRadio Radio, long Address)
        {
            lvDevices.Items.Add(Address.ToString("X12"));
        }

        private void WclBluetoothManager_OnDiscoveringCompleted(object Sender, wclBluetoothRadio Radio, int Error)
        {
            Trace("Discovering completed with result", Error);
            if (lvDevices.Items.Count > 0)
            {
                foreach (ListViewItem Item in lvDevices.Items)
                {
                    Int64 Mac = Convert.ToInt64(Item.Text, 16);
                    String Name;
                    Int32 Res = Radio.GetRemoteName(Mac, out Name);
                    if (Res != wclErrors.WCL_E_SUCCESS)
                        Name = "Error: 0x" + Res.ToString("X8");
                    Item.SubItems.Add(Name);
                    Boolean Paired;
                    Res = Radio.GetRemotePaired(Mac, out Paired);
                    if (Res != wclErrors.WCL_E_SUCCESS)
                        Trace("Get paired status failed", Res);
                    else
                    {
                        if (Paired)
                        {
                            Res = Radio.RemoteUnpair(Mac);
                            if (Res != wclErrors.WCL_E_SUCCESS)
                                Trace("Failed unpair device " + Item.Text, Res);
                            else
                                Trace("Device " + Item.Text + " unpaired");
                        }
                        else
                            Trace("Device " + Item.Text + " is not paired");
                    }
                }
            }
        }

        private void WclBluetoothManager_OnDiscoveringStarted(object Sender, wclBluetoothRadio Radio)
        {
            Trace("Discovering started on radio: " + Radio.ApiName);
            lvDevices.Items.Clear();
        }

        private void WclBluetoothManager_OnClosed(object sender, EventArgs e)
        {
            Trace("Bluetooth Manager has been closed");
        }

        private void WclBluetoothManager_AfterOpen(object sender, EventArgs e)
        {
            Trace("Bluetooth Manager has been opened");
        }

        private void btClear_Click(object sender, System.EventArgs e)
        {
            lbLog.Items.Clear();
        }

        private void Trace(String Msg)
        {
            lbLog.Items.Add(Msg);
        }

        private void Trace(String Msg, Int32 Error)
        {
            Trace(Msg + ": 0x" + Error.ToString("X8"));
        }

        private void fmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            wclBluetoothManager.Close();
        }

        private void btDiscover_Click(object sender, EventArgs e)
        {
            wclBluetoothRadio Radio;
            Int32 Res = wclBluetoothManager.GetRadio(out Radio);
            if (Res != wclErrors.WCL_E_SUCCESS)
                Trace("Get working radio failed", Res);
            else
            {
                Res = Radio.Discover(10, wclBluetoothDiscoverKind.dkBle);
                if (Res != wclErrors.WCL_E_SUCCESS)
                    Trace("Start discovering failed", Res);
            }
        }

        private void btDisconnect_Click(object sender, EventArgs e)
        {
            Int32 Res = wclGattClient.Disconnect();
            if (Res != wclErrors.WCL_E_SUCCESS)
                Trace("Disconnect failed", Res);
        }

        private void btConnect_Click(object sender, EventArgs e)
        {
            if (lvDevices.SelectedItems.Count == 0)
                Trace("Select device");
            else
            {
                wclBluetoothRadio Radio;
                Int32 Res = wclBluetoothManager.GetRadio(out Radio);
                if (Res != wclErrors.WCL_E_SUCCESS)
                    Trace("Get working radio failed", Res);
                else
                {
                    Int64 Mac = Convert.ToInt64(lvDevices.SelectedItems[0].Text, 16);
                    if (wclGattClient.State != wclClientState.csDisconnected)
                        Trace("Already conencted");
                    else
                    {
                        wclGattClient.Address = Mac;
                        Res = wclGattClient.Connect(Radio);
                        if (Res != wclErrors.WCL_E_SUCCESS)
                            Trace("Connect failed", Res);
                        else
                            Trace("Connecting");
                    }
                }
            }
        }
    }
}
