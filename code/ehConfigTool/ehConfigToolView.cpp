// ehConfigToolView.cpp : CehConfigToolView 类的实现
//

#include "stdafx.h"
#include "ehConfigTool.h"

#include "ehConfigToolDoc.h"
#include "ehConfigToolView.h"
#include "MyTreeView.h"
#include "MainFrm.h"
#include "GlobalData.h"

#include "DlgAddRegion.h"
#include "DlgAddOpPoint.h"
#include "ehDataBase.h"

#include "testEhDataBase.h"
#include "testEhDbManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//#define MY_DEBUG


// CehConfigToolView

IMPLEMENT_DYNCREATE(CehConfigToolView, CListView)

BEGIN_MESSAGE_MAP(CehConfigToolView, CListView)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CehConfigToolView::OnNMDblclk)
END_MESSAGE_MAP()

// CehConfigToolView 构造/析构

CehConfigToolView::CehConfigToolView()
{
	// TODO: 在此处添加构造代码

}

CehConfigToolView::~CehConfigToolView()
{
}

BOOL CehConfigToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CehConfigToolView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
    g_GlobalData.m_list = &GetListCtrl();

    CListCtrl& m_list = GetListCtrl();//得到内置的listctrl引用
    
    LONG lStyle;
    lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口风格
    lStyle &= ~LVS_TYPEMASK; //清除显示方式位
    lStyle |= LVS_REPORT; //设置报表风格
    SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle); //设置窗口风格
    DWORD dwStyle = m_list.GetExtendedStyle();
    
    //选中某行使整行高亮（只适用于报表风格的listctrl）
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
    m_list.SetExtendedStyle(dwStyle); //设置扩展风格
    m_list.SetBkColor(RGB(200, 200, 200)); //设置背景颜色
    m_list.SetTextBkColor(RGB(200, 200, 200)); //设置文本背景颜色
    m_list.SetTextColor(RGB(10, 10, 80)); //设置文本颜色        
}


// CehConfigToolView 诊断

#ifdef _DEBUG
void CehConfigToolView::AssertValid() const
{
	CListView::AssertValid();
}

void CehConfigToolView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CehConfigToolDoc* CehConfigToolView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CehConfigToolDoc)));
	return (CehConfigToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CehConfigToolView 消息处理程序

void CehConfigToolView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    // TODO: 在此添加专用代码和/或调用基类   
    UpdateList();
}

void CehConfigToolView::UpdateList(void)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hTemp = p_tree->GetSelectedItem();  
    if (!hTemp)
        return;

    CListCtrl& listCtrl = GetListCtrl();
    listCtrl.DeleteAllItems();

    while (listCtrl.DeleteColumn(0));

    int iData = p_tree->GetItemData(hTemp);

    switch (iData) {
    case TREE_ITEMDATA_ROOT:
        UpdateRootList();
        break;

    case TREE_ITEMDATA_REGION:
        UpdateRegionList();
        break;

    case TREE_ITEMDATA_DEVICE:
        UpdateDeviceList();
        break;

    case TREE_ITEMDATA_OPOPINT:
        UpdateOpPointList();
        break;

    default:
        break;
    }
}

void CehConfigToolView::UpdateRootList(void)
{
    UpdateListWithCaption(g_GlobalData.m_strCurObjName);
}

void CehConfigToolView::UpdateRegionList(void)
{
    CString strCaption;
    strCaption = g_GlobalData.m_strCurObjName + "设备";
    UpdateListWithCaption(strCaption);
}

void CehConfigToolView::UpdateDeviceList(void)
{
    CString strCaption;
    strCaption = g_GlobalData.m_strCurObjName + "操作";
    UpdateListWithCaption(strCaption);
}

