// usbSDKdemo.cpp : �������̨Ӧ�ó������ڵ㡣
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

//ʶ���������ȷ��������
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

//��ʾ��ȷ��
void PrintStatisticalResults()
{
	TotalNumber++;
	NumberToday++;
	TotalAccuracy = (double)TotalCorrect / (double)TotalNumber;
	AccuracyToday = (double)correctToday / (double)NumberToday;
	cout << "********************����ָ��ʶ��ͳ�ƽ��********************" << endl;

	cout << "*������ָ���ܼ�ʶ���������" << TotalNumber << "                               " << endl;
	cout << "*������ָ���ܼ�ʶ����ȷ��������" << TotalCorrect << "                           " << endl;
	cout << "*������ָ���ܼ�ʶ������������" << TotalWrong << "                           " << endl;
	cout << "*������ָ���ܼ�ʶ����ȷ�ʡ���" << TotalAccuracy * 100 << "%" << "                          " << endl;
	cout << "*                                                          " << endl;
	cout << "*������ָ�����ʶ���������" << NumberToday << "                               " << endl;
	cout << "*������ָ�����ʶ����ȷ��������" << correctToday << "                           " << endl;
	cout << "*������ָ�����ʶ������������" << wrongToday << "                           " << endl;
	cout << "*������ָ�����ʶ����ȷ�ʡ���" << AccuracyToday * 100 << "%" << "                          " << endl;

	cout << "************************************************************" << endl << endl;
}

//�洢�ɼ�������
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

//�ɼ�����
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

//������Ϣд��point.txt
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

//����ʵʱ��������
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

// ʶ����жϱ�׼
void DistinguishStandard()
{
	ifstream infile;
	string s;

	infile.open("data\\point\\point.txt");
	getline(infile, s);
	ComputeCharacter cc(s);
	cc.Compute();
	cout << "*                                               *" << endl;
	cout << "*��ʶ����жϱ�׼���¡�" << "                         *" << endl;
	cout << "*�����ݱ߿��������" << cc.XYRatio << "                      *" << endl;
	cout << "*���ʻ�������" << cc.strokeCount << "                                  *" << endl;
	cout << "*������ԡ���" << cc.closeness << "                            *" << endl;
	cout << "*�������ԡ���" << cc.Compactness << "                           *" << endl;
	cout << "*�����ʡ���" << cc.curvature << "                              *" << endl;
	cout << "*����ʼ���򡿣�" << cc.beginDirection << "                         " << endl;
	cout << "*����ֹ���򡿣�" << cc.endDirection << "                            " << endl;
	cout << "*����ʼ�㵽X���ֵ���������߿�ı�ֵ����" << cc.xStartMaxLengthRatio << "  " << endl;
	cout << "*����ʼ�㵽Y���ֵ���������߿�ı�ֵ����" << cc.yStartMaxLengthRatio << "  " << endl;
	cout << "*����ֹ�㵽X���ֵ���������߿�ı�ֵ����" << cc.xEndMaxLengthRatio << "     " << endl;
	cout << "*����ֹ�㵽Y���ֵ���������߿�ı�ֵ����" << cc.yEndMaxLengthRatio << "     " << endl;
	cout << "*����ʼ�㵽X���ֵ��������С�߿�ı�ֵ����" << cc.xStartMinLengthRatio << "   " << endl;
	cout << "*����ʼ�㵽Y���ֵ��������С�߿�ı�ֵ����" << cc.yStartMinLengthRatio << "    " << endl;
	cout << "*���յ����������" << cc.InflectionPointsNumber << "                              *" << endl;

	infile.close();

}

//����ģ��ʶ������
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

//����ѹ����ֵ
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

//�ж�ָ����Ʒ���������
void judgeControlDirection()
{
	if (firstGesture[1].nX > firstGesture[0].nX)
	{
		cout << "����" ;
	}
	else
	{
		cout << "����";
	}
}

//�����������ĵ�����
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

//�����������
void printCommonCharac()
{
	
	cout << "* ����д���򣺴�������                          *" << endl;

	SYSTEMTIME st;
	GetLocalTime(&st);

	cout << "* ����д���ڡ���" <<st.wYear<<"-"<<st.wMonth<<"-"<<st.wDay<<" "<<st.wHour<<":"<<st.wMinute<<":"<<st.wSecond<< endl;
	cout << "* ����д��ʱ����" << (write_end_time - write_start_time)<<"ms                           *"<< endl;
	cout << "* ����дѹ����ֵ����" << calPressAvg() <<"                         *"<< endl;
	cout << "* ��ҳ�롿��0                                   *" << endl;
	cout << "* ���������ࡿ����ϰ                            *" << endl;
	cout << "* ���������ơ���ѧϰ�������                    *" << endl;
	cout << "* ����д����(�߼��������ֵ)����A4(27000,20600) *" << endl;
	cout << "* ��ƫ���                                    *" << endl;
	cout << "* ��λ�����꡿:(" << firstGesture[0].nX << "," << firstGesture[0].nY << ")" << endl;
}

