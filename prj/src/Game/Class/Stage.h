#pragma once
//------------------------------------------------
// �C���N���[�h
//------------------------------------------------

//------------------------------------------------
//!@class �X�e�[�W�N���X
//!@brief �X�e�[�W�̑�����s���N���X 
//------------------------------------------------
class Stage
{
	//! @brief �`��ɕK�v�ȕϐ����܂Ƃ߂��\����
	struct Context
	{
		Model   modelData;	//!< ���f���f�[�^
		Vector3 pos;		//!< ���W
		Vector3 rot;		//!< ����
		Vector3 size;		//!< �T�C�Y
	};

	//! �R���e�L�X�g�̔z��Q�Ɨp�̗񋓑�
	enum STAGETYPE
	{
		Game,		//!< �Q�[���X�e�[�W
		Training,	//!< �g���[�j���O�X�e�[�W
		STAGEMAX,	//!< �ő吔
	};

	//! �X�e�[�W���f���̃R���e�L�X�g
	Context mModelContexts[STAGETYPE::STAGEMAX];

	//! ���[�v�G�t�F�N�g�̃t�@�C���p�X
	const std::string mEFFECTWARPPATH = "data/effect/warp/Warp2.efkefc";

	const float mWARPRANGE = 25.0f;		//!< ���[�v�͈̔�
	Effect  mEffectPlayer;				//!< �G�t�F�N�g�v���C���[
	int	    mEffectWarpHandle;			//!< ���[�v�G�t�F�N�g�̃n���h��
	Vector3 mEffectWarpPos;				//!< ���[�v�G�t�F�N�g�̍��W


private:

	//! @brief �Q�[���X�e�[�W�̏�����
	//! @param _modelPath ���f���̃t�@�C���p�X
	void GameStageInit(std::string _modelPath);

	//! @brief �g���[�j���O�X�e�[�W�̏�����
	//! @param _modelPath ���f���̃t�@�C���p�X
	void TrainingStageInit(std::string _modelPath);

	//! @brief ���[�v�G�t�F�N�g�̃n���h�����擾����
	int GetEffectWarpHandle() const;


public:
	Stage();		//!< �R���X�g���N�^
	~Stage();		//!< �f�X�g���N�^

	//! @brief ���[�v�̔��a���擾����
	//! @return mWARPRANGE �̒l
	float GetWarpRange() const;

	//! @brief ���[�v�G�t�F�N�g�̍��W���擾����
	//! @return mEffectWarpPos �̒l
	Vector3 GetEffectWarpPos() const;

	//! @brief ������
	//! @param _filePaths �e�X�e�[�W�̃t�@�C���p�X���������A�h���X
	void Init(const std::string* _filePaths);		

	void StageDraw();		//!< �Q�[���X�e�[�W�̕`��
	void TrainingDraw();	//!< �g���[�j���O�X�e�[�W�̕`��

	void Exit();			//!< �I��
};