void CehConfigToolView::UpdateOpPointList(void)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hTemp = p_tree->GetSelectedItem();  

    CListCtrl& listCtrl = GetListCtrl();
    listCtrl.InsertColumn( 0, "名称", LVCFMT_CENTER, 80 );
    listCtrl.InsertColumn( 1, "类型", LVCFMT_CENTER, 80 );
    listCtrl.InsertColumn( 2, "操作地址", LVCFMT_CENTER, 80 );
    listCtrl.InsertColumn( 3, "状态地址", LVCFMT_CENTER, 80 );

    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();

    int iDeviceId = pTreeView->GetCurrentDeviceID();
    CString strOpDesc = p_tree->GetItemText(hTemp);

    TagOpPoint opPointData = {0};   
    if (findOpPointDataByDescAndDeviceId(strOpDesc, 
        iDeviceId, &opPointData) == 0) {
            listCtrl.InsertItem(0, opPointData.desc);
            listCtrl.SetItemText(0, 1, opPointData.dataType == DEV_TYPE_BOOL ?
                "开关" : "整数");
            listCtrl.SetItemText(0, 2, opPointData.opAddr);
            listCtrl.SetItemText(0, 3, opPointData.statusAddr);
    }
}

void CehConfigToolView::UpdateListWithCaption(CString strCaption)
{
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hTemp = p_tree->GetSelectedItem();  

    CListCtrl& listCtrl = GetListCtrl();
    listCtrl.InsertColumn( 0, "序号", LVCFMT_CENTER, 80 );
    listCtrl.InsertColumn( 1, strCaption, LVCFMT_CENTER, 600 );

    if (hTemp) {  //显示所有子节点 
        HTREEITEM hChild = p_tree->GetChildItem(hTemp);
        while (hChild) {
            int nItem = listCtrl.GetItemCount();             
            CString strIndex;
            strIndex.Format("%d", nItem + 1);
            listCtrl.InsertItem(nItem, strIndex);
            listCtrl.SetItemText(nItem, 1, p_tree->GetItemText(hChild));
            hChild = p_tree->GetNextSiblingItem(hChild);
        }
    }
}

void CehConfigToolView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
//    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    int nItem = pNMListView->iItem;

    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hTemp = p_tree->GetSelectedItem();  
    if (!hTemp)
        return ;

    int iData = p_tree->GetItemData(hTemp);
    int ret = 0;
    switch (iData) {
        case TREE_ITEMDATA_ROOT:
            ret = ModifyRegion(nItem);
            break;

        case TREE_ITEMDATA_REGION:
            ret = ModifyDevice(nItem);
            break;

        case TREE_ITEMDATA_DEVICE:
            ret = ModifyOpPoint(nItem, MODY_TREE_TYPE_CHILD);    
            break;

        case TREE_ITEMDATA_OPOPINT:
            ret = ModifyOpPoint(nItem, MODY_TREE_TYPE_CUR);  
            break;

        default:
            break;
    }

    if (ret)
        g_GlobalData.m_bIsSaved = FALSE;
}

int CehConfigToolView::ModifyRegion(int iPos)
{
    //get current region data
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();    

    TagRegion regionData = {0};
    if (pTreeView->GetSelectedRegionData(iPos, &regionData))
        return 0;

    //get input
    CString strDesc = regionData.desc;
    CDlgAddRegion dlg;
    dlg.m_iDlgType = DLG_TYPE_MODIFY_REGION;
    dlg.m_pData = &strDesc;
    if (dlg.DoModal() == IDOK) {
        if (strcmp(regionData.desc, strDesc.GetBuffer()) == 0)
            return 0;

        //save to db
        strcpy(regionData.desc, strDesc.GetBuffer());
        if (updateTblData(EH_TBL_REGION, &regionData)) {
            MessageBox("update region failed, please check the input parameters"); 
            return 0;
        }
        else {
            //update tree and list
            pTreeView->UpdateCurChildItemText(iPos, regionData.desc);
            UpdateList();
            return 1;
        }
    }

    return 0;
}

