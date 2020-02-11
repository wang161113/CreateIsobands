#include "pch.h"
#include "CalContourLines.h"
#include <iostream>
#include <time.h>

#define ISOLINES_RETURN if(m_type == ISOLINES) return;

IntMeasureData::IntMeasureData(MeasureData md, int x_num, int y_num)
{
	X = (int)(md.X * x_num);
	if (X >= x_num)
	{ 
		X = x_num - 1;
	}

	Y = (int)(md.Y * y_num);
	if (Y >= y_num)
	{
		Y = y_num - 1;
	}
	
	Z = md.Z;
}

IntMeasureData::~IntMeasureData()
{

}

//----------------------------------------------------------------------
//CcontourLinesInfo

CCalContourLines::CCalContourLines(vector<MeasureData> vecHeightDots, double dImgW /*= 800.0*/, 
	double dImgH /*= 600.0*/, uint nGridW /*= 10*/, uint nGridH /*= 10*/)
	: /*m_HeightDots(HeightDots),*/m_dImgW(dImgW),m_dImgH(dImgH),m_nGridW(nGridW),m_nGridH(nGridH)
{
	m_dImgH += nGridH;
	m_dImgW += nGridW;
	getHeightDotsRect(vecHeightDots);
	transfromHeightDot(vecHeightDots);
	linearInterpolation();
}

CCalContourLines::~CCalContourLines()
{
}


void CCalContourLines::getHeightDotsRect(vector<MeasureData> vecHeightDots)
{
	cout << "获取高程点最大最小X、Y..." << endl;
	m_maxX = vecHeightDots[0].X;
	m_minX = vecHeightDots[0].X;
	m_maxY = vecHeightDots[0].Y;
	m_minY = vecHeightDots[0].Y;

	for (int i = 0; i < vecHeightDots.size();i++)
	{
		MeasureData md = vecHeightDots[i];
		m_maxX = max(m_maxX, md.X);
		m_minX = min(m_minX, md.X);
		m_maxY = max(m_maxY, md.Y);
		m_minY = min(m_minY, md.Y);
	}
}

void CCalContourLines::transfromHeightDot(vector<MeasureData> vecHeightDots)
{
	cout << "转换高程点..." << endl;
	float fWidth = m_maxX - m_minX;
	float fHeight = m_maxY - m_minY;

	for (int i = 0; i < vecHeightDots.size(); i++)
	{
		MeasureData md = vecHeightDots[i];
		m_vecHeightDots.push_back(MeasureData((md.X - m_minX) / fWidth, (md.Y - m_minY) / fHeight, md.Z));
	}
}

void  CCalContourLines::linearInterpolation()
{
	cout << "线性插值..." << endl;

	clock_t start, end;
	start = clock();

	vector<IntMeasureData> measure_data;

	m_nGridX = (int)(m_dImgW / m_nGridW);
	m_nGridY = (int)(m_dImgH / m_nGridH);

	m_GridData.resize(m_nGridX);
	for (int i = 0; i < m_vecHeightDots.size(); i++)
	{
		measure_data.push_back(IntMeasureData(m_vecHeightDots[i], m_nGridX, m_nGridY));
	}
	
	for (int i = 0; i < m_nGridX; i++)
	{
		m_GridData[i].resize(m_nGridY);

		for (int j = 0; j < m_nGridY; j++)
		{
			float fValue = 0;

			float fTotalValue = 0;
			int iSize = 0;

			bool bFind = false;
			//当数据量超过像素的个数时，由于IntMeasureData的取整操作，
			//在同一位置(x,y)可能会有多个值，所以在这里取均值。fTotalValue / iSize;
			for (int k = 0; k < measure_data.size(); k++)
			{
				IntMeasureData imd = measure_data[k];
				if (i == imd.X && j == imd.Y)
				{
					fTotalValue += imd.Z;
					iSize++;
					bFind = true;
					//break;
				}
			}
			fValue = fTotalValue / iSize;

			if (!bFind)
			{
				double D = 0;
				double DV = 0;
				for (int k = 0; k < measure_data.size(); k++)
				{
					IntMeasureData imd = measure_data[k];
					double dDistReciprocal = 1.0 / (pow(double(imd.X - i), 2) + pow(double(imd.Y - j), 2));
					D += dDistReciprocal;
					DV += imd.Z * dDistReciprocal;
				}
				fValue = (float)(DV / D);
			}
			m_GridData[i][j] = fValue;

			if (i != 0 || j != 0)
			{
				m_fHeightMin = min(m_fHeightMin, fValue);
				m_fHeightMax = max(m_fHeightMax, fValue);
			}
			else
			{
				m_fHeightMin = fValue;
				m_fHeightMax = fValue;
			}

		}
	}
	
	end = clock();
	cout << "---耗时：" << ((float)(end - start)) / CLK_TCK << endl;
}


