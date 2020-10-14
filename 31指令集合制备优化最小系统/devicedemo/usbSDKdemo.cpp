// usbSDKdemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<bits/stdc++.h>
#include<fstream>
#include<Windows.h>
#include<thread>
#include"ComputeCharacter.h"
#include"svm_predict.h"
#include"svm_train.h"
#include"FrontDistinguish.h"

using namespace std;

USB_INFO g_usbInfo;
DEVICE_INFO g_deviceInfo;
int nRet = 0;

int NumberToday = 0, correctToday = 0, wrongToday = 0;
int TotalNumber = 0, TotalCorrect = 0, TotalWrong = 0;
double AccuracyToday = 0.0, TotalAccuracy = 0.0;

double time_start, time_end;
double write_start_time = 0, write_end_time = 0;
double CenterX = 0, CenterY = 0;
int lengthX = 0, lengthY = 0;
vector<PEN_INFO> res;
vector<PEN_INFO> firstGesture;
int count1 = 1;
int bihua = 0;
int tag = 1;
int label = 0;
int gestureCount = 1;
int sampleCount = 0;
bool judgeRecognizeResult = false;
string strDevname;
string strPID;
string strVID;

//识别输入的正确手势名称
bool IsCorrectGestureName(const string &gestureName)
{
	int n = gestureName.size();
	if (n <= 0 || n > 3)
	{
		return false;
	}
	else
	{
		if (gestureName[0] == '0' && n == 1)
		{
			label = 0;
			return true;
		}
		else if (gestureName[0] == '-')
		{
			if (gestureName[1] >= '1' && gestureName[1] <= '9')
			{
				if (n == 2)
				{
					label = gestureName[1] - '0';
					return true;
				}
				else
				{
					label = gestureName[1] - '0';
					label = label * 10 + (gestureName[2] - '0');
					return true;
				}
			}
		}
	}
	return false;
}

//显示正确率
void PrintStatisticalResults()
{
	TotalNumber++;
	NumberToday++;
	TotalAccuracy = (double)TotalCorrect / (double)TotalNumber;
	AccuracyToday = (double)correctToday / (double)NumberToday;
	cout << "********************手势指令识别统计结果********************" << endl;

	cout << "*【手势指令总计识别次数】：" << TotalNumber << "                               " << endl;
	cout << "*【手势指令总计识别正确次数】：" << TotalCorrect << "                           " << endl;
	cout << "*【手势指令总计识别错误次数】：" << TotalWrong << "                           " << endl;
	cout << "*【手势指令总计识别正确率】：" << TotalAccuracy * 100 << "%" << "                          " << endl;
	cout << "*                                                          " << endl;
	cout << "*【手势指令今日识别次数】：" << NumberToday << "                               " << endl;
	cout << "*【手势指令今日识别正确次数】：" << correctToday << "                           " << endl;
	cout << "*【手势指令今日识别错误次数】：" << wrongToday << "                           " << endl;
	cout << "*【手势指令今日识别正确率】：" << AccuracyToday * 100 << "%" << "                          " << endl;

	cout << "************************************************************" << endl << endl;
}

//存储采集的数据
void SaveCollectData()
{
	ofstream outfile;


	if (res.size() > 0 || firstGesture.size() > 0)
	{
		if (bihua == 3)
		{
			outfile.open("data\\collectData\\collectDataStroke3.txt", ios::app);
		}
		else if (bihua == 4)
		{
			outfile.open("data\\collectData\\collectDataStroke4.txt", ios::app);
		}
		else
		{
			outfile.open("data\\collectData\\collectDataStroke12.txt", ios::app);
		}
		outfile << label << " ";

		if (bihua == 1)
		{
			outfile << bihua << " ";
			for (int i = 0;i < firstGesture.size();++i)
			{
				outfile << firstGesture[i].nX << " " << firstGesture[i].nY << " ";
			}
		}
		else
		{
			outfile << bihua << " ";
			for (int i = 0;i < res.size();++i)
			{
				outfile << res[i].nX << " " << res[i].nY << " ";
			}
		}
		outfile << endl;
		outfile.close();
	}
}

