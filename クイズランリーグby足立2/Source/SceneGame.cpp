#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include"Camera.h"
#include"EnemyManager.h"
#include"EnemySlime.h"
#include"EffectManager.h"
#include<iostream>

extern int choice;

// 初期化
void SceneGame::Initialize()
{
	
	//ステージ初期化
	stage = new Stage();

	//プレイヤー初期化
	player = new Player();

	//カメラコントローラー初期化
	cameracontroller = new CameraController();

	//カメラ初期設定
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

	//エネミー初期化
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

	//スプライト初期化
	Gamesprite = new Sprite("Data/Sprite/sankaku02.jpg");

	font = new Sprite("Data/Font/font0.png");


	//音楽
	bgm = Audio::Instance().LoadAudioSource("./Data/Audio/BGM/BGM.wav");

	//bgm->Play(true);

	//SE
	game = Audio::Instance().LoadAudioSource("./Data/Audio/テスト/ゲーム.wav");

	game->Play(false);

	start = Audio::Instance().LoadAudioSource("./Data/Audio/テスト/スタート.wav");

}

// 終了化
void SceneGame::Finalize()
{
	//ステージ終了化
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}

	//プレイヤー終了化
	if (player != nullptr)
	{

		delete player;
		player = nullptr;
	}

	//カメラコントローラー終了化
	if (cameracontroller != nullptr)
	{
		delete cameracontroller;
		cameracontroller = nullptr;
	}

	//スプライト終了化
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

// 更新処理
void SceneGame::Update(float elapsedTime)
{

		//ステージ更新処理
		stage->Update(elapsedTime);

		//プレイヤー更新処理
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

		//エフェクト更新処理
		EffectManager::Instance().Update(elapsedTime);
	
		
		++timer;
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	float textureWidth = static_cast<float>(Gamesprite->GetTextureWidth());
	float textureHeight = static_cast<float>(Gamesprite->GetTextureHeight());

	//タイトルスプライト描画
	Gamesprite->Render(dc,
		0, 0, screenWidth, screenHeight,
		0, 0, textureWidth, textureHeight,
		0,
		1, 1, 1, 1);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::Incetance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	
	//// ビュー行列
	//{
	//	DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// カメラの視点（位置）
	//	DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// カメラの注視点（ターゲット）
	//	DirectX::XMFLOAT3 up = { 0, 1, 0 };		// カメラの上方向

	//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	//	DirectX::XMStoreFloat4x4(&rc.view, View);
	//}
	//// プロジェクション行列
	//{
	//	float fovY = DirectX::XMConvertToRadians(45);	// 視野角
	//	float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// 画面縦横比率
	//	float nearZ = 0.1f;	// カメラが映し出すの最近距離
	//	float farZ = 1000.0f;	// カメラが映し出すの最遠距離
	//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	//	DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	//}

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		stage->Render(dc, shader);
		//プレイヤー描画
		player->Render(dc, shader);

		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();

		EnemyManager::Instance().DrawDebugPrimitive();

		for (float i = 0; i < 100; ++i)
		{
			debugrenderer->DrawCylinder({ -13 ,0,i * 20 }, 3, 10, { 0.0f,1.0f,1.0f,1.0f });
			debugrenderer->DrawCylinder({  13 ,0,i * 20 }, 3, 10, { 0.0f,1.0f,1.0f,1.0f });

		}

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		font->textout(dc, "Type:Kanzi", 0, 0, 30, 30, 1, 1, 1, 1);

	}

	// 2DデバッグGUI描画
	{
		//プレイヤーデバッグ描画
		//player->DrawDebugGUI();
	}
}
