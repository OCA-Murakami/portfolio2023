#pragma once
//------------------------------------------------
// �C���N���[�h
//------------------------------------------------

//------------------------------------------------
//!@class Vector3�N���X
//!@brief x,y,z �������A���K���Ⓑ���̌v�Z�������N���X
//------------------------------------------------
class Vector3
{
public:
	// ���̂R��Private�ɂ����
	//���̃N���X�^�Ő錾���ꂽ�ϐ��Ŏg�p�ł��Ȃ�����Public�Ő錾
	float x;
	float y;
	float z;

	Vector3();			//!< �R���X�g���N�^
	Vector3(float _x, float _y, float _z);	//!< �����t���R���X�g���N�^
	~Vector3();			//!< �f�X�g���N�^	

	//-----------------------------
	// �l�̑���֐�
	//-----------------------------
	void Clear();							//!< �[���������֐�
	void Set(float _x, float _y, float _z);	//!< �l�̃Z�b�g�i�P���l���Z�b�g����j
	void Set(const Vector3& _v);					//!< �l�̃Z�b�g�i�ꊇ�Z�b�g�j

	//-----------------------------
	// �����ɂ��Ă̊֐�
	//-----------------------------
	float GetLength()const;				//!< �������擾
	void  Normalize();					//!< ���K��
	void  SetLength(float _length);		//!< ������ݒ�

	//-----------------------------
	// ���Z�q�̃I�[�o�[���[�h
	//-----------------------------
	Vector3& operator = (const Vector3& v);		//!< = ���Z�q�I�[�o�[���[�h
	Vector3& operator += (const Vector3& v);	//!< += ���Z�q�I�[�o�[���[�h
	Vector3& operator -= (const Vector3& v);	//!< -= ���Z�q�I�[�o�[���[�h
	Vector3& operator *= (const Vector3& v);	//!< *= ���Z�q�I�[�o�[���[�h
	Vector3& operator *= (const float& f);	//!< *= ���Z�q�I�[�o�[���[�h(float�^)
	Vector3& operator /= (const Vector3& v);	//!< /= ���Z�q�I�[�o�[���[�h

	//-----------------------------
	// DX���C�u�����Ŏg���ϐ��ւ̌݊�
	//-----------------------------
	VECTOR GetVECTOR() const;				//!< VECTOR�^�ɂ��Ēl���擾
	void   SetVECTOR(const VECTOR& _v);		//!< VECTOR�^�̒l���Z�b�g����
};

Vector3 operator + (const Vector3& _v0, const Vector3& _v1);	//!< +���Z�q�I�[�o�[���[�h
Vector3 operator - (const Vector3& _v0, const Vector3& _v1);	//!< -���Z�q�I�[�o�[���[�h
Vector3 operator * (const Vector3& _v0, const float _f);		//!< *���Z�q�I�[�o�[���[�h
Vector3 operator / (const Vector3& _v0, const float _f);		//!< /���Z�q�I�[�o�[���[�h

float GetVector3Distance(const Vector3& _v0, const Vector3& _v1);	//!< 2��Vector3�̋������v�Z����
float GetVector3Dot(const Vector3& _v0, const Vector3& _v1);		//!< 2��Vector3�̓��ς��v�Z����
Vector3 GetVector3Cross(const Vector3& _v0, const Vector3& _v1);	//!< 2��Vector3�̊O�ς��v�Z����

