// CreateIsobands.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "DrawContourLines.h"
#include "opencv2/opencv.hpp"
using namespace std;
#define _CRT_SECURE_NO_WARNINGS

void GetHeightDotsFromFile(string fileName, vector<MeasureData> &HeightDots)
{
	clock_t clkStart, clkEnd;
	clkStart = clock();

	ifstream inFile(fileName);
	string strLine;

	while (getline(inFile, strLine))
	{
		// 打印整行字符串  
		cout << strLine << endl;

		stringstream ss(strLine);
		string str;
		vector<string> vecLineStr;
		// 按照逗号分隔  
		while (getline(ss, str, ','))
			vecLineStr.push_back(str);

		HeightDots.push_back(MeasureData(atof(vecLineStr[0].c_str()), 
			atof(vecLineStr[1].c_str()), atof(vecLineStr[2].c_str())));
	}

	inFile.close();
	clkEnd = clock();
	cout << "读取数据耗时：" << (clkEnd - clkStart) / CLK_TCK << endl;
}

int main()
{
  	vector<MeasureData> vecHeightDots;
	vecHeightDots.push_back(MeasureData(0, 0, 8));
	vecHeightDots.push_back(MeasureData(0, 1, 9));
	vecHeightDots.push_back(MeasureData(0, 2, 7));
	vecHeightDots.push_back(MeasureData(0, 3, 6));
	vecHeightDots.push_back(MeasureData(0, 4, 3));

	vecHeightDots.push_back(MeasureData(1, 0, 7));
	vecHeightDots.push_back(MeasureData(1, 1, 3));
	vecHeightDots.push_back(MeasureData(1, 2, 5));
	vecHeightDots.push_back(MeasureData(1, 3, 3));
	vecHeightDots.push_back(MeasureData(1, 4, 2));

	vecHeightDots.push_back(MeasureData(2, 0, 8));
	vecHeightDots.push_back(MeasureData(2, 1, 1));
	vecHeightDots.push_back(MeasureData(2, 2, 7));
	vecHeightDots.push_back(MeasureData(2, 3, 8));
	vecHeightDots.push_back(MeasureData(2, 4, 4));

	vecHeightDots.push_back(MeasureData(3, 0, 8));
	vecHeightDots.push_back(MeasureData(3, 1, 6));
	vecHeightDots.push_back(MeasureData(3, 2, 4));
	vecHeightDots.push_back(MeasureData(3, 3, 2));
	vecHeightDots.push_back(MeasureData(3, 4, 6));

	vecHeightDots.push_back(MeasureData(4, 0, 9));
	vecHeightDots.push_back(MeasureData(4, 1, 8));
	vecHeightDots.push_back(MeasureData(4, 2, 3));
	vecHeightDots.push_back(MeasureData(4, 3, 7));
	vecHeightDots.push_back(MeasureData(4, 4, 6));
// 
// 	string strFileName = "..\\重庆DEM\\重庆_高程点数据.csv"; //数据量很大会花费10分钟左右
//	GetHeightDotsFromFile(strFileName, vecHeightDots);

	CDrawContourLines drawCL(vecHeightDots,800,600,5,5);
	drawCL.test();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
