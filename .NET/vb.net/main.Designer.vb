<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class fmMain
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.lvDevices = New System.Windows.Forms.ListView()
        Me.chAddress = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.chName = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.btDiscover = New System.Windows.Forms.Button()
        Me.lbLog = New System.Windows.Forms.ListBox()
        Me.btClear = New System.Windows.Forms.Button()
        Me.btDisconnect = New System.Windows.Forms.Button()
        Me.btConnect = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'lvDevices
        '
        Me.lvDevices.Columns.AddRange(New System.Windows.Forms.ColumnHeader() {Me.chAddress, Me.chName})
        Me.lvDevices.HideSelection = False
        Me.lvDevices.Location = New System.Drawing.Point(12, 41)
        Me.lvDevices.MultiSelect = False
        Me.lvDevices.Name = "lvDevices"
        Me.lvDevices.ShowGroups = False
        Me.lvDevices.Size = New System.Drawing.Size(568, 97)
        Me.lvDevices.TabIndex = 7
        Me.lvDevices.UseCompatibleStateImageBehavior = False
        Me.lvDevices.View = System.Windows.Forms.View.Details
        '
        'chAddress
        '
        Me.chAddress.Text = "Address"
        Me.chAddress.Width = 150
        '
        'chName
        '
        Me.chName.Text = "Name"
        Me.chName.Width = 150
        '
        'btDiscover
        '
        Me.btDiscover.Location = New System.Drawing.Point(12, 12)
        Me.btDiscover.Name = "btDiscover"
        Me.btDiscover.Size = New System.Drawing.Size(75, 23)
        Me.btDiscover.TabIndex = 6
        Me.btDiscover.Text = "Discover"
        Me.btDiscover.UseVisualStyleBackColor = True
        '
        'lbLog
        '
        Me.lbLog.FormattingEnabled = True
        Me.lbLog.Location = New System.Drawing.Point(12, 173)
        Me.lbLog.Name = "lbLog"
        Me.lbLog.Size = New System.Drawing.Size(568, 264)
        Me.lbLog.TabIndex = 11
        '
        'btClear
        '
        Me.btClear.Location = New System.Drawing.Point(505, 144)
        Me.btClear.Name = "btClear"
        Me.btClear.Size = New System.Drawing.Size(75, 23)
        Me.btClear.TabIndex = 10
        Me.btClear.Text = "Clear"
        Me.btClear.UseVisualStyleBackColor = True
        '
        'btDisconnect
        '
        Me.btDisconnect.Enabled = False
        Me.btDisconnect.Location = New System.Drawing.Point(93, 144)
        Me.btDisconnect.Name = "btDisconnect"
        Me.btDisconnect.Size = New System.Drawing.Size(75, 23)
        Me.btDisconnect.TabIndex = 9
        Me.btDisconnect.Text = "Disconnect"
        Me.btDisconnect.UseVisualStyleBackColor = True
        '
        'btConnect
        '
        Me.btConnect.Enabled = False
        Me.btConnect.Location = New System.Drawing.Point(12, 144)
        Me.btConnect.Name = "btConnect"
        Me.btConnect.Size = New System.Drawing.Size(75, 23)
        Me.btConnect.TabIndex = 8
        Me.btConnect.Text = "Connect"
        Me.btConnect.UseVisualStyleBackColor = True
        '
        'fmMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(592, 450)
        Me.Controls.Add(Me.lvDevices)
        Me.Controls.Add(Me.btDiscover)
        Me.Controls.Add(Me.lbLog)
        Me.Controls.Add(Me.btClear)
        Me.Controls.Add(Me.btDisconnect)
        Me.Controls.Add(Me.btConnect)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Name = "fmMain"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "GATT Authentication Demo"
        Me.ResumeLayout(False)

    End Sub

    Private WithEvents lvDevices As ListView
    Private WithEvents chAddress As ColumnHeader
    Private WithEvents chName As ColumnHeader
    Private WithEvents btDiscover As Button
    Private WithEvents lbLog As ListBox
    Private WithEvents btClear As Button
    Private WithEvents btDisconnect As Button
    Private WithEvents btConnect As Button
End Class
