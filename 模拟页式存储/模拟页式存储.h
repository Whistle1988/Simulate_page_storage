
// ģ��ҳʽ�洢.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cģ��ҳʽ�洢App: 
// �йش����ʵ�֣������ ģ��ҳʽ�洢.cpp
//

class Cģ��ҳʽ�洢App : public CWinApp
{
public:
	Cģ��ҳʽ�洢App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cģ��ҳʽ�洢App theApp;