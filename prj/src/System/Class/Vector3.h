#pragma once
//------------------------------------------------
// インクルード
//------------------------------------------------

//------------------------------------------------
//!@class Vector3クラス
//!@brief x,y,z を扱い、正規化や長さの計算を扱うクラス
//------------------------------------------------
class Vector3
{
public:
	// この３つをPrivateにすると
	//このクラス型で宣言された変数で使用できないからPublicで宣言
	float x;
	float y;
	float z;

	Vector3();			//!< コンストラクタ
	Vector3(float _x, float _y, float _z);	//!< 引数付きコンストラクタ
	~Vector3();			//!< デストラクタ	

	//-----------------------------
	// 値の操作関数
	//-----------------------------
	void Clear();							//!< ゼロ初期化関数
	void Set(float _x, float _y, float _z);	//!< 値のセット（１つずつ値をセットする）
	void Set(const Vector3& _v);					//!< 値のセット（一括セット）

	//-----------------------------
	// 長さについての関数
	//-----------------------------
	float GetLength()const;				//!< 長さを取得
	void  Normalize();					//!< 正規化
	void  SetLength(float _length);		//!< 長さを設定

	//-----------------------------
	// 演算子のオーバーロード
	//-----------------------------
	Vector3& operator = (const Vector3& v);		//!< = 演算子オーバーロード
	Vector3& operator += (const Vector3& v);	//!< += 演算子オーバーロード
	Vector3& operator -= (const Vector3& v);	//!< -= 演算子オーバーロード
	Vector3& operator *= (const Vector3& v);	//!< *= 演算子オーバーロード
	Vector3& operator *= (const float& f);	//!< *= 演算子オーバーロード(float型)
	Vector3& operator /= (const Vector3& v);	//!< /= 演算子オーバーロード

	//-----------------------------
	// DXライブラリで使う変数への互換
	//-----------------------------
	VECTOR GetVECTOR() const;				//!< VECTOR型にして値を取得
	void   SetVECTOR(const VECTOR& _v);		//!< VECTOR型の値をセットする
};

Vector3 operator + (const Vector3& _v0, const Vector3& _v1);	//!< +演算子オーバーロード
Vector3 operator - (const Vector3& _v0, const Vector3& _v1);	//!< -演算子オーバーロード
Vector3 operator * (const Vector3& _v0, const float _f);		//!< *演算子オーバーロード
Vector3 operator / (const Vector3& _v0, const float _f);		//!< /演算子オーバーロード

float GetVector3Distance(const Vector3& _v0, const Vector3& _v1);	//!< 2つのVector3の距離を計算する
float GetVector3Dot(const Vector3& _v0, const Vector3& _v1);		//!< 2つのVector3の内積を計算する
Vector3 GetVector3Cross(const Vector3& _v0, const Vector3& _v1);	//!< 2つのVector3の外積を計算する