int CehConfigToolView::ModifyDevice(int iPos)
{
    //get current device data
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();    

    TagDevice deviceData = {0};
    if (pTreeView->GetSelectedDeviceData(iPos, &deviceData))
        return 0;

    //get input
    CString strDesc = deviceData.desc;
    CDlgAddRegion dlg;
    dlg.m_iDlgType = DLG_TYPE_MODIFY_DEVICE;
    dlg.m_pData = &strDesc;
    if (dlg.DoModal() == IDOK) {
        if (strcmp(deviceData.desc, strDesc.GetBuffer()) == 0)
            return 0;

        //save to db
        strcpy(deviceData.desc, strDesc.GetBuffer());
        if (updateTblData(EH_TBL_DEVICE, &deviceData)) {
            MessageBox("update device failed, please check the input parameters"); 
            return 0;
        }
        else {
            //update tree and list
            pTreeView->UpdateCurChildItemText(iPos, deviceData.desc);
            UpdateList();
            return 1;
        }
    }

    return 0;
}

int CehConfigToolView::ModifyOpPoint(int iPos, int iTreeType)
{
    //get current op data
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();    

    TagOpPoint opPointData = {0};
    if (pTreeView->GetSelectedOpPointData(iPos, &opPointData))
        return 0;

    //info customer to input
    CDlgAddOpPoint dlg;
    dlg.m_pData = &opPointData;
    dlg.m_iType = OP_DLG_TYPE_MODIFY;
    if (dlg.DoModal() == IDOK) {
        //update to database
        if (updateTblData(EH_TBL_OPPOINT, &opPointData)) {
            MessageBox("update operation point failed, please check the input parameters");
            return 0;
        }
        else {
            //update tree and list
            if (iTreeType == MODY_TREE_TYPE_CUR)
                pTreeView->UpdateCurItemText(opPointData.desc);
            else if (iTreeType == MODY_TREE_TYPE_CHILD)
                pTreeView->UpdateCurChildItemText(iPos, opPointData.desc);

            UpdateList();

            return 1;
        }
    }  

    return 0;
}

void CehConfigToolView::OnAdd()
{    
    //check select item
    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hTemp = p_tree->GetSelectedItem();  
    if (!hTemp)
        return;

    int iData = p_tree->GetItemData(hTemp);
    int ret = 0;
    
    //get parameters from user(dialog)
    switch (iData) {
        case 0:
            ret = AddRegion();
            break;

        case 1:
            ret = AddDevice();
            break;

        case 2:
            ret = AddOpPoint();

        default:
            break;
    }

    if (ret)
        g_GlobalData.m_bIsSaved = FALSE;
}

int CehConfigToolView::AddRegion(void)
{
    CString strTemp;
    CDlgAddRegion dlg;
    dlg.m_pData = &strTemp;
    if (dlg.DoModal() == IDOK) {
        if (addRegion(strTemp.GetBuffer())) {//save to database
            MessageBox("A region with same name exist, failed  to add");
            return 0;
        }
        else {      
            //update tree and listview
            CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
            CMyTreeView* pTreeView = pMainFrame->GetTreeView();
            pTreeView->AddNewItem(strTemp.GetBuffer(), 1);

            AddListItem(strTemp);

            return 1;
        }
    }

    return 0;
}

int CehConfigToolView::AddDevice(void)
{
    CString strTemp;
    CDlgAddRegion dlg;
    dlg.m_iDlgType = DLG_TYPE_DEVICE;
    dlg.m_pData = &strTemp;
    if (dlg.DoModal() == IDOK) {
        CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
        CMyTreeView* pTreeView = pMainFrame->GetTreeView();
        int id = pTreeView->GetCurrentRegionID();

        if (addDevice(strTemp.GetBuffer(), id)) {
            MessageBox("A device with same name exist, failed  to add");
            return 0;
        }
        else {
            //update tree and listview
            pTreeView->AddNewItem(strTemp.GetBuffer(), 2);
            AddListItem(strTemp);
            return 1;
        }
    }

    return 0;
}