//采集数据
void collectionData()
{
	ofstream outfile;
	if (res.size() > 0)
	{
		outfile.open("test20200909.txt", ios::app);

		outfile << tag << " ";
		for (int i = 0;i < res.size();++i)
		{
			outfile << res[i].nX << " " << res[i].nY << " ";
		}
		outfile << endl;
		res.clear();
		outfile.close();

		cout << "tag:" << tag << "  " << "count:" << count1 << endl;
		if (count1 % 10 == 0)
		{
			tag++;
			count1 = 0;
			cout << "*************************end********************" << endl;
		}
		count1++;
	}
}

//将点信息写入point.txt
void writeToPoint()
{
	ofstream outfile;
	if (res.size() > 0 || firstGesture.size() > 0)
	{
		outfile.open("data\\point\\point.txt");
		outfile << tag << " ";
		
		if (bihua == 1)
		{
			outfile << bihua << " ";
			for (int i = 0;i < firstGesture.size();++i)
			{
				outfile << firstGesture[i].nX << " " << firstGesture[i].nY << " ";
			}
		}
		else
		{
			outfile << bihua << " ";
			for (int i = 0;i < res.size();++i)
			{
				outfile << res[i].nX << " " << res[i].nY << " ";
			}
		}
		outfile << endl;

		outfile.close();
	}
}

//计算实时数据特征
void calCharacter()
{
	ifstream infile;
	ofstream outfile;
	string s;

	infile.open("data\\point\\point.txt");
	outfile.open("data\\point\\pointCharacter.txt");

	getline(infile, s);

	ComputeCharacter cc(s);
	cc.Compute();

	CenterX = cc.CentroidX;
	CenterY = cc.CentroidY;
	lengthX = cc.xLength;
	lengthY = cc.yLength;

	outfile << cc.tag << " ";
	outfile << "0:" << cc.XYRatio << " ";
	outfile << "1:" << cc.strokeCount << " ";
	outfile << "2:" << cc.closeness << " ";
	outfile << "3:" << cc.Compactness << " ";
	outfile << "4:" << cc.curvature << " ";
	outfile << "5:" << cc.beginDirection << " ";
	outfile << "6:" << cc.endDirection << " ";
	outfile << "7:" << cc.xStartMaxLengthRatio << " ";
	outfile << "8:" << cc.yStartMaxLengthRatio << " ";
	outfile << "9:" << cc.xEndMaxLengthRatio << " ";
	outfile << "10:" << cc.yEndMaxLengthRatio << " ";
	outfile << "11:" << cc.xStartMinLengthRatio << " ";
	outfile << "12:" << cc.yStartMinLengthRatio <<" ";
	outfile << "13:" << cc.InflectionPointsNumber;
	outfile << endl;

	infile.close();
	outfile.close();
	
}

// 识别的判断标准
void DistinguishStandard()
{
	ifstream infile;
	string s;

	infile.open("data\\point\\point.txt");
	getline(infile, s);
	ComputeCharacter cc(s);
	cc.Compute();
	cout << "*                                               *" << endl;
	cout << "*【识别的判断标准如下】" << "                         *" << endl;
	cout << "*【横纵边框比例】：" << cc.XYRatio << "                      *" << endl;
	cout << "*【笔划数】：" << cc.strokeCount << "                                  *" << endl;
	cout << "*【封闭性】：" << cc.closeness << "                            *" << endl;
	cout << "*【紧密性】：" << cc.Compactness << "                           *" << endl;
	cout << "*【曲率】：" << cc.curvature << "                              *" << endl;
	cout << "*【初始方向】：" << cc.beginDirection << "                         " << endl;
	cout << "*【终止方向】：" << cc.endDirection << "                            " << endl;
	cout << "*【初始点到X最大值距离与最大边框的比值】：" << cc.xStartMaxLengthRatio << "  " << endl;
	cout << "*【初始点到Y最大值距离与最大边框的比值】：" << cc.yStartMaxLengthRatio << "  " << endl;
	cout << "*【终止点到X最大值距离与最大边框的比值】：" << cc.xEndMaxLengthRatio << "     " << endl;
	cout << "*【终止点到Y最大值距离与最大边框的比值】：" << cc.yEndMaxLengthRatio << "     " << endl;
	cout << "*【初始点到X最大值距离与最小边框的比值】：" << cc.xStartMinLengthRatio << "   " << endl;
	cout << "*【初始点到Y最大值距离与最小边框的比值】：" << cc.yStartMinLengthRatio << "    " << endl;
	cout << "*【拐点的数量】：" << cc.InflectionPointsNumber << "                              *" << endl;

	infile.close();

}

