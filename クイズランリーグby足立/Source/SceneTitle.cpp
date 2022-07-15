#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneGame.h"
#include"SceneLoading.h"
#include"SceneManager.h"
#include"Input/Input.h"

int type;

//初期化
void SceneTitle::Initialize()
{
    //スプライト初期化
    //sprite = new Sprite("Data/Sprite/1590939956.jpg");
    //sprite = new Sprite("Data/Sprite/unknown.png");
    //sprite2 = new Sprite("Data/Sprite/b868f597cae39891.jpg");
    //sprite3 = new Sprite("Data/Sprite/6c074b9957b63de6.jpg");
    for (int i = 0; i < 5; ++i) sprite[i] = nullptr;

    for (int i = 0; i < 5; ++i)
    {
        switch (i) 
        {
        case 0:
            sprite[i] = new Sprite("Data/Sprite/unknown.png");
            break;
        case 1:
            sprite[i] = new Sprite("Data/Sprite/b868f597cae39891.jpg");
            break;
        case 2:
            sprite[i] = new Sprite("Data/Sprite/d2f288d6e155fdd5.jpg");
            break;
        case 3:
            sprite[i] = new Sprite("Data/Sprite/25bed0fc8480988e.jpg");
            break;
        case 4:
            sprite[i] = new Sprite("Data/Sprite/8721e93809496ad5.jpg");
            break;
        }
    }

    ticall = Audio::Instance().LoadAudioSource("./Data/Audio/SE/タイトルコール.wav");

    ticall->Play(false);

    font = new Sprite("Data/Font/font0.png");
}

//終了化
void SceneTitle::Finalize()
{
    //スプライト終了化
    //if (sprite != nullptr)
    //{
    //    delete sprite;
    //    sprite = nullptr;
    //}
    //
    //if (sprite2 != nullptr)
    //{
    //    delete sprite2;
    //    sprite2 = nullptr;
    //}
    //
    //if (sprite3 != nullptr)
    //{
    //    delete sprite3;
    //    sprite3 = nullptr;
    //}
    for (int i = 0; i < 5; ++i)
    {
        if (sprite[i] != nullptr)
        {
            delete sprite[i];
            sprite[i] = nullptr;
        }
    }

    if (font != nullptr)
    {
        delete font;
        font = nullptr;
    }

}

//更新処理
void SceneTitle::Update(float elapsedTime)
{

    //GamePad& gamepad = Input::Instance().GetGamePad();
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();//左スティックの入力情報、右が入力されると+1、左は-1
    float ay = gamePad.GetAxisLY();

    //何かを押したらゲームシーンへ切り替え

    //if (ax > 0.0f|| ax < 0.0f|| ay < 0.0f)
    //{
    //    if (ax > 0.0f) type = ゲーム・アニメ;
    //    else if(ax < 0.0f) type = 一般常識;
    //    else type = 漢字;
    //    SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    //}

    if (ax > 0.0f)
    {
        type = ゲーム・アニメ;
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }
    if (ax < 0.0f)
    {
        type = 一般常識;
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }
    if (ay < 0.0f)
    {
        type = 漢字;
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
    }
}

//描画処理
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; //RGBA（0.0 ～1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        //float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureWidth[5];
        for (int i = 0; i < 5; ++i) textureWidth[i]  = static_cast<float>(sprite[i]->GetTextureWidth());
        float textureHeight[5];
        for (int i = 0; i < 5; ++i) textureHeight[i] = static_cast<float>(sprite[i]->GetTextureHeight());

        //タイトルスプライト描画
        //sprite->Render(dc,
        //    0, 0, screenWidth, screenHeight,
        //    0, 0, textureWidth, textureHeight,
        //    0,
        //    1, 1, 1, 1);
        //
        //sprite2->Render(dc,
        //    0, 30, screenWidth, screenHeight/2/2,
        //    0, 0, 1161, 153,
        //    0,
        //    1, 1, 1, 1);
        ////
        //sprite3->Render(dc,
        //    1, 360, screenWidth / 2 / 2, screenHeight / 2 / 2,
        //    0, 0, 320, 150,
        //    0,
        //    1, 1, 1, 1);
        ////
        //sprite3->Render(dc,
        //    042, 360, screenWidth / 2 / 2, screenHeight / 2 / 2,
        //    0, 0, 320, 150,
        //    0,
        //    1, 1, 1, 1);
        for (int i = 0; i < 5; ++i)
        {
            switch (i)
            {
            case 0:
                sprite[i]->Render(dc,
                    0, 0, screenWidth, screenHeight,
                    0, 0, textureWidth[i], textureHeight[i],
                    0,
                    1, 1, 1, 1);
                break;
            case 1:
                sprite[i]->Render(dc,
                    0, 30, screenWidth, screenHeight / 2 / 2,
                    0, 0, 1161, 153,
                    0,
                    1, 1, 1, 1);
                break;
            case 2:
                sprite[i]->Render(dc,
                    50, 410, screenWidth / 2 / 2, screenHeight / 2 / 2,
                    0, 0, 360, 150,
                    0,
                    1, 1, 1, 1);
                break;
            case 3:
                sprite[i]->Render(dc,
                    479, 410, screenWidth / 2 / 2, screenHeight / 2 / 2,
                    0, 0, 360, 150,
                    0,
                    1, 1, 1, 1);
                break;
            case 4:
                sprite[i]->Render(dc,
                    903, 410, screenWidth / 2 / 2, screenHeight / 2 / 2,
                    0, 0, 360, 150,
                    0,
                    1, 1, 1, 1);
                break;
            }
        }

        font->textout(dc, "Slect Quiz", 330, 300, 60, 60, 1, 1, 1, 1);
        font->textout(dc, "A", 180,  590, 60, 110, 1, 1, 1, 1);
        font->textout(dc, "S", 620,  590, 60, 110, 1, 1, 1, 1);
        font->textout(dc, "D", 1050, 590, 60, 110, 1, 1, 1, 1);
        font->textout(dc, "Presented by: Amateur Voice Actor Production  HIMAWARI", 95, 700, 20, 20, 1, 1, 1, 1);
    }
}