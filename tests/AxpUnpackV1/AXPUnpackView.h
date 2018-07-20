// AXPUnpackView.h : interface of the CAXPUnpackView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXPUNPACKVIEW_H__8224D71F_1634_4C3E_BDF8_08ED89948627__INCLUDED_)
#define AFX_AXPUNPACKVIEW_H__8224D71F_1634_4C3E_BDF8_08ED89948627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IDM_ITEM0					3000

class CAXPUnpackView : public CListView
{
protected: // create from serialization only
	CAXPUnpackView();
	DECLARE_DYNCREATE(CAXPUnpackView)

// Attributes
public:
	CAXPUnpackDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAXPUnpackView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAXPUnpackView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAXPUnpackView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopupMenu(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMenu m_PopupMenu;
};

#ifndef _DEBUG  // debug version in AXPUnpackView.cpp
inline CAXPUnpackDoc* CAXPUnpackView::GetDocument()
   { return (CAXPUnpackDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXPUNPACKVIEW_H__8224D71F_1634_4C3E_BDF8_08ED89948627__INCLUDED_)
