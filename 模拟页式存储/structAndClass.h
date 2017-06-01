#include "stdafx.h"
class para
{
public:
	int memory_count;      //物理块数
	int page_count;      //逻辑页面数
	CString Number;  //页面序列显示
	int t_memory;//内存存取时间
	int t_quick;//快表查询时间
	int t_interr;//中断处理时间
	int  flag1;	 //有无快表判断
};
struct mem{
	int page;
	int flag;
};
struct room
{
	CListBox *pp;//指向编辑框的指针
	CEdit *times;//缺页次数
	CEdit *percent;//缺页率
	CEdit *totaltime;//总时间
};