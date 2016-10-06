VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   8355
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9750
   LinkTopic       =   "Form1"
   ScaleHeight     =   557
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   650
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
   Begin VB.CommandButton Command3 
      Caption         =   "reset"
      Height          =   255
      Left            =   8640
      TabIndex        =   10
      Top             =   1920
      Width           =   975
   End
   Begin VB.TextBox Text2 
      Height          =   270
      Left            =   8640
      TabIndex        =   9
      Text            =   "40"
      Top             =   2640
      Width           =   855
   End
   Begin VB.VScrollBar VScroll3 
      Height          =   3135
      Left            =   8760
      Max             =   100
      TabIndex        =   8
      Top             =   4560
      Value           =   15
      Width           =   255
   End
   Begin VB.CommandButton Command2 
      Caption         =   "set"
      Height          =   255
      Left            =   8520
      TabIndex        =   7
      Top             =   480
      Width           =   1095
   End
   Begin VB.CommandButton Command1 
      Caption         =   "save"
      Height          =   255
      Left            =   8520
      TabIndex        =   6
      Top             =   1200
      Width           =   1095
   End
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   8520
      TabIndex        =   5
      Text            =   "Text1"
      Top             =   120
      Width           =   1095
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Left            =   0
      Max             =   90
      Min             =   -90
      TabIndex        =   3
      Top             =   7680
      Width           =   7695
   End
   Begin VB.VScrollBar VScroll2 
      Height          =   7695
      Left            =   7680
      Max             =   89
      Min             =   1
      TabIndex        =   2
      Top             =   0
      Value           =   46
      Width           =   255
   End
   Begin VB.PictureBox Picture1 
      AutoRedraw      =   -1  'True
      Height          =   7680
      Left            =   0
      ScaleHeight     =   508
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   508
      TabIndex        =   1
      Top             =   0
      Width           =   7680
   End
   Begin VB.VScrollBar VScroll1 
      Height          =   7695
      Left            =   8160
      Max             =   -500
      Min             =   500
      TabIndex        =   0
      Top             =   0
      Width           =   255
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   255
      Left            =   8520
      TabIndex        =   4
      Top             =   3720
      Width           =   1095
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Const Pi As Single = 3.1416
Private Const Pi2 As Single = Pi * 2
Private Const PiH As Single = Pi / 2

Private Dx As New DirectX7
Private Dd As DirectDraw7
Private ddsd As DDSURFACEDESC2
Private ddsd3d As DirectDrawSurface7

Private d3d As Direct3D7
Private d3dDevice As Direct3DDevice7

Private Va() As D3DVERTEX
Private Vb() As D3DVERTEX
Private Vc() As D3DVERTEX
Private Vd() As D3DVERTEX

Private Vcur(5) As D3DVERTEX

Private V_base() As D3DVERTEX


Dim cnt As Integer
Private alti() As Integer
Dim dist As Single
Dim curr_x As Integer
Dim curr_y As Integer


Private Sub Command1_Click()
    Open "a.map" For Binary As 1
    Put #1, 1, cnt
    Put #1, 3, alti
    Close 1
End Sub

