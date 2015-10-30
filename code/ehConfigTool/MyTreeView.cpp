// MyTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "ehConfigTool.h"
#include "MyTreeView.h"
#include "ehConfigToolDoc.h"
#include "GlobalData.h"




// CMyTreeView

IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

CMyTreeView::CMyTreeView()
{

}

CMyTreeView::~CMyTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMyTreeView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CMyTreeView 诊断

#ifdef _DEBUG
void CMyTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyTreeView 消息处理程序

void CMyTreeView::OnInitialUpdate()
{
    CTreeView::OnInitialUpdate();

    // TODO: 在此添加专用代码和/或调用基类
    g_GlobalData.m_tree = &GetTreeCtrl();
    CTreeCtrl* p_tree = g_GlobalData.m_tree;

    p_tree->ModifyStyle(0L, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);    

    g_GlobalData.m_strCurObjName = _T("区域");
    
    UpdateView();
    
    m_currentItem = p_tree->GetRootItem();
    p_tree->SelectItem(m_currentItem);
}

void CMyTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;

    m_currentItem = GetTreeCtrl().GetSelectedItem();
    if (m_currentItem)
        g_GlobalData.m_strCurObjName = GetTreeCtrl().GetItemText(m_currentItem);    

    CehConfigToolDoc* pDoc = (CehConfigToolDoc*)GetDocument();

    pDoc->UpdateAllViews(this);
}

void CMyTreeView::UpdateView(void)
{
    int iRegionCount = 0;
    TagRegion region = {0};
    HTREEITEM hTemp;

    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    p_tree->DeleteAllItems();

    HTREEITEM hRegionRoot = p_tree->InsertItem(_T("区域"), TVI_ROOT);
    p_tree->SetItemData(hRegionRoot, 0);    

    iRegionCount = getTblRecordCount(EH_TBL_REGION);
    for (int i = 0; i < iRegionCount; i++) {
        fetchRecordData(EH_TBL_REGION, i, &region);
        hTemp = p_tree->InsertItem(region.desc, hRegionRoot);

        p_tree->SetItemData(hTemp, 1);  
        
        //addDevice to tree
        AddDeviceToTree(hTemp, region.id);
    }
}

void CMyTreeView::AddDeviceToTree(HTREEITEM hNode, int iRegionId)
{
    int iDeviceCount = 0;
    TagDevice deviceData = {0};
    HTREEITEM hTemp;
    CTreeCtrl* p_tree = g_GlobalData.m_tree;

    iDeviceCount = getTblRecordCount(EH_TBL_DEVICE);
    for (int i = 0; i < iDeviceCount; i++) {
        fetchRecordData(EH_TBL_DEVICE, i, &deviceData);
        if (deviceData.regionId == iRegionId) {
            hTemp = p_tree->InsertItem(deviceData.desc, hNode);
            //addDevice to tree
            p_tree->SetItemData(hTemp, 2); 

            //add operation point to tree
            AddOpPointToTree(hTemp, deviceData.id);
        }
    }
}

void CMyTreeView::AddOpPointToTree(HTREEITEM hNode, int iDeviceId)
{
    int iOpPointCount = 0;
    TagOpPoint opPointData = {0};
    HTREEITEM hTemp;
    CTreeCtrl* p_tree = g_GlobalData.m_tree;

    iOpPointCount = getTblRecordCount(EH_TBL_OPPOINT);
    for (int i = 0; i < iOpPointCount; i++) {
        fetchRecordData(EH_TBL_OPPOINT, i, &opPointData);
        if (opPointData.deviceId == iDeviceId) {
            hTemp = p_tree->InsertItem(opPointData.desc, hNode);
            //add oppoint to tree
            p_tree->SetItemData(hTemp, TREE_ITEMDATA_OPOPINT); 
        }
    }
}

void CMyTreeView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    // TODO: 在此添加专用代码和/或调用基类
}

