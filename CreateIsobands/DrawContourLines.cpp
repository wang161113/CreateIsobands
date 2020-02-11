#include "pch.h"
#include "DrawContourLines.h"
#include <iostream>


CDrawContourLines::CDrawContourLines(vector<MeasureData> vecHeightDots, double dImgW /*= 800.0*/, double dImgH /*= 600.0*/, 
	int nGridW /*= 10*/, int nGridH /*= 10*/)
	: m_vecHeightDots(vecHeightDots), m_dImgW(dImgW), m_dImgH(dImgH),
	m_nGridW(nGridW),m_nGridH(nGridH)
{
}

CDrawContourLines::CDrawContourLines()
{
}

CDrawContourLines::~CDrawContourLines()
{
}

void CDrawContourLines::test()
{
	if (m_vecHeightDots.size() == 0)
	{
		return;
	}

	if (m_dImgH <= 0 || m_dImgW <= 0)
	{
		return;
	}

	CCalContourLines calCLine(m_vecHeightDots, m_dImgW, m_dImgH,m_nGridW,m_nGridH);
	float fHeightMax = calCLine.m_fHeightMax;
	float fHeightMin = calCLine.m_fHeightMin;

	float fGap = (fHeightMax - fHeightMin) / 5;

	cout << "高度最低值：" << fHeightMin << endl;
	cout << "高度最高值：" << fHeightMax << endl;
	cout << "等高线高度间隔：" << fGap << endl;


	Mat img(m_dImgH, m_dImgW, CV_8UC3, Scalar(255, 255, 255));
	for (int i = 1; i < 4; i++)
	{
		vector<vector<Point> > shapes = calCLine.GetContourLines(fHeightMin + fGap * i,CCalContourLines::ISOBANDS);
		DrawImg(img,shapes, Scalar(255/(i+1), 255/(i+1), 255/(i+1)),true);
	}

// 	vector<vector<Point> > shapes = calCLine.GetContourLines(7,CCalContourLines::ISOLINES);
// 	DrawImg(img,shapes, 100,false);

	imwrite("C:\\Users\\Albert\\Desktop\\output翻转前.png", img);
	//翻转图片
	flip(img, img, 0);
	//保存图片
	imwrite("C:\\Users\\Albert\\Desktop\\output.png", img);

	imshow("等高线", img);
	waitKey(0);

}

bool CDrawContourLines::DrawImg(string strFilePath, vector<MeasureData> vecHeightDots,
	vector<float> vecThreshold, vector<int> vecColorIndex,
	double dImgW /*= 800.0*/, double dImgH /*= 600.0*/, uint nGridW /*= 5*/, uint nGridH /*= 5*/)
{
	vector<Scalar> vecColor;
	for (int i =0; i < vecColorIndex.size();i++)
	{
		vecColor.push_back(Scalar(vecColorIndex[i], vecColorIndex[i], vecColorIndex[i]));
	}
	return DrawImg(strFilePath, vecHeightDots, vecThreshold, vecColor, dImgW, dImgH, nGridW, nGridH);
}

void CDrawContourLines::DrawImg(Mat &img, vector<vector<Point> > vecShapes, Scalar color,bool bIsFill)
{
	for (int i=0; i < vecShapes.size(); i++)
	{
		vector<Point> Pts = vecShapes[i];
		if (bIsFill)
		{
			fillConvexPoly(img, Pts, color);
		}
		else
		{

			polylines(img, Pts, true, color);
		}
	}
}

bool CDrawContourLines::DrawImg(string strFilePath, vector<MeasureData> vecHeightDots, 
	vector<float> vecThreshold, vector<Scalar> vecColor, double dImgW /*= 800.0*/,
	double dImgH /*= 600.0*/, uint nGridW /*= 5*/, uint nGridH /*= 5*/)
{
	if (vecHeightDots.size() == 0 || vecThreshold.size() != vecColor.size())
	{
		return false;
	}

	CCalContourLines calCLine(vecHeightDots, dImgW, dImgH, nGridW, nGridH);

	Mat img;
	sort(vecThreshold.begin(), vecThreshold.end());
	//先画低阈值的，再画高阈值的，不然会乱
	for (int i = 0; i < vecThreshold.size(); i++)
	{
		vector<vector<Point> > shapes = calCLine.GetContourLines(vecThreshold[i]);
		//draw contourLines
		DrawImg(img, shapes, vecColor[i]);
	}

	//翻转图片 opencv坐标原点在上角，所以需要X轴翻转
	flip(img, img, 0);
	//save img
	imwrite(strFilePath, img);

	return true;
}

bool CDrawContourLines::DrawImg(string strFilePath, vector<MeasureData> vecHeightDots,
	float threshold, Scalar color, double dImgW /*= 800.0*/, 
	double dImgH /*= 600.0*/, uint nGridW /*= 5*/, uint nGridH /*= 5*/)
{
	vector<float> vecThreshold;
	vecThreshold.push_back(threshold);

	vector<Scalar> vecColor;
	vecColor.push_back(color);

	return DrawImg(strFilePath, vecHeightDots, vecThreshold, vecColor, dImgW, dImgH, nGridW, nGridH);
}

