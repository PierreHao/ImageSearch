#include "API_Interface.h"

int GlobalInit(){

	string vocFile = "../DenSurfProVlad/center.txt";
	string imgPathProVladFile = "../DenSurfProVlad/imgPath.txt";
	string featureBinProVladFile = "../DenSurfProVlad/feaProVlad.bin";


	string imgPathDenseSurfFile = "../DenSurf/imgPath.txt";
	string featureDenseSurfFile = "../DenSurf/feaDenseSurf.bin";

	load(vocFile,imgPathProVladFile,featureBinProVladFile,imgPathDenseSurfFile,featureDenseSurfFile);
	return 1;
}
int Retrieval(Mat img){

	if(!(IRResult.size()==0)){IRResult.clear();}

	search(img,IRResult,feaflags);

	return 1;

}

int ShowToMFC(){

	//��ʼ��
	if(!(SimRe.size()==0)){SimRe.clear();}
	if(!(ShowImgs.size()==0)){ShowImgs.clear();}

	//DENSESURF���
	if(feaflags==DENSESURF){
		if((pageIdx*ImgNumPerPage*3)>IRResult.size()){
			cout<<"��ʾͼƬ����"<<endl;
			return -1;
		}
		int start = (pageIdx-1)*ImgNumPerPage*3;
		for(int i = start;i<(start+ImgNumPerPage*3);i++){
			double sim = IRResult[i].PathDis.second;
			string path = IRResult[i].PathDis.first;
			string name = splitFileName(path);
			Mat tmp = imread(path.c_str());
			SimRe.push_back(sim);
			ShowImgs.push_back(tmp);
			ImgName.push_back(name);
		}
	}
	if(feaflags==DENSESURF_PRO_VLAD){
		if((pageIdx*ImgNumPerPage)>IRResult.size()){
			cout<<"��ʾͼƬ����"<<endl;
			return -1;
		}
		int start = (pageIdx-1)*ImgNumPerPage;
		for(int i = start;i<(start+ImgNumPerPage);i++){
			double sim = IRResult[i].PathDis.second;
			SimRe.push_back(sim);

			string path = IRResult[i].PathDis.first;
			string name = splitFileName(path);
			Mat tmp = imread(path.c_str());
			ShowImgs.push_back(tmp);
			vector<Mat> proTmp;
			proposalMat(tmp,proTmp);

			//���������Ƶ�proposal
			int qIdx = IRResult[i].IdxQuery;
			int DBIdx = IRResult[i].IdxDB;
			ShowImgs.push_back(QueryPro[qIdx]);
			ShowImgs.push_back(proTmp[DBIdx]);
			ImgName.push_back(name);
		}
	}
	return 1;
}