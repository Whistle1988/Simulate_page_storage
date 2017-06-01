// 模拟页式存储Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "模拟页式存储.h"
#include "模拟页式存储Dlg.h"
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
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// C模拟页式存储Dlg 对话框



C模拟页式存储Dlg::C模拟页式存储Dlg(CWnd* pParent /*=NULL*/)
: CDialogEx(C模拟页式存储Dlg::IDD, pParent)
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

void C模拟页式存储Dlg::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(C模拟页式存储Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_start, &C模拟页式存储Dlg::OnBnClickedstart)
	ON_CBN_SELCHANGE(IDC_COMBO1, &C模拟页式存储Dlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_stop, &C模拟页式存储Dlg::OnBnClickedstop)
	ON_BN_CLICKED(IDC_goon, &C模拟页式存储Dlg::OnBnClickedgoon)
	ON_BN_CLICKED(IDC_RADIO1, &C模拟页式存储Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_reset, &C模拟页式存储Dlg::OnBnClickedreset)
	ON_BN_CLICKED(IDC_reset2, &C模拟页式存储Dlg::OnBnClickedreset2)
	ON_BN_CLICKED(IDC_save, &C模拟页式存储Dlg::OnBnClickedsave)
	ON_BN_CLICKED(IDC_open, &C模拟页式存储Dlg::OnBnClickedopen)
	ON_BN_CLICKED(IDC_BUTTON1, &C模拟页式存储Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &C模拟页式存储Dlg::OnBnClickedButton7)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


