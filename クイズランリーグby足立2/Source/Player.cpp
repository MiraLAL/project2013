#include<imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include"Graphics/Graphics.h"
#include"Graphics/Sprite.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileStraite.h"
#include"ProjectileHoming.h"
#include"Quiz_data.h"

#define CTTIME 170

int choice = 0;

//�R���X�g���N�^
Player::Player()
{
    model = new Model("Data/Model/QuizPlayer/QuizPlayer.mdl");
    model->PlayAnimetion(0);

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;


    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/Hit.efk");

    qusti = 0;

    correct = Audio::Instance().LoadAudioSource("./Data/Audio/SE/���� (mp3cut.net).wav");
    no_correct = Audio::Instance().LoadAudioSource("./Data/Audio/SE/�s����4 (mp3cut.net).wav");

    if (quizdata[qusti].qread)
    {
        Qus_Read = Audio::Instance().LoadAudioSource(quizdata[qusti].qread);
    }

    endtime = Audio::Instance().LoadAudioSource("./Data/Audio/�e�X�g/�I��.wav");

    qusSprite = new Sprite("./Data/Sprite/Quiz/���/���.jpg");

    monSprite = new Sprite(quiz_data[0][qusti].questions);

    choice_A = new Sprite("./Data/Sprite/Quiz/A/Choice_A.jpg");


}

//�f�X�g���N�^
Player::~Player()
{
    delete hitEffect;

    delete model;
}

//�X�V����
void Player::Update(float elapsedTime)
{
    if (timer > GAMESTART)
    {
        //�ړ����͏���
        InputMove(elapsedTime);
    }
    //�W�����v���͏���
    //InputJump();

    ////�e�ۓ��͏���
    //InputProjectile();

    //���x�X�V����
    UpdateVeclocity(elapsedTime);

    //�e�ۍX�V����
    projectileManager.Update(elapsedTime);

    //�v���C���[�ƓG�Ƃ̏Փˏ���
    //CollisionPlayerVsEnemies();

    //���딻��
    AnswerJege();

    if (ct > CTTIME)
    {
        //�N�C�Y��ǂ�
        Quiz_Read();
    }

    if (qusti >= 10 && ct > CTTIME && endCall)
    {
        endtime->Play(false);
        endCall = false;
    }
    //�e�ۂƓG�Ƃ̏Փˏ���
    CollsionProjectileVsEnemies();

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    //model->UpdateAnimetion(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);

    ++ct;

}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dcn = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    shader->Draw(dc, model);

    //�e�ە`�揈��
    projectileManager.Render(dc,shader);

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(qusSprite->GetTextureWidth());
    float textureHeight = static_cast<float>(qusSprite->GetTextureHeight());
    
    //���
    qusSprite->Render(dcn,
        250, 150, 150, 100,
        0, 0, textureWidth -50, textureHeight,
        0,
        1, 0, 1, 1);

    //��蕶
    if (ct > CTTIME)
    {
        monSprite->Render(dcn,
            400, 150, 600, 100,
            0, 0, 600, 100,
            0,
            1, 0, 1, 1);
    }

    //�I����A�̕`��
    choice_A->Render(dcn,
        10, 500, 350, 100,
        0, 0, 350, 100,
        0,
        1, 0, 1, 1);

}

//�f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(250, 150), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(750, 100), ImGuiCond_Once);

    if (ImGui::Begin(u8"���", nullptr, ImGuiWindowFlags_NoMouseInputs))
    {

        //�Q�[���X�^�[�g
        if(timer < GAMESTART) ImGui::Text(u8"�Q�[���X�^�[�g(�ǂ����݂̐��_)");


        //�N�C�Y���̖��Ɛ���s�����̕\��
        if (ct < CTTIME && qusti < 10 && corrFlag /*&& timer > GAMESTART*/ && timer > CTTIME)
        {
            ImGui::Text(u8"����");

        }
        else if(ct < CTTIME&& qusti <= 10 && !corrFlag && timer > CTTIME)
        {
            switch (quizdata[qusti - 1].answer)
            {
            case A:
                ImGui::Text(u8"�s�����@����");
                ImGui::Text(quizdata[qusti - 1].choiceWord_A);
                break;
            case B:
                ImGui::Text(u8"�s�����@����");
                ImGui::Text(quizdata[qusti - 1].choiceWord_B);
                break;
            case C:
                ImGui::Text(u8"�s�����@����");
                ImGui::Text(quizdata[qusti - 1].choiceWord_C);
                break;

            }
        }
        else if (ct > CTTIME && qusti < 10)
        {
            ImGui::Text(quizdata[qusti].question);
        }

        //�Q�[���I��
        if (ct > CTTIME + CTTIME && qusti >= 10)
        {
            ImGui::Text(u8"�N�C�Y�I��!������������[");
            ImGui::Text(u8"���𐔁@10��  %d �␳��" ,numcorr );

        }

        if (ct > CTTIME * 3 && qusti >= 10)
        {
            ImGui::Text(u8"���Б��̃W�������̃N�C�Y�ɂ����킵�Ă݂Ă�������!");
            ImGui::Text(u8"�v���C���肪�Ƃ�����i�₵��!", numcorr);
            ImGui::Text(u8"presents �f�l���D�c�Ђ܂��");

            goTitle = true;
        }

    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(10, 500), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiCond_Once);


    if (ImGui::Begin(u8"�I����A(��)", nullptr, ImGuiWindowFlags_NoMouseInputs))
    {
        if (ct > CTTIME && qusti < 10)
        {
            ImGui::Text(quizdata[qusti].choiceWord_A);
        }

    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(450, 500), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiCond_Once);

    if (ImGui::Begin(u8"�I����B(��)", nullptr, ImGuiWindowFlags_NoMouseInputs))
    {
        if (ct > CTTIME && qusti < 10)
        {

            ImGui::Text(quizdata[qusti].choiceWord_B);
        }

    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(920, 500), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(350, 100), ImGuiCond_Once);

    if (ImGui::Begin(u8"�I����C(���F)", nullptr, ImGuiWindowFlags_NoMouseInputs))
    {
        if (ct > CTTIME && qusti < 10)
        {

            ImGui::Text(quizdata[qusti].choiceWord_C);
        }

    }
    ImGui::End();

}


//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //���͏����擾
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
    //�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Incetance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;

    }

    //�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���C
    //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}





//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�Փ˔���p�̃f�o�b�O�~����`��
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�e�ۃf�o�b�O�v���~�e�B�u
    projectileManager.DrawDebugPrimitive();
}

//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //�S�Ă̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //�Փˏ���
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
            
        //�Փˏ���
        if (Collision::IntersectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            ////�����o����̈ʒu�ݒ�
            //enemy->SetPosition(outPosition);

            //�G�̐^��t�߂ɓ����������𔻒�
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            //�ォ�瓥��Â����ꍇ�͏��W�����v����
            if (normal.y > 0.8f)
            {
                //���W�����v����
                //Jump(jumpSpeed * 0.5f);
            }
            else
            {
                //�����o����̈ʒu�ݒ�
                enemy->SetPosition(outPosition);
            }
        }
       
    }
}

//���딻��
void Player::AnswerJege()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //�S�Ă̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;

        //�Փˏ���
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
                corrFlag = true;
                correct->Play(false);
                ++numcorr;
                //Jump(jumpSpeed * 0.5f);
            }
            else
            {
                corrFlag = false;
                no_correct->Play(false);
            }

            enemy->Destroy();
            ct = 0;
                ++qusti;

                if (qusti < 10)
                {
                    delete monSprite;
                    monSprite = nullptr;

                    monSprite = new Sprite(quiz_data[0][qusti].questions);
                    Qus_Read = Audio::Instance().LoadAudioSource(quizdata[qusti].qread);
                }
                readFlag = true;

        }

    }

}




//�ړ����͏���
void Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //�ړ�����
    Move( moveVec.x, moveVec.z, moveSpeed);

    //���񏈗�
    //Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}


//�W�����v���͏���
void Player::InputJump()
{
    //�{�^�����͂ŃW�����v�i�W�����v�񐔐����t��)
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount < jumpLimit)
        {
            //�W�����v
            ++jumpCount;
            Jump(jumpSpeed);
        }
    }

}

//�e�ۓ��͏���
void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //�����e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        //���ˈʒu�i�v���C���[�̍�������j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //����
        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);
    }

    //�ǔ��e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        //���ˈʒu�i�v���C���[�̍�������j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //�^�[�Q�b�g(�f�t�H���g�ł̓v���C���[�̑O��)
        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        //��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)
        {
            //�G�Ƃ̋�������
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

        //����
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);


    }
}

//���n�����Ƃ��ɌĂ΂��
void Player::OnLanding()
{
    jumpCount = 0;
}

//�e�ۂƓG�Ƃ̂̏Փˏ���
void Player::CollsionProjectileVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //�S�Ă̒e�ۂƑS�Ă̓G�𑍓�����ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //�Փˏ���
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
                //�_���[�W��^����
                if (enemy->ApplyDamage(1, 0.5f))
                {
                    //������΂�
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

                    //�q�b�g�G�t�F�N�g�Đ�
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 0.5f;
                        hitEffect->Play(e);
                    }

                    //�e�۔j��
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
        //Qus_Read->Play(false);
        readFlag = false;
    }
}
