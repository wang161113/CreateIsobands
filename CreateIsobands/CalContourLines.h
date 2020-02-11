#pragma once
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;
//��������
class MeasureData
{
public:
	MeasureData(float x,float y, float z)
		:X(x),Y(y),Z(z) 
	{}

public:
	float X;  //����x
	float Y;  //����y
	float Z;  //�߶�
};

//����������е�����
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


//ͨ���߶ȵ����ȸ���------�ο�Marching squares�㷨
//�ο�����1��https://www.cnblogs.com/easymind223/p/3849481.html
//�ο�����2��https://blog.csdn.net/lweiyue/article/details/91490460
class CCalContourLines
{
public:
	enum Type
	{
		ISOBANDS = 0,
		ISOLINES
	};

	/*
	* �������ܣ�	CcontourLinesInfo ���캯��
	* ���������  vector<MeasureData> vecHeightDots ����̵߳㣬��������ܶ�ʱ����ķ�һ����ʱ��
	* ���������  double dImgW  ͼƬ��
	* ���������  double dImgH  ͼƬ��
	* ���������  int nGridW   //���ֵ������-----ֵԽС��ͼ��Խƽ������ʱԽ��
	* ���������  int nGridH   //���ֵ������-----ֵԽС��ͼ��Խƽ������ʱԽ��
	*/
	CCalContourLines(vector<MeasureData> vecHeightDots,double dImgW = 800.0, 
		double dImgH = 600.0, uint nGridW = 5, uint nGridH = 5);
	~CCalContourLines();

	/*
	* �������ܣ�	GetContourLines  ��ȡ�ȸ���
	* ���������  float fthreshold  �ȸ��ߵĸ߶�
	* ����ֵ  ��  vector<vector<Point> >  ���ش���threshold������ÿһ��vector<Point>��Ӧһ��С����
	*/
	vector<vector<Point> > GetContourLines(float fThreshold,Type type = ISOBANDS);

	//�߶ȵ���С���ֵ
	float m_fHeightMin;
	float m_fHeightMax;

private:
	CCalContourLines() {};
	Type m_type;

	//�����С
	uint m_nGridW;
	uint m_nGridH;

	//���ɵ�ͼ���С
	double m_dImgW;
	double m_dImgH;

	//������
	int m_nGridX;
	int m_nGridY;

	//��ɢ�߶ȵ��б�
	vector<MeasureData> m_vecHeightDots;
	//��ɫ�б�
	vector<int> ColorList;

	//�����е�����
	vector<vector<float> > m_GridData;


	//�������ɢ�߶ȵ�������СX\Y
	float m_maxX;
	float m_maxY;
	float m_minX;
	float m_minY;

private:

	//��ȡ�����СXY	
	void getHeightDotsRect(vector<MeasureData> vecHeightDots);
	//ת��HeightDots�����Ա����m_HeightDots
	void transfromHeightDot(vector<MeasureData> vecHeightDots);
	//���Ͳ�ֵ��������
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

