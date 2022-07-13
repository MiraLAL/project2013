#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Charactor.h"
#include"ProjectileManager.h"
#include"Effect.h"
#include"Audio/Audio.h"
#include"Audio/AudioSource.h"

//�v���C���[
class Player : public Charactor
{
public:
    Player();
    ~Player();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();

     //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();


    //�ړ����͏���
    void InputMove(float elapsedTime);

    //�W�����v���͏���
    void InputJump();

    //�e�ۂƓG�̏Փˏ���
    void CollsionProjectileVsEnemies();

    void Quiz_Read();

protected:
    //���n�����Ƃ��ɌĂ΂��
    void OnLanding() override;

    int qusti;

private:
    Model* model = nullptr;

    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;

    float moveSpeed = 20.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    //�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    //���̐��딻��
    void AnswerJege();

    //�W�����v�̕ϐ�
    float jumpSpeed = 20.0f;

    int jumpCount = 0;
    int jumpLimit = 2;

    //�e�ۓ��͏���
    void InputProjectile();


    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;

    std::unique_ptr<AudioSource>correct;        //����
    std::unique_ptr<AudioSource>no_correct;     //�s����

    std::unique_ptr<AudioSource>Qus_Read;     //��ǂ�

    int ct = 0;

    bool readFlag = true; //��ǂ݂��邩


};