// ImageToolDlg.cpp : ���� ����

#include "stdafx.h"
#include "ImageTool.h"
#include "ImageToolDlg.h"
#include <cstring> // memsetȰ���� ���� ����
#include <math.h>
#include <time.h>

#include <stdlib.h>
#include "include_tracking.h"
#include "include_color.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#define BACK_DIF  40
#define which_image 0	//1�̸� ������ + ��������;, 0�̸� ������
#endif

extern int redON;
extern int greenON;
extern int blueON;
extern int yellowON;

extern int redstrong;
extern int greenstrong;
extern int bluestrong;
extern int yellowstrong;

int countall = 0;
int p1_x = 0, p1_y = 0, p2_x = 0, p2_y = 0, p3_x = 0, p3_y = 0, p4_x = 0, p4_y = 0;
int p1_ax = 0, p1_ay = 0, p2_ax = 0, p2_ay = 0;

int trackisOn=0;
int howmanytrack=0;
int tx1=0, tx2=0, ty1=0, ty2=0;

int flag2=1,flag3=1,flag4=1,flag5=1;

BOOL m_bDrawMode;
BOOL Range_DrawMode;
int m_x1 = 0,m_y1 = 0,m_x2 = 0,m_y2 = 0;
int m_x=0, m_y=0;
int p_x=0, p_y=0;
int m_val1=0, m_val2=0, m_val3=0;
int flag = 0;
int shape_check1=0, shape_check2=0, shape_check3=0, shape_check4 =0;

int x1_1, x1_2, y1_1, y1_2;
int x2_1, x2_2, y2_1, y2_2;
int x3_1, x3_2, y3_1, y3_2;
int x4_1, x4_2, y4_1, y4_2;

BITMAPINFO BmInfo;

LPBYTE Show_Cam1 = NULL;
LPBYTE Show_Cam2 = NULL;
LPBYTE Show_Cam3 = NULL;
LPBYTE Show_Cam4 = NULL;
LPBYTE Show_Cam5 = NULL;
LPBYTE Show_Cam6 = NULL;

int ScreenAverage1=0;
int ScreenAverage2=0;
int ScreenAverage3=0;

bool LbuttonisDown=false;	//���콺 ���� Ŭ�� �Ǿ����� Ȯ��
int count = 1;

int result1=0,result2=0,result3=0,result4=0; 

/** ���� ���� ���� �� **/

/** �Լ����� ���� **/
void ShowPixel();
double FPS();
//extern void gray2binary(BYTE** input, BYTE** output);

/** �Լ����� �� **/
///////////////////////////////////////////////////////////////////////////////////////////////////////
extern int hs_test;
int global_variable_test = 1024;

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


// CImageToolDlg ��ȭ ����




CImageToolDlg::CImageToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageToolDlg::IDD, pParent)
	, m_test(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CImageToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM, m_Cam);
	DDX_Control(pDX, IDC_EDIT1, m_Frame);
	DDX_Control(pDX, IDC_EDIT2, m_Frame2);
	DDX_Control(pDX, IDC_EDIT3, m_Frame3);
	DDX_Control(pDX, IDC_EDIT4, m_Frame4);
	DDX_Control(pDX, IDC_EDIT5, m_Frame5);
	DDX_Control(pDX, IDC_EDIT6, m_Frame6);
	DDX_Control(pDX, IDC_EDIT7, m_Frame7);
	DDX_Control(pDX, IDC_EDIT8, m_Frame8);
	DDX_Control(pDX, IDC_EDIT9, m_Frame9);
	DDX_Control(pDX, IDC_EDIT10, m_Frame10);
	DDX_Control(pDX, IDC_EDIT11, m_Frame11);
	DDX_Control(pDX, IDC_EDIT12, m_Frame12);
	DDX_Control(pDX, IDC_EDIT13, m_Frame13);
	DDX_Control(pDX, IDC_EDIT14, m_Frame14);
	DDX_Control(pDX, IDC_EDIT15, m_Frame15);
	DDX_Control(pDX, IDC_EDIT16, m_Frame16);
	DDX_Control(pDX, IDC_EDIT17, m_Frame17);
	DDX_Control(pDX, IDC_EDIT18, m_Frame18);
	DDX_Control(pDX, IDC_EDIT19, m_Frame19);


}
BEGIN_MESSAGE_MAP(CImageToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_Range, &CImageToolDlg::OnBnClickedRange)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CImageToolDlg �޽��� ó����

BOOL CImageToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bDrawMode = FALSE;
	Range_DrawMode = FALSE;
	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString m_strErrMsg = TEXT("");

	m_hCamWnd = capCreateCaptureWindow( TEXT("Capture Window"), WS_CHILD | WS_VISIBLE, 5, 5, WIDTH*2, HEIGHT*2, m_Cam, NULL);
	// call the WM_CAP_DRIVER_CONNECT
	// ��ġ�� ����̽��� ������ 0 ~ 9������ ī�޶� ���� �� �� �ִ�.
	// ������ > ī�޶� �� ���ɳʿ� �ݵ�� 1�� �̻��� ī�޶� ��ġ �Ǿ� �־�� �Ѵ�.
	if(!capDriverConnect(m_hCamWnd, 0)) 
	{	
		m_strErrMsg = "Error #001 : the specified capture driver cannot be connected to the capture window.";		
	} 

	// ���� ����̹� ������ ���� ���� ������
	else if(!capDriverGetCaps(m_hCamWnd, &m_psCapsInfo, sizeof(m_psCapsInfo))) {	
		m_strErrMsg = "Error #006 : the capture window is not connected to a capture driver.";
	}

	else
	{	
		if(m_psCapsInfo.fHasDlgVideoFormat) 
		{	
			if(!capDlgVideoFormat(m_hCamWnd)) 
			{	
				m_strErrMsg = "Error #007: Video Format Dialog Error";
			}
		}
		// m_psCapsInfo.fHasOverlay���� overlay�� ������ ���� ������(=0) ��� �� �� ����.
		if(m_psCapsInfo.fHasOverlay) 
		{	
			if(!capOverlay(m_hCamWnd, FALSE)) 
			{	
				m_strErrMsg = "Error #004 : Overlay fail";
			}
		}
		// 33/1000 ������ ������ ��µȴ�.
		if(!capPreviewRate(m_hCamWnd, 33)) 
		{	
			m_strErrMsg = "Error #002 : the capture window is not connected to a capture driver.";
		}
		if(!capPreview(m_hCamWnd, TRUE)) 
		{	
			m_strErrMsg = "Error #005 : the Preview is failed.";
		}
	}
	// ���� �޽��� 
	if(m_strErrMsg.GetLength() != 0)
	{
		MessageBox(m_strErrMsg);
	}

	capGetVideoFormat(m_hCamWnd, &BmInfo, sizeof(BITMAPINFO)); 
	if(BmInfo.bmiHeader.biBitCount != 24) 
	{	
		BmInfo.bmiHeader.biBitCount = 24; 
		BmInfo.bmiHeader.biCompression = 0;
		BmInfo.bmiHeader.biSizeImage = BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight*3; 
		capSetVideoFormat(m_hCamWnd, &BmInfo, sizeof(BITMAPINFO)); 
	} 
	//	capSetCallbackOnVideoStream(m_hCamWnd, VideoCallbackProc);
	capSetCallbackOnFrame(m_hCamWnd, VideoCallbackProc);




	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}
void CImageToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.
void CImageToolDlg::OnPaint()
{
	

	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CImageToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CImageToolDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	capDriverDisconnect(m_hCamWnd);
	//////////////////////////////////////////////////////////////////////////	free
	if( Show_Cam1 != NULL )	delete []Show_Cam1;
	if( Show_Cam2 != NULL )	delete []Show_Cam2;
	if( Show_Cam3 != NULL )	delete []Show_Cam3;
	if( Show_Cam4 != NULL )	delete []Show_Cam4;
	if( Show_Cam5 != NULL)	delete []Show_Cam5;
	if( Show_Cam6 != NULL)	delete []Show_Cam6;

}

struct track track1;
struct track track2;
struct track track3;
struct track track4;


int k=0, m=0;

int	prstrong=0;
int	pbstrong=0;
int	pystrong=0;
int pgstrong=0;

int redlost=0;
int bluelost=0;
int yellowlost=0;
int greenlost=0;


