#pragma once

class Animation 
{
	// �u�����h�p�Ɏg�p����A�j���[�V�������̏����Ɏg�p����
	enum Order
	{
		Now,		//!< ����
		Before,		//!< �P�O
		OrderMax,	//!< �ő吔
	};

	//! �A�j���[�V�����̍Đ��ɕK�v�Ȓl���W�߂��\����
	struct  Context
	{
		int	  animAttachIdx = -1;			//!< �A�j���[�V�����̃A�^�b�`�ԍ�
		int   animState		= -1;			//!< �e�N���X����󂯎��X�e�[�g�ԍ�
		float nowTime		= 0.0f;			//!< ���Đ�����
		float totalTime		= 0.0f;			//!< ���Đ�����
	};

	const int		 mANIMIDX = 0;			//!< �A�j���[�V�����ԍ��i���ݎg�p���Ă���A�j���[�V�����ԍ��͑S��0������const�j
	std::vector<int> mAnimPool;				//!< �A�j���[�V�����̃n���h��������
	Context			 mContexts[OrderMax];	//!< �A�j���[�V�����Đ��Ɏg���i�u�����h�̂��߂ɂQ�p�Ӂj
	float			 mBlendRatio;			//!< �u�����h��

	// ----------------------------------------------------------
	// �e�N���X����l���擾���Ďg�p����ϐ�
	// ----------------------------------------------------------
	int mModelData;				//!< �e�N���X����擾���郂�f���f�[�^�i�����팸�p�j
	int mBeforeAnimState;		//!< �P�O�ɍĐ�����Ă����A�j���[�V�����̗v�f��������

	// ----------------------------------------------------------
	// Private�֐�
	// ----------------------------------------------------------
	void Loop();			//!< ���[�v����
	void CopyContext();		//!< �A�j���[�V�����̃u�����h�̂��߂�Context�̒l���ڂ�
	void AttachAnimation(const int _orderIdx);
	//Context& operator =(const Context& _assignment);	//!< =���Z�q�I�[�o�[���[�h

public:
	Animation();
	~Animation();

	int SetModelData(const int& _modelData);

	//! @brief Context�^�̕ϐ��̏�����
	void InitContexts();

	//! @brief �A�j���[�V�����̓ǂݍ���
	//! @param _filePath	�ǂݍ��ރA�j���[�V�����̃t�@�C���p�X
	void Load(std::string _filePath);

	//! @brief �A�^�b�`�֐�
	//! @param _stateIdx	�A�^�b�`�������X�e�[�g
	void Attach(const int _stateIdx);

	//! @brief �f�^�b�`�֐�
	//! @param _orderIdx	�f�^�b�`������Context�̗v�f��
	void Detach(const int _orderIdx);

	//! @brief �Đ�
	//! @param _playSpeed	�Đ����x
	//! @param _isLoop		���[�v�t���O
	void Play(float _playSpeed, bool _isLoop = true);

	//! @brief �v���C�����`�F�b�N
	//! @return		true : �Đ���		false : �Đ����Ă��Ȃ�
	bool IsPlay() const;

	//! @brief �Đ��������A�j���[�V�����̃t���[�������Z�b�g����
	//! @param _setTime		�Z�b�g�������t���[����
	void SetAnimTime(const float _setTime);

	//! @brief ���ݍĐ����Ă���A�j���[�V�����̑��Đ�����
	//! @return mContexts[Now]��totalTime�̐��l
	float GetAnimTotalTime() const;

	//! @brief ���ݍĐ����Ă���A�j���[�V�����̌��Đ�����
	//! @return mContexts[Now]��nowTime�̐��l
	float GetAnimNowTime() const;

	//! @brief �����̍��W���擾����
	//! @param _leftBase	�����x�[�X�̃t���[����
	//! @param _rightBase �@�E���x�[�X�̃t���[����
	//! @return �����̍��W
	Vector3 FixedOffset(std::string _leftBase, std::string _rightBase);

	//! @brief �ǂݍ��܂ꂽ�A�j���[�V�����̑������擾����
	//! @return �A�j���[�V�����̑���
	size_t GetAnimPoolSize() const;

	// �������ꂢ���
	int GetAttachIdx() const;

	//! @brief �u�����h�l�̑��������炩�ɂ���C�[�W���O�֐�
	//! @param _blendRatio �u�����h��
	//! @return ��Ԍ�̃u�����h��
	float EaseInOutCubic(float _blendRatio);

	//! @brief �A�j���[�V�����̔j��
	void Exit();

	Vector3 GetBasePos(std::string _baseFrameName);
};