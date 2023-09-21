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

	//! �ǂݍ���
	void Load(const std::string& _fname);
	//! �`��
	void Draw();
	//! ���f���f�[�^�̎擾
	int GetModel() const;

	//! ���f���f�[�^�̔j��
	void Exit();
};

