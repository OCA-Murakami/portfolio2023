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
	// プレイヤーからワープゾーンへの距離を計算する
	Vector3 VecPlayerToWarp = _warpCenterPos - _playerPos;
	float		   distance = VecPlayerToWarp.GetLength();

	if (distance < _warpRange)
	{
		return true;
	}

	return false;
}

// プレイヤーの攻撃の当たり判定の関数
bool CheckAttackHit(Player& _attacker, Boss& _diffender, bool _isDebug)
{
	// 剣の座標を取得
	auto attachFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:Sword_joint");
	auto swordMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachFrame);
	float3 SwordPos = mul(float4(0, 0, 0, 1), cast(swordMat)).xyz;

	// 上で剣の柄の位置が取れたので剣先の位置を設定してる
	Vector3 dir;
	dir.Set(-12.0f, 6.0f, -100.0f);

	// 剣先の座標を取得
	VECTOR newDir = VTransformSR(dir.GetVECTOR(), swordMat);

	// 当たり判定のデバッグ描画
	if (_isDebug)
	{
		// 剣のカプセル
		DrawCapsule3D(cast(SwordPos), cast(SwordPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetSwordSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		// 敵のカプセル
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

// ボスの弱攻撃の当たり判定の関数
bool CheckBossFistAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug)
{
	// 予め設定されている根本と先端の座標を取得する
	Vector3 topPos   = _attacker.GetFistTopPos();
	Vector3 underPos = _attacker.GetFistUnderPos();

	// 拳のマトリックスを取得
	int    attachFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:RightHand");
	MATRIX rightArmMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachFrame);

	// 根本座標とマトリックスを乗算し、最終的な根本座標を取得
	float3 fistPos = mul(float4(underPos.x,underPos.y,underPos.z, 1), cast(rightArmMat)).xyz;

	// マトリックスを使用した座標変換を行い、最終的な先端座標を取得する
	VECTOR newDir = VTransformSR(topPos.GetVECTOR(), rightArmMat);

	// 当たり判定のデバッグ描画
	if (_isDebug)
	{
		DrawCapsule3D(cast(fistPos), cast(fistPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetFistSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		DrawCapsule3D(VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			_diffender.GetCollSize(), 30, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	// 弱攻撃状態で無ければ以下の処理を行わない
	if (!_attacker.IsLightAttack()) return false;

	// 用意した値をDXライブラリの関数に渡して当たり判定をチェック
	return HitCheck_Capsule_Capsule
	(
		cast(fistPos), cast(fistPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetFistSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	);
}

bool CheckBossClawAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug)
{
	// 予め設定されている根本と先端の座標を取得する
	Vector3 topPos   = _attacker.GetClawTopPos();
	Vector3 underPos = _attacker.GetClawUnderPos();

	// 爪のマトリックスを取得
	int    attachFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:LeftHand");
	MATRIX leftArmMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachFrame);

	// 根本座標とマトリックスを乗算し、最終的な根本座標を取得
	float3 clawPos = mul(float4(underPos.x,underPos.y,underPos.z, 1), cast(leftArmMat)).xyz;

	// マトリックスを使用した座標変換を行い、最終的な先端座標を取得する
	VECTOR newDir = VTransformSR(topPos.GetVECTOR(), leftArmMat);

	if (_isDebug)
	{
		// 当たり判定のデバッグ描画
		DrawCapsule3D(cast(clawPos), cast(clawPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetClawSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		DrawCapsule3D(VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
		_diffender.GetCollSize(), 30, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	if (!_attacker.IsHeavyAttack() && !_attacker.IsDown()) return false;

	// 用意した値をDXライブラリの関数に渡して当たり判定をチェック
	return HitCheck_Capsule_Capsule
	(
		cast(clawPos), cast(clawPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetClawSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	);
}

// ボスのコンボ攻撃の当たり判定
bool CheckBossComboAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug)
{

	if (!_attacker.IsComboAttack()) return false;

	// 拳の座標を取得
	int    attachFistFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:RightHand");
	MATRIX rightArmMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachFistFrame);;
	float3 fistPos = mul(float4(0, 0, 0, 1), cast(rightArmMat)).xyz;

	// 爪の座標を取得
	int    attachClawFrame = MV1SearchFrame(_attacker.GetModel(), "mixamorig:LeftHand");
	MATRIX leftArmMat = MV1GetFrameLocalWorldMatrix(_attacker.GetModel(), attachClawFrame);;
	float3 clawPos = mul(float4(0, 0, 0, 1), cast(leftArmMat)).xyz;

	// 拳の当たり判定の球体を用意する
	Vector3 fistDir;
	fistDir.Set(0.0f, 15.0f, 0.0f);
	VECTOR newFistDir = VTransformSR(fistDir.GetVECTOR(), rightArmMat);

	// 爪の当たり判定の球体を用意する
	Vector3 clawDir;
	clawDir.Set(-40.0f, 50.0f, 0.0f);
	VECTOR newDir = VTransformSR(clawDir.GetVECTOR(), leftArmMat);

	// カプセルのデバッグ描画
	if (_isDebug)
	{
		// 爪部分のカプセル
		DrawCapsule3D(cast(clawPos), cast(clawPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetClawSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		// 拳部分のカプセル
		DrawCapsule3D(cast(fistPos), cast(fistPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetFistSize(), 30, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
		// プレイヤーのカプセル
		DrawCapsule3D(VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z),
			_diffender.GetCollSize(), 30, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}


	// 両腕による攻撃なので判定処理を２つ書く
	if (HitCheck_Capsule_Capsule
	(
		// 爪部分の当たり判定処理
		cast(clawPos), cast(clawPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetClawSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	) ||
		HitCheck_Capsule_Capsule
	(	
		// こぶし部分の当たり判定
		cast(fistPos), cast(fistPos + float3(newDir.x, newDir.y, newDir.z)), _attacker.GetFistSize(),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + (_diffender.GetCollSize() / 2), _diffender.GetPos().z + 10.0f),
		VGet(_diffender.GetPos().x, _diffender.GetPos().y + _diffender.GetCollSize() + (_diffender.GetCollSize() / 2), _diffender.GetPos().z), _diffender.GetCollSize()
	))
	{
		return true;
	}

	return false;
}

// ボスのジャンプ攻撃の当たり判定の関数
bool CheckBossJumpAttack(Boss& _attacker, CharacterBase& _diffender, float _impactSize)
{
	// ジャンプ状態で無ければ以下の処理を行わない
	//if (_attacker.IsJump()) return false;

	// ボスとプレイヤーの距離を計算する
	Vector3 vecBtoP = _diffender.GetPos() - _attacker.GetPos();
	float distanceBtoP = vecBtoP.GetLength();

	// この距離が衝撃の範囲内なら当たっている
	if (distanceBtoP < _impactSize) return true;

	return false;
}
