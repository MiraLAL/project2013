#pragma once

#include "Stage.h"
#include "Player.h"
#include "CameraController.h"

#include "Audio/Audio.h"

// ゲームシーン
class SceneGame
{
public:
	SceneGame() {}
	~SceneGame() {}

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	//便利なdeleteするやつ　田上案
	template<class T>
	inline void SafeDelete(T*& a_T);

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();

	//音楽処理
	void Sound();

private:
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;

	std::unique_ptr<AudioSource> bgm;
};

