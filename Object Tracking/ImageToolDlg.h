// ImageToolDlg.h : 헤더 파일
//

#pragma once
#pragma comment(lib, "vfw32.lib")

#include <vfw.h>
#include "afxwin.h"
#define BACK_DIF  40
#define COLOR 3
#define DCOLOR 4
#define RED 0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define CLIP(x) (((x) <0)?0:(((x)>255)?255:(x)))
#define CCLIP(x) (((x) <0)?0:(((x)>360)?360:(x)))
#define WIDTH 640/2
#define HEIGHT 480/2
#define RHO			800
#define THETA		90
#define OFFSET		6
#define RHO_OFFSET	40
#define MAX_CORNER	5000
#define MAX_LABEL	10000

//#define num_rho  (int)(sqrt((double)(WIDTH*WIDTH + HEIGHT*HEIGHT))*2)

// CImageToolDlg 대화 상자
class CImageToolDlg : public CDialog
{
// 생성입니다.
public:
	CImageToolDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	HWND m_hCamWnd;
	CAPDRIVERCAPS m_psCapsInfo;
// 대화 상자 데이터입니다.
	enum { IDD = 102 };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Cam;
	afx_msg void OnDestroy();
	
	CStatic m_Frame;
	CStatic m_Frame2;
	CStatic m_Frame3;
	CStatic m_Frame4;
	CStatic m_Frame5;
	CStatic m_Frame6;
	CStatic m_Frame7;
	CStatic m_Frame8;
	CStatic m_Frame9;
	CStatic m_Frame10;
	CStatic m_Frame11;
	CStatic m_Frame12;
	CStatic m_Frame13;
	CStatic m_Frame14;
	CStatic m_Frame15;
	CStatic m_Frame16;
	CStatic m_Frame17;
	CStatic m_Frame18;
	CStatic m_Frame19;


	
	afx_msg void OnBnClickedRange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChangeH();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();

	afx_msg void OnEnChangeS();
	afx_msg void OnEnChangeL();
	afx_msg void OnEnChangeh2();
	afx_msg void OnEnChanges2();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnChangeEdit8();
	afx_msg void OnChangeEdit9();
	// test
	afx_msg void gd_Draw_Rect();
	int m_test;
};
LRESULT CALLBACK VideoCallbackProc(HWND, LPVIDEOHDR);
typedef struct _LineParam
{
	double rho;
	double ang;
}LineParam;