//调用模型识别手势
void PredictGesture()
{
	int paramNumberPredict = 4;
	char *tmp[4] = { "" };
	tmp[1] = { "data\\point\\pointCharacter.txt" };
	if (bihua == 3)
	{
		tmp[2] = { "data\\model\\modelStroke3.txt" };
	}
	else if (bihua == 4)
	{
		tmp[2] = { "data\\model\\modelStroke4.txt" };
	}
	else
	{
		tmp[2] = { "data\\model\\modelStroke12.txt" };
	}
	tmp[3] = { "data\\point\\outPoint.txt" };
	svmPredict(paramNumberPredict, tmp);
}

//计算压力均值
long calPressAvg()
{
	long avgPress = 0;
	for (int i = 0;i < firstGesture.size();++i)
	{
		avgPress += firstGesture[i].nPress;
	}
	for (int j = 0;j < res.size();++j)
	{
		avgPress += res[j].nPress;
	}

	int n = firstGesture.size() + res.size();
	if (n > 0)
	{
		avgPress = avgPress / n;
	}
	return avgPress;
}

//判断指令控制符正序逆序
void judgeControlDirection()
{
	if (firstGesture[1].nX > firstGesture[0].nX)
	{
		cout << "正序" ;
	}
	else
	{
		cout << "逆序";
	}
}

//计算手势中心点坐标
void CalCenter()
{
	if (bihua == 1)
	{
		long long sumX = 0;
		long long sumY = 0;
		for (int i = 0;i < firstGesture.size();++i)
		{
			sumX += firstGesture[i].nX;
			sumY += firstGesture[i].nY;
		}

		double CentroidX = sumX / firstGesture.size();
		double CentroidY = sumY / firstGesture.size();
	}
	else
	{
		long long sumX = 0;
		long long sumY = 0;
		for (int i = 0;i < firstGesture.size();++i)
		{
			sumX += firstGesture[i].nX;
			sumY += firstGesture[i].nY;
		}

		double CentroidX = sumX / res.size();
		double CentroidY = sumY / res.size();
	}
	
}

//输出共有特征
void printCommonCharac()
{
	
	cout << "* 【书写区域：答题区】                          *" << endl;

	SYSTEMTIME st;
	GetLocalTime(&st);

	cout << "* 【书写日期】：" <<st.wYear<<"-"<<st.wMonth<<"-"<<st.wDay<<" "<<st.wHour<<":"<<st.wMinute<<":"<<st.wSecond<< endl;
	cout << "* 【书写用时】：" << (write_end_time - write_start_time)<<"ms                           *"<< endl;
	cout << "* 【书写压力均值】：" << calPressAvg() <<"                         *"<< endl;
	cout << "* 【页码】：0                                   *" << endl;
	cout << "* 【读物种类】：练习                            *" << endl;
	cout << "* 【读物名称】：学习质量监测                    *" << endl;
	cout << "* 【书写幅面(逻辑坐标最大值)】：A4(27000,20600) *" << endl;
	cout << "* 【偏差】：                                    *" << endl;
	cout << "* 【位置坐标】:(" << firstGesture[0].nX << "," << firstGesture[0].nY << ")" << endl;
}

//输出指令控制符独有特征
void printControlCharac()
{
	cout << "* 【主包体中心点坐标】：                        *" << endl;
	cout << "* 【辅包体中心点坐标】：                        *" << endl;
	cout << "* 【主包体长】：" << lengthX <<"                            *"<< endl;
	cout << "* 【主包体宽】：" << lengthY <<"                            *"<< endl;

	cout << "* 【行笔时序信息】：正序                        *"<< endl;

	cout << "* 【主包体封闭度】：                            *" << endl;
	cout << "* 【辅包体特纬度】：                            *" << endl;
	cout << "* 【场景版面意义排除】：                        *" << endl;
}

void printGestureCount()
{
	cout << "* 【本次使用累计识别次数】：";
	if (gestureCount < 10)
	{
		cout << gestureCount++ << "                   *" << endl;
	}
	else
	{
		cout << gestureCount++ << "                  *" << endl;
	}
}

