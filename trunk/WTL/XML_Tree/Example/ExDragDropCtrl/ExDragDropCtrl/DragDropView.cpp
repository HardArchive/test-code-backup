// DragDropView.cpp : implementation file
//

#include "stdafx.h"
#include "dragdrop.h"
#include "DragDropView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragDropView

IMPLEMENT_DYNCREATE(CDragDropView, CFormView)

CDragDropView::CDragDropView()
	: CFormView(CDragDropView::IDD)
{
	//{{AFX_DATA_INIT(CDragDropView)
	//}}AFX_DATA_INIT
	m_imgList.Create(IDB_SCENE_TREE,16,1,RGB(255,255,255));
}

CDragDropView::~CDragDropView()
{
}

void CDragDropView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDragDropView)
	DDX_Control(pDX, IDC_BTN_UP, m_fbtnUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_fbtnDown);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDragDropView, CFormView)
	//{{AFX_MSG_MAP(CDragDropView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnTreeSelChanged)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_NEWFOLDER, OnBtnNewFolder)
	ON_BN_CLICKED(IDC_BTN_RENAME, OnBtnRename)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDragDropView message handlers

void CDragDropView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
    m_Tree.SetImageList(&m_imgList,TVSIL_NORMAL);
    m_Tree.SetNoDuplicates(TRUE);

    // now that the tree exists, force resize

    RECT rect;
    GetWindowRect(&rect);
    OnSize(SIZE_RESTORED,rect.right-rect.left,rect.bottom-rect.top);

    // populate tree

    HTREEITEM hScene  = m_Tree.InsertItem("scene",     FOLDER_IMAGE, FOLDER_IMAGE);
    HTREEITEM hCams   = m_Tree.InsertItem("cameras",   FOLDER_IMAGE, FOLDER_IMAGE, hScene);
    HTREEITEM hLights = m_Tree.InsertItem("lights",    FOLDER_IMAGE, FOLDER_IMAGE, hScene);
    HTREEITEM hMeshes = m_Tree.InsertItem("meshes",    9,9,hScene);
    HTREEITEM hChrs   = m_Tree.InsertItem("characters",FOLDER_IMAGE, FOLDER_IMAGE, hMeshes);
    HTREEITEM hBlocks = m_Tree.InsertItem("blocks",    FOLDER_IMAGE, FOLDER_IMAGE, hMeshes);
    HTREEITEM hHouses = m_Tree.InsertItem("buildings", FOLDER_IMAGE, FOLDER_IMAGE, hMeshes);
    CString strTmp;

    for(int i = 0; i < 10; i++)
    {
        strTmp.Format("camera %u",i+1);
        m_Tree.InsertItem(LPCTSTR(strTmp),2,2,hCams);
        strTmp.Format("light %u",i+1);
        m_Tree.InsertItem(LPCTSTR(strTmp),5,5,hLights);
        strTmp.Format("character %u",i+1);
        m_Tree.InsertItem(LPCTSTR(strTmp),12,12,hChrs);
        strTmp.Format("block %u",i+1);
        m_Tree.InsertItem(LPCTSTR(strTmp),9,9,hBlocks);
        strTmp.Format("house %u",i+1);
        m_Tree.InsertItem(LPCTSTR(strTmp),10,10,hHouses);
    }

    //  m_Tree.Expand(hHouses,TVE_EXPAND);
    //  m_Tree.Expand(hBlocks,TVE_EXPAND);
    m_Tree.Expand(hChrs,  TVE_EXPAND);
    m_Tree.Expand(hMeshes,TVE_EXPAND);
    m_Tree.Expand(hLights,TVE_EXPAND);
    m_Tree.Expand(hCams,  TVE_EXPAND);
    m_Tree.Expand(hScene, TVE_EXPAND);

    // Set the state of the Up and Down buttons
    m_fbtnUp.EnableWindow(m_Tree.ShouldUpBtnBeEnabled());
    m_fbtnDown.EnableWindow(m_Tree.ShouldDownBtnBeEnabled());
}

//

void CDragDropView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
  // resize tree

  if(m_Tree.m_hWnd)
  {                                                         // otherwise not created yet
    RECT rect;
    GetClientRect(&rect);
    #define BORDER 55
    int iSizeX,iSizeY;
    iSizeX = rect.right-rect.left-BORDER*2;
    iSizeY = rect.bottom-rect.top-BORDER*2;
    if((iSizeX > 0) && (iSizeY > 0)) m_Tree.SetWindowPos(NULL,BORDER,BORDER,iSizeX,iSizeY,SWP_NOZORDER);
    #undef BORDER
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Move tree item one place up
void CDragDropView::OnBtnUp() 
{
	m_Tree.MoveItemUp();
    m_Tree.SetFocus();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Move tree item one place down
void CDragDropView::OnBtnDown() 
{
	m_Tree.MoveItemDown();
    m_Tree.SetFocus();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Tree selection has changed, so check status of Up and Down buttons
void CDragDropView::OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
    UNREFERENCED_PARAMETER(pNMHDR);
    UNREFERENCED_PARAMETER(pResult);
    m_fbtnUp.EnableWindow(m_Tree.ShouldUpBtnBeEnabled());
    m_fbtnDown.EnableWindow(m_Tree.ShouldDownBtnBeEnabled());
}

void CDragDropView::OnBtnNewFolder() 
{
	m_Tree.CreateNewFolder();
}

void CDragDropView::OnBtnRename() 
{
	m_Tree.RenameSelected();
}

void CDragDropView::OnBtnDelete() 
{
	m_Tree.DeleteSelected();
}
