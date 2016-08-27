// ClientDlg.cpp : 实现文件

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CClientDlg 对话框




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DISPALY, &CClientDlg::OnBnClickedButtonDispaly)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CClientDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDOK, &CClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClientDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
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

	CRect rect3;

	list = (CListCtrl*)GetDlgItem(IDC_LIST_IMG);
	list->ModifyStyle(0, LVS_REPORT);
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_GRIDLINES  | LVS_EX_CHECKBOXES);
	list->InsertColumn(0,_T("文件名"));  
	list->InsertColumn(1,_T("路径"));

	list->GetClientRect(rect3);
	list->SetColumnWidth(0,rect3.Width()/2);
	list->SetColumnWidth(1,rect3.Width()/2);
	//垂直滚动条
	list->EnsureVisible(list->GetItemCount()-1,FALSE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//浏览按钮功能实现
void CClientDlg::OnBnClickedButtonDispaly()
{
	//CFileDialog mFileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,_T("所有文件(*.*)|*.*"), NULL);
	CFileDialog mFileDlg(TRUE,_T("jpg"),_T("*.jpg"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("JPEG文件(*.jpg)"),this);

	if(mFileDlg.DoModal()==IDOK)
	{
		CString strPathName=mFileDlg.GetPathName();
		//路径
		CString strDir = strPathName.Left(strPathName.ReverseFind('\\'));
		//MessageBox(strDir);
		RecursiveFindFile(strDir);
	    int i,j;
	    for(i=0; i<list->GetItemCount()-1;i++)
	    for(j=i+1; j<list->GetItemCount();j++){
		   if(list->GetItemText(i,0) == list->GetItemText(j,0)){
			   list->DeleteItem(j);
		   }
	   }
	}
	

}

void CClientDlg::RecursiveFindFile(CString& strPath)
{
	CFileFind ff;
	CString strPath1 = strPath + _T("/*.jpg");
	BOOL bFind = ff.FindFile(strPath1);
	int i=0;

   while(bFind)
   {
      bFind = ff.FindNextFile();
      if (ff.IsDots() || ff.IsSystem() || ff.IsHidden())
         continue;
       
      if (ff.IsDirectory())
      {
         RecursiveFindFile(ff.GetFilePath());
      }
      else
      {
		  CString strFileName = ff.GetFileName();
		  int nRow = list->InsertItem(i,strFileName);
		  list->SetItemText(nRow,1,strPath+_T("\\")+strFileName);
		  i++; 
      }
   }

}

void CClientDlg::OnBnClickedButtonDelete()
{
	int i=0;
	for(i=0; i<list->GetItemCount();i++)
	{
		if(list->GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED||list->GetCheck(i))
		{
			list->DeleteItem(i);
			i--;
		}
	}
}

void CClientDlg::OnBnClickedOk()
{

	((CStatic*)GetDlgItem(IDC_PICTURE))-> ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);
	int i=0;
	//遍历list复选框的确认选项，取出对应的ID的图片指定路径
	CString strPath ;
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


void CClientDlg::SendFile()
{
	#define PORT 4000
	AfxSocketInit(NULL);
	CSocket Clientsock;
	Clientsock.Create();
	Clientsock.Connect(_T("127.0.0.1"),PORT);

	int i=0;
	//遍历list复选框的确认选项，取出对应的ID的图片指定路径
	CString strPathName ;
	for(i=0; i<list->GetItemCount();i++)
	{
		if(list->GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED||list->GetCheck(i))
		{
			
			strPathName = list->GetItemText(i,1);
		}
	}

	WIN32_FIND_DATA FindFileData; 
	FindClose(FindFirstFile(strPathName,&FindFileData));
	Clientsock.Send(&FindFileData,sizeof(WIN32_FIND_DATA)); 

	CFile file;  
    if(!file.Open(strPathName,CFile::modeRead|CFile::typeBinary))  
    {  
         MessageBox(_T("文件不存在"));  
         return ;  
     }  
	UINT nSize = 0;  
    UINT nSend = 0;  
  
    char *szBuff=NULL;  
    //发送  
    while(nSize<FindFileData.nFileSizeLow)  
    {  
            szBuff = new char[1024];  
            memset(szBuff,0x00,1024);  
            nSend = file.Read(szBuff,1024);  
            Clientsock.Send(szBuff,nSend);//发送数据  
            nSize += nSend;  
    }  

	//接收服务器发送回来的内容(该方法会阻塞, 在此等待有内容接收到才继续向下执行)
	//aSocket.Receive((void *)szRecValue, 1024);

    file.Close();  
    delete szBuff;  
    Clientsock.Close();  
	MessageBox(_T("发送图片成功"));
}
void CClientDlg::OnBnClickedButtonSend()
{
	SendFile();
}
