#pragma once
//------------------------------------------------
// インクルード
//------------------------------------------------

//------------------------------------------------
//!@class ステージクラス
//!@brief ステージの操作を行うクラス 
//------------------------------------------------
class Stage
{
	//! @brief 描画に必要な変数をまとめた構造体
	struct Context
	{
		Model   modelData;	//!< モデルデータ
		Vector3 pos;		//!< 座標
		Vector3 rot;		//!< 向き
		Vector3 size;		//!< サイズ
	};

	//! コンテキストの配列参照用の列挙体
	enum STAGETYPE
	{
		Game,		//!< ゲームステージ
		Training,	//!< トレーニングステージ
		STAGEMAX,	//!< 最大数
	};

	//! ステージモデルのコンテキスト
	Context mModelContexts[STAGETYPE::STAGEMAX];

	//! ワープエフェクトのファイルパス
	const std::string mEFFECTWARPPATH = "data/effect/warp/Warp2.efkefc";

	const float mWARPRANGE = 25.0f;		//!< ワープの範囲
	Effect  mEffectPlayer;				//!< エフェクトプレイヤー
	int	    mEffectWarpHandle;			//!< ワープエフェクトのハンドル
	Vector3 mEffectWarpPos;				//!< ワープエフェクトの座標


private:

	//! @brief ゲームステージの初期化
	//! @param _modelPath モデルのファイルパス
	void GameStageInit(std::string _modelPath);

	//! @brief トレーニングステージの初期化
	//! @param _modelPath モデルのファイルパス
	void TrainingStageInit(std::string _modelPath);

	//! @brief ワープエフェクトのハンドルを取得する
	int GetEffectWarpHandle() const;


public:
	Stage();		//!< コンストラクタ
	~Stage();		//!< デストラクタ

	//! @brief ワープの半径を取得する
	//! @return mWARPRANGE の値
	float GetWarpRange() const;

	//! @brief ワープエフェクトの座標を取得する
	//! @return mEffectWarpPos の値
	Vector3 GetEffectWarpPos() const;

	//! @brief 初期化
	//! @param _filePaths 各ステージのファイルパスが入ったアドレス
	void Init(const std::string* _filePaths);		

	void StageDraw();		//!< ゲームステージの描画
	void TrainingDraw();	//!< トレーニングステージの描画

	void Exit();			//!< 終了
};

