
// EnumProcessDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "EnumProcess.h"
#include "EnumProcessDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnumProcessDlg 대화 상자



CEnumProcessDlg::CEnumProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENUMPROCESS_DIALOG, pParent)
	, m_dwPID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEnumProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrl_ListBox);
	DDX_Text(pDX, IDC_EDIT1, m_dwPID);
}

BEGIN_MESSAGE_MAP(CEnumProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CEnumProcessDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEnumProcessDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CEnumProcessDlg 메시지 처리기

BOOL CEnumProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CEnumProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CEnumProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CEnumProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")

void CEnumProcessDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 1. ID 값 구한다. (실행중인 id를 모아주는 함수) ID의 배열을 구한다.

	/*DWORD info = NULL;
	info = GetCurrentProcessId();*/
	
	DWORD ar[512] = {0, };
	DWORD processID = 0;
	DWORD cbNeeded;	// 반환 되는 바이트 수
	DWORD FileName;
	HANDLE h2 = NULL;
	TCHAR szBuffer[255];
	CString strInfo;

	EnumProcesses(ar,sizeof(ar), &cbNeeded);	// ar 배열에 process들의 id 값 들이 생성된다.
	int nProcesses = cbNeeded / sizeof(DWORD);	// 프로세스의 갯수

	for (int i = 0; i<nProcesses; i++)
	{
		processID = ar[i];
		// 2. For문을 돌려서 OpenProcess 제2의 핸들을 얻는다.
		h2 = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, processID);
		// h2 = OpenProcess(PROCESS_TERMINATE|PROCESS_VM_READ, FALSE, processID);
		if (h2 == NULL)
		{
			//strInfo.Format(_T("[PID:%d, Error code:%d]"), processID, ::GetLastError());
			//m_ctrl_ListBox.AddString(strInfo);
			continue;
		}
		else
		{
			// 3. 그것들을 프로그램 정보(ex)프로그램명,PID)열거한다.
			memset(szBuffer, 0, sizeof(szBuffer));
			GetModuleFileNameEx(h2, NULL, szBuffer, sizeof(szBuffer));
			strInfo.Format(_T("[PID:%d] %s"), processID, szBuffer);

			// 4. 정보를 리스트박스에 추가한다.
			m_ctrl_ListBox.AddString(strInfo);

			// 5. 핸들을 닫는다.
			CloseHandle(h2);
		}
	}
}


void CEnumProcessDlg::OnBnClickedButton2()
{
	// 1. 사용자가 입력한 Process ID를 읽어온다.
	UpdateData(1);// 컨트롤 --> 변수

	// 2. 그 ID로 제2의 핸들을 구한다.
	HANDLE h2 = OpenProcess(PROCESS_TERMINATE, FALSE, m_dwPID);
	if (h2 != NULL)
	{
		// 3. 그 핸들로 프로세스를 죽인다.
		TerminateProcess(h2, 0);

		// 4. 핸들을 닫는다.
		CloseHandle(h2);
	}
}
