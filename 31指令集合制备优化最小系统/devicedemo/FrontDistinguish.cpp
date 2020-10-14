#include"FrontDistinguish.h"


//计算拐点的个数
int InflectionPointsNumber(const vector<PEN_INFO> &source)
{
	double slope = 0;
	int InflectionPointsNumber = 0;
	for (int i = 0;i < source.size() - 1;i += 10)
	{
		if (source[i].nX - source[i + 1].nX != 0)
		{
			if (slope == 0)
			{
				slope = (double)(source[i].nY - source[i + 1].nY) / (double)(source[i].nX - source[i + 1].nX);
			}
			else
			{
				double tmp = (double)(source[i].nY - source[i + 1].nY) / (double)(source[i].nX - source[i + 1].nX);
				if (slope*tmp < 0)
				{
					InflectionPointsNumber++;
					slope = tmp;
				}
			}
		}
	}
	return InflectionPointsNumber;
}

//判断是否是-1
bool JudgeOne(const int strokeCount, const vector<PEN_INFO> &source)
{
	if (strokeCount != 2)
	{
		return false;
	}

	int length = source.size();
	int mutationCount = 0;
	for (int i = 0;i < length - 2;++i)
	{
		if (source[i].nX - source[i + 2].nX < 0)
		{
			mutationCount++;
			if (mutationCount >= 4)
			{
				return false;
			}
		}

	}
	return true;
}

//判断是否是-2
bool JudgeTwo(const int strokeCount, const vector<PEN_INFO> &source)
{
	if (strokeCount != 2)
	{
		return false;
	}

	return false;
}

//判断是否是-3
bool JudgeThree(const int strokeCount, const vector<PEN_INFO> &source)
{
	if (strokeCount != 2)
	{
		return false;
	}

	return false;
}

//判断是否是-4
bool JudgeFour(const int strokeCount, const vector<PEN_INFO> &firstStroke, const vector<PEN_INFO> &secondStroke)
{
	if (strokeCount != 3)
	{
		return false;
	}

	if (!JudgeOne(2, secondStroke))
	{
		return false;
	}
	return true;
}

//判断是否是-5
bool JudgeFive(const int strokeCount, const vector<PEN_INFO> &firstStroke, const vector<PEN_INFO> &secondStroke)
{
	if (strokeCount != 3)
	{
		return false;
	}

	return true;
}

//判断是否是-6
bool JudgeSix(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-7
bool JudgeSeven(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-8
bool JudgeEight(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-9
bool JudgeNine(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-10
bool JudgeTen(const int strokeCount, const vector<PEN_INFO> &firstStroke, const vector<PEN_INFO> &secondStroke)
{
	if (strokeCount != 3)
	{
		return false;
	}

	if (!JudgeOne(2, firstStroke))
	{
		return false;
	}

	return true;
}

//识别
//int Disting(const int strokeCount, const vector<PEN_INFO> &source)
//{
//	int res = -1;
//	if (strokeCount <= 0 || strokeCount > 3)
//	{
//		res = -1;
//	}
//	else if (strokeCount == 1)
//	{
//		
//	}
//	else if (strokeCount == 2)
//	{
//		if (JudgeOne(strokeCount, source))
//		{
//			res = 1;
//		}
//		else if (JudgeTwo(strokeCount, source))
//		{
//			res = 2;
//		}
//		else if (JudgeThree(strokeCount, source))
//		{
//			res = 3;
//		}
//		else if (JudgeSix(strokeCount, source))
//		{
//			res = 6;
//		}
//		else if (JudgeSeven(strokeCount, source))
//		{
//			res = 7;
//		}
//		else if (JudgeEight(strokeCount, source))
//		{
//			res = 8;
//		}
//		else if (JudgeNine(strokeCount, source))
//		{
//			res = 9;
//		}
//	}
//	else
//	{
//		if (JudgeFour(strokeCount, source[0], source[1]))
//		{
//			res = 4;
//		}
//		else if (JudgeFive(strokeCount, source))
//		{
//			res = 5;
//		}
//		else if (JudgeTen(strokeCount, source))
//		{
//			res = 10;
//		}
//	}
//
//	return res;
//}