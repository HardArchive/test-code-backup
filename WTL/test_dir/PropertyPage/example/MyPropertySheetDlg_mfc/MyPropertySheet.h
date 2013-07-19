
#ifndef MYPROPERTYSHEET_H
#define MYPROPERTYSHEET_H

#pragma once

#pragma warning(disable : 4996)



// CMyPropertySheet

class CMyPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet)
private:
	void InitList(void);
	void DrawCaption(CDC * pDC, const COLORREF clrCaption);
	void DrawGradientLine(CDC* pDC, COLORREF clrLine, POINT ptStart, POINT ptEnd);

public:
	CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMyPropertySheet();

protected:
	CImageList m_imgList;
	CListCtrl m_wndList;
	int  m_nSelectedItem;
	//列表的字体,大小不能超过列表项的高度
	CFont m_ftList;
	
	COLORREF m_clrTextBkSele ;
	COLORREF m_clrSeprator;
	COLORREF m_clrCaption;
	COLORREF m_clrSelected;

	LPTSTR m_szCaption;
	CRect m_rectPage;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);	

	int AddIcon(HICON icon);
    //设置属性页标题的初始颜色
	void SetCaptionColor(const COLORREF clrCaption);
    //设置列表控件分隔线的初始颜色
	void SetSepratorColor(const COLORREF clrSeprator);
	//设置列表控件某项被选择时的背景色
	void SetSelectedColor(const COLORREF clrSelected);

	//设置列表控件字体
	void SetListFont(CFont * pFont);

	
};


#endif