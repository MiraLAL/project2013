#include "Stage.h"

Stage::Stage()
{
	//ステージモデルを読み込み
	model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}

Stage::~Stage()
{
	//ステージモデルを破棄
	delete model;
}

//更新処理
void Stage::Update(float elapsedTime)
{
	
}

void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//シェーダーにモデルを描画してもらう
	shader->Draw(dc, model);
}