#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Charactor.h"
#include"ProjectileManager.h"
#include"Effect.h"

//プレイヤー
class Player : public Charactor
{
public:
    Player();
    ~Player();

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //デバッグ用GUI描画
    void DrawDebugGUI();

     //デバッグプリミティブ描画
    void DrawDebugPrimitive();


    //移動入力処理
    void InputMove(float elapsedTime);

    //ジャンプ入力処理
    void InputJump();

    //弾丸と敵の衝突処理
    void CollsionProjectileVsEnemies();

protected:
    //着地したときに呼ばれる
    void OnLanding() override;

private:
    Model* model = nullptr;

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    float moveSpeed = 10.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    //ジャンプの変数
    float jumpSpeed = 20.0f;

    int jumpCount = 0;
    int jumpLimit = 2;

    //弾丸入力処理
    void InputProjectile();


    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;

    
};