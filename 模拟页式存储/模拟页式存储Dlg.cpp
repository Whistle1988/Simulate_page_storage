// ģ��ҳʽ�洢Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ģ��ҳʽ�洢.h"
#include "ģ��ҳʽ�洢Dlg.h"
#include "afxdialogex.h"
#include"Thread.h"
#include"structAndClass.h"
#include<iostream>
#include<fstream>
#include"draw.h"
#include"resource.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SLEEP 500

room fiforoom;
room lruroom;
room lfuroom;
room optroom;

int order[50];

HANDLE hThreadfifo;
HANDLE hThreadlru;
HANDLE hThreadlfu;
HANDLE hThreadopt;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cģ��ҳʽ�洢Dlg �Ի���



Cģ��ҳʽ�洢Dlg::Cģ��ҳʽ�洢Dlg(CWnd* pParent /*=NULL*/)
: CDialogEx(Cģ��ҳʽ�洢Dlg::IDD, pParent)
, m_MCount(_T(""))
, m_PCount(_T(""))
, m_from(_T(""))
, m_to(_T(""))
, m_editOrder(_T(""))
, m_tMemory(_T(""))
, m_tQuick(_T(""))
, m_tinterr(_T(""))
, m_radio1(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void Cģ��ҳʽ�洢Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_MCount);
	DDX_Text(pDX, IDC_EDIT2, m_PCount);
	DDX_Text(pDX, IDC_EDIT3, m_from);
	DDX_Text(pDX, IDC_EDIT4, m_to);
	DDX_Text(pDX, IDC_EDIT5, m_editOrder);
	DDX_Text(pDX, IDC_EDIT6, m_tMemory);
	DDX_Text(pDX, IDC_EDIT8, m_tQuick);
	DDX_Text(pDX, IDC_EDIT7, m_tinterr);
	DDX_Radio(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_COMBO1, m_comboOrder);
}

BEGIN_MESSAGE_MAP(Cģ��ҳʽ�洢Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_start, &Cģ��ҳʽ�洢Dlg::OnBnClickedstart)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Cģ��ҳʽ�洢Dlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_stop, &Cģ��ҳʽ�洢Dlg::OnBnClickedstop)
	ON_BN_CLICKED(IDC_goon, &Cģ��ҳʽ�洢Dlg::OnBnClickedgoon)
	ON_BN_CLICKED(IDC_RADIO1, &Cģ��ҳʽ�洢Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_reset, &Cģ��ҳʽ�洢Dlg::OnBnClickedreset)
	ON_BN_CLICKED(IDC_reset2, &Cģ��ҳʽ�洢Dlg::OnBnClickedreset2)
	ON_BN_CLICKED(IDC_save, &Cģ��ҳʽ�洢Dlg::OnBnClickedsave)
	ON_BN_CLICKED(IDC_open, &Cģ��ҳʽ�洢Dlg::OnBnClickedopen)
	ON_BN_CLICKED(IDC_BUTTON1, &Cģ��ҳʽ�洢Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &Cģ��ҳʽ�洢Dlg::OnBnClickedButton7)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


