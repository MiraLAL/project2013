#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include"Camera.h"
#include"EnemyManager.h"
#include"EnemySlime.h"
#include"EffectManager.h"
#include<iostream>

extern int choice;

// ������
void SceneGame::Initialize()
{
	
	//�X�e�[�W������
	stage = new Stage();

	//�v���C���[������
	player = new Player();

	//�J�����R���g���[���[������
	cameracontroller = new CameraController();

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Incetance();
	camera.SetLookAT(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 1, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	//�G�l�~�[������
	 EnemyManager& enemyManager = EnemyManager::Instance();
	//EnemySlime* slime = new EnemySlime();
	//slime->SetPosition(DirectX::XMFLOAT3(0, 0, 5));
	//enemyManager.Register(slime);
	for (int i = 1; i <= 10; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3( 0.0f, 0, i *250));
		enemyManager.Register(slime);

	}
	//EnemySlime* slime = new EnemySlime();

	//slime->SetPosition(DirectX::XMFLOAT3(0, 0, 100));
	//enemyManager.Register(slime);

	//�X�v���C�g������
	Gamesprite = new Sprite("Data/Sprite/sankaku02.jpg");

	font = new Sprite("Data/Font/font0.png");


	//���y
	bgm = Audio::Instance().LoadAudioSource("./Data/Audio/BGM/BGM.wav");

	//bgm->Play(true);

	//SE
	game = Audio::Instance().LoadAudioSource("./Data/Audio/�e�X�g/�Q�[��.wav");

	game->Play(false);

	start = Audio::Instance().LoadAudioSource("./Data/Audio/�e�X�g/�X�^�[�g.wav");

}

// �I����
void SceneGame::Finalize()
{
	//�X�e�[�W�I����
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}

	//�v���C���[�I����
	if (player != nullptr)
	{

		delete player;
		player = nullptr;
	}

	//�J�����R���g���[���[�I����
	if (cameracontroller != nullptr)
	{
		delete cameracontroller;
		cameracontroller = nullptr;
	}

	//�X�v���C�g�I����
	if (Gamesprite != nullptr)
	{
		delete Gamesprite;
		Gamesprite = nullptr;
	}

	if (font != nullptr)
	{
		delete font;
		font = nullptr;
	}

	EnemyManager::Instance().Clear();

}

// �X�V����
void SceneGame::Update(float elapsedTime)
{

		//�X�e�[�W�X�V����
		stage->Update(elapsedTime);

		//�v���C���[�X�V����
		player->Update(elapsedTime);

		if (timer > GAMESTART - 20 && startFlag)
		{
			start->Play(false);
			startFlag = false;
		}

		

		DirectX::XMFLOAT3 target = player->GetPosition();
		target.y += 0.5f;
		cameracontroller->SetTarget(target);
		cameracontroller->Update(elapsedTime);

		EnemyManager::Instance().Update(elapsedTime);

		//�G�t�F�N�g�X�V����
		EffectManager::Instance().Update(elapsedTime);
	
		
		++timer;
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	float textureWidth = static_cast<float>(Gamesprite->GetTextureWidth());
	float textureHeight = static_cast<float>(Gamesprite->GetTextureHeight());

	//�^�C�g���X�v���C�g�`��
	Gamesprite->Render(dc,
		0, 0, screenWidth, screenHeight,
		0, 0, textureWidth, textureHeight,
		0,
		1, 1, 1, 1);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^�ݒ�
	Camera& camera = Camera::Incetance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	
	//// �r���[�s��
	//{
	//	DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// �J�����̎��_�i�ʒu�j
	//	DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// �J�����̒����_�i�^�[�Q�b�g�j
	//	DirectX::XMFLOAT3 up = { 0, 1, 0 };		// �J�����̏����

	//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	//	DirectX::XMStoreFloat4x4(&rc.view, View);
	//}
	//// �v���W�F�N�V�����s��
	//{
	//	float fovY = DirectX::XMConvertToRadians(45);	// ����p
	//	float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// ��ʏc���䗦
	//	float nearZ = 0.1f;	// �J�������f���o���̍ŋߋ���
	//	float farZ = 1000.0f;	// �J�������f���o���̍ŉ�����
	//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	//	DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	//}

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		stage->Render(dc, shader);
		//�v���C���[�`��
		player->Render(dc, shader);

		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();

		EnemyManager::Instance().DrawDebugPrimitive();

		for (float i = 0; i < 100; ++i)
		{
			debugrenderer->DrawCylinder({ -13 ,0,i * 20 }, 3, 10, { 0.0f,1.0f,1.0f,1.0f });
			debugrenderer->DrawCylinder({  13 ,0,i * 20 }, 3, 10, { 0.0f,1.0f,1.0f,1.0f });

		}

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		font->textout(dc, "Type:Kanzi", 0, 0, 30, 30, 1, 1, 1, 1);

	}

	// 2D�f�o�b�OGUI�`��
	{
		//�v���C���[�f�o�b�O�`��
		//player->DrawDebugGUI();
	}
}