//获取预测结果
void printResult()
{
	ifstream infile;
	string s;

	infile.open("data\\point\\outPoint.txt");
	getline(infile, s);
	infile.close();
	cout << "*************手势指令相关信息如下****************" << endl;
	cout << "* 【手势指令识别结果】：";
	
	if (s.size() <= 0)
	{
		cout << "未识别！" << endl;
	}
	else
	{
		int result = 0;
		for (int i = 0;i < s.size();++i)
		{
			result = result * 10 + (s[i] - '0');
		}
		label = result;
		if (result == 0 && bihua == 1)
		{
			cout << "指令控制符              *" << endl;
			printGestureCount();
			printControlCharac();
		}
		else if (result == 0 && bihua == 2)
		{
			{
				cout << -8 << "                      *" << endl;
			}
			printGestureCount();
		}
		else if (result == 8 && bihua == 1)
		{
			cout << "指令控制符              *" << endl;
			printGestureCount();
			printControlCharac();
		}
		/*else if (result == 7 && bihua == 2 && InflectionPointsNumber(res) >= 4)
		{
			{
				cout << -3 << "                      *" << endl;
			}
			printGestureCount();
		}*/
		else
		{
			cout << "-";
			if (result == 10)
			{
				cout << result <<"                     *"<< endl;
			}
			else
			{
				cout << result << "                      *" << endl;
			}
			printGestureCount();
		}
	}
	printCommonCharac();
	DistinguishStandard();
	cout << "***********************end***********************" << endl << endl;
}

//清空数据
void ClearData()
{
	firstGesture.clear();
	res.clear();
	write_start_time = 0;
	write_end_time = 0;
	bihua = 0;
}

//识别手势
void recognize()
{
	if (bihua > 4)
	{
		cout << "【输入错误，未识别！】" << endl;
		cout << "*************************end********************" << endl << endl;
	}
	else
	{
		writeToPoint();
		calCharacter();
		PredictGesture();
		printResult();
	}
	cout << "【识别结果是否正确（正确请输入回车，错误请输入2）】：\n";
}

void ThreadFunc()
{
	time_end = GetTickCount();
	double temp_time = time_start;
	while (time_end - time_start < 1000)
	{
		if (time_start - temp_time > 0.0)return;
		time_end = GetTickCount();

	}
	
	recognize();
}

void tt()
{
	thread t1(ThreadFunc);
	t1.detach();
}

//合并训练数据文件
void MergeTrainFile(char* targetFile, const string& sourceFile1, const string& sourceFile2)
{
	ifstream infile;
	ofstream outfile;
	remove(targetFile);
	infile.open(sourceFile1);
	outfile.open(targetFile, ios::app);
	string s;
	while (getline(infile, s))
	{
		sampleCount++;
		outfile << s << endl;
	}
	infile.close();

	infile.open(sourceFile2);
	while (getline(infile, s))
	{
		sampleCount++;
		outfile << s << endl;
	}
	infile.close();
	outfile.close();
}

