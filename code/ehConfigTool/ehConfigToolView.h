// ehConfigToolView.h : CehConfigToolView 类的接口
//


#pragma once

#define MODY_TREE_TYPE_CUR    1
#define MODY_TREE_TYPE_CHILD  2

class CehConfigToolDoc;

class CehConfigToolView : public CListView
{
protected: // 仅从序列化创建
	CehConfigToolView();
	DECLARE_DYNCREATE(CehConfigToolView)

// 属性
public:
	CehConfigToolDoc* GetDocument() const;

    //update list view
    void UpdateList(void);

private:

    void Test(void);

    void UpdateRootList(void);
    void UpdateRegionList(void);
    void UpdateDeviceList(void);
    void UpdateOpPointList(void);

    void UpdateListWithCaption(CString strCaption);

    //add operation, return: 0 -- not add, 1 -- add
    int AddRegion(void);
    int AddDevice(void);
    int AddOpPoint(void);    

    //delete operation, return: 0 -- not delete, 1 -- delete
    int DeleteRegion(int iPos);
    int DeleteDeviceInRegion(int iPos);
    int DeleteOpPoint(int iPos);     

    //delete operation, return: 0 -- not delete, 1 -- delete
    int DeleteAllRegion(void);
    int DeleteCurSelectRegion(void);
    int DeleteCurSelectDevice(void);
    int DeleteCurSelectOpPoint(void);
    //int DeleteOpPoint(int iPos);    

    //modify, return: 0 -- not modify, 1 -- modify
    int ModifyRegion(int iPos);
    int ModifyDevice(int iPos);
    int ModifyOpPoint(int iPos, int iTreeType);

    void AddListItem(CString strItemText);

// 操作
public:
    void OnAdd();
    void OnDelete();
    void OnDelete2();

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CehConfigToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
    afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // ehConfigToolView.cpp 中的调试版本
inline CehConfigToolDoc* CehConfigToolView::GetDocument() const
   { return reinterpret_cast<CehConfigToolDoc*>(m_pDocument); }
#endif