DWORD WINAPI ThreadFifo(LPVOID GetParameter)
{
	para *p = (para*)GetParameter;
	CListBox *pOut = (CListBox *)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST1));
	int d = 0;
	int memory_num = 0;//�ڴ����ҳ�����
	int t_numfifo = 0;//��ʱ��
	int t_pnum;//����ҳ��ʱ��
	int lack_numfifo = 0;//ȱҳ����
	int rate_fifo;//ȱҳ��
	int k[2] = { NULL, NULL };//���
	struct mem c[100];//�ڴ�
	order[p->page_count];
	for (int i = 0; i < p->page_count; i++)
	{
		t_pnum = 0;
		bool hit = false;
		bool khit = false;
		if (p->flag1 == 0)//flag1Ϊ0ʱ �п�� Ϊ1ʱ��
		{
			for (int j = 0; j < 2; j++)
			{
				if (order[i] == k[j])   //�������
				{
					hit = true;
					khit = true;
					t_pnum += p->t_quick;
				}
			}
		}
		if (!khit)//���û����
		{
			for (int j = 0; j < p->memory_count; j++)
			{
				if (order[i] == c[j].page)//���û���У��ڴ�����
				{
					hit = true;
					if (p->flag1 == 0)
					{
						if (k[1] == NULL)//������
						{
							if (k[0] == NULL)
								k[0] = order[i];
							else
								k[1] = order[i];
						}
						else
						{
							k[0] = k[1];
							k[1] = order[i];
						}
						t_pnum += p->t_quick;
					}
				}
			}
			t_pnum += p->t_memory;
			if (!hit)//�ڴ�δ����
			{
				if (memory_num < p->memory_count)//�ڴ�δ��
				{
					c[memory_num].page = order[i];
					memory_num++;
				}
				else
				{
					for (int k = 0; k < p->memory_count; k++)
					{
						c[k].page = c[k + 1].page;
					}
					c[memory_num - 1].page = order[i];
				}
				if (p->flag1 == 0)
				{
					if (k[1] == NULL)//������
					{
						if (k[0] == NULL)
							k[0] = order[i];
						else
							k[1] = order[i];
					}
					else
					{
						k[0] = k[1];
						k[1] = order[i];
					}
					t_pnum += p->t_quick;
				}
				lack_numfifo++;
				t_pnum += p->t_interr + p->t_memory;
			}
		}
		t_pnum += p->t_memory;//���ȡ�������ڴ�
		t_numfifo += t_pnum;
		CString a, b, e;
		for (int m = 0; m < memory_num; m++)
		{
			b.Format(_T("%d"), c[m].page);
			a = a + " " + b;
		}
		e.Format(_T("%s # %d time: %d"), a, order[i], t_pnum);
		pOut->InsertString(d++, e);
		CString temp1;
		temp1.Format(_T("%d"), lack_numfifo);
		fiforoom.times->SetWindowText(temp1);
		CString temp2;
		temp2.Format(_T("%d"), t_numfifo);
		fiforoom.totaltime->SetWindowText(temp2);
		rate_fifo = lack_numfifo * 100 / (i + 1);
		CString temp3;
		temp3.Format(_T("%d"), rate_fifo);
		fiforoom.percent->SetWindowText(temp3);
		Sleep(SLEEP);
	}

	return 1;
}
//������δʹ��
DWORD WINAPI ThreadLru(LPVOID GetParameter)
{
	para *p = (para*)GetParameter;
	CListBox *pOut = (CListBox *)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2)); //MFC ��ȡ�ؼ�ָ��ķ���GetDlgItem�ı�׼�÷�
	int d = 0;
	int memory_num = 0;//�ڴ����ҳ�����
	int t_numlru = 0;//��ʱ��
	int lack_numlru = 0;
	int rate_lru;
	int t_pnum;//����ҳ��ʱ��
	int k[2] = { NULL, NULL };//���
	struct mem c[100];//�ڴ�
	order[p->page_count];

	for (int i = 0; i < p->page_count; i++)
	{
		t_pnum = 0;
		bool hit = false;
		bool khit = false;
		if (p->flag1 == 0)
		{
			for (int j = 0; j < 2; j++)
			{
				if (order[i] == k[j])   //�������
				{
					hit = true;
					khit = true;
					t_pnum += p->t_quick;
					for (int k = 0; k < memory_num; k++)//�޸��ڴ��и�ҳ���flag
					{
						if (order[i] == c[k].page)
							c[k].flag = i;
					}
				}
			}
		}
		if (!khit)//���û����
		{
			for (int j = 0; j < p->memory_count; j++)
			{
				if (order[i] == c[j].page)//���û���У��ڴ�����
				{
					hit = true;
					c[j].flag = i;
					if (p->flag1 == 0)
					{
						if (k[1] == NULL)//������
						{
							if (k[0] == NULL)
								k[0] = order[i];
							else
								k[1] = order[i];
						}
						else
						{
							k[0] = k[1];
							k[1] = order[i];
						}
						t_pnum += p->t_quick;
					}
				}
			}
			t_pnum += p->t_memory;
			if (!hit)//�ڴ�δ����
			{
				if (memory_num < p->memory_count)//�ڴ�δ��
				{
					c[memory_num].page = order[i];
					c[memory_num].flag = i;
					memory_num++;
				}
				else    //�ڴ���
				{
					int temp;//���Ҫ������ҳ��������
					if (c[0].flag < c[1].flag)
						temp = 0;
					else
						temp = 1;
					for (int j = 2; j<p->memory_count; j++)
					{
						if (c[temp].flag>c[j].flag)
							temp = j;
					}
					c[temp].page = order[i];
					c[temp].flag = i;
				}
				if (p->flag1 == 0)
				{
					if (k[1] == NULL)//������
					{
						if (k[0] == NULL)
							k[0] = order[i];
						else
							k[1] = order[i];
					}
					else
					{
						k[0] = k[1];
						k[1] = order[i];
					}
					t_pnum += p->t_quick;
				}
				lack_numlru++;
				t_pnum += p->t_interr + p->t_memory;
			}
		}
		t_pnum += p->t_memory;//���ȡ�������ڴ�
		t_numlru += t_pnum;
		CString a, b, e;
		for (int m = 0; m < memory_num; m++)
		{
			b.Format(_T("%d"), c[m].page);
			a = a + " " + b;
		}
		e.Format(_T("%s # %d time: %d"), a, order[i], t_pnum);
		pOut->InsertString(d++, e);
		CString temp1;
		temp1.Format(_T("%d"), lack_numlru);
		lruroom.times->SetWindowText(temp1);
		CString temp2;
		temp2.Format(_T("%d"), t_numlru);
		lruroom.totaltime->SetWindowText(temp2);
		rate_lru = lack_numlru * 100 / (i + 1);
		CString temp3;
		temp3.Format(_T("%d"), rate_lru);
		lruroom.percent->SetWindowText(temp3);
		Sleep(SLEEP);
	}
	return 1;
}
//�������ʹ���㷨
DWORD WINAPI ThreadLfu(LPVOID GetParameter)
{
	para *p = (para*)GetParameter;
	CListBox *pOut = (CListBox *)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST3));
	int d = 0;
	int memory_num = 0;//�ڴ����ҳ�����
	int t_numlfu = 0;//��ʱ��
	int lack_numlfu = 0;
	int rate_lfu;
	int t_pnum;//����ҳ��ʱ��
	int k[2] = { NULL, NULL };//���
	struct mem c[100];//�ڴ�
	int T[50];
	order[p->page_count];
	for (int i = 0; i < p->page_count; i++)
	{
		t_pnum = 0;
		bool hit = false;
		bool khit = false;
		if (p->flag1 == 0)
		{
			for (int j = 0; j < 2; j++)
			{
				if (order[i] == k[j])   //�������
				{
					hit = true;
					khit = true;
					t_pnum += p->t_quick;
					for (int k = 0; k < memory_num; k++)//�޸��ڴ��и�ҳ���flag
					{
						if (order[i] == c[k].page)
						{
							c[k].flag++;
							T[k] = i;
						}
					}
				}
			}
		}
		if (!khit)//���û����
		{
			for (int j = 0; j < memory_num; j++)
			{
				if (order[i] == c[j].page)//���û���У��ڴ�����
				{
					hit = true;
					c[j].flag += 1;
					T[j] = i;
					if (p->flag1 == 0)
					{
						if (k[1] == NULL)//������
						{
							if (k[0] == NULL)
								k[0] = order[i];
							else
								k[1] = order[i];
						}
						else
						{
							k[0] = k[1];
							k[1] = order[i];
						}
						t_pnum += p->t_quick;
					}
				}
			}
			t_pnum += p->t_memory;
			if (!hit)//�ڴ�δ����
			{
				if (memory_num < p->memory_count)//�ڴ�δ��
				{
					c[memory_num].page = order[i];
					c[memory_num].flag = 1;
					T[memory_num] = i;
					memory_num++;
				}
				else    //�ڴ���
				{
					int temp;//���Ҫ������ҳ��������
					if (c[0].flag<c[1].flag)
						temp = 0;
					else if (c[0].flag>c[1].flag)
						temp = 1;
					else
					{
						if (T[0] < T[1])
							temp = 0;
						else
							temp = 1;
					}
					for (int j = 2; j < p->memory_count; j++)
					{
						if (c[j].flag < c[temp].flag)
							temp = j;
						else if (c[j].flag == c[temp].flag)
						{
							if (T[j] < T[temp])
								temp = j;
						}
					}
					c[temp].page = order[i];
					c[temp].flag = 1;
					T[temp] = i;
				}
				if (p->flag1 == 0)
				{
					if (k[1] == NULL)//������
					{
						if (k[0] == NULL)
							k[0] = order[i];
						else
							k[1] = order[i];
					}
					else
					{
						k[0] = k[1];
						k[1] = order[i];
					}
					t_pnum += p->t_quick;
				}
				lack_numlfu++;
				t_pnum += p->t_interr + p->t_memory;
			}
		}
		t_pnum += p->t_memory;//���ȡ������ڴ�
		t_numlfu += t_pnum;
		CString a, b, e;
		for (int m = 0; m < memory_num; m++)
		{
			b.Format(_T("%d"), c[m].page);
			a = a + " " + b;
		}
		e.Format(_T("%s # %d time: %d"), a, order[i], t_pnum);
		pOut->InsertString(d++, e);
		CString temp1;
		temp1.Format(_T("%d"), lack_numlfu);
		lfuroom.times->SetWindowText(temp1);
		CString temp2;
		temp2.Format(_T("%d"), t_numlfu);
		lfuroom.totaltime->SetWindowText(temp2);
		rate_lfu = lack_numlfu * 100 / (i + 1);
		CString temp3;
		temp3.Format(_T("%d"), rate_lfu);
		lfuroom.percent->SetWindowText(temp3);
		Sleep(SLEEP);
	}
	return 1;
}
DWORD WINAPI ThreadOpt(LPVOID GetParameter)
{
	para *p = (para*)GetParameter;
	CListBox *pOut = (CListBox *)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST4));
	int d = 0;
	int memory_num = 0;//�ڴ����ҳ�����
	int t_numopt = 0;//��ʱ��
	int lack_numopt = 0;
	int rate_opt;
	int t_pnum;//����ҳ��ʱ��
	int k[2] = { NULL, NULL };//���
	struct mem c[100];//�ڴ�
	order[p->page_count];
	for (int i = 0; i < p->page_count; i++)
	{
		t_pnum = 0;
		bool hit = false;
		bool khit = false;
		if (p->flag1 == 0)
		{
			for (int j = 0; j < 2; j++)
			{
				if (order[i] == k[j])   //�������
				{
					hit = true;
					khit = true;
					t_pnum += p->t_quick;
				}
			}
		}
		if (!khit)//���û����
		{
			for (int j = 0; j < memory_num; j++)
			{
				if (order[i] == c[j].page)//���û���У��ڴ�����
				{
					hit = true;
					if (p->flag1 == 0)
					{
						if (k[1] == NULL)//������
						{
							if (k[0] == NULL)
								k[0] = order[i];
							else
								k[1] = order[i];
						}
						else
						{
							k[0] = k[1];
							k[1] = order[i];
						}
						t_pnum += p->t_quick;
					}
				}
			}
			t_pnum += p->t_memory;
			if (!hit)//�ڴ�δ����
			{
				if (memory_num < p->memory_count)//�ڴ�δ��
				{
					c[memory_num].page = order[i];
					memory_num++;
				}
				else    //�ڴ���
				{
					int next[50];
					int t_chushi = i + 1;//��ʼҪ�ҵ���
					int I;
					bool none = false;
					int c_budeng;//���ȵĴ���
					for (int j = 0; j < p->memory_count; j++)
					{
						for (I = t_chushi, c_budeng = 0; I < p->page_count; I++)
						{
							if (c[j].page != order[I])
								c_budeng++;////����ȴ�����1
							else break;//��ȣ�����
						}
						if (c_budeng < p->page_count - t_chushi)
						{
							next[j] = I;
							none = false;
						}
						else
						{
							none = true;
							c[j].page = order[i];
							break;
						}
					}//ѭ������
					if (!none)
					{
						int t = 0, j;
						for (j = 0; j < p->memory_count; j++)//�ҳ����Ļ���
						{
							t = max(t, next[j]);
						}//tΪ����
						for (j = 0; j < p->memory_count; j++)//�õ�Ҫ�����������±�
						{
							if (next[j] == t)
								break;
						}
						c[j].page = order[i];
					}
				}
				if (p->flag1 == 0)
				{
					if (k[1] == NULL)//������
					{
						if (k[0] == NULL)
							k[0] = order[i];
						else
							k[1] = order[i];
					}
					else
					{
						k[0] = k[1];
						k[1] = order[i];
					}
					t_pnum += p->t_quick;
				}
				lack_numopt++;
				t_pnum += p->t_interr + p->t_memory;
			}
		}
		t_pnum += p->t_memory;//���ȡ������ڴ�
		t_numopt += t_pnum;
		CString a, b, e;
		for (int m = 0; m < memory_num; m++)
		{
			b.Format(_T("%d"), c[m].page);
			a = a + " " + b;
		}
		e.Format(_T("%s # %d time: %d"), a, order[i], t_pnum);
		pOut->InsertString(d++, e);
		CString temp1;
		temp1.Format(_T("%d"), lack_numopt);
		optroom.times->SetWindowText(temp1);
		CString temp2;
		temp2.Format(_T("%d"), t_numopt);
		optroom.totaltime->SetWindowText(temp2);
		rate_opt = lack_numopt * 100 / (i + 1);
		CString temp3;
		temp3.Format(_T("%d"), rate_opt);
		optroom.percent->SetWindowText(temp3);
		Sleep(SLEEP);
	}

	return 1;
}

