// ClientDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

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


// CClientDlg �Ի���




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


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
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

	CRect rect3;

	list = (CListCtrl*)GetDlgItem(IDC_LIST_IMG);
	list->ModifyStyle(0, LVS_REPORT);
	list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_GRIDLINES  | LVS_EX_CHECKBOXES);
	list->InsertColumn(0,_T("�ļ���"));  
	list->InsertColumn(1,_T("·��"));

	list->GetClientRect(rect3);
	list->SetColumnWidth(0,rect3.Width()/2);
	list->SetColumnWidth(1,rect3.Width()/2);
	//��ֱ������
	list->EnsureVisible(list->GetItemCount()-1,FALSE);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�����ť����ʵ��
void CClientDlg::OnBnClickedButtonDispaly()
{
	//CFileDialog mFileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,_T("�����ļ�(*.*)|*.*"), NULL);
	CFileDialog mFileDlg(TRUE,_T("jpg"),_T("*.jpg"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("JPEG�ļ�(*.jpg)"),this);

	if(mFileDlg.DoModal()==IDOK)
	{
		CString strPathName=mFileDlg.GetPathName();
		//·��
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
	//����list��ѡ���ȷ��ѡ�ȡ����Ӧ��ID��ͼƬָ��·��
	CString strPath ;
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


void CClientDlg::SendFile()
{
	#define PORT 4000
	AfxSocketInit(NULL);
	CSocket Clientsock;
	Clientsock.Create();
	Clientsock.Connect(_T("127.0.0.1"),PORT);

	int i=0;
	//����list��ѡ���ȷ��ѡ�ȡ����Ӧ��ID��ͼƬָ��·��
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
         MessageBox(_T("�ļ�������"));  
         return ;  
     }  
	UINT nSize = 0;  
    UINT nSend = 0;  
  
    char *szBuff=NULL;  
    //����  
    while(nSize<FindFileData.nFileSizeLow)  
    {  
            szBuff = new char[1024];  
            memset(szBuff,0x00,1024);  
            nSend = file.Read(szBuff,1024);  
            Clientsock.Send(szBuff,nSend);//��������  
            nSize += nSend;  
    }  

	//���շ��������ͻ���������(�÷���������, �ڴ˵ȴ������ݽ��յ��ż�������ִ��)
	//aSocket.Receive((void *)szRecValue, 1024);

    file.Close();  
    delete szBuff;  
    Clientsock.Close();  
	MessageBox(_T("����ͼƬ�ɹ�"));
}
void CClientDlg::OnBnClickedButtonSend()
{
	SendFile();
}
