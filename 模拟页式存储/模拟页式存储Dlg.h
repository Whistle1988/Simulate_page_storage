
// ģ��ҳʽ�洢Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// Cģ��ҳʽ�洢Dlg �Ի���
class Cģ��ҳʽ�洢Dlg : public CDialogEx
{
// ����
public:
	Cģ��ҳʽ�洢Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