// Cģ��ҳʽ�洢Dlg ��Ϣ�������


BOOL Cģ��ҳʽ�洢Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	fiforoom.pp = (CListBox *)GetDlgItem(IDC_LIST1);
	fiforoom.times = (CEdit *)GetDlgItem(IDC_EDIT9);
	fiforoom.percent = (CEdit *)GetDlgItem(IDC_EDIT10);
	fiforoom.totaltime = (CEdit *)GetDlgItem(IDC_EDIT11);
	lruroom.pp = (CListBox *)GetDlgItem(IDC_LIST2);
	lruroom.times = (CEdit *)GetDlgItem(IDC_EDIT12);
	lruroom.percent = (CEdit *)GetDlgItem(IDC_EDIT13);
	lruroom.totaltime = (CEdit *)GetDlgItem(IDC_EDIT14);
	lfuroom.pp = (CListBox *)GetDlgItem(IDC_LIST3);
	lfuroom.times = (CEdit *)GetDlgItem(IDC_EDIT15);
	lfuroom.percent = (CEdit *)GetDlgItem(IDC_EDIT16);
	lfuroom.totaltime = (CEdit *)GetDlgItem(IDC_EDIT17);
	optroom.pp = (CListBox *)GetDlgItem(IDC_LIST4);
	optroom.times = (CEdit *)GetDlgItem(IDC_EDIT18);
	optroom.percent = (CEdit *)GetDlgItem(IDC_EDIT19);
	optroom.totaltime = (CEdit *)GetDlgItem(IDC_EDIT20);
	//this->m_comboOrder.AddString(_T("�ֶ�����"));
	//this->m_comboOrder.AddString(_T("�������"));

	CString strBmpPath = _T("background.jpg");

	CImage img;

	img.Load(strBmpPath);

	MoveWindow(0, 0, img.GetWidth(), img.GetHeight());

	CBitmap bmpTmp;

	bmpTmp.Attach(img.Detach());

	m_bkBrush.CreatePatternBrush(&bmpTmp);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cģ��ҳʽ�洢Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cģ��ҳʽ�洢Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cģ��ҳʽ�洢Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cģ��ҳʽ�洢Dlg::OnCbnSelchangeCombo1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_editOrder = "";
	CString strOrder, str1, str2;
	int nSel, num, i = 0;
	str1 = "";
	//��ȡ��Ͽ�ؼ����б����ѡ���������
	nSel = m_comboOrder.GetCurSel();
	//����ѡ����������ȡ�����ַ���
	m_comboOrder.GetLBText(nSel, strOrder);

	if (strOrder == "�������")
	{
		//MessageBox(_T("YES"));
		int i;
		for (i = 0; i < _ttoi(m_PCount); i++)
		{
			num = rand() % (_ttoi(m_to) - _ttoi(m_from) + 1) + _ttoi(m_from);//��ʾ��ҳ��ķ�Χ
			order[i] = num;
			str2.Format(_T("%d"), num);
			str1 = str1 + " " + str2;
		}
		m_editOrder = str1;
		// ����Ͽ���ѡ�е��ַ�����ʾ���༭���� 
		CEdit * m_edit = (CEdit *)GetDlgItem(IDC_EDIT5);
		m_edit->SetWindowText(m_editOrder);
	}
	/*else
	{
		GetDlgItem(IDC_EDIT5)->SetWindowText(_T(""));
		int pos1 = 0, pos2 = 0, i = 0;
		do
		{
			pos2 = m_editOrder.Find(' ', pos1);
			if (pos2 == -1)
				order[i] = _tstoi(m_editOrder.Mid(pos1));
			else
			{
				order[i] = _tstoi(m_editOrder.Mid(pos1, pos2 - pos1));
				i++;
				pos1 = pos2 + 1;
			}
		} while (pos2 != -1);
	}*/
}
	
