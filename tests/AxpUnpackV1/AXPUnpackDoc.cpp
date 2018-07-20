// AXPUnpackDoc.cpp : implementation of the CAXPUnpackDoc class
//

#include "stdafx.h"
#include "AXPUnpack.h"

#include "AXPUnpackDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackDoc

IMPLEMENT_DYNCREATE(CAXPUnpackDoc, CDocument)

BEGIN_MESSAGE_MAP(CAXPUnpackDoc, CDocument)
	//{{AFX_MSG_MAP(CAXPUnpackDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackDoc construction/destruction

CAXPUnpackDoc::CAXPUnpackDoc()
{
	// TODO: add one-time construction code here

}

CAXPUnpackDoc::~CAXPUnpackDoc()
{
}

BOOL CAXPUnpackDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	m_AxpFile.UnLoad();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackDoc serialization

void CAXPUnpackDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackDoc diagnostics

#ifdef _DEBUG
void CAXPUnpackDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAXPUnpackDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackDoc commands

BOOL CAXPUnpackDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	m_AxpFile.SetFileName(lpszPathName);
	m_AxpFile.LoadFile();

	return TRUE;
}
