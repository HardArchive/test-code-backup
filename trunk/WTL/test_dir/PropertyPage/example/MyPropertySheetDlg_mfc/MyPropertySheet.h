
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
	//�б������,��С���ܳ����б���ĸ߶�
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
    //��������ҳ����ĳ�ʼ��ɫ
	void SetCaptionColor(const COLORREF clrCaption);
    //�����б�ؼ��ָ��ߵĳ�ʼ��ɫ
	void SetSepratorColor(const COLORREF clrSeprator);
	//�����б�ؼ�ĳ�ѡ��ʱ�ı���ɫ
	void SetSelectedColor(const COLORREF clrSelected);

	//�����б�ؼ�����
	void SetListFont(CFont * pFont);

	
};


#endif