std::vector<std::vector<Point> > CCalContourLines::GetContourLines(float fThreshold, Type type /*= ISOBANDS*/)
{
	cout << "获取等高线:" << fThreshold << endl;
	
	m_type = type;
	clock_t start, end;
	start = clock();

	vector<vector<unsigned char> > binary_data(m_nGridX);
	for (int i = 0; i < m_nGridX; i++)
	{
		binary_data[i].resize(m_nGridY);
		for (int j = 0; j < m_nGridY; j++)
		{
			binary_data[i][j] = (unsigned char)((m_GridData[i][j] >= fThreshold) ? 1 : 0);
		}
	}

	vector<vector<Point> > vecShapes;
	for (int i = 1; i < m_nGridX; i++)
	{
		for (int j = 1; j < m_nGridY; j++)
		{
			int num = (binary_data[i - 1][j - 1] << 3) + (binary_data[i][j - 1] << 2) + (binary_data[i][j] << 1) + binary_data[i - 1][j];
			vector<Point> vecPts;
			switch (num)
			{
			case 0:
				break;
			case 1:
				AddLeft(vecPts, i, j, fThreshold);
				AddLeftBottom(vecPts, i, j);
				AddBottom(vecPts, i, j, fThreshold);
				break;
			case 2:
				AddRight(vecPts, i, j, fThreshold);
				AddRightBottom(vecPts, i, j);
				AddBottom(vecPts, i, j, fThreshold);
				break;
			case 3:
				AddLeft(vecPts, i, j, fThreshold);
				AddLeftBottom(vecPts, i, j);
				AddRightBottom(vecPts, i, j);
				AddRight(vecPts, i, j, fThreshold);
				break;
			case 4:
				AddTop(vecPts, i, j, fThreshold);
				AddRightTop(vecPts, i, j);
				AddRight(vecPts, i, j, fThreshold);
				break;
			case 5:
				AddLeft(vecPts, i, j, fThreshold);
				AddRightTop(vecPts, i, j);
				AddRight(vecPts, i, j, fThreshold);
				AddBottom(vecPts, i, j, fThreshold);
				AddLeftBottom(vecPts, i, j);
				AddLeft(vecPts, i, j, fThreshold);
				break;
			case 6:
				AddTop(vecPts, i, j, fThreshold);
				AddRightTop(vecPts, i, j);
				AddRightBottom(vecPts, i, j);
				AddBottom(vecPts, i, j, fThreshold);
				break;
			case 7:
				AddTop(vecPts, i, j, fThreshold);
				AddRightTop(vecPts, i, j);
				AddRightBottom(vecPts, i, j);
				AddLeftBottom(vecPts, i, j);
				AddLeft(vecPts, i, j, fThreshold);
				break;
			case 8:
				AddTop(vecPts, i, j, fThreshold);
				AddLeftTop(vecPts, i, j);
				AddLeft(vecPts, i, j, fThreshold);
				break;
			case 9:
				AddTop(vecPts, i, j, fThreshold);
				AddLeftTop(vecPts, i, j);
				AddLeftBottom(vecPts, i, j);
				AddBottom(vecPts, i, j, fThreshold);
				break;
			case 10:
				AddTop(vecPts, i, j, fThreshold);
				AddLeftTop(vecPts, i, j);
				AddLeft(vecPts, i, j, fThreshold);
				AddBottom(vecPts, i, j, fThreshold);
				AddRightBottom(vecPts, i, j);
				AddRight(vecPts, i, j, fThreshold);
				break;
			case 11:
				AddTop(vecPts, i, j, fThreshold);
				AddLeftTop(vecPts, i, j);
				AddLeftBottom(vecPts, i, j);
				AddRightBottom(vecPts, i, j);
				AddRight(vecPts, i, j, fThreshold);
				break;
			case 12:
				AddLeft(vecPts, i, j, fThreshold);
				AddRight(vecPts, i, j, fThreshold);
				AddRightTop(vecPts, i, j);
				AddLeftTop(vecPts, i, j);
				break;
			case 13:
				AddRight(vecPts, i, j, fThreshold);
				AddRightTop(vecPts, i, j);
				AddLeftTop(vecPts, i, j);
				AddLeftBottom(vecPts, i, j);
				AddBottom(vecPts, i, j, fThreshold);
				break;
			case 14:
				AddLeft(vecPts, i, j, fThreshold);
				AddBottom(vecPts, i, j, fThreshold);
				AddRightBottom(vecPts, i, j);
				AddRightTop(vecPts, i, j);
				AddLeftTop(vecPts, i, j);
				break;
			case 15:
				AddLeftTop(vecPts, i, j);
				AddRightTop(vecPts, i, j);
				AddRightBottom(vecPts, i, j);
				AddLeftBottom(vecPts, i, j);
				break;
			}
			if (num != 0)
			{
				vecShapes.push_back(vecPts);
			}
		}
	}

	end = clock();
	cout << "----耗时：" << ((float)(end - start)) / CLK_TCK << endl;
	return vecShapes;
}





