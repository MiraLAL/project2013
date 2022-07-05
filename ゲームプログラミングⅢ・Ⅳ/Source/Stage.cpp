#include "Stage.h"

static Stage* instance = nullptr;

//インスタンス取得
Stage& Stage::Instance()
{
    return *instance;
}

//コンストラクタ
Stage::Stage()
{
    instance = this;

    //ステージモデルを読み込み
    model = new Model("data/Model/ExampleStage/ExampleStage.mdl");
}

//デストラクタ
Stage::~Stage()
{
    //ステージモデルを破棄
    delete model;
}

void Stage::Update(float elapsedTime)
{
    //今は特にやることはない
}

void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool Stage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model,hit);
}

