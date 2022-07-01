#include "Projectile.h"
#include "ProjectileManager.h"

Projectile::Projectile(ProjectileManager* manager) :manager(manager) { 
	manager->Register(this); 
}

// デバッグプリミティブ描画
void Projectile::DrawDebugPrimitive()
{

}

void Projectile::Destroy() { manager->Remove(this); }

// 行列更新処理
void Projectile::UpdateTransform()
{
	// とりあえず、仮で回転は無視した行列を作成する。
	transform._11 = scale.x;
	transform._12 = 0.0f;
	transform._13 = 0.0f;
	transform._14 = 0.0f;
	transform._21 = 0.0f;
	transform._22 = scale.y;
	transform._23 = 0.0f;
	transform._24 = 0.0f;
	transform._31 = 0.0f;
	transform._32 = 0.0f;
	transform._33 = scale.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;
}
