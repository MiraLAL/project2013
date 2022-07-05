#include "EnemyManager.h"
#include"Graphics/Graphics.h"
#include"Collision.h"

//�X�V����
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    //enemies�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    //�X�V�������I�������ɂ͔j�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
    for (Enemy* enemy : removes)
    {
        //std::vector����v�f���폜����ꍇ�ɂ̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        //�폜
        delete enemy;

    }

    /////////
    // ��Ƃ��Ƃ�������
    // 
    // std::vector<Projectle*>::iterator it = projectiles.begin();
    // while(it != projectiles.end())
    // {
    //  if(�폜�̏���)
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
    //�j�����X�g���N���A
    removes.clear();

    //�G���m�̏Փˏ���
    CollisionEnemyVsEnemies();
}

//�`�揈��
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }

}

//�G�l�~�[�o�^
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

//�f�o�b�O�v���~�e�B�u�`��
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

//�G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
    //�j�����X�g�ɒǉ�
    removes.emplace_back(enemy);
}