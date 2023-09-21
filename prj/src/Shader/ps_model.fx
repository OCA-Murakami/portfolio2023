//----------------------------------------------------------------------------
//!	@file	ps_model.fx
//!	@brief	MV1モデルピクセルシェーダー
//----------------------------------------------------------------------------
#include "dxlib_ps.h"

//----------------------------------------------------------------------------
// メイン関数
//----------------------------------------------------------------------------
PS_OUTPUT main(PS_INPUT_3D input)
{
	PS_OUTPUT	output;

	float2	uv = input.uv0_;
	float3	N = normalize(input.normal_);	// 法線

	//------------------------------------------------------------
	// 法線マップ
	//------------------------------------------------------------
	N = Normalmap(N, input.worldPosition_, uv);

	//------------------------------------------------------------
	// シャドウ
	//------------------------------------------------------------
	// 影の濃さ
	float  shadow = 1.0;
	// ライトの方向
	float3 L = normalize(float3(1.0f, 1.0f, 1.0f));

	// ランバートの結果と合成
	float lambert = saturate(dot(N, L));


	shadow = min(shadow, lambert);

	static const float FADERAGE = 20.0;
	shadow = lerp(1.0, shadow, saturate((100) * (1.0 / FADERAGE)));

	shadow = saturate(shadow + 0.3);

	//------------------------------------------------------------
	// テクスチャカラーを読み込み
	//------------------------------------------------------------
	float4	textureColor = DiffuseTexture.Sample(DiffuseSampler, uv);

	output.color0_ = textureColor * input.diffuse_;

	output.color0_.rgb *= shadow;

	// 出力パラメータを返す
	return output;
}
