#include "EnemyManager.h"
#include"Graphics/Graphics.h"
#include"Collision.h"

//更新処理
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    //enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
    //更新処理が終わった後には破棄リストに積まれたオブジェクトを削除する
    for (Enemy* enemy : removes)
    {
        //std::vectorから要素を削除する場合にはイテレーターで削除しなければならない
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        //削除
        delete enemy;

    }

    /////////
    // 企業だとこう書く
    // 
    // std::vector<Projectle*>::iterator it = projectiles.begin();
    // while(it != projectiles.end())
    // {
    //  if(削除の条件)
    // {
    //      delete(*it)
    //    it = projectiles.erase(it);
    // }
    //  else
    // {
    //      it++;
    // }
    // }
    // 
    // 
    //破棄リストをクリア
    removes.clear();

    //敵同士の衝突処理
    CollisionEnemyVsEnemies();
}

//描画処理
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }

}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}


void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();

}

//デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugPrimitive();
     }
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    size_t enemyCount = GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemyA  = GetEnemy(i);
        for (int j = i + 1; j < enemyCount; ++j)
        {
            Enemy* enemyB = GetEnemy(j);

            DirectX::XMFLOAT3 outposition;
            /*if (Collision::IntersectSphereVsSphere(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                outposition))
            {
                enemyB->SetPosition(outposition);
            }*/

            if (Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyA->GetHeight(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                enemyB->GetHeight(),
                outposition))
            {
                enemyB->SetPosition(outposition);
            }
        }
    }
}

//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
    //破棄リストに追加
    removes.emplace_back(enemy);
}