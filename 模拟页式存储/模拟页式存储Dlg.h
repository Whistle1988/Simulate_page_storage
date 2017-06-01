
// 模拟页式存储Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// C模拟页式存储Dlg 对话框
class C模拟页式存储Dlg : public CDialogEx
{
// 构造
public:
	C模拟页式存储Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CString m_MCount;
	CString m_PCount;
	CString m_from;
	CString m_to;
	CString m_editOrder;
	CString m_tMemory;
	CString m_tQuick;
	CString m_tinterr;
	CBrush m_bkBrush;
	
	int m_radio1;
	afx_msg void OnBnClickedstart();
	CComboBox m_comboOrder;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedstop();
	afx_msg void OnBnClickedgoon();
	afx_msg void OnBnClickedRadio1();
	int flag;
	afx_msg void OnBnClickedreset();
	afx_msg void OnBnClickedreset2();
	afx_msg void OnBnClickedsave();
	afx_msg void OnBnClickedopen();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
