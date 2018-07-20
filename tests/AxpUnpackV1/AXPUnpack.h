// AXPUnpack.h : main header file for the AXPUNPACK application
//

#if !defined(AFX_AXPUNPACK_H__150FF2EA_953E_40DA_861B_874073DD01EF__INCLUDED_)
#define AFX_AXPUNPACK_H__150FF2EA_953E_40DA_861B_874073DD01EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackApp:
// See AXPUnpack.cpp for the implementation of this class
//

class CAXPUnpackApp : public CWinApp
{
public:
	CAXPUnpackApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAXPUnpackApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAXPUnpackApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXPUNPACK_H__150FF2EA_953E_40DA_861B_874073DD01EF__INCLUDED_)
