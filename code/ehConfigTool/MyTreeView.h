#include "ehDataBase.h"

#pragma once


#define TREE_ITEMDATA_ROOT    0
#define TREE_ITEMDATA_REGION  1
#define TREE_ITEMDATA_DEVICE  2
#define TREE_ITEMDATA_OPOPINT 3


// CMyTreeView 视图

class CMyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyTreeView)

private:
    HTREEITEM m_currentItem;

public:
    void UpdateView(void);    
    void AddNewItem(const char* pDesc, int iData);

    void AddDeviceToTree(HTREEITEM hNode, int iRegionId);
    void AddOpPointToTree(HTREEITEM hNode, int iDeviceId);

    HTREEITEM GetChildNode(HTREEITEM hNode, int iPos);
    void DeleteCurrentSelectedNode(void);
    void DeleteAllCurChildNodes(void);
    void DeleteCurrentChildNode(int iPos);
    void DeleteChildNode(HTREEITEM hNode, int iPos);    
    BOOL IsCurSelectHasChildNode(void);

    int GetCurrentRegionID(void);
    int GetCurrentDeviceID(void);
    int GetCurrentOpPointID(void);

    //查找当前选择的 device, 0 -- find, -1 -- not find
    int GetSelectedRegionData(int iPos, TagRegion* p_data);

    //查找当前选择的 device, 0 -- find, -1 -- not find
    int GetSelectedDeviceData(int iPos, TagDevice* p_data);

    //查找当前选择的 opPoint, 0 -- find, -1 -- not find
    int GetSelectedOpPointData(int iPos, TagOpPoint* p_data);

    int GetSelectedItemData(void);

    int UpdateCurItemText(const char* pDesc);
    int UpdateCurChildItemText(int iPos, const char* pDesc);

protected:
	CMyTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
protected:
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