//��ʼ����������
void Cģ��ҳʽ�洢Dlg::OnBnClickedstart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ʼ���б���е�����
	CListBox* p1 = (CListBox*)GetDlgItem(IDC_LIST1);    
	p1->ResetContent();
	CListBox* p2 = (CListBox*)GetDlgItem(IDC_LIST2);
	p2->ResetContent();
	CListBox* p3 = (CListBox*)GetDlgItem(IDC_LIST3);
	p3->ResetContent();
	CListBox* p4 = (CListBox*)GetDlgItem(IDC_LIST4);
	p4->ResetContent();
	UpdateData(TRUE);
	CString strOrder;
	int nSel;
	//��ȡ��Ͽ�ؼ����б����ѡ���������
	nSel = m_comboOrder.GetCurSel();
	//����ѡ����������ȡ�����ַ���
	m_comboOrder.GetLBText(nSel, strOrder);
	if (strOrder != "�������")
	{
		int pos1 = 0, pos2 = 0, i = 0;
		do
		{
			pos2 = m_editOrder.Find(' ', pos1);
			if (pos2 == -1)
				order[i] = _tstoi(m_editOrder.Mid(pos1));
			else
			{
				order[i] = _tstoi(m_editOrder.Mid(pos1, pos2 - pos1));
				i++;
				pos1 = pos2 + 1;
			}
		} while (pos2 != -1);
	}
	para *parameter = new para;
	parameter->memory_count = _ttoi(m_MCount);
	parameter->page_count = _ttoi(m_PCount);
	parameter->t_memory = _ttoi(m_tMemory);
	parameter->t_quick = _ttoi(m_tQuick);
	parameter->t_interr = _ttoi(m_tinterr);
	parameter->flag1 = m_radio1;
	if (_ttoi(m_MCount) > _ttoi(m_PCount))
	{
		MessageBox(_T("Ҫ���ʵ�ҳ����С��Ҫ�ڴ���������Ҫ����ҳ���û���"));
	}
	else{
		hThreadfifo = CreateThread(NULL, 0, ThreadFifo, parameter, 0, NULL);
		hThreadlru = CreateThread(NULL, 0, ThreadLru, parameter, 0, NULL);
		hThreadlfu = CreateThread(NULL, 0, ThreadLfu, parameter, 0, NULL);
		hThreadopt = CreateThread(NULL, 0, ThreadOpt, parameter, 0, NULL);
	}
	
}

