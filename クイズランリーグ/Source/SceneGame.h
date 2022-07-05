#pragma once

#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include"Audio/AudioSource.h"

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

private:
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameracontroller = nullptr;

	std::unique_ptr<AudioSource>bgm;
};
