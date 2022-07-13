#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneGame.h"
#include"SceneLoading.h"
#include"SceneManager.h"
#include"Input/Input.h"

//������
void SceneTitle::Initialize()
{
    //�X�v���C�g������
    //sprite = new Sprite("Data/Sprite/1590939956.jpg");
    sprite2 = new Sprite("Data/Sprite/b868f597cae39891.jpg");

    ticall = Audio::Instance().LoadAudioSource("./Data/Audio/SE/�^�C�g���R�[��.wav");

    ticall->Play(false);

    font = new Sprite("Data/Font/font0.png");
}

//�I����
void SceneTitle::Finalize()
{
    //�X�v���C�g�I����
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (sprite2 != nullptr)
    {
        delete sprite2;
        sprite2 = nullptr;
    }


    if (font != nullptr)
    {
        delete font;
        font = nullptr;
    }

}

//�X�V����
void SceneTitle::Update(float elapsedTime)
{

    GamePad& gamepad = Input::Instance().GetGamePad();

    //��������������Q�[���V�[���֐؂�ւ�
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

//�`�揈��
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; //RGBA�i0.0 �`1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2D�X�v���C�g�`��
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());

        //�^�C�g���X�v���C�g�`��
        //sprite->Render(dc,
        //    0, 0, screenWidth, screenHeight,
        //    0, 0, textureWidth, textureHeight,
        //    0,
        //    1, 1, 1, 1);

#if true
        sprite2->Render(dc,
            0, 30, screenWidth, screenHeight/2/2,
            0, 0, 1161, 153,
            0,
            1, 1, 1, 1);
#endif
        //�^�C�g���摜�ɉB��ĕ`��ł��Ȃ�?
        font->textout(dc, "Common Sense", 0, 360, 60, 60, 1, 1, 1, 1);
        font->textout(dc, "Kanzi", 428, 360, 60, 60, 1, 1, 1, 1);
        font->textout(dc, "Otaku", 853, 360, 60, 60, 1, 1, 1, 1);


    }

}