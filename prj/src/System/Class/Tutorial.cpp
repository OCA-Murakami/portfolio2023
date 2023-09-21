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
		// �����Ȃ񂩂���G���[�\�L������
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
	// �t�@�C������P�s�������ĕ������ǂݍ���
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
	// �S�Ẵe���b�v���`�悳��؂��Ă���Ί֐��𔲂���
	if (mLineRefeNum == mLineBuf.size() - 1 && IsFinishDrawLine())
	{
		return;
	}

	++sec;

	if (sec > 10)
	{
		sec = 0;

		// ������̏I�[�܂Ői�񂾂�A���̕������ݒ肷��
		if (IsFinishDrawLine())
		{
			CharaBufInit();

			// ������̎Q�Ɣԍ���i�߂�
			++mLineRefeNum;
			mLineCharaIdx = 0;
		}

		// �Q�o�C�g���A�f�[�^���ڂ�
		mCharaBuf[0][mCharaCursorX * 2]		= mLineBuf[mLineRefeNum].at(mLineCharaIdx);
		mCharaBuf[0][mCharaCursorX * 2 + 1] = mLineBuf[mLineRefeNum].at(static_cast<__int64>(mLineCharaIdx) + 1);

		// �Q�ƃJ�[�\�����P�i�߂�
		++mCharaCursorX;

		// ���s����
		if (IsMaxCharaWidth())
		{
			NewLineTelops();
		}

		// ��������̎Q�ƈʒu���Q�o�C�g���i�߂�
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
