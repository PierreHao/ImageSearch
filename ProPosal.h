
// ProPosal.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProPosalApp: 
// �йش����ʵ�֣������ ProPosal.cpp
//

class CProPosalApp : public CWinApp
{
public:
	CProPosalApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProPosalApp theApp;