//输出训练样本数
void printTrainCount()
{
	if (sampleCount < 10)
	{
		cout << "* 【样本数】：" << sampleCount << "                                *" << endl;
		cout << "* 【正确样本数】：" << sampleCount - TotalWrong << "                         *" << endl;
		cout << "* 【错误样本数】：" << TotalWrong << "                           *" << endl;
		cout << "* 【训练源编号】：       【提供者】：          *" << endl;
	}
	else if (sampleCount < 100)
	{
		cout << "* 【样本数】：" << sampleCount << "                               *" << endl;
		cout << "* 【正确样本数】：" << sampleCount - TotalWrong << "                         *" << endl;
		cout << "* 【错误样本数】：" << TotalWrong << "                           *" << endl;
		cout << "* 【训练源编号】：       【提供者】：          *" << endl;
	}
	else if (sampleCount < 1000)
	{
		cout << "* 【样本数】：" << sampleCount << "                              *" << endl;
		cout << "* 【正确样本数】：" << sampleCount - TotalWrong << "                          *" << endl;
		cout << "* 【错误样本数】：" << TotalWrong << "                           *" << endl;
		cout << "* 【训练源编号】：       【提供者】：          *" << endl;
	}
	else
	{
		cout << "* 【样本数】：" << sampleCount << "                             *" << endl;
		cout << "* 【正确样本数】：" << sampleCount - TotalWrong << "                          *" << endl;
		cout << "* 【错误样本数】：" << TotalWrong << "                           *" << endl;
		cout << "* 【训练源编号】：       【提供者】：          *" << endl;
	}
}
//计算训练模型用到的特征
void CalTrainDataCharac(char* targetFile, const string& sourceFile)
{
	ifstream infile;
	ofstream outfile;
	remove(targetFile);
	infile.open(sourceFile);
	outfile.open(targetFile, ios::app);
	string s;
	while (getline(infile, s))
	{
		ComputeCharacter cc(s);
		cc.Compute();
		outfile << cc.tag << " ";
		outfile << "0:" << cc.XYRatio << " ";
		outfile << "1:" << cc.strokeCount << " ";
		outfile << "2:" << cc.closeness << " ";
		outfile << "3:" << cc.Compactness << " ";
		outfile << "4:" << cc.curvature << " ";
		outfile << "5:" << cc.beginDirection << " ";
		outfile << "6:" << cc.endDirection << " ";
		outfile << "7:" << cc.xStartMaxLengthRatio << " ";
		outfile << "8:" << cc.yStartMaxLengthRatio << " ";
		outfile << "9:" << cc.xEndMaxLengthRatio << " ";
		outfile << "10:" << cc.yEndMaxLengthRatio << " ";
		outfile << "11:" << cc.xStartMinLengthRatio << " ";
		outfile << "12:" << cc.yStartMinLengthRatio << " ";
		outfile << "13:" << cc.InflectionPointsNumber;
		outfile << endl;
	}
	infile.close();
	outfile.close();
}

//训练模型
void trainModel()
{
	cout << "********************模型训练********************" << endl;
	sampleCount = 0;
	MergeTrainFile("data\\temp\\tempStroke12.txt","data\\collectData\\collectDataStroke12.txt", "data\\train\\trainStroke12.txt");
	MergeTrainFile("data\\temp\\tempStroke3.txt", "data\\collectData\\collectDataStroke3.txt", "data\\train\\trainStroke3.txt");
	MergeTrainFile("data\\temp\\tempStroke4.txt", "data\\collectData\\collectDataStroke4.txt", "data\\train\\trainStroke4.txt");
	cout << "* 【开始计算特征】                             *" << endl;
	CalTrainDataCharac("data\\character\\characterStroke12.txt", "data\\temp\\tempStroke12.txt");
	CalTrainDataCharac("data\\character\\characterStroke3.txt", "data\\temp\\tempStroke3.txt");
	CalTrainDataCharac("data\\character\\characterStroke4.txt", "data\\temp\\tempStroke4.txt");
	cout << "* 【特征计算完毕】                             *" << endl;
	int paramNumberPredict = 7;
	char *tmp[7] = { "" };
	tmp[1] = { "-s" };
	tmp[2] = { "0" };
	tmp[3] = { "-t" };
	tmp[4] = { "0" };
	tmp[5] = { "data\\character\\characterStroke12.txt" };
	tmp[6] = { "data\\model\\modelStroke12.txt" };
	cout << "* 【开始训练特模型，请稍后】                   *" << endl;
	svmTrain(paramNumberPredict, tmp);

	tmp[5] = { "data\\character\\characterStroke3.txt" };
	tmp[6] = { "data\\model\\modelStroke3.txt" };
	svmTrain(paramNumberPredict, tmp);

	tmp[5] = { "data\\character\\characterStroke4.txt" };
	tmp[6] = { "data\\model\\modelStroke4.txt" };
	svmTrain(paramNumberPredict, tmp);

	cout << "* 【模型训练成功】                             *" << endl;
	cout << "* 【指令集类别总数】：11 (指令控制符，-1~-10)  *" << endl;
	printTrainCount();
	cout << "*********************end************************" << endl << endl;;
}

//字符串转年月日
//格式：date:2020/12/31
void StrToDate(const string &s, string &year, string &month, string &day)
{
	int index = 0;
	while (s[index] != ':')
	{
		index++;
	}
	index++;
	while (s[index] != '/')
	{
		year += s[index++];
	}
	index++;
	while (s[index] != '/')
	{
		month += s[index++];
	}
	index++;
	while (index < s.size())
	{
		day += s[index++];
	}
}

