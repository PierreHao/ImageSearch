#include "Utils.h"
#include <fstream>
#include <io.h>
using namespace std;
using namespace cv;

void getFiles( string path, vector<string>& files )
{
	//�ļ����
	long   hFile   =   0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

string intToString(int i) {
	stringstream os;
	os << i;
	return os.str();
}

string splitFileName(string file) {
	size_t start = file.find_last_of("/\\") + 1;
	//size_t end = file.find_last_of(".");
	return file.substr(start);
}

int computeProposalNums(string imageFilePath) {
	ifstream imageInfo(imageFilePath.c_str());
	int imageNum = 0;
	imageInfo >> imageNum;
	int allProposalNum = 0;
	for (int i = 0; i < imageNum; ++i) {
		string path;
		int proposalNum = 0;
		imageInfo >> path >> proposalNum;
		allProposalNum += proposalNum;
	}
	return allProposalNum;
}

float* newArray(int col) {
	float* array = new float[col];
	memset(array, 0, col*sizeof(float));
	return array;
}

void freeArray(float* array) {
	if (!array) delete[] array;
	array = NULL;
}

void freeVectorArray(vector<float*>& arrays) {
	int n = arrays.size();
	for (int i = 0; i < n; ++i) {
		delete[] arrays[i];
		arrays[i] = NULL;
	}
}

float** newMatrix(int row, int col) {
	float **Array = new float *[row];
	for(int r = 0; r < row; r++)
	{
		Array[r] = new float[col];
		memset(Array[r], 0, col*sizeof(float));
	}
	return Array;
}

void freeMatrix(float** matrix, int row) {
	for (int r = 0; r < row; ++r) delete[] matrix[r];
	delete [] matrix;
	matrix = NULL;
}

void freeFeatureMap(unordered_map<string, vector<float*> >& imageMap) {
	for(auto& image : imageMap) {
		int n = image.second.size();
		for (int i = 0; i < n; ++i) {
			delete [] image.second[i];
			image.second[i] = NULL;
		}
	}
}

void featureBinToMat(string featureFile, Mat& trainSet, int row, int col) {
	FILE* fid;
	fid = fopen(featureFile.c_str(), "rb");
	for (int r = 0; r < row; ++r) {
		for (int c = 0; c < col; ++c) {
			fread(&trainSet.at<float>(r, c), sizeof(float), 1, fid);
		}
	}
	fclose(fid);
}

void loadFeatureBin(string trainFile, 
						   string featureFile, 
						   int featureSize,
						   unordered_map<string, vector<float*> >& featureMap,
						   bool head) {
	ifstream imageInfo(trainFile.c_str());
	FILE* fid;
	fid = fopen(featureFile.c_str(), "rb");

#if DEBUG
	ofstream outfile("..\\..\\loadFeatureFile.txt");
#endif
	// headΪtrue�������ļ�ͷ������ֵ
	if (head) {
		int row, col;
		fread(&row, sizeof(int), 1, fid);
		fread(&col, sizeof(int), 1, fid);
		cout << row << " , " << col << endl;
	}

	int imgSize;
	imageInfo >> imgSize;
	for (int i = 0; i < imgSize; ++i) {
		string imagePath;
		int proposalsNum;
		imageInfo >> imagePath >> proposalsNum;
#if DEBUG
		outfile << splitFileName(imagePath) << endl;
		outfile << proposalsNum << endl;
#endif
		for (int r = 0; r < proposalsNum; ++r) {
			float* feature = newArray(featureSize);
			for (int c = 0; c < featureSize; ++c) {
				fread(&feature[c], sizeof(float),1, fid);
			}
#if DEBUG
			for (int c = 0; c < featureSize; ++c) {
				outfile << feature[c] << " ";
			}
			outfile << endl;
#endif
			featureMap[imagePath].push_back(feature);
		}
	}
	imageInfo.close();
	fclose(fid);
}

void loadVocabulary( string file, float** matrix, int row, int col) {
	ifstream infile(file.c_str());
	if(!infile) {
		cout<<"open file failed: "<< file << endl;
		return;
	}
	for(int r = 0; r < row; r++) {
		for(int c = 0; c < col; c++) {
			infile >> matrix[r][c];
		}
	}
	infile.close();
}
void loadPCAMatrix(string pcaFile, float** matrix, int row_, int col_) {
	FILE* fid;
	fid = fopen(pcaFile.c_str(), "rb");
	int row, col;
	fread(&row, sizeof(int), 1, fid);
	fread(&col, sizeof(int), 1, fid);
	assert(row == row_);
	assert(col == col_);
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			fread(&matrix[i][j], sizeof(float), 1, fid);
		}
	}
	fclose(fid);
}


