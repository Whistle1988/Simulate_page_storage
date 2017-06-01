
#include"stdafx.h"
#include<cstdlib>
#include <cstdio>
#include<cstring>
#include"engine.h"
#include<fstream>
using namespace std;

void test()
{
	const int BUFFER_SIZE = 1024;
	char buffern[BUFFER_SIZE];
	Engine* ep;
	mxArray *horizonal = NULL;
	//mxArray *vertical = NULL;
	mxArray *a0 = NULL;
	mxArray *b0 = NULL;
	mxArray *c0 = NULL;
	mxArray *d0 = NULL;
	mxArray *a1 = NULL;
	mxArray *b1 = NULL;
	mxArray *c1 = NULL;
	mxArray *d1 = NULL;

	if ((ep = engOpen("")) == NULL)
	{
		printf("Engine Fail");
	}
	engOutputBuffer(ep, buffern, BUFFER_SIZE);
	printf("Init Success");
	double q_a[10];
	double q_b[10];
	double q_c[10];
	double q_d[10];
	double s_a[10];
	double s_b[10];
	double s_c[10];
	double s_d[10];
	fstream in("test.txt");
	int flag;
	int j = 0, m = 0;
	int count = 20;
	while (count-- && in >> flag){
		if (!flag){ in >> q_a[j] >> q_b[j] >> q_c[j] >> q_d[j]; j++; }
		else { in >> s_a[m] >> s_b[m] >> s_c[m] >> s_d[m]; m++; }
	}
	double hor[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//创建数组
	horizonal = mxCreateDoubleMatrix(1, 10, mxREAL);//横轴
	a0 = mxCreateDoubleMatrix(1, 10, mxREAL);
	b0 = mxCreateDoubleMatrix(1, 10, mxREAL);
	c0 = mxCreateDoubleMatrix(1, 10, mxREAL);
	d0 = mxCreateDoubleMatrix(1, 10, mxREAL);
	a1 = mxCreateDoubleMatrix(1, 10, mxREAL);
	b1 = mxCreateDoubleMatrix(1, 10, mxREAL);
	c1 = mxCreateDoubleMatrix(1, 10, mxREAL);
	d1 = mxCreateDoubleMatrix(1, 10, mxREAL);

	//赋值
	memcpy((void *)mxGetPr(horizonal), (void *)hor, sizeof(hor));

	memcpy((void *)mxGetPr(a0), (void *)q_a, sizeof(q_a));
	memcpy((void *)mxGetPr(b0), (void *)q_b, sizeof(q_b));
	memcpy((void *)mxGetPr(c0), (void *)q_c, sizeof(q_c));
	memcpy((void *)mxGetPr(d0), (void *)q_d, sizeof(q_d));

	memcpy((void *)mxGetPr(a1), (void *)s_a, sizeof(s_a));
	memcpy((void *)mxGetPr(b1), (void *)s_b, sizeof(s_b));
	memcpy((void *)mxGetPr(c1), (void *)s_c, sizeof(s_c));
	memcpy((void *)mxGetPr(d1), (void *)s_d, sizeof(s_d));
	//写入matlab引擎空间
	engPutVariable(ep, "hor", horizonal);
	engPutVariable(ep, "q_a", a0);
	engPutVariable(ep, "q_b", b0);
	engPutVariable(ep, "q_c", c0);
	engPutVariable(ep, "q_d", d0);
	engPutVariable(ep, "s_a", a1);
	engPutVariable(ep, "s_b", b1);
	engPutVariable(ep, "s_c", c1);
	engPutVariable(ep, "s_d", d1);

	engEvalString(ep, "plot(hor,q_a,'-o',hor,q_b,'-o',hor,q_c,'-o',hor,q_d,'-o',hor,s_a,'-X',hor,s_b,'-X',hor,s_c,'-X',hor,s_d,'-X')");
	engEvalString(ep,"legend('有快表的FIFO曲线','有快表的LRU曲线','有快表的LFU曲线','有快表的OPT曲线','无快表的FIFO曲线','无快表的LRU曲线','无快表的LFU曲线','无快表的OPT曲线')");
	engEvalString(ep, "hold on");
	engEvalString(ep, " ylim([20 60]) ");
	engEvalString(ep, " title('页面置换算法缺页率曲线图（缺页率/实验次数）') ");

	getchar();
	
}
