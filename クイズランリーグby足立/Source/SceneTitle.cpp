#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneGame.h"
#include"SceneLoading.h"
#include"SceneManager.h"
#include"Input/Input.h"

//初期化
void SceneTitle::Initialize()
{
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/1590939956.jpg");

    ticall = Audio::Instance().LoadAudioSource("./Data/Audio/SE/タイトルコール.wav");

    ticall->Play(false);

    font = new Sprite("Data/Font/font0.png");

}

//終了化
void SceneTitle::Finalize()
{
    //スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
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

    GamePad& gamepad = Input::Instance().GetGamePad();

    //何かを押したらゲームシーンへ切り替え
    const GamePadButton anyButton =
          GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y
        ;
    if (gamepad.GetButtonDown() & anyButton)
    {
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
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());

        ////タイトルスプライト描画
        //sprite->Render(dc,
        //    0, 0, screenWidth, screenHeight,
        //    0, 0, textureWidth, textureHeight,
        //    0,
        //    1, 1, 1, 1);


    }
    //タイトル画像に隠れて描画できない?
    font->textout(dc, "Common Sense", 0, 360, 60, 60, 1, 1, 1, 1);
    font->textout(dc, "Kanzi", 428, 360, 60, 60, 1, 1, 1, 1);
    font->textout(dc, "Otaku", 853, 360, 60, 60, 1, 1, 1, 1);

}