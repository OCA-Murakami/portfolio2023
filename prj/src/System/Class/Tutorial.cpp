#include <vector>
#include <string>
#include <fstream>
#include "../WinMain.h"
#include "Tutorial.h"

std::vector<std::string> Tutorial::LoadTelops(std::string _filePath) const
{
	std::ifstream ifs(_filePath.c_str());
	std::string lineStr;
	std::vector<std::string> tmpVector;

	if (!ifs.is_open())
	{
		// ここなんかしらエラー表記したい
	}

	while (!ifs.eof())
	{
		std::getline(ifs, lineStr);
		tmpVector.emplace_back(lineStr);
	}

	return tmpVector;
}

void Tutorial::CharaBufInit()
{
	for (int w = 0; w < mWIDTHMAX; ++w)
	{
		for (int h = 0; h < mHEIGHTMAX; ++h)
		{
			mCharaBuf[h][w] = '\0';
		}
	}

	mCharaCursorX = 0;
	mCharaCursorY = 0;
}

void Tutorial::NewLineTelops()
{
	mCharaCursorX = 0;
	++mCharaCursorY;
}

bool Tutorial::IsMaxCharaWidth() const
{
	if (mCharaCursorX >= mWIDTHMAX)
	{
		return true;
	}

	return false;
}

bool Tutorial::IsFinishDrawLine() const
{
	if (mLineCharaIdx == static_cast<int>(mLineBuf[mLineRefeNum].size()))
	{
		return true;
	}

	return false;
}

Tutorial::Tutorial():
	mLineRefeNum(0),mLineCharaIdx(0),
	mCharaBuf{ '\0' },mCharaCursorX(0),mCharaCursorY(0)
{
}

Tutorial::~Tutorial()
{
}

void Tutorial::Init(std::string _filePath)
{
	// ファイルから１行ずつ分けて文字列を読み込む
	mLineBuf = LoadTelops(_filePath);
	mLineRefeNum  = 0;
	mLineCharaIdx = 0;

	for (int w = 0; w < mWIDTHMAX; ++w)
	{
		for (int h = 0; h < mHEIGHTMAX; ++h)
		{
			mCharaBuf[h][w] =  '\0';
		}
	}
	mCharaCursorX = 0;
	mCharaCursorY = 0;
}

void Tutorial::Update()
{
	// 全てのテロップが描画され切っていれば関数を抜ける
	if (mLineRefeNum == mLineBuf.size() - 1 && IsFinishDrawLine())
	{
		return;
	}

	++sec;

	if (sec > 10)
	{
		sec = 0;

		// 文字列の終端まで進んだら、次の文字列を設定する
		if (IsFinishDrawLine())
		{
			CharaBufInit();

			// 文字列の参照番号を進める
			++mLineRefeNum;
			mLineCharaIdx = 0;
		}

		// ２バイト分、データを移す
		mCharaBuf[0][mCharaCursorX * 2]		= mLineBuf[mLineRefeNum].at(mLineCharaIdx);
		mCharaBuf[0][mCharaCursorX * 2 + 1] = mLineBuf[mLineRefeNum].at(static_cast<__int64>(mLineCharaIdx) + 1);

		// 参照カーソルを１つ進める
		++mCharaCursorX;

		// 改行処理
		if (IsMaxCharaWidth())
		{
			NewLineTelops();
		}

		// 文字列内の参照位置を２バイト分進める
		mLineCharaIdx += 2;

	}
}

void Tutorial::Draw()
{
	DrawString(0, 270, mCharaBuf[0], GetColor(255, 255, 255));
}

size_t Tutorial::GetLineSize() const
{
	return mLineBuf.size();
}
