#include "Model.h"
#include "../dxlib/DxLib.h"

#include <sstream>

Model::Model() :
	mModel(-1), mPixelShaderHandle(-1), mVertexShaderHandle(0)
{
}

Model::~Model()
{
}

void Model::Load(const std::string& _fname)
{
	mModel = MV1LoadModel(_fname.c_str());

	LoadShader();
}

void Model::LoadShader()
{
	// 頂点シェーダー読み込み
	{
		// トライアングルリストの頂点タイプの最大数
		const int TRIANGLETYPEMAX = 8;

		std::stringstream sstr;
		for (int i = 0; i < TRIANGLETYPEMAX; ++i)
		{
			sstr << "data/Shader/vs_model_" << i << ".vso";
			mVertexShaderHandle.emplace_back(LoadVertexShader(sstr.str().c_str()));
			sstr.str("");
		}
	}

	// ピクセルシェーダー読み込み
	{
		mPixelShaderHandle = LoadPixelShader("data/Shader/ps_model.pso");
	}
}

void Model::DrawByMesh(int _mesh)
{
	// オリジナルシェーダーを使用をONにする
	MV1SetUseOrigShader(true);

	for (int tri = 0; tri < MV1GetMeshTListNum(mModel, _mesh); ++tri)
	{
		// トライアングルリスト番号
		int tlist = MV1GetMeshTList(mModel, _mesh, tri);

		// 頂点データタイプ
		int vertex_type = MV1GetTriangleListVertexType(mModel, tlist);

		SetUseVertexShader(mVertexShaderHandle[vertex_type]);
		SetUsePixelShader(mPixelShaderHandle);

		// 描画
		MV1DrawTriangleList(mModel, tlist);
	}
	MV1SetUseOrigShader(false);
}

void Model::Draw()
{
#if 0
	MV1DrawModel(m_model);
	return;
#endif

	for (int mesh = 0; mesh < MV1GetMeshNum(mModel); ++mesh) 
	{   
		// モデルに含まれるメッシュの数
		DrawByMesh(mesh);
	}
}

int Model::GetModel() const
{
	return mModel;
}

void Model::Exit()
{
	MV1DeleteModel(mModel);
}
