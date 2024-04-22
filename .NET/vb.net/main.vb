Public Class fmMain
    Private WithEvents wclBluetoothManager As wclBluetoothManager
    Private WithEvents wclGattClient As wclGattClient

    Private Sub fmMain_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        wclBluetoothManager = New wclBluetoothManager()
        wclGattClient = New wclGattClient()

        Dim Res As Int32 = wclBluetoothManager.Open()
        If Res <> wclErrors.WCL_E_SUCCESS Then Trace("Bluetooth Manager open failed", Res)
    End Sub

    Private Sub wclBluetoothManager_AfterOpen(sender As Object, e As EventArgs) Handles wclBluetoothManager.AfterOpen
        Trace("Bluetooth Manager has been opened")
    End Sub

    Private Sub wclBluetoothManager_OnAuthenticationCompleted(Sender As Object, Radio As wclBluetoothRadio, Address As Long, [Error] As Integer) Handles wclBluetoothManager.OnAuthenticationCompleted
        If [Error] <> wclErrors.WCL_E_SUCCESS Then
            Trace("Authentication failed", [Error])
            wclGattClient.Disconnect()
        Else
            Trace("Paired. Read services")
            Dim Services() As wclGattService = Nothing
            Dim Res As Int32 = wclGattClient.ReadServices(wclGattOperationFlag.goNone, Services)
            If Res <> wclErrors.WCL_E_SUCCESS Then
                Trace("Read services failed", Res)
            Else
                If Services Is Nothing OrElse Services.Length = 0 Then
                    Trace("No services were found")
                Else
                    Dim i As Int32 = 0
                    For Each Service As wclGattService In Services
                        Trace("Service[" + i.ToString() + "] handle 0x" + Service.Handle.ToString("X4"))
                        i = i + 1
                    Next
                End If
            End If
        End If
    End Sub

    Private Sub wclBluetoothManager_OnClosed(sender As Object, e As EventArgs) Handles wclBluetoothManager.OnClosed
        Trace("Bluetooth Manager has been closed")
    End Sub

    Private Sub wclBluetoothManager_OnConfirm(Sender As Object, Radio As wclBluetoothRadio, Address As Long, ByRef Confirm As Boolean) Handles wclBluetoothManager.OnConfirm
        Trace("On confirm pairing (Just Works)")
        Confirm = True
    End Sub

    Private Sub wclBluetoothManager_OnDeviceFound(Sender As Object, Radio As wclBluetoothRadio, Address As Long) Handles wclBluetoothManager.OnDeviceFound
        lvDevices.Items.Add(Address.ToString("X12"))
    End Sub

    Private Sub wclBluetoothManager_OnDiscoveringCompleted(Sender As Object, Radio As wclBluetoothRadio, [Error] As Integer) Handles wclBluetoothManager.OnDiscoveringCompleted
        Trace("Discovering completed with result", [Error])
        If lvDevices.Items.Count > 0 Then
            For Each Item As ListViewItem In lvDevices.Items
                Dim Mac As Int64 = Convert.ToInt64(Item.Text, 16)
                Dim Name As String = ""
                Dim Res As Int32 = Radio.GetRemoteName(Mac, Name)
                If Res <> wclErrors.WCL_E_SUCCESS Then Name = "Error: 0x" + Res.ToString("X8")
                Item.SubItems.Add(Name)
                Dim Paired As Boolean
                Res = Radio.GetRemotePaired(Mac, Paired)
                If Res <> wclErrors.WCL_E_SUCCESS Then
                    Trace("Get paired status failed", Res)
                Else
                    If Paired Then
                        Res = Radio.RemoteUnpair(Mac)
                        If Res <> wclErrors.WCL_E_SUCCESS Then
                            Trace("Failed unpair device " + Item.Text, Res)
                        Else
                            Trace("Device " + Item.Text + " unpaired")
                        End If
                    Else
                        Trace("Device " + Item.Text + " is not paired")
                    End If
                End If
            Next
        End If

        btDiscover.Enabled = True
        btConnect.Enabled = (lvDevices.SelectedItems.Count > 0)
    End Sub

    Private Sub wclBluetoothManager_OnDiscoveringStarted(Sender As Object, Radio As wclBluetoothRadio) Handles wclBluetoothManager.OnDiscoveringStarted
        Trace("Discovering started on radio: " + Radio.ApiName)
        lvDevices.Items.Clear()
    End Sub

    Private Sub wclBluetoothManager_OnNumericComparison(Sender As Object, Radio As wclBluetoothRadio, Address As Long, Number As UInteger, ByRef Confirm As Boolean) Handles wclBluetoothManager.OnNumericComparison
        Trace("Numeric comparison pairing")
        Confirm = True
    End Sub

    Private Sub wclGattClient_OnConnect(Sender As Object, [Error] As Integer) Handles wclGattClient.OnConnect
        If [Error] <> wclErrors.WCL_E_SUCCESS Then
            Trace("Connect failed", [Error])

            btDiscover.Enabled = True
            btDisconnect.Enabled = False
            btConnect.Enabled = (lvDevices.SelectedItems.Count > 0)
        Else
            Trace("Connected. Try to pair")
            Dim Res As Int32 = wclGattClient.Radio.RemotePair(wclGattClient.Address)
            If Res <> wclErrors.WCL_E_SUCCESS Then
                Trace("Pair failed", Res)
                wclGattClient.Disconnect()
            End If
        End If
    End Sub

    Private Sub wclGattClient_OnDisconnect(Sender As Object, Reason As Integer) Handles wclGattClient.OnDisconnect
        Trace("Disconnected with reason", Reason)
        Trace("Unpair with device")
        Dim Res As Int32 = wclGattClient.Radio.RemoteUnpair(wclGattClient.Address)
        If Res <> wclErrors.WCL_E_SUCCESS Then
            Trace("Unpair failed", Res)
        Else
            Trace("Device unpaired")
        End If

        btDiscover.Enabled = True
        btDisconnect.Enabled = False
        btConnect.Enabled = (lvDevices.SelectedItems.Count > 0)
    End Sub

    Private Sub btClear_Click(sender As Object, e As EventArgs) Handles btClear.Click
        lbLog.Items.Clear()
    End Sub

    Private Sub Trace(Msg As String)
        lbLog.Items.Add(Msg)
    End Sub

    Private Sub Trace(Msg As String, [Error] As Int32)
        Trace(Msg + ": 0x" + [Error].ToString("X8"))
    End Sub

    Private Sub fmMain_FormClosing(sender As Object, e As FormClosingEventArgs) Handles MyBase.FormClosing
        wclBluetoothManager.Close()
    End Sub

    Private Sub btDiscover_Click(sender As Object, e As EventArgs) Handles btDiscover.Click
        Dim Radio As wclBluetoothRadio = Nothing
        Dim Res As Int32 = wclBluetoothManager.GetRadio(Radio)
        If Res <> wclErrors.WCL_E_SUCCESS Then
            Trace("Get working radio failed", Res)
        Else
            Res = Radio.Discover(10, wclBluetoothDiscoverKind.dkBle)
            If Res <> wclErrors.WCL_E_SUCCESS Then
                Trace("Start discovering failed", Res)
            Else
                btDiscover.Enabled = False
            End If
        End If
    End Sub

    Private Sub btDisconnect_Click(sender As Object, e As EventArgs) Handles btDisconnect.Click
        Dim Res As Int32 = wclGattClient.Disconnect()
        If Res <> wclErrors.WCL_E_SUCCESS Then Trace("Disconnect failed", Res)
    End Sub

    Private Sub btConnect_Click(sender As Object, e As EventArgs) Handles btConnect.Click
        If lvDevices.SelectedItems.Count = 0 Then
            Trace("Select device")
        Else
            Dim Radio As wclBluetoothRadio = Nothing
            Dim Res As Int32 = wclBluetoothManager.GetRadio(Radio)
            If Res <> wclErrors.WCL_E_SUCCESS Then
                Trace("Get working radio failed", Res)
            Else
                Dim Mac As Int64 = Convert.ToInt64(lvDevices.SelectedItems(0).Text, 16)
                If wclGattClient.State <> wclClientState.csDisconnected Then
                    Trace("Already conencted")
                Else
                    wclGattClient.Address = Mac
                    Res = wclGattClient.Connect(Radio)
                    If Res <> wclErrors.WCL_E_SUCCESS Then
                        Trace("Connect failed", Res)
                    Else
                        Trace("Connecting")

                        btConnect.Enabled = False
                        btDisconnect.Enabled = True
                        btDiscover.Enabled = False
                    End If
                End If
            End If
        End If
    End Sub

    Private Sub lvDevices_SelectedIndexChanged(sender As Object, e As EventArgs) Handles lvDevices.SelectedIndexChanged
        btConnect.Enabled = (lvDevices.SelectedItems.Count > 0 And btDiscover.Enabled And wclGattClient.State = wclClientState.csDisconnected)
    End Sub
End Class
