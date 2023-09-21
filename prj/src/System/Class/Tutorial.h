#pragma once
//-----------------------------------------------------------
//!@class Tutorialクラス
//!@brief チュートリアル時に描画するテロップ処理をする
//-----------------------------------------------------------
class Tutorial
{
	// -----------------------------------------------------
	// 定数
	// -----------------------------------------------------
	const int mWIDTHMAX  = 20;		//!< 1行当たりの文字数
	const int mHEIGHTMAX = 3;		//!< 最大行数
	const int mCHARABITE = 2;		//!< ANSI形式の１文字当たりのバイト数

	// -----------------------------------------------------
	// 外部ファイルから行単位で取得した文字列を扱う
	// -----------------------------------------------------
	std::vector<std::string> mLineBuf;						//!< ファイルから取得した１行を格納する
	int						 mLineRefeNum;					//!< 各行参照用の番号
	int						 mLineCharaIdx;					//!< 行内の１文字参照用の番号

	// -----------------------------------------------------
	// 文字列を１文字単位で扱う
	// -----------------------------------------------------
	char mCharaBuf[3][20 * 2 + 1];			//!< １行文字列を１文字ずつ区切って格納する
	int  mCharaCursorX;						//!< X方向の参照位置
	int  mCharaCursorY;						//!< Y方向の参照位置




	int sec;

private:

	//! @brief 外部ファイルからテロップをロード
	//! @return １行ずつ区切って格納された文字列
	std::vector<std::string> LoadTelops(std::string _filePath) const;

	//! @brief 1文字バッファの初期化
	void CharaBufInit();

	//! @brief 描画するテロップの改行
	void NewLineTelops();

	//! @brief 描画しているテロップが１行文満たしたかチェック
	bool IsMaxCharaWidth() const;

	//! @brief １行当りのテロップ描画終了チェック
	//! @return true : １行分のテロップ描画終了		false : まだ描画中
	bool IsFinishDrawLine() const;

public:
	Tutorial();		//!< コンストラクタ
	~Tutorial();	//!< デストラクタ

	//! @brief 初期化
	//! @param _filePath	読み込むファイルのパス 
	void Init(std::string _filePath);

	void Update();

	void Draw();

	//! @brief テキストファイルから読み込んだ行数を取得する
	//! @return mLineStrのサイズ
	size_t GetLineSize() const;
};