//���ָ����Ʒ���������
void printControlCharac()
{
	cout << "* �����������ĵ����꡿��                        *" << endl;
	cout << "* �����������ĵ����꡿��                        *" << endl;
	cout << "* �������峤����" << lengthX <<"                            *"<< endl;
	cout << "* �����������" << lengthY <<"                            *"<< endl;

	cout << "* ���б�ʱ����Ϣ��������                        *"<< endl;

	cout << "* ���������նȡ���                            *" << endl;
	cout << "* ����������γ�ȡ���                            *" << endl;
	cout << "* ���������������ų�����                        *" << endl;
}

void printGestureCount()
{
	cout << "* ������ʹ���ۼ�ʶ���������";
	if (gestureCount < 10)
	{
		cout << gestureCount++ << "                   *" << endl;
	}
	else
	{
		cout << gestureCount++ << "                  *" << endl;
	}
}

//��ȡԤ����
void printResult()
{
	ifstream infile;
	string s;

	infile.open("data\\point\\outPoint.txt");
	getline(infile, s);
	infile.close();
	cout << "*************����ָ�������Ϣ����****************" << endl;
	cout << "* ������ָ��ʶ��������";
	
	if (s.size() <= 0)
	{
		cout << "δʶ��" << endl;
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
			cout << "ָ����Ʒ�              *" << endl;
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
			cout << "ָ����Ʒ�              *" << endl;
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

//�������
void ClearData()
{
	firstGesture.clear();
	res.clear();
	write_start_time = 0;
	write_end_time = 0;
	bihua = 0;
}

//ʶ������
void recognize()
{
	if (bihua > 4)
	{
		cout << "���������δʶ�𣡡�" << endl;
		cout << "*************************end********************" << endl << endl;
	}
	else
	{
		writeToPoint();
		calCharacter();
		PredictGesture();
		printResult();
	}
	cout << "��ʶ�����Ƿ���ȷ����ȷ������س�������������2������\n";
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

//�ϲ�ѵ�������ļ�
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

//���ѵ��������
void printTrainCount()
{
	if (sampleCount < 10)
	{
		cout << "* ������������" << sampleCount << "                                *" << endl;
		cout << "* ����ȷ����������" << sampleCount - TotalWrong << "                         *" << endl;
		cout << "* ����������������" << TotalWrong << "                           *" << endl;
		cout << "* ��ѵ��Դ��š���       ���ṩ�ߡ���          *" << endl;
	}
	else if (sampleCount < 100)
	{
		cout << "* ������������" << sampleCount << "                               *" << endl;
		cout << "* ����ȷ����������" << sampleCount - TotalWrong << "                         *" << endl;
		cout << "* ����������������" << TotalWrong << "                           *" << endl;
		cout << "* ��ѵ��Դ��š���       ���ṩ�ߡ���          *" << endl;
	}
	else if (sampleCount < 1000)
	{
		cout << "* ������������" << sampleCount << "                              *" << endl;
		cout << "* ����ȷ����������" << sampleCount - TotalWrong << "                          *" << endl;
		cout << "* ����������������" << TotalWrong << "                           *" << endl;
		cout << "* ��ѵ��Դ��š���       ���ṩ�ߡ���          *" << endl;
	}
	else
	{
		cout << "* ������������" << sampleCount << "                             *" << endl;
		cout << "* ����ȷ����������" << sampleCount - TotalWrong << "                          *" << endl;
		cout << "* ����������������" << TotalWrong << "                           *" << endl;
		cout << "* ��ѵ��Դ��š���       ���ṩ�ߡ���          *" << endl;
	}
}
//����ѵ��ģ���õ�������
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

//ѵ��ģ��
void trainModel()
{
	cout << "********************ģ��ѵ��********************" << endl;
	sampleCount = 0;
	MergeTrainFile("data\\temp\\tempStroke12.txt","data\\collectData\\collectDataStroke12.txt", "data\\train\\trainStroke12.txt");
	MergeTrainFile("data\\temp\\tempStroke3.txt", "data\\collectData\\collectDataStroke3.txt", "data\\train\\trainStroke3.txt");
	MergeTrainFile("data\\temp\\tempStroke4.txt", "data\\collectData\\collectDataStroke4.txt", "data\\train\\trainStroke4.txt");
	cout << "* ����ʼ����������                             *" << endl;
	CalTrainDataCharac("data\\character\\characterStroke12.txt", "data\\temp\\tempStroke12.txt");
	CalTrainDataCharac("data\\character\\characterStroke3.txt", "data\\temp\\tempStroke3.txt");
	CalTrainDataCharac("data\\character\\characterStroke4.txt", "data\\temp\\tempStroke4.txt");
	cout << "* ������������ϡ�                             *" << endl;
	int paramNumberPredict = 7;
	char *tmp[7] = { "" };
	tmp[1] = { "-s" };
	tmp[2] = { "0" };
	tmp[3] = { "-t" };
	tmp[4] = { "0" };
	tmp[5] = { "data\\character\\characterStroke12.txt" };
	tmp[6] = { "data\\model\\modelStroke12.txt" };
	cout << "* ����ʼѵ����ģ�ͣ����Ժ�                   *" << endl;
	svmTrain(paramNumberPredict, tmp);

	tmp[5] = { "data\\character\\characterStroke3.txt" };
	tmp[6] = { "data\\model\\modelStroke3.txt" };
	svmTrain(paramNumberPredict, tmp);

	tmp[5] = { "data\\character\\characterStroke4.txt" };
	tmp[6] = { "data\\model\\modelStroke4.txt" };
	svmTrain(paramNumberPredict, tmp);

	cout << "* ��ģ��ѵ���ɹ���                             *" << endl;
	cout << "* ��ָ������������11 (ָ����Ʒ���-1~-10)  *" << endl;
	printTrainCount();
	cout << "*********************end************************" << endl << endl;;
}

//�ַ���ת������
//��ʽ��date:2020/12/31
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

//�ַ���ת����ͳ�ƽ��
//��ʽ��T:XX F:XXX
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

//�ַ���תȫ��ͳ�ƽ��
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
//����ʹ�ü�¼
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

//����ͳ�ƽ��
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

//�ϲ��ⲿ�����ļ�
void MergerExternalDataFile(const string &sourceDataFile12, const string &sourceDataFile3, const string &sourceStatisticalFile)
{
	cout << "**********************************" << endl;
	cout << "*�����ݺϲ���ʼ��                *" << endl;

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
	
	cout << "*�����ݺϲ��ɹ���                *" << endl;
	//MergeTrainFile();
	cout << "*���ϲ���������������" << sampleCount <<"        *"<< endl;
	cout << "*��������ԭ����������" << sampleCount - (externalCorrect + externalWrong) << "        *" << endl;
	cout << "*���������ⲿ����������" << (externalCorrect + externalWrong) << "       *" << endl;
	cout << "**********************************" << endl<<endl;
	infile.close();
}


//���벻ͬ�ʻ���������
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
	// �ʼ����ݻر�
	bool status = false;
	

	virtual void onDataPacket(const PEN_INFO &penInfo) {
		//cout << "�ʼ������¼��� [nX: " << penInfo.nX << " nY: " << penInfo.nY << " ѹ�У� " << penInfo.nPress << " ��״̬�� " << (int)penInfo.nStatus << "����ʶ��" << endl;
		
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
		//	//cout << " �ʽӴ���д��]\n";
		//	break;
		//case 0x00:
		//	//cout << " ���뿪��д��]\n";
		//	//cout << " �Ƿ��˳�������1 �˳�����\n";
		//	break;
		//case 0x10:
		//	//cout << " ������]\n";
		//	break;
		//default:
		//	cout << "------\n";
		//	break;
		//}

	}

	// �豸���
	virtual void onDeviceChanged(eDeviceStatus status, eDeviceType type) {
		switch (status)
		{
		case DEVICE_IN:
		{
			cout << "�豸�����¼�����\n";
			GetInstance()->GetDeviceInfo(0, g_usbInfo);
			GetInstance()->GetDeviceInfo(0, g_deviceInfo);
			GetInstance()->ConnectOpen();
			cout << "���豸\n";
			std::string strDevname = g_deviceInfo.szDevName;
			std::string strPID = std::to_string(g_usbInfo.nProductNum);
			std::string strVID = std::to_string(g_usbInfo.nVendorNum);
			cout << "	�豸���ƣ� " << strDevname << endl;
			cout << "	�豸Product ID: " << strPID << endl;
			cout << "	�豸Vendor ID: " << strVID << endl;
		}
		break;
		case DEVICE_OUT:
			cout << "�豸�γ��¼�����\n";
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
	cout << "=== 31ָ����Ʊ��Ż���Сϵͳ-V1.0 ===" << endl;
	// ��ȡSDK ʵ��
	RobotPenController* pRbtInstance = GetInstance();
	int nDevCount = pRbtInstance->GetDeviceCount();
	//cout << nDevCount << endl;
	if (nDevCount < 1) {
		cout << "��δ��⵽�豸��\n";
		cout << "��������豸��������������\n";
		cout << "�������˳���\n";
		system("pause");
		return 0;
	}
	else
	{
		cout << "���豸�Ѽ�⵽��\n";
	}

	do
	{
		// ��ȡ�豸��Ϣ
		if (!pRbtInstance->GetDeviceInfo(0, g_usbInfo)) {
			cout << "��δ��ȡ��usb ��Ϣ��\n";
			return 0;
		}
		if (!pRbtInstance->GetDeviceInfo(0, g_deviceInfo)) {
			cout << "��δ��ȡ���豸��Ϣ��\n";
			return 0;
		}
		strDevname = g_deviceInfo.szDevName;
		strPID = std::to_string(g_usbInfo.nProductNum);
		strVID = std::to_string(g_usbInfo.nVendorNum);


		cout << "����ʼ��SDK��\n";
		pRbtInstance->ConnectInitialize(g_deviceInfo.type, &g_myEventHandler);
		//pRbtInstance->ConnectInitialize(g_deviceInfo.type,NULL,NULL);
		cout << "�����豸��\n";
		nRet = pRbtInstance->ConnectOpen();
		if (nRet == 0) {
			cout << "���豸���ơ��� " << strDevname << endl;
			cout << "���豸Product ID���� " << strPID << endl;
			cout << "���豸Vendor ID����" << strVID << endl << endl;
		}
		else
		{
			cout << "�����豸 ʧ��, ����š���" << nRet << endl;
		}

	} while (false);

	int nCin = -1;
	string tempInput;
	cout << "���������д��ѡ����ز�����0-ѵ��ģ��  100-�˳�  200-�ϲ��ⲿѵ�����ݡ���\n" << endl;
	while (true)
	{
		nCin = cin.get();
		if (nCin == 10)
		{
			if (tempInput.size() <= 0)
			{
				cout << "����������ӵ����ݴ洢�ļ�����\n" << endl;
				cout << "*************************end********************" << endl << endl;
				SaveCollectData();
				ClearData();
				correctToday++;
				TotalCorrect++;
				PrintStatisticalResults();
			}
			else
			{
				//tempInput:0-ѵ��ģ�� 100-�˳� 200-�ϲ��ⲿ���� 2-ʶ�����
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
					cout << "��������Ҫ�ϲ����ⲿ�����ļ������ⲿͳ�ƽ���ļ���(�����ļ�������data\\externalDataFile�ļ�����,ͳ�ƽ���ļ������data\\externalStatiscalResults�ļ�����)����" << endl;
					cin >> sourceDataFile12;
					cin >> sourceDataFile3;
					cin >> sourceStatisticalFile;
					MergerExternalDataFile(sourceDataFile12, sourceDataFile3, sourceStatisticalFile);
				}
				else if (tempInput == "2")
				{
					char sign;
					cout << "���Ƿ�ɾ������(y/n)����" << endl;
					cin >> sign;
					if (sign == 'Y' || sign == 'y')
					{
						cout << "������������ɾ����\n" << endl;
						cout << "*************************end********************" << endl << endl;
					}
					else if (sign == 'N' || sign == 'n')
					{
						cout << "��������������ȷ��ǩ(ָ����Ʒ���0����������Ϊ-1~-30)����";
						string gestureName;
						cin >> gestureName;
						if (IsCorrectGestureName(gestureName))
						{
							SaveCollectData();
							cout << "����������ӵ����ݴ洢�ļ�����\n" << endl;
							cout << "*************************end********************" << endl << endl;
						}
						else
						{
							cout << "����ǩ������󣬴���������ɾ����\n" << endl;
							cout << "*************************end********************" << endl << endl;
						}
					}
					else
					{
						cout << "��������󣡣�����" << endl;
						cout << "������������ɾ����\n" << endl;
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
			cout << "���������д��ѡ����ز�����0-ѵ��ģ��  100-�˳�  200-�ϲ��ⲿѵ�����ݡ���\n" << endl;
		}
		else
		{
			char ch = nCin;
			tempInput += ch;
		}
	}
	cout << "���Ͽ��豸���ӡ�\n" << endl;
	// �ر����ӵ��豸
	GetInstance()->ConnectDispose();
	// ����SDKʵ��
	DestroyInstance();
	UpdateUsageRecord();
	return 0;
}

