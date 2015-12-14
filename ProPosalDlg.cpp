
// ProPosalDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProPosal.h"
#include "ProPosalDlg.h"
#include "afxdialogex.h"
#include "Preprocess.h"
#include "CvvImage.h"
#include <string>
#include "afx.h"
#include "stdio.h"
#include "WzdSplash.h"
#include "Test.h"
//#pragma comment(lib,"opencv_highgui248d.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CProPosalDlg �Ի���



CProPosalDlg::CProPosalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProPosalDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProPosalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboType);
}

BEGIN_MESSAGE_MAP(CProPosalDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenPicture, &CProPosalDlg::Openpicture_Button)
	ON_BN_CLICKED(IDC_ExtractProposal, &CProPosalDlg::Extractproposal_Button)
	ON_BN_CLICKED(IDC_PrePage, &CProPosalDlg::Prepage)
	ON_BN_CLICKED(IDC_NextPage, &CProPosalDlg::Nextpage)
	ON_BN_CLICKED(IDC_BUTTON2, &CProPosalDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CProPosalDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CProPosalDlg::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CProPosalDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON5, &CProPosalDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CProPosalDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CProPosalDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CProPosalDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CProPosalDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CProPosalDlg::OnBnClickedButton10)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProPosalDlg ��Ϣ�������

BOOL CProPosalDlg::OnInitDialog()
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
	ratio = 1;// picture �ؼ������ű�������Ϊ 1 
	Pic_Size = 300;
	Pic_Size_2 = 142;
	//����������ʼ��
	m_comboType.AddString(_T("DENSESURF"));
	m_comboType.AddString(_T("DENSESURF_PRO_VLAD"));
	//��������  2015.12.07
	CWzdSplash wndSplash;                 //���������������ʵ��  
	wndSplash.Create(IDB_BITMAP1);
	wndSplash.CenterWindow();
	wndSplash.UpdateWindow(); // send WM_PAINT

	//���������ļ�
	string labelFile = "F:\\Data\\train_ground_truth.txt";
	string centerFile = "../DenSurfProVlad/center.txt";
	string featureBinFile = "F:\\Data\\featureAfterPCA.bin";
	string imageNameFile = "F:\\Data\\imgName.txt";
	string pcaMatrixFile = "F:\\Data\\pcaMatrix.bin";
	string rotateMatrixFile = "F:\\Data\\rotateMatrix.bin";
	GlobalInit(labelFile,centerFile,pcaMatrixFile,imageNameFile,featureBinFile,rotateMatrixFile);
	/*   
	int GlobalInit(string labelFile,
	string centerFile,
	string pcaMatrixFile,
	string imageNameFile,
	string featureBinFile,
	string rotateMatrixFile 
	)

	*/
	//������������
	Sleep(100);
	pic_open = FALSE;  //ͼƬδ��
	pic_search = FALSE;//δ����
	SetTimer(1,1800,NULL);
	feaflags = DENSESURF;
	wndSplash.DestroyWindow();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CProPosalDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProPosalDlg::OnPaint()
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

		//if(pic_open)
		//{
		// cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//��ԭͼ����ѹ����ͼƬ�ؼ��߳�Pic_Size

		//    ShowImage(&IplImage(Img), IDC_MAIN,IDC_EDIT1+10,_T("ԭͼƬ"));        // ������ʾͼƬ����
		// AfxMessageBox("ll");
		//}
	}
	else
	{
		/*   ͼƬ�򿪣����������ػ�
		if(pic_open)
		{
		cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//��ԭͼ����ѹ����ͼƬ�ؼ��߳�Pic_Size

		ShowImage(&IplImage(Img), IDC_MAIN,IDC_EDIT1+10,_T("ԭͼƬ"));        // ������ʾͼƬ����
		//AfxMessageBox("ll");
		cv::Mat ShowImgs2;
		for(int i = 0;i < 9;i++)
		{  
		ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i+(pageIdx-1)*9]);
		// double2 CString
		char buffer[39];
		sprintf_s(buffer,"%s",neighbors[i+(pageIdx-1)*9].c_str());
		//ImgName.push_back(name);
		string str =buffer;

		ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
		}
		}   */
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CProPosalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProPosalDlg::Openpicture_Button()
{
	CFileDialog dlg(
		TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("All Files (*.*) |*.*||"), NULL
		);
	dlg.m_ofn.lpstrTitle = _T("��ѡ��һ����������ͼ��");// ���ļ��Ի���ı�����

	if (dlg.DoModal() != IDOK)             // �ж��Ƿ���ͼƬ
	{
		AfxMessageBox(_T("��ѡ����ʵ��ļ�·��"));
		return;
	}

	mPath = dlg.GetPathName();	    // ��ȡͼƬ·��
	// ·��ת��Ϊ char*
	/*
	const size_t strsize = (mPath.GetLength() + 1) * 2;
	char *mPath_str = new char[strsize];
	size_t sz = 0;
	wcstombs_s(&sz, mPath_str, strsize, mPath, _TRUNCATE);
	*/

	sourceImg = cv::imread((LPCSTR)mPath);
	//sourceImg = cv::imread("K:\\anxingle\\anxingle.jpg");
	//sourceImg = cv::Mat(cvLoadImage((LPCSTR)mPath));

	//sourceImg = cv::imread(mPath_str.GetBuffer());
	//mPath(mPath_str)��Ϊ����·����("E://Programs/Images/Lena.jpg" )  

	if (sourceImg.empty())      // �ж��Ƿ�ɹ���ȡͼƬ,��TheImage==NULL�򷵻أ���ִ������Ĳ���
	{
		AfxMessageBox(_T(mPath));
		AfxMessageBox(_T("��ȡͼƬʧ��"));
		return;
	}

	cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//��ԭͼ����ѹ����ͼƬ�ؼ��߳�Pic_Size

	ShowImage(&IplImage(Img), IDC_MAIN,IDC_EDIT1+10,_T("ԭͼƬ"));        // ������ʾͼƬ����
	pic_open = TRUE;

}


void CProPosalDlg::ShowImage(IplImage* img,unsigned int ID,unsigned int Edit_ID,CString str)
{
	if (!img)      // �ж��Ƿ�ɹ���ȡͼƬ,��TheImage==NULL�򷵻أ���ִ������Ĳ���
	{
		AfxMessageBox(_T("��ȡͼƬʧ��"));
		return;
	}
	CDC* pDC = GetDlgItem(ID)->GetDC();   // �����ʾ�ؼ��� DC
	HDC hDC = pDC->GetSafeHdc();			 // ��ȡ HDC(�豸���) �����л�ͼ����	

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int ID_size = rect.bottom>rect.right ? rect.right : rect.bottom;//���Picture�ؼ��ĳߴ�
	//int b=rect.right;
	pDC->FillSolidRect(&rect, RGB(240, 240, 240));//��һ���Ի�����ɫ��䣬��Ȼ����ͼƬ���еط�һֱ����
	int iw = img->width;
	int ih = img->height;
	if (iw >= ih)	
		ratio = ((float)iw) / ID_size;
	else			
		ratio = ((float)ih) / ID_size;
	SetRect(rect, 0, 0, floor((float)iw / ratio), floor((float)ih / ratio));//�ı�rect��С		


	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);	// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	if(feaflags == DENSESURF_PRO_VLAD)
	{
		if ( Edit_ID == IDC_EDIT1 || Edit_ID == IDC_EDIT4 || Edit_ID == IDC_EDIT7)
			GetDlgItem(Edit_ID)->SetWindowText(str);
		else
		{
			if(Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
				GetDlgItem(Edit_ID)->SetWindowTextA("              ");
		}
	}
	else
	{
		if(Edit_ID >= IDC_EDIT1 && Edit_ID <= IDC_EDIT9)
			GetDlgItem(Edit_ID)->SetWindowText(str);
	}
	ReleaseDC(pDC);

}



// ����ͼƬ
void CProPosalDlg::Extractproposal_Button()
{

	pageIdx = 1;
	/*
	testWithEuler(string type,
	cv::Mat imgSrc,
	vector<pair<float*, string> >& trainFeatures,
	unordered_map<string, unordered_set<string> >& categoryLabels)
	*/
	neighbors.clear();
	if(!pic_open)
	{
		AfxMessageBox("���ͼƬ��");
		return ;
	}
	//vector<string>(neighbors).swap(neighbors);
	testWithEuler("abc ",sourceImg,trainFeatures,categoryLabels);
	//AfxMessageBox("debug1");
	//Retrieval(sourceImg);
	//vector<cv::Mat>ShowImgs2;
	cv::Mat ShowImgs2;
	if(!neighbors.size())
	{
		AfxMessageBox("��ʾͼƬ������");
	}
	else
	{  	
		if(feaflags == DENSESURF_PRO_VLAD)
		{  
			/*    debug for the ShowImgs2[i];
			for(int i = 0;i < 9;)
			{ 

			ShowImgs2[i] = cv::imread(trainPath+"\\"+neighbors[i]);
			ShowImgs2[i+1] = cv::imread(trainPath+"\\"+neighbors[i+1]);
			ShowImgs2[i+2] = cv::imread(trainPath+"\\"+neighbors[i+2]);

			//neighbors[i]
			// double2 CString
			char buffer[39];
			char buffer2[39];
			char buffer3[39];
			sprintf_s(buffer,"%s",neighbors[i]);
			sprintf_s(buffer2,"%s",neighbors[i+1]);
			sprintf_s(buffer3,"%s",neighbors[i+2]);

			string str =buffer;
			string str2 = buffer2;
			string str3 = buffer3;
			ShowImage(&IplImage(ShowImgs2[i]),IDC_T1+i,IDC_EDIT1+i,str.c_str());  //str.Format����%f��,i��; 

			ShowImage(&IplImage(ShowImgs2[i+1]),IDC_T1+i+1,IDC_EDIT1+i+1,str2.c_str());
			ShowImage(&IplImage(ShowImgs2[i+2]),IDC_T1+i+2,IDC_EDIT1+i+2,str3.c_str());
			i = i+3;
			*/
			//}
		}
		else
		{
			for(int i = 0;i < 9;i++)
			{  
				ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i]);

				// double2 CString
				char buffer[39];
				sprintf_s(buffer,"%s",neighbors[i].c_str());

				//ImgName.push_back(name);
				string str =buffer;

				ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
				pic_search = TRUE;

			}
		}
	}
}


