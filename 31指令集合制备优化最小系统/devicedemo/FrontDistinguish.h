#pragma once
#include<vector>
#include "stdafx.h"

using namespace std;

int InflectionPointsNumber(const vector<PEN_INFO> &source);

//判断是否是-1
bool JudgeOne(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-2
bool JudgeTwo(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-3
bool JudgeThree(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-4
bool JudgeFour(const int strokeCount, const vector<PEN_INFO> &firstStroke, const vector<PEN_INFO> &secondStroke);

//判断是否是-5
bool JudgeFive(const int strokeCount, const vector<PEN_INFO> &firstStroke, const vector<PEN_INFO> &secondStroke);

//判断是否是-6
bool JudgeSix(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-7
bool JudgeSeven(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-8
bool JudgeEight(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-9
bool JudgeNine(const int strokeCount, const vector<PEN_INFO> &source);

//判断是否是-10
bool JudgeTen(const int strokeCount, const vector<PEN_INFO> &firstStroke, const vector<PEN_INFO> &secondStroke);

//识别
int Disting(const int strokeCount, const vector<PEN_INFO> &firstStroke, const vector<PEN_INFO> &secondStroke);