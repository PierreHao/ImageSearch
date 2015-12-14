#include "stdafx.h"
#include <fstream>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "Train.h"
#include "Configure.h"
#include "Utils.h"
#include "ImgConAna.h"
#include "ImgFeature.h"
#include "PCAUtils.h"

using namespace cv;


void train(string vocabularyFile, string imagePath, string imagePathFile, string featureBinFile,FEAFLAGS flags) {
	// ��ȡ�뱾
	if(flags==DENSESURF_PRO_VLAD){
		vocabulary = newMatrix(clusterCount, surfDescriptorSize);
		loadVocabulary(vocabularyFile, vocabulary, clusterCount, surfDescriptorSize);
	}

	// ��ȡ�ļ����е�ͼƬ
	vector<string> files;
	getFiles(imagePath, files);

	// ���������image path�ļ�
	ofstream imagePathOutfile(imagePathFile.c_str());
	// ��������������ļ�
	FILE* fin;
	fin = fopen(featureBinFile.c_str(), "wb");

#if DEBUG
	ofstream featureOutfile("..\\..\\surfVladFeature.txt");
#endif

	clock_t start;
	start = clock();

	size_t imgNums = files.size();
	Mat featureMat(imgNums, surfVladFeatureSize, CV_32FC1);
	imagePathOutfile << imgNums << endl;
	ofstream errorLog("log.txt");
	for (size_t n = 0; n < imgNums; ++n) {
		Mat imgSrc = imread(files[n]);
		// ��ȡproposals
		vector<Mat> proposals;
		proposals.push_back(imgSrc);
		size_t pNums = proposals.size();
		imagePathOutfile << files[n] << endl;
		imagePathOutfile << pNums << endl;
#if DEBUG
		featureOutfile << splitFileName(files[n]) << endl;
		featureOutfile << pNums << endl;
#endif
		// ��ÿ��proposals��ȡ����
		for (size_t i = 0; i < pNums; ++i) {
			// proposal��һ��
			Mat stdImg;
			resize(proposals[i], stdImg, Size(320, 320));

			// ��ȡSurf����
			if(flags==DENSESURF){
				IplImage tmp = IplImage(stdImg);
				SurfFeature feature;
				feature.computeFeature(&tmp);
				float * featureArray = feature.GetFeature();
				feature2Bin(featureArray, denseSurfFeatureSize, fin);
				feature.FreeFeature(featureArray);
			}
			else{
				// ��ȡSurf&VLAD����
				ImgFeature feature(stdImg);
				bool extractSucc = feature.computeSurfVladFeature(vocabulary, clusterCount, surfDescriptorSize);
				if (!extractSucc) errorLog << files[n] << ":" << i << " error." << endl;
				float *featureArray = feature.GetFeature();
				feature2Bin(featureArray, clusterCount * surfDescriptorSize, fin);
				// store features to featureMat
				for (int c = 0; c < surfVladFeatureSize; ++c) {
					featureMat.at<float>(n, c) = featureArray[c];
				}
				feature.FreeFeature(featureArray);
			}
#if DEBUG
			for (int i = 0; i < clusterCount * surfDescriptorSize; ++i) {
				featureOutfile << featureArray[i] << " ";
			}
			featureOutfile << endl;
#endif
			/*cout << files[n] << ":" << i << " end." << endl;*/
			//feature.FreeFeature(featureArray);
		}
		cout << files[n] << ":" << n << " end." << endl;
	}
	errorLog.close();
	imagePathOutfile.close();
	fclose(fin);
	if(flags==DENSESURF_PRO_VLAD){
		freeMatrix(vocabulary, clusterCount);
	}
#if DEBUG
	featureOutfile.close();
#endif
	clock_t end = clock();
	cout << "Total Time: " << (float)(end - start) / CLOCKS_PER_SEC << endl;

	// PCA
	// ����mean
	string meanFile = "..\\meanValues.bin";
	// ����eigen values
	string eigenValuesFile = "..\\eigenValues.txt";
	// ����eigenvectors
	string eigenVectorsFile = "..\\eigenVectors.bin";
	// ���PCA��ά�������
	string pcaFeatureFile = "..\\featureAfterPCA.bin";
	pcaTrain(featureMat, meanFile, eigenValuesFile, eigenVectorsFile, pcaFeatureFile);
}