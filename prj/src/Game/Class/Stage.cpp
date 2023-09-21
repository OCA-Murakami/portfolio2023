#include <string>

#include "../../System/WinMain.h"
#include "../../System/Class/Vector3.h"
#include "../../System//Class/Effect.h"
#include "../../System/Class/Model.h"
#include "Stage.h"

void Stage::GameStageInit(std::string _modelPath)
{
	mModelContexts[STAGETYPE::Game].modelData.Load(_modelPath);
	mModelContexts[STAGETYPE::Game].pos.Set(0.0f,-10.0f,0.0f);
	mModelContexts[STAGETYPE::Game].rot.Clear();
	mModelContexts[STAGETYPE::Game].size.Set(0.5f, 0.5f, 0.5f);
}

void Stage::TrainingStageInit(std::string _modelPath)
{
	mModelContexts[STAGETYPE::Training].modelData.Load(_modelPath);
	mModelContexts[STAGETYPE::Training].pos.Set(0.0f, -10.0f, 0.0f);
	mModelContexts[STAGETYPE::Training].rot.Clear();
	mModelContexts[STAGETYPE::Training].size.Set(0.5f, 0.5f, 0.5f);

	mEffectWarpHandle = LoadEffekseerEffect(mEFFECTWARPPATH.c_str(),15.0f);
	mEffectWarpPos.Set(0.0f, 0.0f, -200.0f);
}

int Stage::GetEffectWarpHandle() const
{
	return mEffectWarpHandle;
}

// �R���X�g���N�^
Stage::Stage() :
	mEffectWarpHandle(-1)
{
	mEffectWarpPos.Set(0.0f, 0.0f, 0.0f);
}

// �f�X�g���N�^
Stage::~Stage()
{
}

float Stage::GetWarpRange() const
{
	return mWARPRANGE;
}

Vector3 Stage::GetEffectWarpPos() const
{
	return mEffectWarpPos;
}

// ������
void Stage::Init(const std::string* _filePaths)
{
	GameStageInit(_filePaths[STAGETYPE::Game]);
	TrainingStageInit(_filePaths[STAGETYPE::Training]);
}

// �`��
void Stage::StageDraw()
{
	MV1SetPosition(mModelContexts[STAGETYPE::Game].modelData.GetModel(), mModelContexts[STAGETYPE::Game].pos.GetVECTOR());
	MV1SetRotationXYZ(mModelContexts[STAGETYPE::Game].modelData.GetModel(), mModelContexts[STAGETYPE::Game].rot.GetVECTOR());
	MV1SetScale(mModelContexts[STAGETYPE::Game].modelData.GetModel(), mModelContexts[STAGETYPE::Game].size.GetVECTOR());
	mModelContexts[STAGETYPE::Game].modelData.Draw();
}

void Stage::TrainingDraw()
{
	// �X�e�[�W�̕`��
	MV1SetPosition(mModelContexts[STAGETYPE::Training].modelData.GetModel(), mModelContexts[STAGETYPE::Training].pos.GetVECTOR());
	MV1SetRotationXYZ(mModelContexts[STAGETYPE::Training].modelData.GetModel(), mModelContexts[STAGETYPE::Training].rot.GetVECTOR());
	MV1SetScale(mModelContexts[STAGETYPE::Training].modelData.GetModel(), mModelContexts[STAGETYPE::Training].size.GetVECTOR());
	mModelContexts[STAGETYPE::Training].modelData.Draw();


	// ���[�v�Đ�����
	if (mEffectPlayer.CheckPlayEnd())
	{
		mEffectPlayer.ReleasePlay();
	}

	// �G�t�F�N�g�̕`��
	if (!mEffectPlayer.IsPlay())
	{
		mEffectPlayer.SetPlay(GetEffectWarpHandle(), GetEffectWarpPos());
	}

}

// �I��
void Stage::Exit()
{
	MV1DeleteModel(mModelContexts[STAGETYPE::Game].modelData.GetModel());
}
