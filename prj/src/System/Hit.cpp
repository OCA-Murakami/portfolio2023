#include "Cast.h"
#include "../Game/Class/CharacterBase.h"
#include "../Game//Class/Boss.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"

#include "../System/Hit.h"

bool CheckBallHit(const Vector3& _p0, const float _r0,
	const Vector3& _p1, const float _r1)
{
	float distance = GetVector3Distance(_p0, _p1);
	if (distance < _r0 + _r1) return true;

	return false;
}

bool CheckWarpZoneHit(const Vector3 _playerPos, const Vector3 _warpCenterPos, const float _warpRange)
{
	// �v���C���[���烏�[�v�]�[���ւ̋������v�Z����
	Vector3 VecPlayerToWarp = _warpCenterPos - _playerPos;
	float		   distance = VecPlayerToWarp.GetLength();

	if (distance < _warpRange)
	{
		return true;
	}

	return false;
}

// �v���C���[�̍U���̓����蔻��̊֐�
bool CheckAttackHit(Player& _attacker, Boss& _diffender, bool _isDebug)
{
	// ���̍��W���擾
	auto attachFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:Sword_joint");
	auto swordMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachFrame);
	float3 SwordPos = mul(float4(0, 0, 0, 1), cast(swordMat)).xyz;

	// ��Ō��̕��̈ʒu����ꂽ�̂Ō���̈ʒu��ݒ肵�Ă�
	Vector3 dir;
	dir.Set(-12.0f, 6.0f, -100.0f);

	// ����̍��W���擾
	VECTOR newDir = VTransformSR(dir.GetVECTOR(), swordMat);

	// �����蔻��̃f�o�b�O�`��
	if (_isDebug)
	{
		// ���̃J�v�Z��
		DrawCapsule3D(cast(SwordPos), cast(SwordPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetSwordSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		// �G�̃J�v�Z��
		DrawCapsule3D(VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			_diffender.GetCollSize(), 30, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	return HitCheck_Capsule_Capsule
	(
		cast(SwordPos), cast(SwordPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetSwordSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	);
}

// �{�X�̎�U���̓����蔻��̊֐�
bool CheckBossFistAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug)
{
	// �\�ߐݒ肳��Ă��鍪�{�Ɛ�[�̍��W���擾����
	Vector3 topPos   = _attacker.GetFistTopPos();
	Vector3 underPos = _attacker.GetFistUnderPos();

	// ���̃}�g���b�N�X���擾
	int    attachFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:RightHand");
	MATRIX rightArmMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachFrame);

	// ���{���W�ƃ}�g���b�N�X����Z���A�ŏI�I�ȍ��{���W���擾
	float3 fistPos = mul(float4(underPos.x,underPos.y,underPos.z, 1), cast(rightArmMat)).xyz;

	// �}�g���b�N�X���g�p�������W�ϊ����s���A�ŏI�I�Ȑ�[���W���擾����
	VECTOR newDir = VTransformSR(topPos.GetVECTOR(), rightArmMat);

	// �����蔻��̃f�o�b�O�`��
	if (_isDebug)
	{
		DrawCapsule3D(cast(fistPos), cast(fistPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetFistSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		DrawCapsule3D(VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			_diffender.GetCollSize(), 30, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	// ��U����ԂŖ�����Έȉ��̏������s��Ȃ�
	if (!_attacker.IsLightAttack()) return false;

	// �p�ӂ����l��DX���C�u�����̊֐��ɓn���ē����蔻����`�F�b�N
	return HitCheck_Capsule_Capsule
	(
		cast(fistPos), cast(fistPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetFistSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	);
}

bool CheckBossClawAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug)
{
	// �\�ߐݒ肳��Ă��鍪�{�Ɛ�[�̍��W���擾����
	Vector3 topPos   = _attacker.GetClawTopPos();
	Vector3 underPos = _attacker.GetClawUnderPos();

	// �܂̃}�g���b�N�X���擾
	int    attachFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:LeftHand");
	MATRIX leftArmMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachFrame);

	// ���{���W�ƃ}�g���b�N�X����Z���A�ŏI�I�ȍ��{���W���擾
	float3 clawPos = mul(float4(underPos.x,underPos.y,underPos.z, 1), cast(leftArmMat)).xyz;

	// �}�g���b�N�X���g�p�������W�ϊ����s���A�ŏI�I�Ȑ�[���W���擾����
	VECTOR newDir = VTransformSR(topPos.GetVECTOR(), leftArmMat);

	if (_isDebug)
	{
		// �����蔻��̃f�o�b�O�`��
		DrawCapsule3D(cast(clawPos), cast(clawPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetClawSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		DrawCapsule3D(VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
		_diffender.GetCollSize(), 30, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	if (!_attacker.IsHeavyAttack() && !_attacker.IsDown()) return false;

	// �p�ӂ����l��DX���C�u�����̊֐��ɓn���ē����蔻����`�F�b�N
	return HitCheck_Capsule_Capsule
	(
		cast(clawPos), cast(clawPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetClawSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	);
}

// �{�X�̃R���{�U���̓����蔻��
bool CheckBossComboAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug)
{

	if (!_attacker.IsComboAttack()) return false;

	// ���̍��W���擾
	int    attachFistFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:RightHand");
	MATRIX rightArmMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachFistFrame);;
	float3 fistPos = mul(float4(0, 0, 0, 1), cast(rightArmMat)).xyz;

	// �܂̍��W���擾
	int    attachClawFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:LeftHand");
	MATRIX leftArmMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachClawFrame);;
	float3 clawPos = mul(float4(0, 0, 0, 1), cast(leftArmMat)).xyz;

	// ���̓����蔻��̋��̂�p�ӂ���
	Vector3 fistDir;
	fistDir.Set(0.0f, 15.0f, 0.0f);
	VECTOR newFistDir = VTransformSR(fistDir.GetVECTOR(), rightArmMat);

	// �܂̓����蔻��̋��̂�p�ӂ���
	Vector3 clawDir;
	clawDir.Set(-40.0f, 50.0f, 0.0f);
	VECTOR newDir = VTransformSR(clawDir.GetVECTOR(), leftArmMat);

	// �J�v�Z���̃f�o�b�O�`��
	if (_isDebug)
	{
		// �ܕ����̃J�v�Z��
		DrawCapsule3D(cast(clawPos), cast(clawPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetClawSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		// �������̃J�v�Z��
		DrawCapsule3D(cast(fistPos), cast(fistPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetFistSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		// �v���C���[�̃J�v�Z��
		DrawCapsule3D(VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			_diffender.GetCollSize(), 30, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}


	// ���r�ɂ��U���Ȃ̂Ŕ��菈�����Q����
	if (HitCheck_Capsule_Capsule
	(
		// �ܕ����̓����蔻�菈��
		cast(clawPos), cast(clawPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetClawSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	) ||
		HitCheck_Capsule_Capsule
	(	
		// ���Ԃ������̓����蔻��
		cast(fistPos), cast(fistPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetFistSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	))
	{
		return true;
	}

	return false;
}

// �{�X�̃W�����v�U���̓����蔻��̊֐�
bool CheckBossJumpAttack(Boss& _attacker, CharacterBase& _diffender, float _impactSize)
{
	// �W�����v��ԂŖ�����Έȉ��̏������s��Ȃ�
	//if (_attacker.IsJump()) return false;

	// �{�X�ƃv���C���[�̋������v�Z����
	Vector3 vecBtoP = _diffender.GetPos() - _attacker.GetPos();
	float distanceBtoP = vecBtoP.GetLength();

	// ���̋������Ռ��͈͓̔��Ȃ瓖�����Ă���
	if (distanceBtoP < _impactSize) return true;

	return false;
}