Sub refre()

    Dim i As Single
    Dim j As Single
    Dim ax As Single
    Dim ay As Single
    Dim az As Single
    For i = 0 To cnt - 1
        For j = 0 To cnt - 1
            If j Mod 2 = 0 Then
                V_base(i, j).x = i - cnt / 2
                V_base(i, j).y = alti(i, j) / 100
                V_base(i, j).z = (j - cnt / 2) * 0.866
            Else
                V_base(i, j).x = i - cnt / 2 + 0.5
                V_base(i, j).y = alti(i, j) / 100
                V_base(i, j).z = (j - cnt / 2) * 0.866
            End If
            If curr_x = i And curr_y = j Then
                ax = V_base(i, j).x
                ay = V_base(i, j).y
                az = V_base(i, j).z
                Vcur(0).x = ax - 0.3
                Vcur(0).y = ay
                Vcur(0).z = az - 0.3
            
                Vcur(1).x = ax + 0.3
                Vcur(1).y = ay
                Vcur(1).z = az - 0.3
            
                Vcur(2).x = ax + 0.3
                Vcur(2).y = ay
                Vcur(2).z = az + 0.3
            
                Vcur(3).x = ax - 0.3
                Vcur(3).y = ay
                Vcur(3).z = az + 0.3
            
                Vcur(4).x = ax - 0.3
                Vcur(4).y = ay
                Vcur(4).z = az - 0.3
            End If
        Next j
    Next i
    
    Dim idx As Long

    idx = 0
    For i = 0 To cnt - 2
        For j = 0 To cnt / 2 - 1
            Va(idx) = V_base(i, j * 2 + 1)
            idx = idx + 1
            Va(idx) = V_base(i + 1, j * 2)
            idx = idx + 1
            Va(idx) = V_base(i + 1, j * 2)
            idx = idx + 1
            Va(idx) = V_base(i, j * 2)
            idx = idx + 1
            Va(idx) = V_base(i, j * 2)
            idx = idx + 1
            Va(idx) = V_base(i, j * 2 + 1)
            idx = idx + 1
        Next j
    Next i
    
    
    idx = 0
    For i = 0 To cnt - 2
        For j = 0 To cnt / 2 - 1
            Vb(idx) = V_base(i, j * 2 + 1)
            idx = idx + 1
            Vb(idx) = V_base(i + 1, j * 2 + 1)
            idx = idx + 1
            Vb(idx) = V_base(i + 1, j * 2 + 1)
            idx = idx + 1
            Vb(idx) = V_base(i + 1, j * 2)
            idx = idx + 1
            Vb(idx) = V_base(i + 1, j * 2)
            idx = idx + 1
            Vb(idx) = V_base(i, j * 2 + 1)
            idx = idx + 1
        Next j
    Next i
    
    idx = 0
    For i = 0 To cnt - 2
        For j = 0 To cnt / 2 - 2
            Vc(idx) = V_base(i, j * 2 + 2)
            idx = idx + 1
            Vc(idx) = V_base(i + 1, j * 2 + 2)
            idx = idx + 1
            Vc(idx) = V_base(i + 1, j * 2 + 2)
            idx = idx + 1
            Vc(idx) = V_base(i, j * 2 + 1)
            idx = idx + 1
            Vc(idx) = V_base(i, j * 2 + 1)
            idx = idx + 1
            Vc(idx) = V_base(i, j * 2 + 2)
            idx = idx + 1
        Next j
    Next i
    
    
    idx = 0
    For i = 0 To cnt - 2
        For j = 0 To cnt / 2 - 2
            Vd(idx) = V_base(i + 1, j * 2 + 2)
            idx = idx + 1
            Vd(idx) = V_base(i + 1, j * 2 + 1)
            idx = idx + 1
            Vd(idx) = V_base(i + 1, j * 2 + 1)
            idx = idx + 1
            Vd(idx) = V_base(i, j * 2 + 1)
            idx = idx + 1
            Vd(idx) = V_base(i, j * 2 + 1)
            idx = idx + 1
            Vd(idx) = V_base(i + 1, j * 2 + 2)
            idx = idx + 1
        Next j
    Next i
   
    
    
    
    
    
    Dim y As Single
    Dim matWorld As D3DMATRIX
    Dx.RotateYMatrix matWorld, HScroll1.Value / 90 * Pi
    y = VScroll2.Value / 90 * Pi - Pi / 2
    Dim matView  As D3DMATRIX
    Dx.IdentityMatrix matView
    
    Dx.ViewMatrix matView, MakeVector(0, Sin(y) * dist, Cos(y) * dist), MakeVector(0, 0, 0), MakeVector(0, 1, 0), 0
    d3dDevice.SetTransform D3DTRANSFORMSTATE_VIEW, matView
    d3dDevice.SetTransform D3DTRANSFORMSTATE_WORLD, matWorld
    

    
    
    
    
    
    
    
    Dim RECT(1) As D3DRECT
    RECT(0) = Maked3dRect(0, 512, 0, 512)
    d3dDevice.Clear 1, RECT(), D3DCLEAR_TARGET, Dx.CreateColorRGBA(0.5, 0.5, 0.5, 0), 0, 0
    
    
    
    d3dDevice.BeginScene
    
    d3dDevice.DrawPrimitive D3DPT_LINELIST, D3DFVF_VERTEX, Va(0), (cnt - 1) * cnt / 2 * 6, D3DDP_DEFAULT
    d3dDevice.DrawPrimitive D3DPT_LINELIST, D3DFVF_VERTEX, Vb(0), (cnt - 1) * cnt / 2 * 6, D3DDP_DEFAULT
    d3dDevice.DrawPrimitive D3DPT_LINELIST, D3DFVF_VERTEX, Vc(0), (cnt - 1) * cnt / 2 * 6, D3DDP_DEFAULT
    d3dDevice.DrawPrimitive D3DPT_LINELIST, D3DFVF_VERTEX, Vd(0), (cnt - 1) * cnt / 2 * 6, D3DDP_DEFAULT
    
    d3dDevice.DrawPrimitive D3DPT_LINESTRIP, D3DFVF_VERTEX, Vcur(0), 5, D3DDP_DEFAULT
    
    d3dDevice.EndScene
    
    ddsd3d.BltToDC Picture1.hDC, MakeRect(512, 0, 512, 0), MakeRect(512, 0, 512, 0)

    Picture1.Refresh
    DoEvents
