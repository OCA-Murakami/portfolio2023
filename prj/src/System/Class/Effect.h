#pragma once
//------------------------------------------------
//!@class Effectクラス
//!@brief エフェクトの再生に必要な操作をまとめてる
//------------------------------------------------
class Effect
{
	int  playingHandle;		//!< 再生したいエフェクトのハンドルを入れる
	bool isPlaying;			//!< 重複再生を防ぐ用（エフェクトによって使用しない場合もある）
public:
	Effect();		//!< コンストラクタ
	~Effect();		//!< デストラクタ

	void SetPlay(const int _effectHandle, Vector3 _effectPos);
	
	void Stop(const int _handle);

	void ReleasePlay();
	bool IsPlay()		const;
	bool CheckPlayEnd() const;
};

