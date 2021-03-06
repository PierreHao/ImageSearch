#include "stdafx.h"
#include "Configure.h"


const std::string trainPath = "K:\\qcz\\AliSearch\\train_image\\Competition";
const std::string testPath = "K:\\qcz\\AliSearch\\train_image\\Competition";

const int surfDescriptorSize = 64;
const int surfBlockSize = 100;
const int denseSurfFeatureSize = surfBlockSize * surfDescriptorSize;
const int clusterCount = 128;
const int surfVladFeatureSize = clusterCount * surfDescriptorSize; 
const int featureAfterPCASize = 256;
const int nearestK = 100;
//const int nbits = 256;
const int categoryNum = 559;

FEAFLAGS feaflags = DENSESURF;
float** vocabulary = 0;
float** pcaMatrix = 0;
float** rotationMatrix = 0;

std::unordered_map<std::string, std::vector<float*> > trainFeatureProVladMap;

std::unordered_map<std::string, std::vector<float*> > trainFeatureDenseSurfMap;

std::vector<QueryRe > IRResult;
std::vector<cv::Mat > QueryPro;

std::vector<double > SimRe;
std::vector<std::string > ImgName;
std::vector<cv::Mat > ShowImgs;
int pageIdx = 1;  //当前页面
const int ImgNumPerPage = 3;

std::unordered_map<string, std::unordered_set<string> > categoryLabels;
std::vector<std::pair<float*, string> > trainFeatures;

std::vector<string> neighbors;// 搜索结果名称