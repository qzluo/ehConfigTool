#ifndef __GLOBALDATA_H__
#define __GLOBALDATA_H__

class CGlobalData
{
public:
    CString m_strCurObjName;
    CTreeCtrl* m_tree;
    CListCtrl* m_list;
    BOOL m_bIsSaved;

public:
	CGlobalData() {
        m_bIsSaved = TRUE;
		m_tree = NULL;
		m_list = NULL;
	};
};

extern CGlobalData g_GlobalData;

#endif  //__GLOBALDATA_H__