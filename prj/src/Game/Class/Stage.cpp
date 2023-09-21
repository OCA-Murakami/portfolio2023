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

// コンストラクタ
Stage::Stage() :
	mEffectWarpHandle(-1)
{
	mEffectWarpPos.Set(0.0f, 0.0f, 0.0f);
}

// デストラクタ
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

// 初期化
void Stage::Init(const std::string* _filePaths)
{
	GameStageInit(_filePaths[STAGETYPE::Game]);
	TrainingStageInit(_filePaths[STAGETYPE::Training]);
}

// 描画
void Stage::StageDraw()
{
	MV1SetPosition(mModelContexts[STAGETYPE::Game].modelData.GetModel(), mModelContexts[STAGETYPE::Game].pos.GetVECTOR());
	MV1SetRotationXYZ(mModelContexts[STAGETYPE::Game].modelData.GetModel(), mModelContexts[STAGETYPE::Game].rot.GetVECTOR());
	MV1SetScale(mModelContexts[STAGETYPE::Game].modelData.GetModel(), mModelContexts[STAGETYPE::Game].size.GetVECTOR());
	mModelContexts[STAGETYPE::Game].modelData.Draw();
}

void Stage::TrainingDraw()
{
	// ステージの描画
	MV1SetPosition(mModelContexts[STAGETYPE::Training].modelData.GetModel(), mModelContexts[STAGETYPE::Training].pos.GetVECTOR());
	MV1SetRotationXYZ(mModelContexts[STAGETYPE::Training].modelData.GetModel(), mModelContexts[STAGETYPE::Training].rot.GetVECTOR());
	MV1SetScale(mModelContexts[STAGETYPE::Training].modelData.GetModel(), mModelContexts[STAGETYPE::Training].size.GetVECTOR());
	mModelContexts[STAGETYPE::Training].modelData.Draw();


	// ループ再生処理
	if (mEffectPlayer.CheckPlayEnd())
	{
		mEffectPlayer.ReleasePlay();
	}

	// エフェクトの描画
	if (!mEffectPlayer.IsPlay())
	{
		mEffectPlayer.SetPlay(GetEffectWarpHandle(), GetEffectWarpPos());
	}

}

// 終了
void Stage::Exit()
{
	MV1DeleteModel(mModelContexts[STAGETYPE::Game].modelData.GetModel());
}
