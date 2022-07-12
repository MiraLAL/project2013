#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Charactor.h"
#include"ProjectileManager.h"
#include"Effect.h"
#include"Audio/Audio.h"
#include"Audio/AudioSource.h"

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

    void Quiz_Read();

protected:
    //着地したときに呼ばれる
    void OnLanding() override;

    int qusti;

private:
    Model* model = nullptr;

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    float moveSpeed = 20.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    //問題の正誤判定
    void AnswerJege();

    //ジャンプの変数
    float jumpSpeed = 20.0f;

    int jumpCount = 0;
    int jumpLimit = 2;

    //弾丸入力処理
    void InputProjectile();


    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;

    std::unique_ptr<AudioSource>correct;        //正解
    std::unique_ptr<AudioSource>no_correct;     //不正解

    std::unique_ptr<AudioSource>Qus_Read;     //問読み

    int ct = 0;

    bool readFlag = true; //問読みするか


};