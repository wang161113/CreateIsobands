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
	* 函数介绍：	DrawImg
	* 输入参数：  string strFilePath					生成的等高线图保存位置
	* 输入参数：  vector<MeasureData> vecHeightDots	传入的高程点
	* 输入参数：  vector<float> vecThreshold			绘制等高线的一组阈值(高度)
	* 输入参数：  vector<int> vecColorIndex			vecColorIndex的个数需要与vecThreshold一致,RGB三通道将会被设为同一值
	* 输入参数：  double dImgW						图片宽
	* 输入参数：  double dImgH						图片高
	* 输入参数：  int nGridW							分割网格宽，值越小，图像越光滑
	* 输入参数：  int nGridH							分割网格高，值越小，图像越光滑
	* 返回值  ：  bool								是否成功
	*/
	bool DrawImg(string strFilePath, vector<MeasureData> vecHeightDots, vector<float> vecThreshold,
		vector<int> vecColorIndex, double dImgW = 800.0,
		double dImgH = 600.0, uint nGridW = 5, uint nGridH = 5);

	//scalar对应BGR
	bool DrawImg(string strFilePath, vector<MeasureData> vecHeightDots, vector<float> vecThreshold,
		vector<Scalar> vecColor, double dImgW = 800.0,
		double dImgH = 600.0, uint nGridW = 5, uint nGridH = 5);

	//绘制一条等高线
	bool DrawImg(string strFilePath, vector<MeasureData> vecHeightDots, float threshold,
		Scalar color, double dImgW = 800.0,
		double dImgH = 600.0, uint nGridW = 5, uint nGridH = 5);

private:
	vector<MeasureData> m_vecHeightDots;
	//生成的图像大小
	double m_dImgW;
	double m_dImgH;

	int m_nGridW;
	int m_nGridH;

	/*
	* 函数介绍：	Draw
	* 输入参数：  vector<vector<Point> > shapes  封闭区域点组
	* 输入参数：  int colorIndex 
	* 返回值  ：  void
	*/
	void DrawImg(Mat &img, vector<vector<Point> > vecShapes, Scalar color, bool bIsFill = true);
};