End Sub


Private Sub Command2_Click()
    cnt = Text1.Text
    ReDim alti(cnt - 1, cnt - 1) As Integer
End Sub


Private Sub Command3_Click()
    Dim i As Integer, j As Integer
    For i = 0 To cnt - 1
        For j = 0 To cnt - 1
            alti(i, j) = 0
        Next j
    Next i
End Sub

Private Sub Form_Load()
    Open "a.map" For Binary As 1
    Get #1, 1, cnt
    ReDim alti(cnt - 1, cnt - 1) As Integer
    Get #1, 3, alti
    Close 1
    
    
    
    
    
    Set Dd = Dx.DirectDrawCreate("")
    Dd.SetCooperativeLevel Form1.hWnd, DDSCL_NORMAL

    
    ddsd.lFlags = DDSD_HEIGHT Or DDSD_WIDTH Or DDSD_CAPS
    ddsd.lWidth = 512
    ddsd.lHeight = 512
    ddsd.ddsCaps.lCaps = DDSCAPS_OFFSCREENPLAIN Or DDSCAPS_3DDEVICE
    Set ddsd3d = Dd.CreateSurface(ddsd)
    
    Dim d3d As Direct3D7
    Set d3d = Dd.GetDirect3D
    Set d3dDevice = d3d.CreateDevice("IID_IDirect3DHALDevice", ddsd3d)
    'Set d3dDevice = d3d.CreateDevice("IID_IDirect3DRGBDevice", ddsd3d)
    Dim VPDesc As D3DVIEWPORT7
    VPDesc.lWidth = 512
    VPDesc.lHeight = 512
    VPDesc.minz = 0
    VPDesc.maxz = 1
    d3dDevice.SetViewport VPDesc
    
    
    Dim matProj As D3DMATRIX
    Dx.IdentityMatrix matProj
    Dx.ProjectionMatrix matProj, 0, 1, Pi / 2
    d3dDevice.SetTransform D3DTRANSFORMSTATE_PROJECTION, matProj
    
    
    
    Dim mtrl As D3DMATERIAL7
    mtrl.Ambient = MakeColor(1, 1, 1, 1)
    d3dDevice.SetMaterial mtrl
    d3dDevice.SetRenderState D3DRENDERSTATE_AMBIENT, Dx.CreateColorRGBA(1, 1, 1, 0)
    
    
    ReDim Va((cnt - 1) * (cnt / 2 * 6)) As D3DVERTEX
    ReDim Vb((cnt - 1) * (cnt / 2 * 6)) As D3DVERTEX
    ReDim Vc((cnt - 1) * (cnt / 2 * 6)) As D3DVERTEX
    ReDim Vd((cnt - 1) * (cnt / 2 * 6)) As D3DVERTEX
    
    ReDim V_base(cnt, cnt) As D3DVERTEX
    
    dist = 15
    refre
End Sub

Private Sub HScroll1_Change()
    refre
End Sub

Private Sub HScroll1_Scroll()
    refre
End Sub


Private Sub Picture1_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
    If Button = 1 Then
        curr_x = (512 - x) / 512 * cnt
        curr_y = y / 512 * cnt
        VScroll1.Value = alti(curr_x, curr_y)
        Label1.Caption = VScroll1.Value
        refre
    End If

End Sub

Private Sub Picture1_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
    If Button = 1 Then
        curr_x = (512 - x) / 512 * cnt
        curr_y = y / 512 * cnt
        VScroll1.Value = alti(curr_x, curr_y)
        Label1.Caption = VScroll1.Value
        refre
    End If

End Sub

