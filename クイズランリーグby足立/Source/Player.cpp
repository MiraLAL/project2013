#include<imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include"Graphics/Graphics.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileStraite.h"
#include"ProjectileHoming.h"
#include"Quiz_data.h"

#define CTTIME 170

int choice = 0;

//コンストラクタ
Player::Player()
{
    model = new Model("Data/Model/QuizPlayer/QuizPlayer.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;


    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/Hit.efk");

    qusti = 0;

    correct = Audio::Instance().LoadAudioSource("./Data/Audio/SE/正解 (mp3cut.net).wav");
    no_correct = Audio::Instance().LoadAudioSource("./Data/Audio/SE/不正解4 (mp3cut.net).wav");

    if (quizdata[0].qread)
    {
        Qus_Read = Audio::Instance().LoadAudioSource(quizdata[0].qread);
    }
}

//デストラクタ
Player::~Player()
{
    delete hitEffect;

    delete model;
}

//更新処理
void Player::Update(float elapsedTime)
{
    if (timer > GAMESTART)
    {
        //移動入力処理
        InputMove(elapsedTime);
    }
    //ジャンプ入力処理
    //InputJump();

    ////弾丸入力処理
    //InputProjectile();

    //速度更新処理
    UpdateVeclocity(elapsedTime);

    //弾丸更新処理
    projectileManager.Update(elapsedTime);

    //プレイヤーと敵との衝突処理
    //CollisionPlayerVsEnemies();

    //正誤判定
    AnswerJege();

    if (ct > CTTIME)
    {
        //クイズ問読み
        Quiz_Read();
    }
    //弾丸と敵との衝突処理
    CollsionProjectileVsEnemies();

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);

    
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    //弾丸描画処理
    projectileManager.Render(dc,shader);
}

//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(250, 150), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(750, 100), ImGuiCond_Once);

    if (ImGui::Begin(u8"問題", nullptr, ImGuiWindowFlags_NoMouseInputs))
    {
        if (ct > CTTIME && qusti < 10)
        {
            ImGui::Text(quizdata[qusti].question);
        }

    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(10, 500), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiCond_Once);

    if (ImGui::Begin(u8"選択肢A(赤)", nullptr, ImGuiWindowFlags_NoMouseInputs))
    {
        if (ct > CTTIME && qusti < 10)
        {
            ImGui::Text(quizdata[qusti].choiceWord_A);
        }

    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(450, 500), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiCond_Once);

    if (ImGui::Begin(u8"選択肢B(青)", nullptr, ImGuiWindowFlags_NoMouseInputs))
    {
        if (ct > CTTIME && qusti < 10)
        {

            ImGui::Text(quizdata[qusti].choiceWord_B);
        }

    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(920, 500), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiCond_Once);

    if (ImGui::Begin(u8"選択肢C(黄色)", nullptr, ImGuiWindowFlags_NoMouseInputs))
    {
        if (ct > CTTIME && qusti < 10)
        {

            ImGui::Text(quizdata[qusti].choiceWord_C);
        }

    }
    ImGui::End();

    ++ct;
}


//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    if(ax < 0.0f && gamePad.GetButtonDown())
    {
        choice -= 1;
    }
    else if (ax > 0.0f && gamePad.GetButtonDown())
    {
        choice += 1;
    }


    if (choice < -1) choice = -1;
    if (choice > 1) choice = 1;
    //カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Incetance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //移動ベクトルはXZ平面に水平なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {
        //単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;

    }

    //スティックの水平入力値をカメラ右方向に反映し，
    //スティックの垂直入力値をカメラ前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}





//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //弾丸デバッグプリミティブ
    projectileManager.DrawDebugPrimitive();
}

//プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //全ての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;

        //if (Collision::IntersectSphereVsSphere(
        //    position,
        //    radius,
        //    enemy->GetPosition(),
        //    enemy->GetRadius(),
        //    outPosition))
        //{
        //    enemy->SetPosition(outPosition);
        //}
            
        //衝突処理
        if (Collision::IntersectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            ////押し出し後の位置設定
            //enemy->SetPosition(outPosition);

            //敵の真上付近に当たったかを判定
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            //上から踏んづけた場合は小ジャンプする
            if (normal.y > 0.8f)
            {
                //小ジャンプする
                //Jump(jumpSpeed * 0.5f);
            }
            else
            {
                //押し出し後の位置設定
                enemy->SetPosition(outPosition);
            }
        }
       
    }
}

//正誤判定
void Player::AnswerJege()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //全ての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;

        //衝突処理
        if (Collision::IntersectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {

            if (choice == quizdata[qusti].answer)
            {
                correct->Play(false);
                Jump(jumpSpeed * 0.5f);

                
            }
            else
            {
                no_correct->Play(false);
            }

            enemy->Destroy();
            ct = 0;
                ++qusti;

                if (qusti < 10)
                {
                    Qus_Read = Audio::Instance().LoadAudioSource(quizdata[qusti].qread);
                }
                readFlag = true;

        }

    }

}




//移動入力処理
void Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move( moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    //Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}


//ジャンプ入力処理
void Player::InputJump()
{
    //ボタン入力でジャンプ（ジャンプ回数制限付き)
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount < jumpLimit)
        {
            //ジャンプ
            ++jumpCount;
            Jump(jumpSpeed);
        }
    }

}

//弾丸入力処理
void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //直線弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        //発射位置（プレイヤーの腰あたり）
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //発射
        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);
    }

    //追尾弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        //発射位置（プレイヤーの腰当たり）
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //ターゲット(デフォルトではプレイヤーの前方)
        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        //一番近くの敵をターゲットにする
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)
        {
            //敵との距離判定
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
            }


        }

        //発射
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);


    }
}

//着地したときに呼ばれる
void Player::OnLanding()
{
    jumpCount = 0;
}

//弾丸と敵とのの衝突処理
void Player::CollsionProjectileVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //全ての弾丸と全ての敵を総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCykunder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition
            ))
            {
                //ダメージを与える
                if (enemy->ApplyDamage(1, 0.5f))
                {
                    //吹き飛ばす
                    {
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();

                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        enemy->AddImpulse(impulse);
                    }

                    //ヒットエフェクト再生
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 0.5f;
                        hitEffect->Play(e);
                    }

                    //弾丸破棄
                    projectile->Destroy();
                }
            }
        }
    }
}

void Player::Quiz_Read()
{
    if (timer > GAMESTART && readFlag && qusti < 10)
    {
        Qus_Read->Play(false);
        readFlag = false;
    }
}