int CehConfigToolView::AddOpPoint(void)
{
    TagOpPoint opPointData = {0};
    CDlgAddOpPoint dlg;
    dlg.m_pData = &opPointData;
    if (dlg.DoModal() == IDOK) {
        //get device id
        CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
        CMyTreeView* pTreeView = pMainFrame->GetTreeView();
        int iDeviceID = pTreeView->GetCurrentDeviceID();

        //save to database
        if (addOpPoint(opPointData.desc, iDeviceID, opPointData.dataType, 
            opPointData.opAddr, opPointData.statusAddr)) {
            MessageBox("add operation point failed, please check the input parameters");
            return 0;
        }
        else {
            //update tree and list view
            pTreeView->AddNewItem(opPointData.desc, 3);
            AddListItem(opPointData.desc);
            return 1;
        }
    }    

    return 0;
}

void CehConfigToolView::AddListItem(CString strItemText)
{
    CListCtrl& listCtrl = GetListCtrl();

    int nItem = listCtrl.GetItemCount();             
    CString strIndex;
    strIndex.Format("%d", nItem + 1);
    listCtrl.InsertItem(nItem, strIndex);
    listCtrl.SetItemText(nItem, 1, strItemText);
}

void CehConfigToolView::OnDelete()
{    
#ifdef MY_DEBUG
    Test();
    return;
#endif


    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hTemp = p_tree->GetSelectedItem();  
    if (!hTemp)
        return ;

    int iData = p_tree->GetItemData(hTemp);
    int ret = 0;
    switch (iData) {
        case TREE_ITEMDATA_ROOT:
            ret = DeleteAllRegion();
            break;

        case TREE_ITEMDATA_REGION:
            ret = DeleteCurSelectRegion();
            break;

        case TREE_ITEMDATA_DEVICE:
            ret = DeleteCurSelectDevice();    
            break;

        case TREE_ITEMDATA_OPOPINT:
            ret = DeleteCurSelectOpPoint();  
            break;

        default:
            break;
    }

    if (ret)
        g_GlobalData.m_bIsSaved = FALSE;
        
}

int CehConfigToolView::DeleteAllRegion(void)
{
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();

    int iData = pTreeView->GetSelectedItemData();
    if (iData != TREE_ITEMDATA_ROOT) 
        return 0;

    if (pTreeView->IsCurSelectHasChildNode() == FALSE)
        return 0;

    pTreeView->DeleteAllCurChildNodes();
    CListCtrl& listCtrl = GetListCtrl();
    listCtrl.DeleteAllItems();
    return 1;
}

int CehConfigToolView::DeleteCurSelectRegion(void)
{
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();

    int iData = pTreeView->GetSelectedItemData();
    if (iData != TREE_ITEMDATA_REGION) 
        return 0;

    //find region id
    int iRegionId = pTreeView->GetCurrentRegionID();

    //delete from db
    if (deleteDataById(EH_TBL_REGION, iRegionId))
        return 0;

    //update tree and list
    pTreeView->DeleteCurrentSelectedNode();
    
    return 1;
}

int CehConfigToolView::DeleteCurSelectDevice(void)
{
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();

    int iData = pTreeView->GetSelectedItemData();
    if (iData != TREE_ITEMDATA_DEVICE) 
        return 0;

    //find device id
    int iDeviceId = pTreeView->GetCurrentDeviceID();

    //delete from db
    if (deleteDataById(EH_TBL_DEVICE, iDeviceId))
        return 0;

    //update tree and list
    pTreeView->DeleteCurrentSelectedNode();
    return 1;
}

int CehConfigToolView::DeleteCurSelectOpPoint(void)
{
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();

    int iData = pTreeView->GetSelectedItemData();
    if (iData != TREE_ITEMDATA_OPOPINT) 
        return 0;

    //find op point id
    int iOpPointId = pTreeView->GetCurrentOpPointID();

    //delete from db
    if (deleteDataById(EH_TBL_OPPOINT, iOpPointId))
        return 0;

    //update tree and list
    pTreeView->DeleteCurrentSelectedNode();
    return 1;
}

