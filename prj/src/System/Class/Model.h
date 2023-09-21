#pragma once
#include <string>
#include <vector>

class Model
{
	int				 mModel;
	int				 mPixelShaderHandle;
	std::vector<int> mVertexShaderHandle;

	void LoadShader();
	void DrawByMesh(int _mesh);

public:
	Model();
	~Model();

	//! 読み込み
	void Load(const std::string& _fname);
	//! 描画
	void Draw();
	//! モデルデータの取得
	int GetModel() const;

	//! モデルデータの破棄
	void Exit();
};