//��һҳ
void CProPosalDlg::Prepage()
{
	//AfxMessageBox(_T("�������һҳ�İ�ť"));
	cv::Mat ShowImgs2;
	if(!neighbors.size())
	{
		AfxMessageBox("��ʾͼƬ������");
		return;
	}
	if(pageIdx == 1)
	{
		AfxMessageBox(_T("�Ѿ��ǵ�һҳ��"));
		return;
	}
	else
	{
		pageIdx--;
		//ShowToMFC();
		if(feaflags == DENSESURF_PRO_VLAD)
		{/*
		 for(int i = 0;i < 9;)
		 {  
		 // double2 CString
		 char buffer[39];
		 if(i == 0)
		 sprintf_s(buffer,"%f %s",SimRe[0],ImgName[0].c_str());
		 else 
		 {
		 if(i == 3)
		 sprintf_s(buffer,"%f %s",SimRe[1],ImgName[1].c_str());
		 else
		 sprintf_s(buffer,"%f %s",SimRe[2],ImgName[2].c_str());
		 }
		 //ImgName.push_back(name);
		 string str =buffer;
		 ShowImage(&IplImage(ShowImgs[i]),IDC_T1+i,IDC_EDIT1+i,str.c_str());  //str.Format����%f��,i��; 

		 ShowImage(&IplImage(ShowImgs[i+1]),IDC_T1+i+1,IDC_EDIT1+i+1,str.c_str());
		 ShowImage(&IplImage(ShowImgs[i+2]),IDC_T1+i+2,IDC_EDIT1+i+2,str.c_str());
		 i = i+3;
		 } */
		}
		else
		{
			for(int i = 0;i < 9;i++)
			{  
				ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i+(pageIdx-1)*9]);
				// double2 CString
				char buffer[39];
				sprintf_s(buffer,"%s",neighbors[i+(pageIdx-1)*9].c_str());
				//ImgName.push_back(name);
				string str =buffer;

				ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
			}
		}
	}
}