void CMyTreeView::AddNewItem(const char* pDesc, int iData)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;

    if (m_currentItem) {
        HTREEITEM hTemp = p_tree->InsertItem(pDesc, m_currentItem);
        p_tree->SetItemData(hTemp, iData); 
    }
}

int CMyTreeView::GetCurrentRegionID(void)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    int iCurrentData = p_tree->GetItemData(m_currentItem);
    if (iCurrentData <= TREE_ITEMDATA_ROOT)
        return -1;

    HTREEITEM hTemp = NULL;
    if (iCurrentData == TREE_ITEMDATA_REGION)
        hTemp = m_currentItem;
    else if (iCurrentData == TREE_ITEMDATA_DEVICE)
        hTemp = p_tree->GetParentItem(m_currentItem);
    else if (iCurrentData == TREE_ITEMDATA_OPOPINT) {
        hTemp = p_tree->GetParentItem(m_currentItem);
        hTemp = p_tree->GetParentItem(hTemp);
    }

    if (!hTemp)
        return -1;

    CString strRegionDesc = p_tree->GetItemText(hTemp);
    TagRegion regionData = {0};
    if (findDataByDesc(EH_TBL_REGION, 
        strRegionDesc.GetBuffer(), &regionData))
        return -1;

    return regionData.id;
}

int CMyTreeView::GetCurrentDeviceID(void)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    int iCurrentData = p_tree->GetItemData(m_currentItem);
    if (iCurrentData <= TREE_ITEMDATA_REGION)
        return -1;

    HTREEITEM hTemp = NULL;
    if (iCurrentData == TREE_ITEMDATA_OPOPINT)
        hTemp = p_tree->GetParentItem(m_currentItem);
    else if (iCurrentData == TREE_ITEMDATA_DEVICE)
        hTemp = m_currentItem;

    if (!hTemp)
        return -1;

    CString strDeviceDesc = p_tree->GetItemText(hTemp);
    int iRegionId = GetCurrentRegionID();

    TagDevice deviceData = {0};
    if (findDeviceDataByDescAndRegionId(strDeviceDesc.GetBuffer(), 
        iRegionId, &deviceData))
        return -1;

    return deviceData.id;
}

int CMyTreeView::GetCurrentOpPointID(void)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    int iCurrentData = p_tree->GetItemData(m_currentItem);
    if (iCurrentData <= TREE_ITEMDATA_DEVICE)
        return -1;

    HTREEITEM hTemp = m_currentItem;

    if (!hTemp)
        return -1;

    CString strDeviceDesc = p_tree->GetItemText(m_currentItem);
    int iDeviceId = GetCurrentDeviceID();

    TagOpPoint opPointData = {0};
    if (findOpPointDataByDescAndDeviceId(strDeviceDesc.GetBuffer(), 
        iDeviceId, &opPointData))
        return -1;

    return opPointData.id;
}

int CMyTreeView::GetSelectedRegionData(int iPos, TagRegion* p_data)
{
    CString strDesc;
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return -1;

    int iData = p_tree->GetItemData(hSelectedTreeItem);
    if (iData < TREE_ITEMDATA_ROOT)
        return -1;

    if (iData == TREE_ITEMDATA_ROOT) {
        HTREEITEM hTemp = GetChildNode(hSelectedTreeItem, iPos);
        if (!hTemp)
            return -1;

        strDesc = p_tree->GetItemText(hTemp);
    }

    return findDataByDesc(EH_TBL_REGION,
        strDesc.GetBuffer(), p_data);
}

int CMyTreeView::GetSelectedDeviceData(int iPos, TagDevice* p_data)
{
    //get region id
    int iRegionID = GetCurrentRegionID();

    //get device description
    CString strDesc;
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return -1;

    int iData = p_tree->GetItemData(hSelectedTreeItem);
    if (iData < TREE_ITEMDATA_REGION)
        return -1;

    if (iData == TREE_ITEMDATA_REGION) {
        HTREEITEM hTemp = GetChildNode(hSelectedTreeItem, iPos);
        if (!hTemp)
            return -1;

        strDesc = p_tree->GetItemText(hTemp);
    }

    //find device data
    return findDeviceDataByDescAndRegionId(strDesc.GetBuffer(), 
        iRegionID, p_data);
}

