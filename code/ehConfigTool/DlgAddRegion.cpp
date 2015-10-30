// DlgAddRegion.cpp : 实现文件
//

#include "stdafx.h"
#include "ehConfigTool.h"
#include "DlgAddRegion.h"


// CDlgAddRegion 对话框

IMPLEMENT_DYNAMIC(CDlgAddRegion, CDialog)

CDlgAddRegion::CDlgAddRegion(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddRegion::IDD, pParent)
{
    m_pData = NULL;
}

CDlgAddRegion::~CDlgAddRegion()
{
}

void CDlgAddRegion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddRegion, CDialog)
END_MESSAGE_MAP()


// CDlgAddRegion 消息处理程序

void CDlgAddRegion::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    GetDlgItemText(IDC_EDIT1, *m_pData);

    CDialog::OnOK();
}

BOOL CDlgAddRegion::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    if (m_iDlgType == DLG_TYPE_DEVICE)
        SetWindowText("添加设备");
    else if (m_iDlgType == DLG_TYPE_MODIFY_REGION)
        SetWindowText("修改区域");
    else if (m_iDlgType == DLG_TYPE_MODIFY_DEVICE)
        SetWindowText("修改设备");

    SetDlgItemText(IDC_EDIT1, *m_pData);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