//����
void Cģ��ҳʽ�洢Dlg::OnBnClickedstop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SuspendThread(hThreadfifo);
	SuspendThread(hThreadlru);
	SuspendThread(hThreadlfu);
	SuspendThread(hThreadopt);
}

//����
void Cģ��ҳʽ�洢Dlg::OnBnClickedgoon()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ResumeThread(hThreadfifo);
	ResumeThread(hThreadlru);
	ResumeThread(hThreadlfu);
	ResumeThread(hThreadopt);
}

void Cģ��ҳʽ�洢Dlg::OnBnClickedRadio1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	switch (m_radio1)
	{
	case 0:
		//MessageBox(_T("��"));
		flag = 1;
		break;
	default:
		break;
	}
}

//���ò���
void Cģ��ҳʽ�洢Dlg::OnBnClickedreset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str = _T("");
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	GetDlgItem(IDC_EDIT2)->SetWindowText(str);
	GetDlgItem(IDC_EDIT3)->SetWindowText(str);
	GetDlgItem(IDC_EDIT4)->SetWindowText(str);
	GetDlgItem(IDC_EDIT5)->SetWindowText(str);
	GetDlgItem(IDC_EDIT6)->SetWindowText(str);
	GetDlgItem(IDC_EDIT7)->SetWindowText(str);
	GetDlgItem(IDC_EDIT8)->SetWindowText(str);
	GetDlgItem(IDC_EDIT1)->SetFocus();
	m_comboOrder.SetCurSel(-1);
}