int CMyTreeView::GetSelectedOpPointData(int iPos, TagOpPoint* p_data)
{
    //get device id
    int iDeviceID = GetCurrentDeviceID();

    //get operation description
    CString strDesc;
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return -1;

    int iData = p_tree->GetItemData(hSelectedTreeItem);
    if (iData < TREE_ITEMDATA_DEVICE)
        return -1;

    if (iData == TREE_ITEMDATA_DEVICE) {
        HTREEITEM hTemp = GetChildNode(hSelectedTreeItem, iPos);
        if (!hTemp)
            return -1;

        strDesc = p_tree->GetItemText(hTemp);
    }
    else if (iData == TREE_ITEMDATA_OPOPINT) 
        strDesc = p_tree->GetItemText(hSelectedTreeItem);

    //find op data

    return findOpPointDataByDescAndDeviceId(strDesc.GetBuffer(), 
        iDeviceID, p_data);
}

int CMyTreeView::GetSelectedItemData(void)
{
    int iRet = -1;
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return -1;

    iRet = p_tree->GetItemData(hSelectedTreeItem);
    return iRet;
}

HTREEITEM CMyTreeView::GetChildNode(HTREEITEM hNode, int iPos)
{
    HTREEITEM hRet = NULL;
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    if (!hNode)
        return NULL;
    
    int iIndex = 0;
    HTREEITEM hChild = p_tree->GetChildItem(hNode);
    while (hChild) {
        if (iIndex == iPos) {
            hRet = hChild;
            break;
        }

        hChild = p_tree->GetNextSiblingItem(hChild);
        iIndex++;
    }

    return hRet;
}

int CMyTreeView::UpdateCurItemText(const char* pDesc)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return -1;

    p_tree->SetItemText(hSelectedTreeItem, pDesc);

    return 0;
}

int CMyTreeView::UpdateCurChildItemText(int iPos, const char* pDesc)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return -1;

    int iIndex = 0;
    HTREEITEM hChild = p_tree->GetChildItem(hSelectedTreeItem);
    while (hChild) {
        if (iIndex == iPos) {
            p_tree->SetItemText(hChild, pDesc);
            return 0;
        }

        hChild = p_tree->GetNextSiblingItem(hChild);
        iIndex++;
    }

    return -1;
}

void CMyTreeView::DeleteCurrentSelectedNode(void)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return ;

    HTREEITEM hTemp = p_tree->GetParentItem(hSelectedTreeItem);
    p_tree->DeleteItem(hSelectedTreeItem);
    p_tree->SelectItem(hTemp);
}

void CMyTreeView::DeleteAllCurChildNodes(void)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return ;

    HTREEITEM hChild = NULL;
    while (hChild = p_tree->GetChildItem(hSelectedTreeItem))
        p_tree->DeleteItem(hChild);
}

void CMyTreeView::DeleteCurrentChildNode(int iPos)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return ;

    DeleteChildNode(hSelectedTreeItem, iPos);
}

void CMyTreeView::DeleteChildNode(HTREEITEM hNode, int iPos)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    if (!hNode)
        return ;
    
    int iIndex = 0;
    HTREEITEM hChild = p_tree->GetChildItem(hNode);
    while (hChild) {
        if (iIndex == iPos) {
            p_tree->DeleteItem(hChild);
            return ;
        }

        hChild = p_tree->GetNextSiblingItem(hChild);
        iIndex++;
    }
}

BOOL CMyTreeView::IsCurSelectHasChildNode(void)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hSelectedTreeItem = p_tree->GetSelectedItem();  
    if (!hSelectedTreeItem)
        return FALSE;

    return p_tree->GetChildItem(hSelectedTreeItem) ? TRUE : FALSE;
}