#include "stdafx.h"
class para
{
public:
	int memory_count;      //�������
	int page_count;      //�߼�ҳ����
	CString Number;  //ҳ��������ʾ
	int t_memory;//�ڴ��ȡʱ��
	int t_quick;//����ѯʱ��
	int t_interr;//�жϴ���ʱ��
	int  flag1;	 //���޿���ж�
};
struct mem{
	int page;
	int flag;
};
struct room
{
	CListBox *pp;//ָ��༭���ָ��
	CEdit *times;//ȱҳ����
	CEdit *percent;//ȱҳ��
	CEdit *totaltime;//��ʱ��
};