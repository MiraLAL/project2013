#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"

//ステージ
class Stage
{
public:
	Stage();
	~Stage();

	//更新処理
	void Update(float elapsedTime);
	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);//ID3D11DeviceContext描画に必要(GPU使用にも)

private:
	Model* model = nullptr;
};