//字符串转今日统计结果
//格式：T:XX F:XXX
void StrToTodayResult(const string &s)
{
	int index = 0;
	string tmp;
	while (s[index] != ':')
	{
		++index;
	}
	index++;

	while (s[index] != ' ')
	{
		tmp += s[index++];
	}
	correctToday = atoi(tmp.c_str());
	tmp.clear();

	while (s[index] != ':')
	{
		index++;
	}
	index++;

	while (index < s.size())
	{
		tmp += s[index++];
	}
	wrongToday = atoi(tmp.c_str());

	NumberToday = correctToday + wrongToday;
}

//字符串转全部统计结果
void StrToTotalResult(const string &s)
{
	int index = 0;
	string tmp;
	while (s[index] != ':')
	{
		++index;
	}
	index++ ;

	while (s[index] != ' ')
	{
		tmp += s[index++];
	}
	TotalCorrect = atoi(tmp.c_str());
	tmp.clear();

	while (s[index] != ':')
	{
		index++;
	}
	index++;

	while (index < s.size())
	{
		tmp += s[index++];
	}
	TotalWrong = atoi(tmp.c_str());

	TotalNumber = TotalCorrect + TotalWrong;
}
//加载使用记录
void LoadUsageRecord()
{
	ifstream infile;
	
	infile.open("data\\StatisticalResults\\StatisticalResults.txt");
	string s,year,month,day;
	getline(infile, s);
	StrToDate(s, year, month, day);

	getline(infile, s);
	StrToTotalResult(s);

	getline(infile, s);
	StrToTodayResult(s);

	infile.close();
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (year != to_string(long long(st.wYear)) || month != to_string(long long(st.wMonth)) || day != to_string(long long(st.wDay)))
	{
		correctToday = 0;
		wrongToday = 0;
		NumberToday = 0;
	}
	
	infile.open("data\\train\\trainStroke12.txt");
	while (getline(infile, s))
	{
		sampleCount++;
	}
	infile.close();

	infile.open("data\\train\\trainStroke3.txt");
	while (getline(infile, s))
	{
		sampleCount++;
	}
	infile.close();
}

//更新统计结果
void UpdateUsageRecord()
{
	remove("data\\StatisticalResults\\StatisticalResults.txt");
	ofstream outfile;

	SYSTEMTIME st;
	GetLocalTime(&st);

	outfile.open("data\\StatisticalResults\\StatisticalResults.txt",ios::app);
	outfile << "date:" << st.wYear << "/" << st.wMonth << "/" << st.wDay << endl;
	outfile << "Total T:" << TotalCorrect << " " << "F:" << TotalWrong << endl;
	outfile << "Today T:" << correctToday << " " << "F:" << wrongToday << endl;
	outfile.close();
}

//合并外部数据文件
void MergerExternalDataFile(const string &sourceDataFile12, const string &sourceDataFile3, const string &sourceStatisticalFile)
{
	cout << "**********************************" << endl;
	cout << "*【数据合并开始】                *" << endl;

	ifstream infile;
	string s;
	ofstream outfile;
	infile.open("data\\externalDataFile\\"+sourceDataFile12);
	outfile.open("data\\collectData\\collectDataStroke12.txt", ios::app);

	while (getline(infile, s))
	{
		outfile << s << endl;
		//cout << s << endl;
	}
	infile.close();
	outfile.close();
	
	infile.open("data\\externalDataFile\\" + sourceDataFile3);
	outfile.open("data\\collectData\\collectDataStroke3.txt", ios::app);

	while (getline(infile, s))
	{

		outfile << s << endl;
	}
	infile.close();
	outfile.close();

	infile.open("data\\externalStatiscalResults\\" + sourceStatisticalFile);
	
	getline(infile, s);
	
	getline(infile, s);

	int externalCorrect = 0;
	int externalWrong = 0;
	int index = 0;
	string tmp;
	while (s[index] != ':')
	{
	++index;
	}
	index++;

	while (s[index] != ' ')
	{
	tmp += s[index++];
	}
	externalCorrect = atoi(tmp.c_str());
	tmp.clear();

	while (s[index] != ':')
	{
	index++;
	}
	index++;

	while (index < s.size())
	{
	tmp += s[index++];
	}
	externalWrong = atoi(tmp.c_str());

	TotalCorrect += externalCorrect;
	TotalWrong += externalWrong;
	TotalNumber = TotalCorrect + TotalWrong;
	
	cout << "*【数据合并成功】                *" << endl;
	//MergeTrainFile();
	cout << "*【合并后样本总数】：" << sampleCount <<"        *"<< endl;
	cout << "*【来自于原样本数】：" << sampleCount - (externalCorrect + externalWrong) << "        *" << endl;
	cout << "*【来自于外部样本数】：" << (externalCorrect + externalWrong) << "       *" << endl;
	cout << "**********************************" << endl<<endl;
	infile.close();
}


