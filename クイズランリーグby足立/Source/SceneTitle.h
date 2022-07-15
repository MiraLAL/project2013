#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include"Audio/AudioSource.h"

//�^�C�g���V�[��
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render() override;

    enum Type
    {
        �Q�[���E�A�j��, //0
        ��ʏ펯,//1
        ����,//2
    };
    

private:
    //Sprite* sprite  = nullptr;
    //Sprite* sprite2 = nullptr;
    //Sprite* sprite3 = nullptr;
    Sprite* sprite[5];

    Sprite* font = nullptr;

    std::unique_ptr<AudioSource>ticall;  //�^�C�g���R�[��

};