DWORD WINAPI ThreadFifo(LPVOID GetParameter)
{
	para *p = (para*)GetParameter;
	CListBox *pOut = (CListBox *)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST1));
	int d = 0;
	int memory_num = 0;//内存里的页面个数
	int t_numfifo = 0;//总时间
	int t_pnum;//单个页面时间
	int lack_numfifo = 0;//缺页次数
	int rate_fifo;//缺页率
	int k[2] = { NULL, NULL };//快表
	struct mem c[100];//内存
	order[p->page_count];
	for (int i = 0; i < p->page_count; i++)
	{
		t_pnum = 0;
		bool hit = false;
		bool khit = false;
		if (p->flag1 == 0)//flag1为0时 有快表 为1时无
		{
			for (int j = 0; j < 2; j++)
			{
				if (order[i] == k[j])   //快表命中
				{
					hit = true;
					khit = true;
					t_pnum += p->t_quick;
				}
			}
		}
		if (!khit)//快表没命中
		{
			for (int j = 0; j < p->memory_count; j++)
			{
				if (order[i] == c[j].page)//快表没命中，内存命中
				{
					hit = true;
					if (p->flag1 == 0)
					{
						if (k[1] == NULL)//放入快表
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
			if (!hit)//内存未命中
			{
				if (memory_num < p->memory_count)//内存未满
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
					if (k[1] == NULL)//放入快表
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
		t_pnum += p->t_memory;//最后取数访问内存
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
//最近最久未使用
DWORD WINAPI ThreadLru(LPVOID GetParameter)
{
	para *p = (para*)GetParameter;
	CListBox *pOut = (CListBox *)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST2)); //MFC 获取控件指针的方法GetDlgItem的标准用法
	int d = 0;
	int memory_num = 0;//内存里的页面个数
	int t_numlru = 0;//总时间
	int lack_numlru = 0;
	int rate_lru;
	int t_pnum;//单个页面时间
	int k[2] = { NULL, NULL };//快表
	struct mem c[100];//内存
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
				if (order[i] == k[j])   //快表命中
				{
					hit = true;
					khit = true;
					t_pnum += p->t_quick;
					for (int k = 0; k < memory_num; k++)//修改内存中该页面的flag
					{
						if (order[i] == c[k].page)
							c[k].flag = i;
					}
				}
			}
		}
		if (!khit)//快表没命中
		{
			for (int j = 0; j < p->memory_count; j++)
			{
				if (order[i] == c[j].page)//快表没命中，内存命中
				{
					hit = true;
					c[j].flag = i;
					if (p->flag1 == 0)
					{
						if (k[1] == NULL)//放入快表
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
			if (!hit)//内存未命中
			{
				if (memory_num < p->memory_count)//内存未满
				{
					c[memory_num].page = order[i];
					c[memory_num].flag = i;
					memory_num++;
				}
				else    //内存满
				{
					int temp;//存放要调出的页面物理快号
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
					if (k[1] == NULL)//放入快表
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
		t_pnum += p->t_memory;//最后取数访问内存
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
//最近最少使用算法
DWORD WINAPI ThreadLfu(LPVOID GetParameter)
{
	para *p = (para*)GetParameter;
	CListBox *pOut = (CListBox *)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_LIST3));
	int d = 0;
	int memory_num = 0;//内存里的页面个数
	int t_numlfu = 0;//总时间
	int lack_numlfu = 0;
	int rate_lfu;
	int t_pnum;//单个页面时间
	int k[2] = { NULL, NULL };//快表
	struct mem c[100];//内存
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
				if (order[i] == k[j])   //快表命中
				{
					hit = true;
					khit = true;
					t_pnum += p->t_quick;
					for (int k = 0; k < memory_num; k++)//修改内存中该页面的flag
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
		if (!khit)//快表没命中
		{
			for (int j = 0; j < memory_num; j++)
			{
				if (order[i] == c[j].page)//快表没命中，内存命中
				{
					hit = true;
					c[j].flag += 1;
					T[j] = i;
					if (p->flag1 == 0)
					{
						if (k[1] == NULL)//放入快表
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
			if (!hit)//内存未命中
			{
				if (memory_num < p->memory_count)//内存未满
				{
					c[memory_num].page = order[i];
					c[memory_num].flag = 1;
					T[memory_num] = i;
					memory_num++;
				}
				else    //内存满
				{
					int temp;//存放要调出的页面物理快号
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
					if (k[1] == NULL)//放入快表
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
		t_pnum += p->t_memory;//最后取输访问内存
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
	int memory_num = 0;//内存里的页面个数
	int t_numopt = 0;//总时间
	int lack_numopt = 0;
	int rate_opt;
	int t_pnum;//单个页面时间
	int k[2] = { NULL, NULL };//快表
	struct mem c[100];//内存
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
				if (order[i] == k[j])   //快表命中
				{
					hit = true;
					khit = true;
					t_pnum += p->t_quick;
				}
			}
		}
		if (!khit)//快表没命中
		{
			for (int j = 0; j < memory_num; j++)
			{
				if (order[i] == c[j].page)//快表没命中，内存命中
				{
					hit = true;
					if (p->flag1 == 0)
					{
						if (k[1] == NULL)//放入快表
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
			if (!hit)//内存未命中
			{
				if (memory_num < p->memory_count)//内存未满
				{
					c[memory_num].page = order[i];
					memory_num++;
				}
				else    //内存满
				{
					int next[50];
					int t_chushi = i + 1;//初始要找的数
					int I;
					bool none = false;
					int c_budeng;//不等的次数
					for (int j = 0; j < p->memory_count; j++)
					{
						for (I = t_chushi, c_budeng = 0; I < p->page_count; I++)
						{
							if (c[j].page != order[I])
								c_budeng++;////不相等次数加1
							else break;//相等，跳出
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
					}//循环结束
					if (!none)
					{
						int t = 0, j;
						for (j = 0; j < p->memory_count; j++)//找出最大的换掉
						{
							t = max(t, next[j]);
						}//t为最大的
						for (j = 0; j < p->memory_count; j++)//得到要换出的数组下标
						{
							if (next[j] == t)
								break;
						}
						c[j].page = order[i];
					}
				}
				if (p->flag1 == 0)
				{
					if (k[1] == NULL)//放入快表
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
		t_pnum += p->t_memory;//最后取输访问内存
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

// C模拟页式存储Dlg 消息处理程序


BOOL C模拟页式存储Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
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
	//this->m_comboOrder.AddString(_T("手动输入"));
	//this->m_comboOrder.AddString(_T("随机产生"));

	CString strBmpPath = _T("background.jpg");

	CImage img;

	img.Load(strBmpPath);

	MoveWindow(0, 0, img.GetWidth(), img.GetHeight());

	CBitmap bmpTmp;

	bmpTmp.Attach(img.Detach());

	m_bkBrush.CreatePatternBrush(&bmpTmp);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C模拟页式存储Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C模拟页式存储Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C模拟页式存储Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C模拟页式存储Dlg::OnCbnSelchangeCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_editOrder = "";
	CString strOrder, str1, str2;
	int nSel, num, i = 0;
	str1 = "";
	//获取组合框控件的列表框中选中项的索引
	nSel = m_comboOrder.GetCurSel();
	//根据选中项索引获取该项字符串
	m_comboOrder.GetLBText(nSel, strOrder);

	if (strOrder == "随机产生")
	{
		//MessageBox(_T("YES"));
		int i;
		for (i = 0; i < _ttoi(m_PCount); i++)
		{
			num = rand() % (_ttoi(m_to) - _ttoi(m_from) + 1) + _ttoi(m_from);//表示的页面的范围
			order[i] = num;
			str2.Format(_T("%d"), num);
			str1 = str1 + " " + str2;
		}
		m_editOrder = str1;
		// 将组合框中选中的字符串显示到编辑框中 
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
	
//开始，创建进程
void C模拟页式存储Dlg::OnBnClickedstart()
{
	// TODO:  在此添加控件通知处理程序代码
	//初始化列表框中的内容
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
	//获取组合框控件的列表框中选中项的索引
	nSel = m_comboOrder.GetCurSel();
	//根据选中项索引获取该项字符串
	m_comboOrder.GetLBText(nSel, strOrder);
	if (strOrder != "随机产生")
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
		MessageBox(_T("要访问的页面数小于要内存数，不需要进行页面置换！"));
	}
	else{
		hThreadfifo = CreateThread(NULL, 0, ThreadFifo, parameter, 0, NULL);
		hThreadlru = CreateThread(NULL, 0, ThreadLru, parameter, 0, NULL);
		hThreadlfu = CreateThread(NULL, 0, ThreadLfu, parameter, 0, NULL);
		hThreadopt = CreateThread(NULL, 0, ThreadOpt, parameter, 0, NULL);
	}
	
}

//挂起
void C模拟页式存储Dlg::OnBnClickedstop()
{
	// TODO:  在此添加控件通知处理程序代码
	SuspendThread(hThreadfifo);
	SuspendThread(hThreadlru);
	SuspendThread(hThreadlfu);
	SuspendThread(hThreadopt);
}

//唤醒
void C模拟页式存储Dlg::OnBnClickedgoon()
{
	// TODO:  在此添加控件通知处理程序代码
	ResumeThread(hThreadfifo);
	ResumeThread(hThreadlru);
	ResumeThread(hThreadlfu);
	ResumeThread(hThreadopt);
}

void C模拟页式存储Dlg::OnBnClickedRadio1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	switch (m_radio1)
	{
	case 0:
		//MessageBox(_T("是"));
		flag = 1;
		break;
	default:
		break;
	}
}

//重置参数
void C模拟页式存储Dlg::OnBnClickedreset()
{
	// TODO:  在此添加控件通知处理程序代码
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

//默认参数
void C模拟页式存储Dlg::OnBnClickedreset2()
{
	// TODO:  在此添加控件通知处理程序代码
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

//保存
void C模拟页式存储Dlg::OnBnClickedsave()
{
	// TODO:  在此添加控件通知处理程序代码
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
		CFileDialog fileDlg(false, _T("txt"), NULL, 0, _T("文本文件(*.txt)|*.txt|所有文件(*.*|*.*||)"), this);
		if (fileDlg.DoModal() == IDOK)
		{
			CString filepathname;
			filepathname = fileDlg.GetPathName();
			int  i, temp1, temp2, temp3, temp4;
			CStringA str, str1, str2, str3;
			str = filepathname;
			ofstream fout(str, ios::app);
			UpdateData(TRUE);
			fout << "物理参数：" << endl;
			fout << "物理块数为：" << _ttoi(m_MCount) << endl;
			fout << "逻辑页面数为：" << _ttoi(m_PCount) << endl;
			fout << "逻辑页面范围：" << _ttoi(m_from) << "~" << _ttoi(m_to) << endl;
			fout << "逻辑页面序列为：";
			fout << order[0];
			for (i = 1; i < _ttoi(m_PCount); i++)
			{
				fout << "," << order[i];
			}
			fout << "\n";
			fout << "时间参数：" << endl;
			fout << "快表存取时间为：" << _ttoi(m_tQuick) << endl;
			fout << "内存存取时间：" << _ttoi(m_tMemory) << endl;
			fout << "中断处理时间：" << _ttoi(m_tinterr) << endl;
			fout << "------------------------------------------------" << endl;
			CString out;
			fout << "**********FIFO算法过程**********" << endl;
			temp1 = _ttoi(m_PCount);//得到编辑框里所有的行数
			for (i = 0; i < temp1; i++)
			{
				fiforoom.pp->GetText(i, out);//输出每一行
				str = out;
				fout << str << endl;
			}
			//缺页次数
			fiforoom.times->GetWindowText(out);
			str1 = out;
			//缺页率
			fiforoom.percent->GetWindowText(out);
			str2 = out;
			//总时间
			fiforoom.totaltime->GetWindowText(out);
			str3 = out;
			fout << "缺页次数：" << str1 << "      " << "缺页率：" << str2 << "      " << "总时间：" << str3 << endl;
			fout << "------------------------------------------------" << endl;
			fout << "**********LRU算法过程**********" << endl;
			temp2 = _ttoi(m_PCount);//得到编辑框里所有的行数
			for (i = 0; i < temp2; i++)
			{	//CString out;
				lruroom.pp->GetText(i, out);//输出每一行
				str = out;
				fout << str << endl;
			}
			//缺页次数
			lruroom.times->GetWindowText(out);
			str1 = out;
			//缺页率
			lruroom.percent->GetWindowText(out);
			str2 = out;
			//总时间
			lruroom.totaltime->GetWindowText(out);
			str3 = out;
			fout << "缺页次数：" << str1 << "      " << "缺页率：" << str2 << "      " << "总时间：" << str3 << endl;

			fout << "------------------------------------------------" << endl;
			fout << "**********LFU算法过程**********" << endl;
			temp3 = _ttoi(m_PCount);//得到编辑框里所有的行数
			for (i = 0; i < temp3; i++)
			{	//CString out;
				lfuroom.pp->GetText(i, out);//输出每一行
				str = out;
				fout << str << endl;
			}
			//缺页次数
			lfuroom.times->GetWindowText(out);
			str1 = out;
			//缺页率
			lfuroom.times->GetWindowText(out);
			str2 = out;
			//总时间
			lfuroom.totaltime->GetWindowText(out);
			str3 = out;
			fout << "缺页次数：" << str1 << "      " << "缺页率：" << str2 << "      " << "总时间：" << str3 << endl;
			fout << "------------------------------------------------" << endl;
			fout << "**********OPT算法过程**********" << endl;
			temp4 = _ttoi(m_PCount);//得到编辑框里所有的行数
			for (i = 0; i < temp4; i++)
			{	//CString out;
				optroom.pp->GetText(i, out);//输出每一行
				str = out;
				fout << str << endl;
			}
			//缺页次数
			optroom.times->GetWindowText(out);
			str1 = out;
			//缺页率
			optroom.percent->GetWindowText(out);
			str2 = out;
			//总时间
			optroom.totaltime->GetWindowText(out);
			str3 = out;
			fout << "缺页次数：" << str1 << "      " << "缺页率：" << str2 << "      " << "总时间：" << str3 << endl;
		}
	}
	else{
		MessageBox(_T("有线程尚未结束！不能保存..."));
	}
}


void C模拟页式存储Dlg::OnBnClickedopen()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog filedlg(TRUE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("文本文档(*.txt)|*.txt|所有文档(*.*)|*.*||"), this);
	//显示打开文件对话框
	if (filedlg.DoModal() == IDOK)
	{
		CString filepathname;
		filepathname = filedlg.GetPathName();
		ShellExecute(NULL, _T("open"), filepathname, NULL, NULL, SW_SHOW);
	}
}


void C模拟页式存储Dlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
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
		CFileDialog fileDlg(false, _T("txt"), NULL, 0, _T("文本文件(*.txt)|*.txt|所有文件(*.*|*.*||)"), this);
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
			fout << m_radio1 << endl;//是否用快表
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
		MessageBox(_T("有线程尚未结束！不能保存..."));
	}
}

void C模拟页式存储Dlg::OnBnClickedButton7()
{
	// TODO:  在此添加控件通知处理程序代码
	test();
	
}

//为了实现美化对话框背景的效果，我们需要让我们的对话框响应WM_CTLCOLOR消息，
//每当我们的对话框或者它的子控件需要重绘时，我们的对话框都会收到这个消息
HBRUSH C模拟页式存储Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
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
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	
	return hbr;
}
