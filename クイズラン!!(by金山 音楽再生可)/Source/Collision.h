#pragma once

#include <DirectXMath.h>
using namespace DirectX;

// �R���W����
class Collision
{
public:
	/// <summary>
	/// ���Ƌ��̌�������
	/// </summary>
	/// <param name="positionA">A�̈ʒu</param>
	/// <param name="radiusA">A�̔��a</param>
	/// <param name="positionB">B�̈ʒu</param>
	/// <param name="radiusB">B�̔��a</param>
	/// <param name="outPositionB">B�������o�����ʒu</param>
	/// <returns>true ...�Փ˂��Ă���</returns>
	static bool IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA,float radiusA,
										const DirectX::XMFLOAT3& positionB,float radiusB, 
										      DirectX::XMFLOAT3& outPositionB);
	// �~���Ɖ~���̌�������
	static bool IntersectCylinderVsCylinder(const XMFLOAT3& positionA,float radiusA,float heightA,
											const XMFLOAT3& positionB,float radiusB,float heightB,
											      XMFLOAT3& outPositionB);
};
