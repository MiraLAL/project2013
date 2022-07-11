#pragma once

#include "Graphics/Shader.h"
#include"Charactor.h"

//�G�l�~�[
class Enemy : public Charactor
{
public:
    Enemy(){}
    ~Enemy() override{}

    //�X�V����
    virtual void Update(float elapsedTime) = 0;

    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void DrawDebugPrimitive();

    //�j��
    void Destroy();
};
