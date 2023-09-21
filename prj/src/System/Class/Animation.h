#pragma once

class Animation 
{
	// ブレンド用に使用するアニメーション情報の順序に使用する
	enum Order
	{
		Now,		//!< 現在
		Before,		//!< １つ前
		OrderMax,	//!< 最大数
	};

	//! アニメーションの再生に必要な値を集めた構造体
	struct  Context
	{
		int	  animAttachIdx = -1;			//!< アニメーションのアタッチ番号
		int   animState		= -1;			//!< 各クラスから受け取るステート番号
		float nowTime		= 0.0f;			//!< 現再生時間
		float totalTime		= 0.0f;			//!< 総再生時間
	};

	const int		 mANIMIDX = 0;			//!< アニメーション番号（現在使用しているアニメーション番号は全て0だからconst）
	std::vector<int> mAnimPool;				//!< アニメーションのハンドルを入れる
	Context			 mContexts[OrderMax];	//!< アニメーション再生に使う（ブレンドのために２つ用意）
	float			 mBlendRatio;			//!< ブレンド率

	// ----------------------------------------------------------
	// 各クラスから値を取得して使用する変数
	// ----------------------------------------------------------
	int mModelData;				//!< 各クラスから取得するモデルデータ（引数削減用）
	int mBeforeAnimState;		//!< １つ前に再生されていたアニメーションの要素数を入れる

	// ----------------------------------------------------------
	// Private関数
	// ----------------------------------------------------------
	void Loop();			//!< ループ処理
	void CopyContext();		//!< アニメーションのブレンドのためにContextの値を移す
	void AttachAnimation(const int _orderIdx);
	//Context& operator =(const Context& _assignment);	//!< =演算子オーバーロード

public:
	Animation();
	~Animation();

	int SetModelData(const int& _modelData);

	//! @brief Context型の変数の初期化
	void InitContexts();

	//! @brief アニメーションの読み込み
	//! @param _filePath	読み込むアニメーションのファイルパス
	void Load(std::string _filePath);

	//! @brief アタッチ関数
	//! @param _stateIdx	アタッチしたいステート
	void Attach(const int _stateIdx);

	//! @brief デタッチ関数
	//! @param _orderIdx	デタッチしたいContextの要素数
	void Detach(const int _orderIdx);

	//! @brief 再生
	//! @param _playSpeed	再生速度
	//! @param _isLoop		ループフラグ
	void Play(float _playSpeed, bool _isLoop = true);

	//! @brief プレイ中かチェック
	//! @return		true : 再生中		false : 再生していない
	bool IsPlay() const;

	//! @brief 再生したいアニメーションのフレーム数をセットする
	//! @param _setTime		セットしたいフレーム数
	void SetAnimTime(const float _setTime);

	//! @brief 現在再生しているアニメーションの総再生時間
	//! @return mContexts[Now]のtotalTimeの数値
	float GetAnimTotalTime() const;

	//! @brief 現在再生しているアニメーションの現再生時間
	//! @return mContexts[Now]のnowTimeの数値
	float GetAnimNowTime() const;

	//! @brief 両足の座標を取得する
	//! @param _leftBase	左足ベースのフレーム名
	//! @param _rightBase 　右足ベースのフレーム名
	//! @return 両足の座標
	Vector3 FixedOffset(std::string _leftBase, std::string _rightBase);

	//! @brief 読み込まれたアニメーションの総数を取得する
	//! @return アニメーションの総数
	size_t GetAnimPoolSize() const;

	// 正味これいらん
	int GetAttachIdx() const;

	//! @brief ブレンド値の増加を滑らかにするイージング関数
	//! @param _blendRatio ブレンド率
	//! @return 補間後のブレンド率
	float EaseInOutCubic(float _blendRatio);

	//! @brief アニメーションの破棄
	void Exit();

	Vector3 GetBasePos(std::string _baseFrameName);
};