LRESULT CALLBACK VideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{	
	////////////////////////////////////////////////////////////////////////////
	//�Լ� �� �����
	int x, y, i, j;
	int Average = 0;
	int ThresHold;

	int just_one=1;
	int shape_check2_2=0;
	int *max1 = new int;
	int *max2 = new int;
	int *max3 = new int;
	int *max4 = new int;

	*max1 = 30;
	*max2 = 30;
	*max3 = 30;
	*max4 = 30;
	//int ColorValue;
	int temp[256];
	clock_t Time_Start,Time_end;
	double duration;
	Time_Start = clock();
	if(Show_Cam1==NULL)	Show_Cam1 = new BYTE[HEIGHT * WIDTH * 3];
	if(Show_Cam2==NULL)	Show_Cam2 = new BYTE[HEIGHT * WIDTH * 3];
	if(Show_Cam3==NULL)	Show_Cam3 = new BYTE[HEIGHT * WIDTH * 3];
	if(Show_Cam4==NULL)	Show_Cam4 = new BYTE[HEIGHT * WIDTH * 3];
	if(Show_Cam5==NULL)	Show_Cam5 = new BYTE[HEIGHT * WIDTH * 3];
	if(Show_Cam6==NULL)	Show_Cam6 = new BYTE[HEIGHT * WIDTH * 3];
	/////////////////////////////////////////
	//	240*320
	BYTE** GrayImage = new BYTE*[HEIGHT];
	BYTE** Screen1 = new BYTE*[HEIGHT];
	BYTE** Screen2 = new BYTE*[HEIGHT];
	BYTE** Screen3 = new BYTE*[HEIGHT];
	BYTE** Screen4 = new BYTE*[HEIGHT];
	BYTE** Screen5 = new BYTE*[HEIGHT];
	BYTE** Screen6 = new BYTE*[HEIGHT];

	BYTE** temp1 = new BYTE*[HEIGHT];
	BYTE** temp2 = new BYTE*[HEIGHT];	
	BYTE** temp3 = new BYTE*[HEIGHT];
	BYTE** temp4 = new BYTE*[HEIGHT];

	BYTE** Screen_r = new BYTE*[HEIGHT];
	BYTE** Screen_b = new BYTE*[HEIGHT];
	BYTE** Screen_y = new BYTE*[HEIGHT];
	BYTE** Screen_g = new BYTE*[HEIGHT];


	BYTE** Red = new BYTE*[HEIGHT];
	BYTE** Green = new BYTE*[HEIGHT];
	BYTE** Blue = new BYTE*[HEIGHT];

	float omg[HEIGHT][WIDTH]={0,};
	
	for (y=0; y<HEIGHT; y++)
	{
		GrayImage[y] = new BYTE[WIDTH];
		Screen1[y] = new BYTE[WIDTH];
		Screen2[y] = new BYTE[WIDTH];
		Screen3[y] = new BYTE[WIDTH];
		Screen4[y] = new BYTE[WIDTH];
		Screen5[y] = new BYTE[WIDTH];
		Screen6[y] = new BYTE[WIDTH];
		
		temp1[y] = new BYTE[WIDTH];
		temp2[y] = new BYTE[WIDTH];
		
		temp3[y] = new BYTE[WIDTH];
		temp4[y] = new BYTE[WIDTH];

		Screen_r[y] = new BYTE[WIDTH];
		Screen_g[y] = new BYTE[WIDTH];
		Screen_b[y] = new BYTE[WIDTH];
		Screen_y[y] = new BYTE[WIDTH];

		Red[y] = new BYTE[WIDTH];
		Green[y] = new BYTE[WIDTH];
		Blue[y] = new BYTE[WIDTH];

		memset(GrayImage[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen1[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen2[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen3[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen4[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen5[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen6[y],0,sizeof(BYTE)*WIDTH);

		memset(Screen_r[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen_y[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen_g[y],0,sizeof(BYTE)*WIDTH);
		memset(Screen_b[y],0,sizeof(BYTE)*WIDTH);

		memset(temp1[y], 0,sizeof(BYTE)*WIDTH);
		memset(temp2[y],0,sizeof(BYTE)*WIDTH);
		memset(temp3[y], 0,sizeof(BYTE)*WIDTH);
		memset(temp4[y],0,sizeof(BYTE)*WIDTH);
		memset(Red[y],0,sizeof(BYTE)*WIDTH);
		memset(Green[y],0,sizeof(BYTE)*WIDTH);
		memset(Blue[y],0,sizeof(BYTE)*WIDTH);
	}
	memset(temp,0,sizeof(int)*256);
		
	//������ 240 * 320 ���� �ٲ��ֱ�
	for(j=0, y=0; j<HEIGHT*2; j=j+2, y++)
	{	
		for( i=0, x=0; i<WIDTH*2; i=i+2, x++)
		{	
			Show_Cam1[(y*(WIDTH)+x)*3]   = lpVHdr->lpData[(j*(WIDTH*2)+i)*3];
			Show_Cam1[(y*(WIDTH)+x)*3+1] = lpVHdr->lpData[(j*(WIDTH*2)+i)*3+1];
			Show_Cam1[(y*(WIDTH)+x)*3+2] = lpVHdr->lpData[(j*(WIDTH*2)+i)*3+2];

			BYTE B = Show_Cam1[(y*(WIDTH)+x)*3];
			BYTE G = Show_Cam1[(y*(WIDTH)+x)*3+1];
			BYTE R = Show_Cam1[(y*(WIDTH)+x)*3+2];
			Blue[y][x] = B;
			Green[y][x] = G;
			Red[y][x] = R;


			BYTE gray = (BYTE)((R + G + B )/3.0);
			GrayImage[y][x] = gray;

			Screen2[y][x]=GrayImage[y][x];
			
		}
	}
/** �Լ� ��� ���� **/

	//ȭ�鿡 �ѷ����� �����
#if which_image
	clear_img(temp1);
	clear_img(temp2);
	clear_img(temp3);
	clear_img(temp4);
	include_detect_color(Red,Green,Blue, temp1, temp2, 1);
	include_erode(temp2, temp3);
	include_dilate(temp3, temp4);
	include_Detect_Contour(temp4, Screen3);

	clear_img(temp1);
	clear_img(temp2);
	clear_img(temp3);
	clear_img(temp4);
	include_detect_color(Red,Green,Blue, temp1, temp2, 2);
	include_erode(temp2, temp3);
	include_dilate(temp3, temp4);
	include_Detect_Contour(temp4, Screen4);

	clear_img(temp1);
	clear_img(temp2);
	clear_img(temp3);
	clear_img(temp4);
	include_detect_color(Red,Green,Blue, temp1, temp2, 3);
	include_erode(temp2, temp3);
	include_dilate(temp3, temp4);
	include_Detect_Contour(temp4, Screen5);

	clear_img(temp1);
	clear_img(temp2);
	clear_img(temp3);
	clear_img(temp4);
	include_detect_color(Red,Green,Blue, temp1, temp2, 4);
	include_erode(temp2, temp3);
	include_dilate(temp3, temp4);
	include_Detect_Contour(temp4, Screen6);
	clear_img(temp1);
	clear_img(temp2);
	clear_img(temp3);
	clear_img(temp4);
#else
	clear_img(temp1);
	include_detect_color(Red,Green,Blue, temp1, Screen3, 1);
	clear_img(temp1);
	include_detect_color(Red,Green,Blue, temp1, Screen4, 2);
	clear_img(temp1);
	include_detect_color(Red,Green,Blue, temp1, Screen5, 3);
	clear_img(temp1);
	include_detect_color(Red,Green,Blue, temp1, Screen6, 4);
	clear_img(temp1);	
#endif
	
	draw_red_dif(Red, Green,Blue, Screen_r);
	draw_blue_dif(Red, Green,Blue, Screen_b);
	draw_yellow_dif(Red, Green,Blue, Screen_y);
	draw_green_dif(Red, Green,Blue, Screen_g);
	//////////////////////�ӽ�
	include_erode(Screen_b,temp1);
	
	if(redstrong==0&&result1==0&&countall==10)shape_check1 = include_Detect_this_Color(Red, Green, Blue, 1, Screen_r);		//red
	if(bluestrong==0&&result2==0&&countall==20)shape_check2 = include_Detect_this_Color(Red, Green, Blue, 2, Screen_b);		//blue
	if(yellowstrong==0&&result3==0&&countall==30)shape_check3 = include_Detect_this_Color(Red, Green, Blue, 3, Screen_y);	//yellow
	if(greenstrong==0&&result4==0&&countall==40)shape_check4 = include_Detect_this_Color(Red, Green, Blue, 4, Screen_g);	//green
	
	if(countall>40){
		countall=0;
	}
	countall+=10;		

	if(shape_check1==1)
		redstrong=1;
	if(shape_check2==1)
		bluestrong=1;
	if(shape_check3==1)
		yellowstrong=1;
	if(shape_check4==1)
		greenstrong=1;
	
	if(shape_check1 && redstrong){//R
		redON=1;
		track1=include_tracking_roi(Screen_r, track1, 50, Screen2, max1, Red, Green, Blue);
		if(*max1>track1.level) track1.level=*max1;
		result1=1;
		redON=0;
	}
	else{
		redstrong=0;
	}
	if(shape_check2 && bluestrong){//B
		blueON=1;
		track2=include_tracking_roi(temp1, track2, 50, Screen2, max2, Red, Green, Blue);
		if(*max2>track2.level) track2.level=*max2;
		result2=1;
		blueON=0;
	}
	else{
		Init_track(track2);
		bluestrong=0;
	}
	if(shape_check3 && yellowstrong){//Y
		yellowON=1;
		track3=include_tracking_roi(Screen_y, track3, 50, Screen2, max3, Red, Green, Blue);
		if(*max3>track3.level) track3.level=*max3;
		result3=1;
		yellowON=0;
	}
	else{
		yellowstrong=0;
	}
	if(shape_check4 && greenstrong){//G
		greenON=1;
		track4=include_tracking_roi(Screen_g, track4, 50, Screen2, max4, Red, Green, Blue);
		if(*max4>track4.level) track4.level=*max4;
		result4=1;
		greenON=0;		
	}
	else{
		greenstrong=0;
	}


	int timetofind=3;	//10�� false���� �ν�����.

	if(redstrong==0){
		if(redlost<=0){
		result1=0;
		shape_check1 = 0;
		Init_track(track1);
		}

	}
	else{ 
		shape_check1=1;
		redlost=timetofind;
	}

	if(redlost>0){
		redlost-=1;
		redstrong=1;
	}


	
	if(bluestrong==0){
		if(bluelost<=0){
		result2=0;
		shape_check2 = 0;
		Init_track(track2);
		}

	}
	else{ 
		shape_check2=1;
		bluelost=timetofind;
	}

	if(bluelost>0){
		bluelost-=1;
		bluestrong=1;
	}

	if(yellowstrong==0){
		if(yellowlost<=0){
		result3=0;
		shape_check3 = 0;
		Init_track(track3);
		}

	}
	else{ 
		shape_check3=1;
		yellowlost=timetofind;
	}

	if(yellowlost>0){
		yellowlost-=1;
		yellowstrong=1;
	}


	if(greenstrong==0){
		if(greenlost<=0){
		result4=0;
		shape_check4 = 0;
		Init_track(track4);
		}

	}
	else{ 
		shape_check4=1;
		greenlost=timetofind;
	}

	if(greenlost>0){
		greenlost-=1;
		greenstrong=1;		
	}	

	prstrong=redstrong;
	pbstrong=bluestrong;
	pystrong=yellowstrong;
	pgstrong=greenstrong;


	

/** �Լ� ��� �� **/
	
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���� ���� ���

	for( y=0; y<HEIGHT; y++)
	{	
		for( x=0; x<WIDTH; x++)
		{	
			Show_Cam1[(y*(WIDTH)+x)*3]   = Blue[y][x];
			Show_Cam1[(y*(WIDTH)+x)*3+1] = Green[y][x];
			Show_Cam1[(y*(WIDTH)+x)*3+2] = Red[y][x];


			Show_Cam2[(y*WIDTH+x)*3]   = Screen2[y][x];			//	Image 2
			Show_Cam2[(y*WIDTH+x)*3+1] = Screen2[y][x];
			Show_Cam2[(y*WIDTH+x)*3+2] = Screen2[y][x];

			Show_Cam3[(y*WIDTH+x)*3]   = Screen3[y][x];			//	Image 3
			Show_Cam3[(y*WIDTH+x)*3+1] = Screen3[y][x];
			Show_Cam3[(y*WIDTH+x)*3+2] = Screen3[y][x];

			Show_Cam4[(y*WIDTH+x)*3]   = Screen4[y][x];
			Show_Cam4[(y*WIDTH+x)*3+1] = Screen4[y][x];
			Show_Cam4[(y*WIDTH+x)*3+2] = Screen4[y][x];
			
			Show_Cam5[(y*WIDTH+x)*3]   = Screen5[y][x];							//	Image 5
			Show_Cam5[(y*WIDTH+x)*3+1] = Screen5[y][x];
			Show_Cam5[(y*WIDTH+x)*3+2] = Screen5[y][x];

			
			Show_Cam6[(y*WIDTH+x)*3]   = Screen6[y][x];
			Show_Cam6[(y*WIDTH+x)*3+1] = Screen6[y][x];
			Show_Cam6[(y*WIDTH+x)*3+2] = Screen6[y][x];

		}
	}

	//////////////////////////////////////////////////////////////////////////
	BmInfo.bmiHeader.biWidth = WIDTH;
	BmInfo.bmiHeader.biHeight = HEIGHT;
	BmInfo.bmiHeader.biSizeImage = BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight*3;
	CImageToolDlg *pDlg = ( CImageToolDlg* )AfxGetMainWnd();
	CDC* pDC = pDlg->GetDC();

	SetDIBitsToDevice(pDC->GetSafeHdc(), 1000, 20 , WIDTH, HEIGHT, 0, 0, 0, HEIGHT,
	 	Show_Cam1, &BmInfo, DIB_RGB_COLORS);

	SetDIBitsToDevice(pDC->GetSafeHdc(), 670, 20 , WIDTH, HEIGHT, 0, 0, 0, HEIGHT,
		Show_Cam2, &BmInfo, DIB_RGB_COLORS);

	SetDIBitsToDevice(pDC->GetSafeHdc(), 670, 265 , WIDTH, HEIGHT, 0, 0, 0, HEIGHT,
		Show_Cam3, &BmInfo, DIB_RGB_COLORS);

	SetDIBitsToDevice(pDC->GetSafeHdc(), 670, 510 , WIDTH, HEIGHT, 0, 0, 0, HEIGHT,
		Show_Cam4, &BmInfo, DIB_RGB_COLORS);

	SetDIBitsToDevice(pDC->GetSafeHdc(), 345, 510 , WIDTH, HEIGHT, 0, 0, 0, HEIGHT,
		Show_Cam5, &BmInfo, DIB_RGB_COLORS);

	SetDIBitsToDevice(pDC->GetSafeHdc(), 20, 510 , WIDTH, HEIGHT, 0, 0, 0, HEIGHT,
		Show_Cam6, &BmInfo, DIB_RGB_COLORS);

	ReleaseDC(hWnd,*pDC);
	BmInfo.bmiHeader.biWidth = WIDTH*2;
	BmInfo.bmiHeader.biHeight = HEIGHT*2;
	BmInfo.bmiHeader.biSizeImage = BmInfo.bmiHeader.biWidth*BmInfo.bmiHeader.biHeight*3;

	for(y=0; y<HEIGHT; y++)
	{
		delete [] GrayImage[y];
		delete [] Screen1[y];
		delete [] Screen2[y];
		delete [] Screen3[y];
		delete [] Screen4[y];
		delete [] Screen5[y];
		delete [] Screen6[y];

		delete [] Screen_r[y];
		delete [] Screen_y[y];
		delete [] Screen_g[y];
		delete [] Screen_b[y];

		delete [] temp1[y];
		delete [] temp2[y];
		delete [] temp3[y];
		delete [] temp4[y];

		delete [] Red[y];
		delete [] Green[y];
		delete [] Blue[y];
	}
	delete [] GrayImage;
	delete [] Screen1;
	delete [] Screen2;
	delete [] Screen3;
	delete [] Screen4;
	delete [] Screen5;
	delete [] Screen6;

	delete [] Screen_r;
	delete [] Screen_y;
	delete [] Screen_g;
	delete [] Screen_b;


	delete [] temp1;
	delete [] temp2;
	delete [] temp3;
	delete [] temp4;

	delete [] Red;
	delete [] Green;
	delete [] Blue;

	delete max1, max2, max3,max4;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time_end = clock();
	duration = (float)(Time_end - Time_Start) /CLOCKS_PER_SEC;
	CString str;
	//�ؽ�Ʈ ���!
	str.Format("%f",1/duration);
	pDlg->m_Frame.SetWindowText(str);
	
	str.Format("��ü â������ ���콺 ��ġ m_x=%4d m_y=%3d",m_x,m_y);
	pDlg->m_Frame2.SetWindowText(str);
	str.Format("x=%3d y=%3d | ��1(B)=%4d ��2(G)=%4d ��3(R)=%4d",p_x,p_y, m_val1, m_val2, m_val3);
	pDlg->m_Frame3.SetWindowText(str);
	str.Format("���� ���� ��� ��1(B)=%3d ��1(G)=%3d ��1(R)=%3d", ScreenAverage1,ScreenAverage2,ScreenAverage3);
	
	ShowPixel();
	return (LRESULT)TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//�Լ� ���� �� ����
///////////////////////////////////////////////


void ShowPixel(){
	
	if(m_x>=1000 && m_x<1000+WIDTH && m_y>=20 && m_y<20+HEIGHT){
		p_x=m_x-1000;
		p_y=HEIGHT-(m_y-20);
		m_val1=Show_Cam1[(p_y*WIDTH+p_x)*3];
		m_val2=Show_Cam1[(p_y*WIDTH+p_x)*3+1];
		m_val3=Show_Cam1[(p_y*WIDTH+p_x)*3+2];
	}
	else if(m_x>=670 && m_x<670+WIDTH && m_y>=20 && m_y<20+HEIGHT){
		p_x=m_x-670;
		p_y=HEIGHT-(m_y-20);
		m_val1=Show_Cam2[(p_y*WIDTH+p_x)*3];
		m_val2=Show_Cam2[(p_y*WIDTH+p_x)*3+1];
		m_val3=Show_Cam2[(p_y*WIDTH+p_x)*3+2];
	}
	else if(m_x>=670 && m_x<670+WIDTH && m_y>=265 && m_y<265+HEIGHT){
		p_x=m_x-670;
		p_y=HEIGHT-(m_y-265);
		m_val1=Show_Cam3[(p_y*WIDTH+p_x)*3];
		m_val2=Show_Cam3[(p_y*WIDTH+p_x)*3+1];
		m_val3=Show_Cam3[(p_y*WIDTH+p_x)*3+2];
	}
	else if(m_x>=670 && m_x<670+WIDTH && m_y>=510 && m_y<510+HEIGHT){
		p_x=m_x-670;
		p_y=HEIGHT-(m_y-510);
		m_val1=Show_Cam4[(p_y*WIDTH+p_x)*3];
		m_val2=Show_Cam4[(p_y*WIDTH+p_x)*3+1];
		m_val3=Show_Cam4[(p_y*WIDTH+p_x)*3+2];
	}
	else if(m_x>=345 && m_x<345+WIDTH && m_y>=510 && m_y<510+HEIGHT){
		p_x=m_x-345;
		p_y=HEIGHT-(m_y-510);
		m_val1=Show_Cam5[(p_y*WIDTH+p_x)*3];
		m_val2=Show_Cam5[(p_y*WIDTH+p_x)*3+1];
		m_val3=Show_Cam5[(p_y*WIDTH+p_x)*3+2];
	}
	else if(m_x>=20 && m_x<20+WIDTH && m_y>=510 && m_y<510+HEIGHT){
		p_x=m_x-20;
		p_y=HEIGHT-(m_y-510);
		m_val1=Show_Cam6[(p_y*WIDTH+p_x)*3];
		m_val2=Show_Cam6[(p_y*WIDTH+p_x)*3+1];
		m_val3=Show_Cam6[(p_y*WIDTH+p_x)*3+2];
	}
}

//FPS����
double FPS()
{
	double FPS_Value;
	static clock_t t;
	FPS_Value = (float)1/((clock() - t)/ CLOCKS_PER_SEC);
	return FPS_Value;
}

void CImageToolDlg::OnBnClickedRange()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Range_DrawMode = TRUE;
	if (flag == 0) {
		flag = 1;
	}
	else
		flag = 0;


}
void CImageToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	printf("wtf button down\n");

	LbuttonisDown=true;
	m_x1=m_x2=point.x;
	m_y1=m_y2=point.y;

}
void CImageToolDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	LbuttonisDown=false;
	printf("wtf button up\n");
	
	int p_x1=0, p_x2=0, p_y1=0, p_y2=0;
	int tmp;
	int size=1;

	if(m_y1<m_y2){	//������ ���� ������
		tmp=m_y1;
		m_y1=m_y2;
		m_y2=tmp;
	}
	if(m_x1>m_x2){
		tmp=m_x1;
		m_x1=m_x2;
		m_x2=tmp;
	}

	ScreenAverage1=0;
	ScreenAverage2=0;
	ScreenAverage3=0;


	if(m_x2>=1000 && m_x2<1000+WIDTH && m_y2>=20 && m_y2<20+HEIGHT){
		p_x2=m_x2-1000;
		p_y2=HEIGHT-(m_y2-20);
		if(m_x1<1000)
			p_x1=0;
		else
			p_x1=m_x1-1000;
		if(m_y1<20)
			p_y1=0;
		else
			p_y1=HEIGHT-(m_y1-20);

		for(int i=p_y1; i<p_y2+1; i++){
			for(int j=p_x1; j<p_x2+1; j++){
				ScreenAverage1+=Show_Cam1[(i*WIDTH+j)*3];
				ScreenAverage2+=Show_Cam1[(i*WIDTH+j)*3+1];
				ScreenAverage3+=Show_Cam1[(i*WIDTH+j)*3+2];
			}
		}

	}
	else if(m_x2>=670 && m_x2<670+WIDTH && m_y2>=20 && m_y2<20+HEIGHT){
		p_x2=m_x2-670;
		p_y2=HEIGHT-(m_y2-20);
		if(m_x1<670)
			p_x1=0;
		else
			p_x1=m_x1-670;
		if(m_y1<20)
			p_y1=0;
		else
			p_y1=HEIGHT-(m_y1-20);

		for(int i=p_y1; i<p_y2+1; i++){
			for(int j=p_x1; j<p_x2+1; j++){
				ScreenAverage1+=Show_Cam2[(i*WIDTH+j)*3];
				ScreenAverage2+=Show_Cam2[(i*WIDTH+j)*3+1];
				ScreenAverage3+=Show_Cam2[(i*WIDTH+j)*3+2];
			}
		}
	}
	else if(m_x2>=670 && m_x2<670+WIDTH && m_y2>=265 && m_y2<265+HEIGHT){
		p_x2=m_x2-670;
		p_y2=HEIGHT-(m_y2-265);
		if(m_x1<670)
			p_x1=0;
		else
			p_x1=m_x1-670;
		if(m_y1<265)
			p_y1=0;
		else
			p_y1=HEIGHT-(m_y1-265);

		for(int i=p_y1; i<p_y2+1; i++){
			for(int j=p_x1; j<p_x2+1; j++){
				ScreenAverage1+=Show_Cam3[(i*WIDTH+j)*3];
				ScreenAverage2+=Show_Cam3[(i*WIDTH+j)*3+1];
				ScreenAverage3+=Show_Cam3[(i*WIDTH+j)*3+2];
			}
		}
	}
	else if(m_x2>=670 && m_x2<670+WIDTH && m_y2>=510 && m_y2<510+HEIGHT){
		p_x2=m_x2-670;
		p_y2=HEIGHT-(m_y2-510);
		if(m_x1<670)
			p_x1=0;
		else
			p_x1=m_x1-670;
		if(m_y1<510)
			p_y1=0;
		else
			p_y1=HEIGHT-(m_y1-510);

		for(int i=p_y1; i<p_y2+1; i++){
			for(int j=p_x1; j<p_x2+1; j++){
				ScreenAverage1+=Show_Cam4[(i*WIDTH+j)*3];
				ScreenAverage2+=Show_Cam4[(i*WIDTH+j)*3+1];
				ScreenAverage3+=Show_Cam4[(i*WIDTH+j)*3+2];
			}
		}
	}
	else if(m_x2>=345 && m_x2<345+WIDTH && m_y2>=510 && m_y2<510+HEIGHT){
		p_x2=m_x2-345;
		p_y2=HEIGHT-(m_y2-510);
		if(m_x1<345)
			p_x1=0;
		else
			p_x1=m_x1-345;
		if(m_y1<510)
			p_y1=0;
		else
			p_y1=HEIGHT-(m_y1-510);

		for(int i=p_y1; i<p_y2+1; i++){
			for(int j=p_x1; j<p_x2+1; j++){
				ScreenAverage1+=Show_Cam5[(i*WIDTH+j)*3];
				ScreenAverage2+=Show_Cam5[(i*WIDTH+j)*3+1];
				ScreenAverage3+=Show_Cam5[(i*WIDTH+j)*3+2];
			}
		}
	}
	else if(m_x2>=20 && m_x2<20+WIDTH && m_y2>=510 && m_y2<510+HEIGHT){
		p_x2=m_x2-20;
		p_y2=HEIGHT-(m_y2-510);
		if(m_x1<20)
			p_x1=0;
		else
			p_x1=m_x1-20;
		if(m_y1<510)
			p_y1=0;
		else
			p_y1=HEIGHT-(m_y1-510);

		for(int i=p_y1; i<p_y2+1; i++){
			for(int j=p_x1; j<p_x2+1; j++){
				ScreenAverage1+=Show_Cam6[(i*WIDTH+j)*3];
				ScreenAverage2+=Show_Cam6[(i*WIDTH+j)*3+1];
				ScreenAverage3+=Show_Cam6[(i*WIDTH+j)*3+2];
			}
		}
		
	}
	size=((p_y2+1-p_y1)*(p_x2+1-p_x1));
	ScreenAverage1/=size;
	ScreenAverage2/=size;
	ScreenAverage3/=size;	

	howmanytrack+=1;
	
	printf("Track is started\n");
	
	m_x1=m_x2=m_y1=m_y2=0;
}
void CImageToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	m_x=point.x;
	m_y=point.y;
	

	if(LbuttonisDown==true){
		m_x2=point.x;
		m_y2=point.y;

		
		CClientDC dc(this);
		dc.Rectangle(m_x1,m_y1,m_x2,m_y2);
	}
}