//��һҳ
void CProPosalDlg::Nextpage()
{
	//AfxMessageBox(_T("�������һҳ�İ�ť"));

	if(pageIdx <= 10)
		pageIdx++;
	else 
	pageIdx = 11;
	if(pageIdx == 11)
	{
		AfxMessageBox("ͼƬ��ʾ������");
		//pageIdx = 11;
		return ;
	}
	cv::Mat ShowImgs2;
	if(neighbors.size() == 0 ||!neighbors.size())
	{
		AfxMessageBox("��ʾͼƬ������");
		return;
	}

	if(pageIdx <= 10)
	{

		/*
		if(feaflags == DENSESURF_PRO_VLAD)
		{  
			for(int i = 0;i < 9;)
			{  
			// double2 CString
			char buffer[39];
			if(i == 0)
			sprintf_s(buffer,"%f %s",SimRe[0],ImgName[0].c_str());
			else 
			{
			if(i == 3)
			sprintf_s(buffer,"%f %s",SimRe[1],ImgName[1].c_str());
			else
			sprintf_s(buffer,"%f %s",SimRe[2],ImgName[2].c_str());
			}
			//ImgName.push_back(name);
			string str =buffer;
			ShowImage(&IplImage(ShowImgs[i]),IDC_T1+i,IDC_EDIT1+i,str.c_str());  //str.Format����%f��,i��; 

			ShowImage(&IplImage(ShowImgs[i+1]),IDC_T1+i+1,IDC_EDIT1+i+1,str.c_str());
			ShowImage(&IplImage(ShowImgs[i+2]),IDC_T1+i+2,IDC_EDIT1+i+2,str.c_str());
			i = i+3;
			}  
		} */

			
			for(int i = 0;i < 9;i++)
			{  
				ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i+(pageIdx-1)*9]);
				// double2 CString
				char buffer[39];
				sprintf_s(buffer,"%s",neighbors[i+(pageIdx-1)*9].c_str());
				//ImgName.push_back(name);
				string str =buffer;

				ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
			}
		
	}
}

//��һ���Ŵ�ť
void CProPosalDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowImage(&IplImage(ShowImgs[0]),IDC_MAIN2,IDC_EDIT1+10,_T("��һ���Ŵ�ť"));
}

//���ĸ��Ŵ�ť
void CProPosalDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowImage(&IplImage(ShowImgs[3]),IDC_MAIN2,IDC_EDIT1+10,_T("��4���Ŵ�ť"));
}


//���߸��Ŵ�ť
void CProPosalDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowImage(&IplImage(ShowImgs[6]),IDC_MAIN2,IDC_EDIT1+10,_T("��7���Ŵ�ť"));
}


void CProPosalDlg::OnCbnSelchangeCombo1()
{
	pageIdx = 1;
	int nSel;
	nSel = m_comboType.GetCurSel();
	if(nSel == 0)
		feaflags = DENSESURF;
	//AfxMessageBox("��һ��");
	else
		feaflags = DENSESURF_PRO_VLAD;
}

//�ڶ���
void CProPosalDlg::OnBnClickedButton5()
{
	ShowImage(&IplImage(ShowImgs[1]),IDC_MAIN2,IDC_EDIT1+10,_T("��2���Ŵ�ť"));
}
//�����
void CProPosalDlg::OnBnClickedButton6()
{
	ShowImage(&IplImage(ShowImgs[4]),IDC_MAIN2,IDC_EDIT1+10,_T("��5 ���Ŵ�ť"));
}

//�ڰ˸�
void CProPosalDlg::OnBnClickedButton7()
{
	ShowImage(&IplImage(ShowImgs[7]),IDC_MAIN2,IDC_EDIT1+10,_T("��8���Ŵ�ť"));
}

//������
void CProPosalDlg::OnBnClickedButton8()
{
	ShowImage(&IplImage(ShowImgs[2]),IDC_MAIN2,IDC_EDIT1+10,_T("�������Ŵ�ť"));
}

//��
void CProPosalDlg::OnBnClickedButton9()
{
	ShowImage(&IplImage(ShowImgs[5]),IDC_MAIN2,IDC_EDIT1+10,_T("��6���Ŵ�ť"));
}

//��
void CProPosalDlg::OnBnClickedButton10()
{
	ShowImage(&IplImage(ShowImgs[8]),IDC_MAIN2,IDC_EDIT1+10,_T("��9���Ŵ�ť"));
}


void CProPosalDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(pic_open)
	{
		cv::Mat Img = cv::Mat(zoomImgByMaxSide(&IplImage(sourceImg), Pic_Size), false);//��ԭͼ����ѹ����ͼƬ�ؼ��߳�Pic_Size

		ShowImage(&IplImage(Img), IDC_MAIN,IDC_EDIT1+10,_T("ԭͼƬ"));        // ������ʾͼƬ����
		//AfxMessageBox("ll");
		if(pic_search){
			cv::Mat ShowImgs2;
			for(int i = 0;i < 9;i++)
			{  
				ShowImgs2 = cv::imread(trainPath+"\\"+neighbors[i+(pageIdx-1)*9]);
				// double2 CString
				char buffer[39];
				sprintf_s(buffer,"%s",neighbors[i+(pageIdx-1)*9].c_str());
				//ImgName.push_back(name);
				string str =buffer;

				ShowImage(&IplImage(ShowImgs2),IDC_T1+i,IDC_EDIT1+i,str.c_str());  
			}
		}
	}  
	CDialogEx::OnTimer(nIDEvent);
}
