// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ehConfigTool.h"

#include "MainFrm.h"
#include "MyTreeView.h"
#include "ehConfigToolView.h"

#include "ehDataBase.h"

#include "GlobalData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_COMMAND(ID_ITEM_ADD, OnAdd)
    ON_COMMAND(ID_ITEM_DELETE, OnDelete)
    ON_COMMAND(IDM_FILE_LOAD, &CMainFrame::OnFileLoad)
    ON_COMMAND(IDM_FILE_SAVE, &CMainFrame::OnFileSave)
    ON_COMMAND(IDM_FILE_NEW, &CMainFrame::OnFileNew)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (!m_wndSplitter.CreateStatic(this, 1, 2) ||
        !m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyTreeView), CSize(180, 0), pContext) ||
        !m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CehConfigToolView), CSize(0, 0), pContext))
        return FALSE;

    return TRUE;
}

void CMainFrame::OnAdd()
{
    GetListView()->OnAdd(); 
}

void CMainFrame::OnDelete()
{
    GetListView()->OnDelete(); 
}

CMyTreeView* CMainFrame::GetTreeView()
{
    return (CMyTreeView*)m_wndSplitter.GetPane(0, 0);
}

CehConfigToolView* CMainFrame::GetListView()
{
    return (CehConfigToolView*)m_wndSplitter.GetPane(0, 1);
}



void CMainFrame::OnFileLoad()
{
    // TODO: 在此添加命令处理程序代码
    //check whether save
    if (!CheckSaved())
        return;

    char szFilter[] = {"json文件(*.json)|*.json|所有文件 (*.*)|*.*||"};  

    CFileDialog dlg(TRUE,   //TRUE是创建打开文件对话框，FALSE则创建的是保存文件对话框 
        "json",                                            //默认的打开文件的类型 
        NULL,                                               //默认打开的文件名 
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,             //打开只读文件 
        szFilter);    //所有可以打开的文件类型 

    if (dlg.DoModal() == IDOK) { 
        CString strFilePath = dlg.GetPathName();
        init_Db(strFilePath.GetBuffer());
        GetTreeView()->OnInitialUpdate();
        GetListView()->UpdateList();
        g_GlobalData.m_bIsSaved = TRUE;

        GetActiveDocument()->SetTitle(strFilePath);
    }
}

void CMainFrame::OnFileSave()
{
    // TODO: 在此添加命令处理程序代码
    char szFilter[] = {"json文件(*.json)|*.json|所有文件 (*.*)|*.*||"};  

    CFileDialog dlg(FALSE, "json", NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
        szFilter, NULL);  

    if(dlg.DoModal() == IDOK)  
    {  
        save_Db(dlg.GetPathName());
        g_GlobalData.m_bIsSaved = TRUE;
    }
}

void CMainFrame::OnFileNew()
{
    // TODO: 在此添加命令处理程序代码
    if (!CheckSaved())
        return;

    init_Db();
    GetTreeView()->OnInitialUpdate();
    GetListView()->UpdateList();

    GetActiveDocument()->SetTitle("无标题");

    g_GlobalData.m_bIsSaved = TRUE;
}

int CMainFrame::CheckSaved(void)
{
    if (g_GlobalData.m_bIsSaved)
        return 1;

    int ret = 0;
    ret = MessageBox("当前配置已被修改，还未保存，是否继续？", 
        "保存提示", MB_YESNO);

    return ret == IDYES ? 1 : 0;
}
void CMainFrame::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (!CheckSaved())
        return;

    CFrameWnd::OnClose();
}