void loadRotationMatrix(string rotateMatrixFile, float** matrix, int row_, int col_) {
	FILE* fid;
	fid = fopen(rotateMatrixFile.c_str(), "rb");
	int row, col;
	fread(&row, sizeof(int), 1, fid);
	fread(&col, sizeof(int), 1, fid);
	assert(row == row_);
	assert(col == col_);
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			fread(&matrix[i][j], sizeof(float), 1, fid);
		}
	}
	fclose(fid);
}


void loadTrainFeatures(string imageNameFile, 
					   string featureBinFile, 
					   vector<pair<float*, string> >& trainFeatures) {
	ifstream imgNameFin(imageNameFile);
	FILE* fid;
	fid = fopen(featureBinFile.c_str(), "rb");
	int imageNum;
	imgNameFin >> imageNum;
	int row, col;
	fread(&row, sizeof(int), 1, fid);
	fread(&col, sizeof(int), 1, fid);
	assert(row == imageNum);
	assert(col == featureAfterPCASize);
	for (int i = 0; i < row; ++i) {
		string name;
		imgNameFin >> name;
		float* feature = new float[col];
		for (int j = 0; j < col; ++j) {
			fread(&feature[j], sizeof(float), 1, fid);
		}
		trainFeatures.push_back(make_pair(feature, name));
	}
	imgNameFin.close();
	fclose(fid);
}


void loadTrainBitFeatures(string imageNameFile, 
						  string featureBinFile, 
						  vector<pair<bitvec, string> > trainFeatures) {
	ifstream imgNameFin(imageNameFile);
	FILE* fid;
	fid = fopen(featureBinFile.c_str(), "rb");
	int imageNum;
	imgNameFin >> imageNum;
	int row, col;
	fread(&row, sizeof(int), 1, fid);
	fread(&col, sizeof(int), 1, fid);
	assert(row == imageNum);
	assert(col == featureAfterPCASize / nbits);
	for (int i = 0; i < row; ++i) {
		string name;
		imgNameFin >> name;
		bitvec bvs;
		for (int j = 0; j < col; ++j) {
			int num;
			fread(&num, sizeof(int), 1, fid);
			bvs.push_back(num);
		}
		trainFeatures.push_back(make_pair(bvs, name));
	}
	imgNameFin.close();
	fclose(fid);
}

void feature2Bin(float* feature, int featureSize, FILE* fin) {
	for (int i = 0; i < featureSize; ++i) {
		fwrite(feature + i, sizeof(float), 1, fin);
	}
}

void mat2Bin(const char* filename, const Mat& src, int row, int col) {
	FILE* fin;
	fin = fopen(filename, "wb");
	fwrite(&row, sizeof(int), 1, fin);
	fwrite(&col, sizeof(int), 1, fin);
	for (int r = 0; r < row; ++r) {
		for (int c = 0; c < col; ++c) {
			fwrite(&(src.at<float>(r, c)), sizeof(float), 1, fin);
		}
	}
	fclose(fin);
}

void bin2Mat(const char* filename, Mat& dst) {
	FILE* fid;
	fid = fopen(filename, "rb");
	int row;
	int col;
	fread(&row, sizeof(int), 1, fid);
	fread(&col, sizeof(int), 1, fid);
	assert(row == dst.rows);
	assert(col == dst.cols);
	dst = Mat::zeros(row,col, CV_32F);
	for (int r = 0; r < row; ++r) {
		for (int c = 0; c < col; ++c) {
			fread(&(dst.at<float>(r, c)), sizeof(float), 1, fid);
		}
	}
	fclose(fid);
}