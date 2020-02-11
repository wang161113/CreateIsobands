#pragma once
#include "CalContourLines.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

class CDrawContourLines
{
public:
	CDrawContourLines(vector<MeasureData> vecHeightDots, double dImgW = 800.0,
		double dImgH = 600.0, int nGridW = 5, int nGridH = 5);

	CDrawContourLines();
	~CDrawContourLines();

	void test();

	/*
	* �������ܣ�	DrawImg
	* ���������  string strFilePath					���ɵĵȸ���ͼ����λ��
	* ���������  vector<MeasureData> vecHeightDots	����ĸ̵߳�
	* ���������  vector<float> vecThreshold			���Ƶȸ��ߵ�һ����ֵ(�߶�)
	* ���������  vector<int> vecColorIndex			vecColorIndex�ĸ�����Ҫ��vecThresholdһ��,RGB��ͨ�����ᱻ��Ϊͬһֵ
	* ���������  double dImgW						ͼƬ��
	* ���������  double dImgH						ͼƬ��
	* ���������  int nGridW							�ָ������ֵԽС��ͼ��Խ�⻬
	* ���������  int nGridH							�ָ�����ߣ�ֵԽС��ͼ��Խ�⻬
	* ����ֵ  ��  bool								�Ƿ�ɹ�
	*/
	bool DrawImg(string strFilePath, vector<MeasureData> vecHeightDots, vector<float> vecThreshold,
		vector<int> vecColorIndex, double dImgW = 800.0,
		double dImgH = 600.0, uint nGridW = 5, uint nGridH = 5);

	//scalar��ӦBGR
	bool DrawImg(string strFilePath, vector<MeasureData> vecHeightDots, vector<float> vecThreshold,
		vector<Scalar> vecColor, double dImgW = 800.0,
		double dImgH = 600.0, uint nGridW = 5, uint nGridH = 5);

	//����һ���ȸ���
	bool DrawImg(string strFilePath, vector<MeasureData> vecHeightDots, float threshold,
		Scalar color, double dImgW = 800.0,
		double dImgH = 600.0, uint nGridW = 5, uint nGridH = 5);

private:
	vector<MeasureData> m_vecHeightDots;
	//���ɵ�ͼ���С
	double m_dImgW;
	double m_dImgH;

	int m_nGridW;
	int m_nGridH;

	/*
	* �������ܣ�	Draw
	* ���������  vector<vector<Point> > shapes  ����������
	* ���������  int colorIndex 
	* ����ֵ  ��  void
	*/
	void DrawImg(Mat &img, vector<vector<Point> > vecShapes, Scalar color, bool bIsFill = true);
};

