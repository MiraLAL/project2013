#include"Charactor.h"
#include"Stage.h"

extern int choice;

//行列更新処理
void Charactor::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;

    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);

}

//移動処理
void Charactor::Move( float vx, float vz, float speed)
{
    //移動ベクトルを設定
    moveVecX = vx;
    moveVecZ = vz;

    //最大速度設定
    maxMoveSpeed = speed;
}


//旋回処理
void Charactor::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //進行ベクトルがゼロベクトルの場合は処理する必要なし
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f) return;

    //進行ベクトルを単位ベクトル化
    vx /= length;
    vz /= length;

    //自身の回転から前方向を求める
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //回転角を求めるため、2つの単位ベクトルの内積を計算する
    float dot = (frontX * vx) + (frontZ * vz);

    //内積値は-1.0から1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど1.0にちかづくという性質を利用して回転速度を調整する
    float rot = 1.0f - dot;
    if (rot > speed) rot = speed;


    //左右判定を行うために2つの単位ベクトルの外積を計算する
    float cross = (frontX * vz) - (frontZ * vx);

    //2Dの外積値が正の場合か負の場合によって左右判定が行える
    //左右判定を行うことによって左右回転を選択する
    if (cross < 0.0f)
    {
        angle.y += rot;
    }
    else {
        angle.y -= rot;
    }
}

//ジャンプ処理
void Charactor::Jump(float speed)
{
    //上方向の力を設定
    velocity.y = speed;
}

bool Charactor::ApplyDamage(int damage, float invincbleTime)
{
    //ダメージが0の場合は健康状態を変更する必要がない
    if (damage == 0) return false;

    //死亡している場合には健康状態を変更しない
    if (health <= 0) return false;

    //無敵時間中はダメージを与えない
    if (invincibleTimer > 0.0f) return false;

    //無敵時間設定
    invincibleTimer = invincbleTime;

    //ダメージ処理
    health -= damage;

    //死亡通知
    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }

    //健康状態が変更した場合はtrue を返す
    return true;
}



void Charactor::UpdateVeclocity(float elapsedTime)
{
    //経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    //垂直速力更新処理
    UpdateVerticalVeclocity(elapsedFrame);

    //水平速力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    //垂直移動更新処理
    UpdateVerticalMove(elapsedTime);

    if (Delay(GAMESTART,timer))
    {
        //水平移動更新処理
        UpdateHorizontalMove(elapsedTime);
    }

    ++timer;
}

//垂直速力更新処理
void Charactor::UpdateVerticalVeclocity(float elapsedFrame)
{
    //重力処理
    velocity.y += gravity * elapsedFrame;

}


//水平速力更新処理
void Charactor::UpdateHorizontalVelocity(float elapsedFrame)
{
    //XZ平面の速力を減速する
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    if (length > 0.0f)
    {
        //摩擦力
        float fraction = this->friction * elapsedFrame;

        //空中にいるときは摩擦力を減らす
        //if (!isGround) friction *= airControl;

        //摩擦による横方向の減速処理
        if (length > fraction)
        {
            //単位ベクトル化
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * fraction;
            velocity.z -= vz * fraction;
        }
        //横方向の速力が摩擦以下になったので速力を無効化
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //XZ平面の速力を加速する
    if (length <= maxMoveSpeed)
    {
        //移動ベクトルがゼロでないなら加速する
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        if (moveVecLength > 0.0f)
        {
            //加速力
            float acceleration = this->acceleration * elapsedFrame;

            //if (!isGround) acceleration *= airControl;

            //移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //最大速度制限
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }



        }

    }
    else
    {
        velocity.x = moveVecX * maxMoveSpeed;
        velocity.z = moveVecZ * maxMoveSpeed;
    }
    //移動ベクトル
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}




//水平移動更新処理
void Charactor::UpdateHorizontalMove(float elapsedTime)
{
    //移動処理
        position.x = choice * choiceMove;
  
    //position.x += velocity.x * elapsedTime;
    position.z += maxMoveSpeed * elapsedTime;
 }



//垂直移動更新処理
void Charactor::UpdateVerticalMove(float elapsedTime)
{
    //垂直方向の移動量
    float my = velocity.y * elapsedTime;

    //落下中
    if (my < 0.0f)
    {
        //レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset,position.z };
        //レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

        //レイキャストによる地面判定
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            //地面に設置している
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            //空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    //上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }


}


void Charactor::UpdateInvincleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}





//衝撃を与える
void Charactor::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //速力に力を加える
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;

}

//待機時間
bool Charactor::Delay(int time,int timer)
{
     int i = timer;

    if (time > i)
    {
        ++i;
        return false;
    }
    else
    {   
        return true;
    }
}