float CCalContourLines::V(float min, float max, float fthreshold)
{
	return (fthreshold - min) / (max - min);
}

void CCalContourLines::AddLeft(vector<Point> &list, int x, int y, float fthreshold)
{
	list.push_back(Point((x - 1) * m_nGridW, (y - 1 + V(m_GridData[x - 1][y - 1], m_GridData[x - 1][y], fthreshold)) * m_nGridH));
}

void CCalContourLines::AddRight(vector<Point> &list, int x, int y, float fthreshold)
{
	list.push_back(Point(x * m_nGridW, (y - 1 + V(m_GridData[x][y - 1], m_GridData[x][y], fthreshold)) * m_nGridH));
}

void CCalContourLines::AddTop(vector<Point> &list, int x, int y, float fthreshold)
{
	list.push_back(Point((x - 1 + V(m_GridData[x - 1][y - 1], m_GridData[x][y - 1], fthreshold)) * m_nGridW, (y - 1) * m_nGridH));
}

void CCalContourLines::AddBottom(vector<Point> &list, int x, int y, float fthreshold)
{
	list.push_back(Point((x - 1 + V(m_GridData[x - 1][y], m_GridData[x][y], fthreshold)) * m_nGridW, y * m_nGridH));
}

void CCalContourLines::AddLeftTop(vector<Point> &list, int x, int y)
{
	ISOLINES_RETURN
	list.push_back(Point((x - 1) * m_nGridW, (y - 1) * m_nGridH));
}

void CCalContourLines::AddRightTop(vector<Point> &list, int x, int y)
{
	ISOLINES_RETURN
	list.push_back(Point(x * m_nGridW, (y - 1) * m_nGridH));
}

void CCalContourLines::AddLeftBottom(vector<Point> &list, int x, int y)
{
	ISOLINES_RETURN
	list.push_back(Point((x - 1) * m_nGridW, y * m_nGridH));
}

void CCalContourLines::AddRightBottom(vector<Point> &list, int x, int y)
{
	ISOLINES_RETURN
	list.push_back(Point(x * m_nGridW, y * m_nGridH));
}