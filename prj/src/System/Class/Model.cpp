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
	// ���_�V�F�[�_�[�ǂݍ���
	{
		// �g���C�A���O�����X�g�̒��_�^�C�v�̍ő吔
		const int TRIANGLETYPEMAX = 8;

		std::stringstream sstr;
		for (int i = 0; i < TRIANGLETYPEMAX; ++i)
		{
			sstr << "data/Shader/vs_model_" << i << ".vso";
			mVertexShaderHandle.emplace_back(LoadVertexShader(sstr.str().c_str()));
			sstr.str("");
		}
	}

	// �s�N�Z���V�F�[�_�[�ǂݍ���
	{
		mPixelShaderHandle = LoadPixelShader("data/Shader/ps_model.pso");
	}
}

void Model::DrawByMesh(int _mesh)
{
	// �I���W�i���V�F�[�_�[���g�p��ON�ɂ���
	MV1SetUseOrigShader(true);

	for (int tri = 0; tri < MV1GetMeshTListNum(mModel, _mesh); ++tri)
	{
		// �g���C�A���O�����X�g�ԍ�
		int tlist = MV1GetMeshTList(mModel, _mesh, tri);

		// ���_�f�[�^�^�C�v
		int vertex_type = MV1GetTriangleListVertexType(mModel, tlist);

		SetUseVertexShader(mVertexShaderHandle[vertex_type]);
		SetUsePixelShader(mPixelShaderHandle);

		// �`��
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
		// ���f���Ɋ܂܂�郁�b�V���̐�
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
