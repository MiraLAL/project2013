#pragma once

#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include"Audio/AudioSource.h"
#include"Graphics/Sprite.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;

	DebugRenderer* debugrenderer = Graphics::Instance().GetDebugRenderer();

private:
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameracontroller = nullptr;
	Sprite* Gamesprite = nullptr;

	std::unique_ptr<AudioSource>bgm;

	std::unique_ptr<AudioSource>game;		//ゲーム
	std::unique_ptr<AudioSource>start;		//スタート


	int timer;					//タイマー

	bool startFlag = true;		//スタートを切ったかどうか

	Sprite* font = nullptr;


};
