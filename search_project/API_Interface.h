#ifndef _H_API_INTERFACE_H
#define _H_API_INTERFACE_H

#include "Configure.h"
#include "ImgConAna.h"
#include "ImgFeature.h"
#include "Learn.h"
#include "LoadFile.h"
#include "NearestNeighbors.h"
#include "ostuRemoveBorder.h"
#include "Search.h"
#include "SurfFeature.h"
#include "Train.h"
#include "Utils.h"
#include "VLADFeature.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int GlobalInit();	//����������ݿ�--ȫ�ֱ���
int Retrieval(Mat img);	//����ͼƬ�ļ������--�����ȫ�ֱ���QueryRe
int ShowToMFC();	//���ݷ�ҳ��Ҫ���õ���ʾ��ͼƬ����Ӧ�� proposal && ���ƶ�

#endif