void CehConfigToolView::OnDelete2()
{
    //check select item
    //有选择才删除   
    int nItem = -1;
    CListCtrl& listCtrl = GetListCtrl();
    POSITION pos = listCtrl.GetFirstSelectedItemPosition();
    if (!pos) 
        return;
    
    nItem = listCtrl.GetNextSelectedItem(pos);

    CTreeCtrl* p_tree = g_GlobalData.m_tree;
    HTREEITEM hTemp = p_tree->GetSelectedItem();  
    if (!hTemp)
        return ;

    int iData = p_tree->GetItemData(hTemp);
    int ret = 0;
    switch (iData) {
        case TREE_ITEMDATA_ROOT:
            ret = DeleteRegion(nItem);
            break;

        case TREE_ITEMDATA_REGION:
            ret = DeleteDeviceInRegion(nItem);
            break;

        case TREE_ITEMDATA_DEVICE:
            ret = DeleteOpPoint(nItem);    
            break;

        case TREE_ITEMDATA_OPOPINT:
            ret = DeleteOpPoint(nItem);  
            break;

        default:
            break;
    }

    if (ret)
        g_GlobalData.m_bIsSaved = FALSE;
}

int CehConfigToolView::DeleteRegion(int iPos)
{
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();

    //find region id
    TagRegion regionData = {0};
    if (pTreeView->GetSelectedRegionData(iPos, &regionData))
        return 0;

    //delete from db
    if (deleteDataById(EH_TBL_REGION, regionData.id))
        return 0;

    //update tree and list
    int iData = pTreeView->GetSelectedItemData();
    if (iData == TREE_ITEMDATA_ROOT) {
        pTreeView->DeleteCurrentChildNode(iPos);
        CListCtrl& listCtrl = GetListCtrl();
        listCtrl.DeleteItem(iPos);

        return 1;
    }

    return 0;
}

int CehConfigToolView::DeleteDeviceInRegion(int iPos)
{
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();    

    //find device id
    TagDevice deviceData = {0};
    if (pTreeView->GetSelectedDeviceData(iPos, &deviceData))
        return 0;

    //delete device from db
    if (deleteDataById(EH_TBL_DEVICE, deviceData.id))
        return 0;

    //update tree and list
    int iData = pTreeView->GetSelectedItemData();
    if (iData == TREE_ITEMDATA_REGION) {
        pTreeView->DeleteCurrentChildNode(iPos);
        CListCtrl& listCtrl = GetListCtrl();
        listCtrl.DeleteItem(iPos);
        return 1;
    }

    return 0;
}

int CehConfigToolView::DeleteOpPoint(int iPos)
{
    //get op point
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    CMyTreeView* pTreeView = pMainFrame->GetTreeView();    

    TagOpPoint opPointData = {0};
    if (pTreeView->GetSelectedOpPointData(iPos, &opPointData))
        return 0;

    //delete from db
    if (deleteDataById(EH_TBL_OPPOINT, opPointData.id))
        return 0;

    //update tree and list
    int iData = pTreeView->GetSelectedItemData();
    if (iData == TREE_ITEMDATA_DEVICE) {
        pTreeView->DeleteCurrentChildNode(iPos);
        CListCtrl& listCtrl = GetListCtrl();
        listCtrl.DeleteItem(iPos);
        return 1;
    }
    else if (iData == TREE_ITEMDATA_OPOPINT) {
        pTreeView->DeleteCurrentSelectedNode();
        return 1;
    }

    return 0;
}

void CehConfigToolView::Test(void)
{
    CTestEhDataBase testInst;
    //testInst.test();

    //CTestEhDbManager ehDbManagerTester;

    //ehDbManagerTester.test();
}