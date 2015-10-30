// ehConfigToolDoc.cpp : CehConfigToolDoc 类的实现
//

#include "stdafx.h"
#include "ehConfigTool.h"

#include "ehConfigToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CehConfigToolDoc

IMPLEMENT_DYNCREATE(CehConfigToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CehConfigToolDoc, CDocument)
END_MESSAGE_MAP()


// CehConfigToolDoc 构造/析构

CehConfigToolDoc::CehConfigToolDoc()
{
	// TODO: 在此添加一次性构造代码

}

CehConfigToolDoc::~CehConfigToolDoc()
{
}

BOOL CehConfigToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CehConfigToolDoc 序列化

void CehConfigToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CehConfigToolDoc 诊断

#ifdef _DEBUG
void CehConfigToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CehConfigToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CehConfigToolDoc 命令

void CehConfigToolDoc::SetTitle(LPCTSTR lpszTitle)
{
    // TODO: 在此添加专用代码和/或调用基类

    CDocument::SetTitle(lpszTitle);
}
