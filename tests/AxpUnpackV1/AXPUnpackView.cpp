// AXPUnpackView.cpp : implementation of the CAXPUnpackView class
//

#include "stdafx.h"
#include "AXPUnpack.h"

#include "AXPUnpackDoc.h"
#include "AXPUnpackView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackView

IMPLEMENT_DYNCREATE(CAXPUnpackView, CListView)

BEGIN_MESSAGE_MAP(CAXPUnpackView, CListView)
	//{{AFX_MSG_MAP(CAXPUnpackView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND_RANGE(IDM_ITEM0, IDM_ITEM0, OnPopupMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackView construction/destruction

CAXPUnpackView::CAXPUnpackView()
{
	// TODO: add construction code here

}

CAXPUnpackView::~CAXPUnpackView()
{
}

BOOL CAXPUnpackView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackView drawing

void CAXPUnpackView::OnDraw(CDC* pDC)
{
	CAXPUnpackDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CAXPUnpackView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	CListCtrl& ListCtrl = GetListCtrl();
	ListCtrl.DeleteAllItems();
	vector<string> &FileList = ((CAXPUnpackDoc*)GetDocument())->m_AxpFile.m_FileList;
	vector<string>::size_type i, len;
	len = FileList.size();
	for (i = 0; i < len; i++)
	{
		ListCtrl.InsertItem(i, CString(FileList[i].c_str()));
	}

}

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackView diagnostics

#ifdef _DEBUG
void CAXPUnpackView::AssertValid() const
{
	CListView::AssertValid();
}

void CAXPUnpackView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CAXPUnpackDoc* CAXPUnpackView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAXPUnpackDoc)));
	return (CAXPUnpackDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAXPUnpackView message handlers

int CAXPUnpackView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CListCtrl& ListCtrl = GetListCtrl();
	
	CRect rect;
	GetClientRect(rect);
	ListCtrl.ModifyStyle(0, LVS_REPORT | LVS_NOSORTHEADER | LVS_SINGLESEL);
	ListView_SetExtendedListViewStyle(ListCtrl.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	ListCtrl.InsertColumn(0, TEXT("文件名"), LVCFMT_LEFT, rect.Width() / 5 * 3);
	ListCtrl.InsertColumn(1, TEXT("大小"), LVCFMT_LEFT, rect.Width() / 5);
	ListCtrl.InsertColumn(2, TEXT("属性"), LVCFMT_LEFT, rect.Width() / 5);
	
	m_PopupMenu.CreatePopupMenu();
	m_PopupMenu.AppendMenu(MF_STRING, IDM_ITEM0, TEXT("提取文件"));
	return 0;
}

void CAXPUnpackView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CListCtrl& ListCtrl = GetListCtrl();
	
	ListCtrl.SetColumnWidth(0,  cx / 5 * 3);
	ListCtrl.SetColumnWidth(1,  cx / 5);
	ListCtrl.SetColumnWidth(2,  cx / 5);
}

void CAXPUnpackView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListView::OnRButtonDown(nFlags, point);
	if (GetListCtrl().GetSelectedCount() != 0)
	{
		CRect rect;
		GetWindowRect(&rect); 
		m_PopupMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x + rect.left, point.y + rect.top, this, NULL);
	}
}

void CAXPUnpackView::OnPopupMenu(UINT nID)
{
	switch (nID)
	{
	case IDM_ITEM0:
		{
			TCHAR szFilter[] = TEXT("All Files (*.*)|*.*|");
			CListCtrl &ListCtrl = GetListCtrl();
			POSITION pos = ListCtrl.GetFirstSelectedItemPosition();
			if (pos != NULL)
			{
				int nItem = ListCtrl.GetNextSelectedItem(pos);
				CString strFileName = ListCtrl.GetItemText(nItem, 0);
				if (strFileName.ReverseFind('/') != -1)
				{
					strFileName = strFileName.Mid(strFileName.ReverseFind('/') + 1);
				}
				CFileDialog dlg(FALSE, NULL, strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,\
					szFilter, NULL);
				if (dlg.DoModal() == IDOK)
				{
					strFileName = dlg.GetPathName();
					if (((CAXPUnpackDoc*)GetDocument())->m_AxpFile.GenerateFile(nItem,\
						strFileName))
					{
						CString strMsg;
						strMsg.Format(TEXT("文件提取成功,保存位置[%s]!"), strFileName);
						AfxMessageBox(strMsg);
					}
				}
			}
		}
		break;	
	default:
		break;
	}
}