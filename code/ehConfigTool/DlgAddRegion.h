#pragma once

#define DLG_TYPE_REGION  0
#define DLG_TYPE_DEVICE  1
#define DLG_TYPE_MODIFY_REGION 2
#define DLG_TYPE_MODIFY_DEVICE 3

// CDlgAddRegion 对话框

class CDlgAddRegion : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddRegion)

public:
    int m_iDlgType;
    CString* m_pData;

public:
	CDlgAddRegion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddRegion();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDREGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
    virtual void OnOK();
public:
    virtual BOOL OnInitDialog();
};