Private Sub set_alti(x As Integer, y As Integer, alt As Integer)
    Dim d As Single
    d = Text2.Text
    
    If x < 1 Or x > cnt - 2 Then Exit Sub
    If y < 1 Or y > cnt - 2 Then Exit Sub
    If Abs(alt - alti(x, y)) > 100 Then Exit Sub
    alti(x, y) = alt
    
    If alt - alti(x + 1, y) > d Then set_alti x + 1, y, alti(x, y) - d
    If alt - alti(x + 1, y) < -d Then set_alti x + 1, y, alti(x, y) + d
    If alt - alti(x - 1, y) > d Then set_alti x - 1, y, alti(x, y) - d
    If alt - alti(x - 1, y) < -d Then set_alti x - 1, y, alti(x, y) + d
    If alt - alti(x, y + 1) > d Then set_alti x, y + 1, alti(x, y) - d
    If alt - alti(x, y + 1) < -d Then set_alti x, y + 1, alti(x, y) + d
    If alt - alti(x, y - 1) > d Then set_alti x, y - 1, alti(x, y) - d
    If alt - alti(x, y - 1) < -d Then set_alti x, y - 1, alti(x, y) + d
    If y Mod 2 = 0 Then
        If alt - alti(x - 1, y + 1) > d Then set_alti x - 1, y + 1, alti(x, y) - d
        If alt - alti(x - 1, y + 1) < -d Then set_alti x - 1, y + 1, alti(x, y) + d
        If alt - alti(x - 1, y - 1) > d Then set_alti x - 1, y - 1, alti(x, y) - d
        If alt - alti(x - 1, y - 1) < -d Then set_alti x - 1, y - 1, alti(x, y) + d
    Else
        If alt - alti(x + 1, y + 1) > d Then set_alti x + 1, y + 1, alti(x, y) - d
        If alt - alti(x + 1, y + 1) < -d Then set_alti x + 1, y + 1, alti(x, y) + d
        If alt - alti(x + 1, y - 1) > d Then set_alti x + 1, y - 1, alti(x, y) - d
        If alt - alti(x + 1, y - 1) < -d Then set_alti x + 1, y - 1, alti(x, y) + d
    End If
End Sub

Private Sub VScroll1_Change()
    set_alti curr_x, curr_y, VScroll1.Value
    Label1.Caption = VScroll1.Value
    refre
End Sub

Private Sub VScroll1_Scroll()
    set_alti curr_x, curr_y, VScroll1.Value
    Label1.Caption = VScroll1.Value
    refre
End Sub
Public Function cut(a As Single) As Single
    cut = Int(a * 10000) / 10000
End Function
Public Function MakeVector(x As Single, y As Single, z As Single) As D3DVECTOR
    MakeVector.x = x
    MakeVector.y = y
    MakeVector.z = z
End Function

Public Function MakeVertex(x As Single, y As Single, z As Single, nx As Single, ny As Single, nz As Single, tu As Single, tv As Single) As D3DVERTEX
    MakeVertex.x = x
    MakeVertex.y = y
    MakeVertex.z = z
    MakeVertex.nx = nx
    MakeVertex.ny = ny
    MakeVertex.nz = nz
    MakeVertex.tu = tu
    MakeVertex.tv = tv
End Function


Public Function MakeRect(bottom As Long, left As Long, right As Long, top As Long) As RECT
    MakeRect.bottom = bottom
    MakeRect.left = left
    MakeRect.right = right
    MakeRect.top = top
End Function

Public Function Maked3dRect(X1 As Long, X2 As Long, Y1 As Long, Y2 As Long) As D3DRECT
    Maked3dRect.X1 = X1
    Maked3dRect.X2 = X2
    Maked3dRect.Y1 = Y1
    Maked3dRect.Y2 = Y2
End Function

Public Function MakeColor(r, g, b, a) As D3DCOLORVALUE
    MakeColor.r = r
    MakeColor.g = g
    MakeColor.b = b
    MakeColor.a = a
End Function
Public Function MakeVert(x As Single, y As Single, z As Single) As D3DVERTEX
    MakeVert.x = x
    MakeVert.y = y
    MakeVert.z = z
    MakeVert.nx = 0
    MakeVert.ny = 0
    MakeVert.nz = 0
    MakeVert.tu = 0
    MakeVert.tv = 0
End Function


Private Sub VScroll2_Change()
    refre
End Sub

Private Sub VScroll2_Scroll()
    refre
End Sub

Private Sub VScroll3_Change()
    dist = VScroll3.Value
    refre
End Sub

Private Sub VScroll3_Scroll()
    dist = VScroll3.Value
    refre

End Sub
