// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
#pragma once
#define _USE_MATH_DEFINES
#define USE_GAME_TIMER

#include <iostream>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <stack>
#include <boost/mem_fn.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "DxLib.h"
#include "DixSmartPtr.h"
#include "Timer.h"
#include "Hazama.h"

#ifdef _DEBUG
#pragma comment(lib, "MyLibrary.lib")
#pragma comment(lib, "MyTimer_d.lib")
#else
#pragma comment(lib, "MyLibraryRelease.lib")
#pragma comment(lib, "MyTimer.lib")
#endif

static const float RIGHT_EDGE_POSITION = 640.0f;
static const float LEFT_EDGE_POSITION = 0.0f;
static const float UPPER_EDGE_POSITION = 0.0f;
static const float LOWER_EDGE_POSITION = 480.0f;

static const float INITIAL_SELF_POSITION_X = 0.0f;
static const float INITIAL_SELF_POSITION_Y = 200.0f;

static const float INITIAL_FOE_POSITION = 640.0f;

static const int MAX_INITIAL_POS_Y = 440;

static const float SCORE_QAURTER_HALF = 0.125f;
static const float SCORE_HALF = 0.5f;
static const float SCORE_NORMAL = 1.0f;
static const float SCORE_DOUBLE = 2.0f;

static const int NUM_ITEM_TYPES = 8;

enum DIFFICULTIES{VERY_EASY = 1, VERY_EASY_HELD, EASY, EASY_HELD, NORMAL, NORMAL_HELD, HARD, HARD_HELD, VERY_HARD
, VERY_HARD_HELD};