//分离不同笔划数的数据
void SeparateDataByStroke()
{
	ifstream infile;
	infile.open("data\\collectData.txt");
	string s;

	while (getline(infile, s))
	{
		ComputeCharacter cc(s);
		cc.Compute();
		cc.InrcStroketCount();
	}
	
}

class CMyEventHandler : public IRobotEventHandler
{
	// 笔迹数据回报
	bool status = false;
	

	virtual void onDataPacket(const PEN_INFO &penInfo) {
		//cout << "笔迹数据事件： [nX: " << penInfo.nX << " nY: " << penInfo.nY << " 压感： " << penInfo.nPress << " 笔状态： " << (int)penInfo.nStatus << "手势识别" << endl;
		
		if ((int)penInfo.nPress > 0)
		{
			status = true;
			if (write_start_time == 0)
			{
				write_start_time = GetTickCount();
			}
			if (bihua == 0)
			{
				firstGesture.push_back(penInfo);
			}
			else
			{
				res.push_back(penInfo);
			}
			time_start = GetTickCount();
		}
		else
		{
			if (status == true)
			{
				write_end_time = GetTickCount();
				bihua++;
				tt();
				status = false;
			}
			
		}
		//switch (penInfo.nStatus)
		//{
		//case 0x11:
		//case 0x21:
		//case 0x31:
		//	//cout << " 笔接触手写板]\n";
		//	break;
		//case 0x00:
		//	//cout << " 笔离开手写板]\n";
		//	//cout << " 是否退出（输入1 退出）：\n";
		//	break;
		//case 0x10:
		//	//cout << " 笔悬浮]\n";
		//	break;
		//default:
		//	cout << "------\n";
		//	break;
		//}

	}

	// 设备插拔
	virtual void onDeviceChanged(eDeviceStatus status, eDeviceType type) {
		switch (status)
		{
		case DEVICE_IN:
		{
			cout << "设备插入事件发生\n";
			GetInstance()->GetDeviceInfo(0, g_usbInfo);
			GetInstance()->GetDeviceInfo(0, g_deviceInfo);
			GetInstance()->ConnectOpen();
			cout << "打开设备\n";
			std::string strDevname = g_deviceInfo.szDevName;
			std::string strPID = std::to_string(g_usbInfo.nProductNum);
			std::string strVID = std::to_string(g_usbInfo.nVendorNum);
			cout << "	设备名称： " << strDevname << endl;
			cout << "	设备Product ID: " << strPID << endl;
			cout << "	设备Vendor ID: " << strVID << endl;
		}
		break;
		case DEVICE_OUT:
			cout << "设备拔出事件发生\n";
			GetInstance()->ConnectDispose();
			break;
		default:
			break;
		}
	}
};

CMyEventHandler g_myEventHandler;