//Ĭ�ϲ���
void Cģ��ҳʽ�洢Dlg::OnBnClickedreset2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("3"));
	GetDlgItem(IDC_EDIT2)->SetWindowText(_T("15"));
	GetDlgItem(IDC_EDIT3)->SetWindowText(_T("3"));
	GetDlgItem(IDC_EDIT4)->SetWindowText(_T("6"));
	GetDlgItem(IDC_EDIT5)->SetWindowText(_T("4 6 5 3 4 3 5 5 5 3 4 4 4 6 4"));
	GetDlgItem(IDC_EDIT6)->SetWindowText(_T("10"));
	GetDlgItem(IDC_EDIT7)->SetWindowText(_T("1000"));
	GetDlgItem(IDC_EDIT8)->SetWindowText(_T("5"));
	m_comboOrder.SetCurSel(0);
}

//����
void Cģ��ҳʽ�洢Dlg::OnBnClickedsave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DWORD exitCode1;
	DWORD exitCode2;
	DWORD exitCode3;
	DWORD exitCode4;

	GetExitCodeThread(hThreadfifo, &exitCode1);
	GetExitCodeThread(hThreadlfu, &exitCode2);
	GetExitCodeThread(hThreadlru, &exitCode3);
	GetExitCodeThread(hThreadopt, &exitCode4);
	if ((exitCode1 != STILL_ACTIVE) && (exitCode2 != STILL_ACTIVE)
		&& (exitCode3 != STILL_ACTIVE) && (exitCode4 != STILL_ACTIVE))
	{
		CFileDialog fileDlg(false, _T("txt"), NULL, 0, _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*|*.*||)"), this);
		if (fileDlg.DoModal() == IDOK)
		{
			CString filepathname;
			filepathname = fileDlg.GetPathName();
			int  i, temp1, temp2, temp3, temp4;
			CStringA str, str1, str2, str3;
			str = filepathname;
			ofstream fout(str, ios::app);
			UpdateData(TRUE);
			fout << "���������" << endl;
			fout << "�������Ϊ��" << _ttoi(m_MCount) << endl;
			fout << "�߼�ҳ����Ϊ��" << _ttoi(m_PCount) << endl;
			fout << "�߼�ҳ�淶Χ��" << _ttoi(m_from) << "~" << _ttoi(m_to) << endl;
			fout << "�߼�ҳ������Ϊ��";
			fout << order[0];
			for (i = 1; i < _ttoi(m_PCount); i++)
			{
				fout << "," << order[i];
			}
			fout << "\n";
			fout << "ʱ�������" << endl;
			fout << "����ȡʱ��Ϊ��" << _ttoi(m_tQuick) << endl;
			fout << "�ڴ��ȡʱ�䣺" << _ttoi(m_tMemory) << endl;
			fout << "�жϴ���ʱ�䣺" << _ttoi(m_tinterr) << endl;
			fout << "------------------------------------------------" << endl;
			CString out;
			fout << "**********FIFO�㷨����**********" << endl;
			temp1 = _ttoi(m_PCount);//�õ��༭�������е�����
			for (i = 0; i < temp1; i++)
			{
				fiforoom.pp->GetText(i, out);//���ÿһ��
				str = out;
				fout << str << endl;
			}
			//ȱҳ����
			fiforoom.times->GetWindowText(out);
			str1 = out;
			//ȱҳ��
			fiforoom.percent->GetWindowText(out);
			str2 = out;
			//��ʱ��
			fiforoom.totaltime->GetWindowText(out);
			str3 = out;
			fout << "ȱҳ������" << str1 << "      " << "ȱҳ�ʣ�" << str2 << "      " << "��ʱ�䣺" << str3 << endl;
			fout << "------------------------------------------------" << endl;
			fout << "**********LRU�㷨����**********" << endl;
			temp2 = _ttoi(m_PCount);//�õ��༭�������е�����
			for (i = 0; i < temp2; i++)
			{	//CString out;
				lruroom.pp->GetText(i, out);//���ÿһ��
				str = out;
				fout << str << endl;
			}
			//ȱҳ����
			lruroom.times->GetWindowText(out);
			str1 = out;
			//ȱҳ��
			lruroom.percent->GetWindowText(out);
			str2 = out;
			//��ʱ��
			lruroom.totaltime->GetWindowText(out);
			str3 = out;
			fout << "ȱҳ������" << str1 << "      " << "ȱҳ�ʣ�" << str2 << "      " << "��ʱ�䣺" << str3 << endl;

			fout << "------------------------------------------------" << endl;
			fout << "**********LFU�㷨����**********" << endl;
			temp3 = _ttoi(m_PCount);//�õ��༭�������е�����
			for (i = 0; i < temp3; i++)
			{	//CString out;
				lfuroom.pp->GetText(i, out);//���ÿһ��
				str = out;
				fout << str << endl;
			}
			//ȱҳ����
			lfuroom.times->GetWindowText(out);
			str1 = out;
			//ȱҳ��
			lfuroom.times->GetWindowText(out);
			str2 = out;
			//��ʱ��
			lfuroom.totaltime->GetWindowText(out);
			str3 = out;
			fout << "ȱҳ������" << str1 << "      " << "ȱҳ�ʣ�" << str2 << "      " << "��ʱ�䣺" << str3 << endl;
			fout << "------------------------------------------------" << endl;
			fout << "**********OPT�㷨����**********" << endl;
			temp4 = _ttoi(m_PCount);//�õ��༭�������е�����
			for (i = 0; i < temp4; i++)
			{	//CString out;
				optroom.pp->GetText(i, out);//���ÿһ��
				str = out;
				fout << str << endl;
			}
			//ȱҳ����
			optroom.times->GetWindowText(out);
			str1 = out;
			//ȱҳ��
			optroom.percent->GetWindowText(out);
			str2 = out;
			//��ʱ��
			optroom.totaltime->GetWindowText(out);
			str3 = out;
			fout << "ȱҳ������" << str1 << "      " << "ȱҳ�ʣ�" << str2 << "      " << "��ʱ�䣺" << str3 << endl;
		}
	}
	else{
		MessageBox(_T("���߳���δ���������ܱ���..."));
	}
}


