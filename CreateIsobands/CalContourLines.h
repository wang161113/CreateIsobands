#pragma once
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;
//测量数据
class MeasureData
{
public:
	MeasureData(float x,float y, float z)
		:X(x),Y(y),Z(z) 
	{}

public:
	float X;  //坐标x
	float Y;  //坐标y
	float Z;  //高度
};

//存放在网格中的数据
class IntMeasureData
{
public:
	IntMeasureData(MeasureData md, int x_num, int y_num);
	~IntMeasureData();


public:
	int X;
	int Y;
	float Z;
};


//通过高度点计算等高线------参考Marching squares算法
//参考链接1：https://www.cnblogs.com/easymind223/p/3849481.html
//参考链接2：https://blog.csdn.net/lweiyue/article/details/91490460
class CCalContourLines
{
public:
	enum Type
	{
		ISOBANDS = 0,
		ISOLINES
	};

	/*
	* 函数介绍：	CcontourLinesInfo 构造函数
	* 输入参数：  vector<MeasureData> vecHeightDots 传入高程点，点的数量很多时，会耗费一定的时间
	* 输入参数：  double dImgW  图片宽
	* 输入参数：  double dImgH  图片高
	* 输入参数：  int nGridW   //区分的网格宽-----值越小，图像越平滑，耗时越久
	* 输入参数：  int nGridH   //区分的网格高-----值越小，图像越平滑，耗时越久
	*/
	CCalContourLines(vector<MeasureData> vecHeightDots,double dImgW = 800.0, 
		double dImgH = 600.0, uint nGridW = 5, uint nGridH = 5);
	~CCalContourLines();

	/*
	* 函数介绍：	GetContourLines  获取等高线
	* 输入参数：  float fthreshold  等高线的高度
	* 返回值  ：  vector<vector<Point> >  返回大于threshold的区域，每一个vector<Point>对应一块小区域
	*/
	vector<vector<Point> > GetContourLines(float fThreshold,Type type = ISOBANDS);

	//高度的最小最大值
	float m_fHeightMin;
	float m_fHeightMax;

private:
	CCalContourLines() {};
	Type m_type;

	//网格大小
	uint m_nGridW;
	uint m_nGridH;

	//生成的图像大小
	double m_dImgW;
	double m_dImgH;

	//网格数
	int m_nGridX;
	int m_nGridY;

	//离散高度点列表
	vector<MeasureData> m_vecHeightDots;
	//颜色列表
	vector<int> ColorList;

	//网格中的数据
	vector<vector<float> > m_GridData;


	//传入的离散高度点的最大最小X\Y
	float m_maxX;
	float m_maxY;
	float m_minX;
	float m_minY;

private:

	//获取最大最小XY	
	void getHeightDotsRect(vector<MeasureData> vecHeightDots);
	//转换HeightDots存入成员变量m_HeightDots
	void transfromHeightDot(vector<MeasureData> vecHeightDots);
	//线型插值修正轮廓
	void linearInterpolation();


	float V(float min, float max, float fthreshold);
	void AddLeft(vector<Point> &list, int x, int y, float fthreshold);
	void AddRight(vector<Point> &list, int x, int y, float fthreshold);
	void AddTop(vector<Point> &list, int x, int y, float fthreshold);
	void AddBottom(vector<Point> &list, int x, int y, float fthreshold);
	void AddLeftTop(vector<Point> &list, int x, int y);
	void AddRightTop(vector<Point> &list, int x, int y);
	void AddLeftBottom(vector<Point> &list, int x, int y);
	void AddRightBottom(vector<Point> &list, int x, int y);
};