int main()
{
	LoadUsageRecord();
	//SeparateDataByStroke();
	cout << "=== 31指令集合制备优化最小系统-V1.0 ===" << endl;
	// 获取SDK 实例
	RobotPenController* pRbtInstance = GetInstance();
	int nDevCount = pRbtInstance->GetDeviceCount();
	//cout << nDevCount << endl;
	if (nDevCount < 1) {
		cout << "【未检测到设备】\n";
		cout << "【请插入设备后重新启动程序】\n";
		cout << "【程序退出】\n";
		system("pause");
		return 0;
	}
	else
	{
		cout << "【设备已检测到】\n";
	}

	do
	{
		// 获取设备信息
		if (!pRbtInstance->GetDeviceInfo(0, g_usbInfo)) {
			cout << "【未获取到usb 信息】\n";
			return 0;
		}
		if (!pRbtInstance->GetDeviceInfo(0, g_deviceInfo)) {
			cout << "【未获取到设备信息】\n";
			return 0;
		}
		strDevname = g_deviceInfo.szDevName;
		strPID = std::to_string(g_usbInfo.nProductNum);
		strVID = std::to_string(g_usbInfo.nVendorNum);


		cout << "【初始化SDK】\n";
		pRbtInstance->ConnectInitialize(g_deviceInfo.type, &g_myEventHandler);
		//pRbtInstance->ConnectInitialize(g_deviceInfo.type,NULL,NULL);
		cout << "【打开设备】\n";
		nRet = pRbtInstance->ConnectOpen();
		if (nRet == 0) {
			cout << "【设备名称】： " << strDevname << endl;
			cout << "【设备Product ID】： " << strPID << endl;
			cout << "【设备Vendor ID】：" << strVID << endl << endl;
		}
		else
		{
			cout << "【打开设备 失败, 错误号】：" << nRet << endl;
		}

	} while (false);

	int nCin = -1;
	string tempInput;
	cout << "【请继续书写或选择相关操作：0-训练模型  100-退出  200-合并外部训练数据】：\n" << endl;
	while (true)
	{
		nCin = cin.get();
		if (nCin == 10)
		{
			if (tempInput.size() <= 0)
			{
				cout << "【数据已添加到数据存储文件！】\n" << endl;
				cout << "*************************end********************" << endl << endl;
				SaveCollectData();
				ClearData();
				correctToday++;
				TotalCorrect++;
				PrintStatisticalResults();
			}
			else
			{
				//tempInput:0-训练模型 100-退出 200-合并外部数据 2-识别错误
				if (tempInput == "0")
				{
					trainModel();
				}
				else if (tempInput == "100")
				{
					break;
				}
				else if (tempInput == "200")
				{
					string sourceDataFile12;
					string sourceDataFile3;
					string sourceStatisticalFile;
					cout << "【请输入要合并的外部数据文件名和外部统计结果文件名(数据文件请存放至data\\externalDataFile文件夹下,统计结果文件请放入data\\externalStatiscalResults文件夹下)】：" << endl;
					cin >> sourceDataFile12;
					cin >> sourceDataFile3;
					cin >> sourceStatisticalFile;
					MergerExternalDataFile(sourceDataFile12, sourceDataFile3, sourceStatisticalFile);
				}
				else if (tempInput == "2")
				{
					char sign;
					cout << "【是否删除数据(y/n)】：" << endl;
					cin >> sign;
					if (sign == 'Y' || sign == 'y')
					{
						cout << "【错误数据已删除】\n" << endl;
						cout << "*************************end********************" << endl << endl;
					}
					else if (sign == 'N' || sign == 'n')
					{
						cout << "【请输入手势正确标签(指令控制符：0，其余手势为-1~-30)】：";
						string gestureName;
						cin >> gestureName;
						if (IsCorrectGestureName(gestureName))
						{
							SaveCollectData();
							cout << "【数据已添加到数据存储文件！】\n" << endl;
							cout << "*************************end********************" << endl << endl;
						}
						else
						{
							cout << "【标签输入错误，错误数据已删除】\n" << endl;
							cout << "*************************end********************" << endl << endl;
						}
					}
					else
					{
						cout << "【输入错误！！！】" << endl;
						cout << "【错误数据已删除】\n" << endl;
						cout << "*************************end********************" << endl << endl;
					}
					ClearData();
					wrongToday++;
					TotalWrong++;
					PrintStatisticalResults();
					cin.get();
				}
			}
			tempInput.clear();
			UpdateUsageRecord();
			cout << "【请继续书写或选择相关操作：0-训练模型  100-退出  200-合并外部训练数据】：\n" << endl;
		}
		else
		{
			char ch = nCin;
			tempInput += ch;
		}
	}
	cout << "【断开设备连接】\n" << endl;
	// 关闭连接的设备
	GetInstance()->ConnectDispose();
	// 销毁SDK实例
	DestroyInstance();
	UpdateUsageRecord();
	return 0;
}

