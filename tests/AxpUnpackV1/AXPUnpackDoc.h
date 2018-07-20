// AXPUnpackDoc.h : interface of the CAXPUnpackDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXPUNPACKDOC_H__0B402988_415B_4A6C_B62A_0E3E3CFC1A12__INCLUDED_)
#define AFX_AXPUNPACKDOC_H__0B402988_415B_4A6C_B62A_0E3E3CFC1A12__INCLUDED_

#include "AXPFile.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAXPUnpackDoc : public CDocument
{
protected: // create from serialization only
	CAXPUnpackDoc();
	DECLARE_DYNCREATE(CAXPUnpackDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAXPUnpackDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	CAXPFile m_AxpFile;
	virtual ~CAXPUnpackDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAXPUnpackDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXPUNPACKDOC_H__0B402988_415B_4A6C_B62A_0E3E3CFC1A12__INCLUDED_)
