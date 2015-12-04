#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
#include <string>
#include <vector>
#include <unordered_map>
#include <opencv2/opencv.hpp>
using namespace cv;

typedef struct{
	std::pair<std::string,double> PathDis;
	int IdxQuery;
	int IdxDB;
}QueryRe;

typedef enum{
	DENSESURF = 0,
	DENSESURF_PRO_VLAD,
}FEAFLAGS;

#define DEBUG 0

extern const std::string trainPath; // ͼƬ���ַ
extern const std::string testPath;

extern const int surfDescriptorSize; // surf����ά��
extern const int surfBlockSize; // ÿ��ͼƬ��surf��������
extern const int denseSurfFeatureSize; // ��dense surf��ʽ��ȡ��ÿ��ͼƬ������ά��
extern const int clusterCount; // ����������뱾����
extern const int surfVladFeatureSize;  // ��dense surf + VLAD��ʽ��ȡ��ÿ��ͼƬ������γ��
extern const int featureAfterPCASize; // PCA��ά�������ά��
extern const int nearestK; // ���ص������ͼƬ�ĸ���
extern const int categoryNum; // ����Ͱ�ͼƬ�������ݵ�С�����
const int nbits = 256; // �����λ��

extern FEAFLAGS feaflags;

extern std::vector<QueryRe > IRResult;	//����������
extern std::vector<cv::Mat > QueryPro;	//����query��proposal���

extern std::vector<double > SimRe;	//������������֮��ľ���
extern std::vector<cv::Mat > ShowImgs;//��������õ���ͼƬ
extern std::vector<std::string > ImgName; //��������õ���ͼƬ��·��

extern int pageIdx;	//MFC�з�ҳ��ҳ������
extern const int ImgNumPerPage;	//ÿҳ��ʾ���ͼ��Ŀ


extern float** vocabulary; // �����뱾
extern float** pcaMatrix; // PCA����
extern float** rotationMatrix; // ITQ��ת����
//	DenseSurf + VLAD + Proposal
extern std::unordered_map<std::string, std::vector<float*> > trainFeatureProVladMap; // �����ͼƬ��ÿ��ͼƬ��Ӧ������

//	DenseSurf
extern std::unordered_map<std::string, std::vector<float*> > trainFeatureDenseSurfMap; // �����ͼƬ��ÿ��ͼƬ��Ӧ��DenseSurf����
#endif