void Cģ��ҳʽ�洢Dlg::OnBnClickedopen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog filedlg(TRUE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("�ı��ĵ�(*.txt)|*.txt|�����ĵ�(*.*)|*.*||"), this);
	//��ʾ���ļ��Ի���
	if (filedlg.DoModal() == IDOK)
	{
		CString filepathname;
		filepathname = filedlg.GetPathName();
		ShellExecute(NULL, _T("open"), filepathname, NULL, NULL, SW_SHOW);
	}
}


void Cģ��ҳʽ�洢Dlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DWORD exitCode1;
	DWORD exitCode2;
	DWORD exitCode3;
	DWORD exitCode4;
	GetExitCodeThread(hThreadfifo, &exitCode1);
	GetExitCodeThread(hThreadlfu, &exitCode2);
	GetExitCodeThread(hThreadlru, &exitCode3);
	GetExitCodeThread(hThreadopt, &exitCode4);
	if ((exitCode1 != STILL_ACTIVE) && (exitCode2 != STILL_ACTIVE)
		&& (exitCode3 != STILL_ACTIVE) && (exitCode4 != STILL_ACTIVE))
	{
		CFileDialog fileDlg(false, _T("txt"), NULL, 0, _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*|*.*||)"), this);
		if (fileDlg.DoModal() == IDOK)
		{
			CString filepathname;
			filepathname = fileDlg.GetPathName();
			filepathname = fileDlg.GetPathName();
			CString str, out;
			int a,b,c,d;
			str = filepathname;
			ofstream fout(str, ios::app);
			UpdateData(TRUE);
			fout << m_radio1 << endl;//�Ƿ��ÿ��
			fiforoom.percent->GetWindowText(out);
			a = _ttoi(out);
			fout << a << " ";

			lfuroom.percent->GetWindowText(out);
			b = _ttoi(out);
			fout <<  b << " " ;

			lruroom.percent->GetWindowText(out);
			c = _ttoi(out);
			fout  << c << " ";

			optroom.percent->GetWindowText(out);
			d= _ttoi(out);

			fout <<  d << " " <<  endl;

		}
		
	}
	else{
		MessageBox(_T("���߳���δ���������ܱ���..."));
	}
}

void Cģ��ҳʽ�洢Dlg::OnBnClickedButton7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	test();
	
}

//Ϊ��ʵ�������Ի��򱳾���Ч����������Ҫ�����ǵĶԻ�����ӦWM_CTLCOLOR��Ϣ��
//ÿ�����ǵĶԻ�����������ӿؼ���Ҫ�ػ�ʱ�����ǵĶԻ��򶼻��յ������Ϣ
HBRUSH Cģ��ҳʽ�洢Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_bkBrush;
	}
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC:
	case IDC_RADIO1:
	case IDC_RADIO2:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
		
	default:
		break;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	
	return hbr;
}
