#pragma once

#include "Graphics/Shader.h"
#include"Charactor.h"

//エネミー
class Enemy : public Charactor
{
public:
    Enemy(){}
    ~Enemy() override{}

    //更新処理
    virtual void Update(float elapsedTime) = 0;

    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void DrawDebugPrimitive();

    //破棄
    void Destroy();
};
