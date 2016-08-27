// ServerDlg.cpp : 实现文件

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
#define PORT 4000
CWinThread *pThread = NULL;
bool m_exit = false;
int i=0;
CListCtrl* list;
unsigned int sendFile(LPVOID lParam)
{
	while(!m_exit)  
    {    
	
		AfxSocketInit(NULL);
		m_exit = false;

		CSocket Serversock;  
		CSocket Clientsock;
		
		if(!Serversock.Socket())  
		{  
			CString str;  
			str.Format(_T("Socket创建失败: %d"),GetLastError());  
			AfxMessageBox(str);  
		}  
	  
		BOOL bOptVal = TRUE;  
		int bOptLen = sizeof(BOOL);  
		Serversock.SetSockOpt(SO_REUSEADDR,(void *)&bOptVal,bOptLen,SOL_SOCKET);  
	  
	     
		if(!Serversock.Bind(PORT))  
		{  
			CString str;  
			str.Format(_T("Socket绑定失败: %d"),GetLastError());  
			AfxMessageBox(str);  
		}  
	      
		if(!Serversock.Listen(10))  
		{  
			CString str;  
			str.Format(_T("Socket监听失败: %d"),GetLastError());  
			AfxMessageBox(str);  
		}  

		
        if(!Serversock.Accept(Clientsock)) //等待接收   
        {  
            continue;  
        }  
        else  
        {  
            WIN32_FIND_DATA FileInfo;  
            Clientsock.Receive(&FileInfo,sizeof(WIN32_FIND_DATA));  
  
            CFile file;  
			file.Open(FileInfo.cFileName,CFile::modeCreate | CFile::modeWrite);  
            int length = sizeof(FileInfo.cFileName); 

			
  
            UINT nSize = 0;  
            UINT nData = 0;  
  
            char *szBuff=NULL;  
  
            while(nSize<FileInfo.nFileSizeLow)  
            {  
                szBuff = new char[1024];  
                memset(szBuff,0x00,1024);  
                nData=Clientsock.Receive(szBuff,1024);  
                file.Write(szBuff,nData); 
                nSize+=nData;
            }  
			CString strPath = _T("D:\\Visual Studio 2013\\Projects\\Server\\Server");
			int nRow = list->InsertItem(i,FileInfo.cFileName);
			list->SetItemText(nRow,1,strPath+_T("\\")+FileInfo.cFileName);
			i++; 
  
			//发送内容给客户端
			//serverSocket.Send(“Have Receive The Msg”, 50);
            delete szBuff;  
           // Serversock.Close();  
            Clientsock.Close();
            file.Close();    
            AfxMessageBox(_T("文件接收成功..."));  
        }
	}
	return 0;
}

void StopServer()
{
	m_exit = true;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
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


// CServerDlg 对话框




CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CServerDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_IDENTFIED, &CServerDlg::OnBnClickedButtonIdentfied)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//list设置
	CRect rect3;
	list = (CListCtrl*)GetDlgItem(IDC_LIST_IMG);
	list->ModifyStyle(0, LVS_REPORT);
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_GRIDLINES  | LVS_EX_CHECKBOXES);
	list->InsertColumn(0,_T("文件名"));  
	list->InsertColumn(1,_T("路径"));

	list->GetClientRect(rect3);
	list->SetColumnWidth(0,rect3.Width()/3);
	list->SetColumnWidth(1,rect3.Width()*2/3);
	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedButtonStart()
{
	pThread = new CWinThread(sendFile, (LPVOID)this);

	pThread->CreateThread(CREATE_SUSPENDED);

	pThread->ResumeThread();

}

void CServerDlg::OnBnClickedButtonClose()
{
	StopServer();
	
	pThread->SuspendThread();

	delete pThread;

	pThread = NULL;

}

void CServerDlg::OnBnClickedButtonIdentfied()
{

	((CStatic*)GetDlgItem(IDC_PICTURE))-> ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);
	int i=0;
	//遍历list复选框的确认选项，取出对应的ID的图片指定路径
	CString strPath;
	for(i=0; i<list->GetItemCount();i++)
	{
		if(list->GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED||list->GetCheck(i))
		{	
			strPath = list->GetItemText(i,1);

	//根据路径查找该路径下指定图片
	CDC *pDC=NULL;  
	pDC=GetDlgItem(IDC_PICTURE)->GetDC(); 

	CFileStatus fstatus;    
	CFile file;    
	ULONGLONG cb;    
		  
	// 打开文件并检测文件的有效性  
    if (!file.Open(strPath,CFile::modeRead))  
	 {  
		 return;  
	 }  
	 if (!file.GetStatus(strPath,fstatus))  
	 {  
		 return ;  
	 }  
	 if ((cb =fstatus.m_size)<=0)  
	 {  
		 return ;  
	 }  	  
	 // 根据文件大小分配内存空间,记得释放内存  
	 HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);    
	  if (hGlobal== NULL)   
	  {  
		  return ;  
	  }  
	  // 锁定刚才分配的内存空间  
	  LPVOID pvData = NULL;    
	  pvData = GlobalLock(hGlobal);  
	  if (pvData == NULL)    
	 {    
		 GlobalFree(hGlobal);  // 记得释放内存  
		 return ;  
	  }   
		  
	 // 将文件放到流中  
	  IStream *pStm;    
	  file.Read(pvData,(unsigned int)cb);    
	  GlobalUnlock(hGlobal);    
	  CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

	  IPicture *pPic;   
	  if(OleLoadPicture(pStm,(LONG)fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)!=S_OK)   
	  {   
		  GlobalFree(hGlobal);  // 记得释放内存  
		  return ;  
	  }
	  OLE_XSIZE_HIMETRIC hmWidth;    
	  OLE_YSIZE_HIMETRIC hmHeight;    
	  pPic->get_Width(&hmWidth);    
	  pPic->get_Height(&hmHeight);
	  RECT rtWnd;  
	  pDC->GetWindow()->GetWindowRect(&rtWnd);  
	  int iWndWidth=rtWnd.right-rtWnd.left;  
	  int iWndHeight=rtWnd.bottom-rtWnd.top; 
	  if(FAILED(pPic->Render(*pDC,0,0,iWndWidth,iWndHeight,0,hmHeight,hmWidth,-hmHeight,NULL)))    
	  {  
		  pPic->Release();  
		  GlobalFree(hGlobal);  // 记得释放内存  
		  return ;  
	  }  

	  pPic->Release();   
	  GlobalFree(hGlobal);  // 记得释放内存  
	  return ;
	  		}
	}

}