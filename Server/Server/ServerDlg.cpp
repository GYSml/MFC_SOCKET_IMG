// ServerDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
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
			str.Format(_T("Socket����ʧ��: %d"),GetLastError());  
			AfxMessageBox(str);  
		}  
	  
		BOOL bOptVal = TRUE;  
		int bOptLen = sizeof(BOOL);  
		Serversock.SetSockOpt(SO_REUSEADDR,(void *)&bOptVal,bOptLen,SOL_SOCKET);  
	  
	     
		if(!Serversock.Bind(PORT))  
		{  
			CString str;  
			str.Format(_T("Socket��ʧ��: %d"),GetLastError());  
			AfxMessageBox(str);  
		}  
	      
		if(!Serversock.Listen(10))  
		{  
			CString str;  
			str.Format(_T("Socket����ʧ��: %d"),GetLastError());  
			AfxMessageBox(str);  
		}  

		
        if(!Serversock.Accept(Clientsock)) //�ȴ�����   
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
  
			//�������ݸ��ͻ���
			//serverSocket.Send(��Have Receive The Msg��, 50);
            delete szBuff;  
           // Serversock.Close();  
            Clientsock.Close();
            file.Close();    
            AfxMessageBox(_T("�ļ����ճɹ�..."));  
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

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CServerDlg �Ի���




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


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//list����
	CRect rect3;
	list = (CListCtrl*)GetDlgItem(IDC_LIST_IMG);
	list->ModifyStyle(0, LVS_REPORT);
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_GRIDLINES  | LVS_EX_CHECKBOXES);
	list->InsertColumn(0,_T("�ļ���"));  
	list->InsertColumn(1,_T("·��"));

	list->GetClientRect(rect3);
	list->SetColumnWidth(0,rect3.Width()/3);
	list->SetColumnWidth(1,rect3.Width()*2/3);
	
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//����list��ѡ���ȷ��ѡ�ȡ����Ӧ��ID��ͼƬָ��·��
	CString strPath;
	for(i=0; i<list->GetItemCount();i++)
	{
		if(list->GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED||list->GetCheck(i))
		{	
			strPath = list->GetItemText(i,1);

	//����·�����Ҹ�·����ָ��ͼƬ
	CDC *pDC=NULL;  
	pDC=GetDlgItem(IDC_PICTURE)->GetDC(); 

	CFileStatus fstatus;    
	CFile file;    
	ULONGLONG cb;    
		  
	// ���ļ�������ļ�����Ч��  
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
	 // �����ļ���С�����ڴ�ռ�,�ǵ��ͷ��ڴ�  
	 HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);    
	  if (hGlobal== NULL)   
	  {  
		  return ;  
	  }  
	  // �����ղŷ�����ڴ�ռ�  
	  LPVOID pvData = NULL;    
	  pvData = GlobalLock(hGlobal);  
	  if (pvData == NULL)    
	 {    
		 GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		 return ;  
	  }   
		  
	 // ���ļ��ŵ�����  
	  IStream *pStm;    
	  file.Read(pvData,(unsigned int)cb);    
	  GlobalUnlock(hGlobal);    
	  CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

	  IPicture *pPic;   
	  if(OleLoadPicture(pStm,(LONG)fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)!=S_OK)   
	  {   
		  GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
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
		  GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		  return ;  
	  }  

	  pPic->Release();   
	  GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
	  return ;
	  		}
	}

}