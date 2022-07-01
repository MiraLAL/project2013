#include "Collision.h"

//���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA,
										const DirectX::XMFLOAT3& positionB, float radiusB,
											  DirectX::XMFLOAT3& outPositionB)
{
	//B��A�̒P�ʃx�N�g�����Z�o
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB,PositionA);
	DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);//��悳�ꂽ�̂��Ԃ����
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// ��������
	float range = radiusA + radiusB;
	if (lengthSq > range * range) { return false; }

	// A��B�������o��
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);
	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	return true;
}

bool Collision::IntersectCylinderVsCylinder(const XMFLOAT3& positionA, float radiusA, float heightA,
	                                        const XMFLOAT3& positionB, float radiusB, float heightB, 
	                                              XMFLOAT3& outPositionB)
{
	// A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (positionA.y > positionB.y + heightB) return false;
	// A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (positionA.y + heightA < positionB.y) return false;

	// XZ���ʂł͈̔̓`�F�b�N
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float range = radiusA + radiusB;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range) return false;

	// A��B�������o��
	vx /= distXZ;
	vz /= distXZ;
	outPositionB.x = positionA.x + (vx * range);
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + (vz * range);

	return true;
}
