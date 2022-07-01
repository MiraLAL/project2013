#pragma once

#include <DirectXMath.h>
using namespace DirectX;

// コリジョン
class Collision
{
public:
	/// <summary>
	/// 球と球の交差判定
	/// </summary>
	/// <param name="positionA">Aの位置</param>
	/// <param name="radiusA">Aの半径</param>
	/// <param name="positionB">Bの位置</param>
	/// <param name="radiusB">Bの半径</param>
	/// <param name="outPositionB">Bが押し出される位置</param>
	/// <returns>true ...衝突している</returns>
	static bool IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA,float radiusA,
										const DirectX::XMFLOAT3& positionB,float radiusB, 
										      DirectX::XMFLOAT3& outPositionB);
	// 円柱と円柱の交差判定
	static bool IntersectCylinderVsCylinder(const XMFLOAT3& positionA,float radiusA,float heightA,
											const XMFLOAT3& positionB,float radiusB,float heightB,
											      XMFLOAT3& outPositionB);
};
