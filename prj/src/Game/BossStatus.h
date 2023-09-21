#pragma once
//---------------------------------------------------------------------------
//!	@file	BossStatud.h
//! @brief	CSV�t�@�C������̒l���󂯎�邽�߂̍\���̂Ɨ񋓑�
//---------------------------------------------------------------------------
#include "../System/WinMain.h"
#include "../System/Class/Vector3.h"

//! @brief attackPower�����₷�����邽�߂̗񋓑�
enum BOSSATTACKTYPE
{
	Light,		//!< ��U��
	Heavy,		//!< ���U��
	Combo,		//!< �R���{�U��
	Jump,		//!< �W�����v�U��
	AttackMax,	//!< �ő吔
};

//! @brief �U���p�̃J�v�Z�����W�̐�[�ƍ��{�p
enum BOSSATTACKCOLLTYPE
{
	Top,		//!< ��[
	Under,		//!< ���{
	AttackCollMax,	//!< �ő吔
};

enum BOSSCOLLTYPE
{
	Body,			//!< �{�f�B
	FistTopX,		//!< ���̐�[X���W
	FistTopY,		//!< ���̐�[Y���W
	FistTopZ,		//!< ���̐�[Z���W
	FistUnderX,		//!< ���̍��{X���W
	FistUnderY,		//!< ���̍��{Y���W
	FistUnderZ,		//!< ���̍��{Z���W
	FistSize,		//!< ���T�C�Y
	ClawTopX,		//!< �܂̐�[X���W
	ClawTopY,		//!< �܂̐�[Y���W
	ClawTopZ,		//!< �܂̐�[Z���W
	ClawUnderX,		//!< �܂̍��{X���W
	ClawUnderY,		//!< �܂̍��{Y���W
	ClawUnderZ,		//!< �܂̍��{Z���W
	ClawSize,		//!< �܃T�C�Y
	CollMax,		//!< �ő吔
};

//! @brief [,]��؂�Ŏ擾�����e������̎Q�Ɨp
enum BOSSSTATUSTYPE
{
	HpMax,			//!< HP
	LightPower,		//!< ��U����
	HeavyPower,		//!< ���U����
	ComboPower,		//!< �R���{�U����
	JumpPower,		//!< �W�����v�U����
	MoveSpeed,		//!< �ړ����x
	CollSize,		//!< ��U�����̓����蔻��T�C�Y
	StatusMax,		//!< �ő吔
};

//! @brief  CSV�t�@�C������l���󂯎��ϐ��Q
struct BossStatus
{
	float hpMax									 = 0.0f;					//!< HP
	float attackPower[BOSSATTACKTYPE::AttackMax] = {0.0f,0.0f,0.0f,0.0f};	//!< �U�����
	float moveSpeed								 = 0.0f;					//!< �ړ����x
	float collSize								 = 0.0f;					//!< ��U�����̓����蔻��T�C�Y
};

//! @brief �����蔻��Ɏg�p����e�ϐ����󂯎��
struct BossCollInfo
{
	float   bodySize = 0.0f;								//!< �̂̃J�v�Z���T�C�Y
	Vector3 fistPos[BOSSATTACKCOLLTYPE::AttackCollMax];		//!< �������J�v�Z���̍��W
	float   fistSize = 0.0f;								//!< �������J�v�Z���̃T�C�Y
	Vector3 clawPos[BOSSATTACKCOLLTYPE::AttackCollMax];		//!< �ܕ����J�v�Z���̍��W
	float   clawSize = 0.0f;								//!< �ܕ����J�v�Z���̃T�C�Y
};
