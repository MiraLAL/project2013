#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include"Audio/AudioSource.h"

//タイトルシーン
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    //初期化
    void Initialize() override;

    //終了化
    void Finalize() override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render() override;

    enum Type
    {
        ゲーム・アニメ, //0
        一般常識,//1
        漢字,//2
    };
    

private:
    //Sprite* sprite  = nullptr;
    //Sprite* sprite2 = nullptr;
    //Sprite* sprite3 = nullptr;
    Sprite* sprite[5];

    Sprite* font = nullptr;

    std::unique_ptr<AudioSource>ticall;  //タイトルコール

};

