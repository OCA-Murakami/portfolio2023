#pragma once
//------------------------------------------------
// �����蔻��Ɏg���֐�
//------------------------------------------------
#include "../System/Cast.h"
#include "../Game/Class/Boss.h"
#include "../Game/Class/Player.h"

// ---------------------------------------------------------------------------
// �I�u�W�F�N�g�̓����蔻��
// ---------------------------------------------------------------------------

//! �����m�̓����蔻��
//! @param &_p0 �P�ڂ̋��̍��W
//! @param  _r0 �P�ڂ̋��̔��a
//! @param &_p1 �Q�ڂ̋��̍��W
//! @param  _r1 �Q�ڂ̋��̔��a
bool CheckBallHit(const Vector3& _p0, const float _r0,
				  const Vector3& _p1, const float _r1);

//! @brief �g���[�j���O�X�e�[�W�̃��[�v�]�[���ƃv���C���[�̓����蔻��
//! @param _playerPos		�v���C���[�̍��W
//! @param _warpCenterPos	���[�v�]�[���̒��S���W
//! @return true : �]�[���ɐG��Ă���			false : �]�[���ɐG��Ă��Ȃ�
bool CheckWarpZoneHit(const Vector3 _playerPos, const Vector3 _warpCenterPos, const float _warpRange);

// ---------------------------------------------------------------------------
// �U���֘A�̓����蔻��
// ---------------------------------------------------------------------------

//! @brief �v���C���[�̍U������̊֐�
//! @param _attacker		�v���C���[�̃I�u�W�F�N�g
//! @param _diffenPos		�G�̍��W
//! @param _isDebug			�����蔻��̃f�o�b�O�`��
//! @return					true : �������Ă�	false : �������ĂȂ�
bool CheckAttackHit(Player& _attacker, Boss& _diffender, bool _isDebug);

//! @brief �{�X�̎�U������̊֐�
//! @param _attacker		�{�X�̃I�u�W�F�N�g
//! @param _diffender		�v���C���[�̃I�u�W�F�N�g
//! @param _isDebug			�f�o�b�O�`������邩�ǂ���
//! @return					true : �������Ă���		false : �������Ă��Ȃ�
bool CheckBossFistAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug);

//! @brief �{�X�̋��U���̓����蔻��֐�
//! @param _attacker	�{�X�̃I�u�W�F�N�g
//! @param _diffender	�v���C���[�̃I�u�W�F�N�g
//! @param _isDebug		�f�o�b�O�`��ɂ���
//! @return				true : �������Ă���			false : �������Ă��Ȃ�
bool CheckBossClawAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug);

//! @brief �{�X�̃R���{�U���̓����蔻��֐�
//! @param _attacker	�{�X�̃I�u�W�F�N�g
//! @param _diffender	�v���C���[�̃I�u�W�F�N�g
//! @param _isDebug		�f�o�b�O�`��ɂ���
//! @return				true : �������Ă���			false : �������Ă��Ȃ�
bool CheckBossComboAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug);

bool CheckBossJumpAttack(Boss& _attacker, CharacterBase& _diffender, float _impactSize);