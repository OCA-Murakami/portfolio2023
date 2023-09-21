#pragma once
//------------------------------------------------
// �C���N���[�h
//------------------------------------------------

//------------------------------------------------
//!@class �J�����N���X
//!@brief  
//------------------------------------------------
class Camera
{

	const float	  mANGLEMOVERATIO = 0.02f;		//!< �J�����A���O���̈ړ����x
	const float	  mLOOKDISTANCE   = 30.0f;		//!< �����_�܂ł̋���
		  float	  mAngleH;						//!< ���������̊p�x
		  float	  mAngleV;						//!< ���������̊p�x
		  Vector3 mPos;							//!< ���W
		  Vector3 mLookPos;						//!< �����_
		  int	  mShakeTimer;					//!< �J������h�炷�^�C�}�[

	//-----------------------------
	// ���[�r�[�����o���Ɏg���ϐ�
	//-----------------------------
	const int mMOVIESHAKETIME = 30;	//!< ���o���ŃJ������h�炷����
	Vector3	  mBasePos;				//!< ���o�I�����ɂ��ė~������ʒu
	Vector3	  mBaseLookPos;			//!< ���o�I�����Ɍ��ė~������ʒu
	bool	  mIsShakeMovie;		//!< ���o���ŃJ������h�炷�t���O
	bool	  misFinishMovie;		//!< ���o�̏I���t���O

	//-----------------------------
	// ���b�N�I���̏����Ɏg���ϐ�
	//-----------------------------
	const CharacterBase* mTargetEnemy = nullptr;	//!< ��������G
	const Player*		 mPlayerInfo  = nullptr;	//!< �ǔ�����v���C���[�̏��

	void AngleUpdate();		//!< �}�E�X�Œ����_�̑���

public:
	Camera();		//!< �R���X�g���N�^
	~Camera();		//!< �f�X�g���N�^

	float   GetAngleHorizontal() const;		//!< �J�����̐����������擾����
	Vector3 ReflectionAngle()    const;		//!< �J�����̊p�x�����W�ɔ��f������
	void    ShakeUpdate();					//!< �J������h�炷

	//-----------------------------
	// ���[�r�[�����o���Ɏg���֐�
	//-----------------------------
	void SetFinishMovie();			//!< ���o�̏I��
	bool IsFinishMovie() const;		//!< ���o�̏I���`�F�b�N
	void SetMovieShakeTime();		//!< ���o�̒����_��h�炷���Ԃ��Z�b�g����
	void SetMovieShake();			//!< �J������h�炷�t���O���Z�b�g����
	bool IsMovieShake() const;		//!< �J������h�炷�t���O���`�F�b�N

	//------------------------------------
	// ���b�N�I�������̊֐�
	//------------------------------------
	void SetTargetInfo(const CharacterBase* _enemy);							//!< ���b�N�I�����A��������Ώۂ��Z�b�g
	void SetPlayerInfo(const Player* _player);									//!< �v���C���[�̏����擾
	void SetTargetPos(Vector3 _targetPos);										//!< �G�ɃJ������������i�^�[�Q�b�g�@�\�j
	void SetBehindPlayer(const Vector3& _playerPos, const float& _playerRot);	//!< �v���C���[�̌��ֈړ�����
	void ReleaseLockOn();														//!< ���b�N�I������������u�Ԃ̊p�x��ێ�������

	void Init();					//!< ������
	void MovieInit();				//!< ���[�r�[���[�h�p�̏�����
	void MovieUpdate();				//!< ���[�r�[�����o���̏���
	void Update(bool _isDebug);		//!< �X�V
	void Draw();					//!< �`��
	void Exit();					//!< �I��
};

