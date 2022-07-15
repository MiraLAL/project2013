#pragma once

#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include"Audio/AudioSource.h"
#include"Graphics/Sprite.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

	DebugRenderer* debugrenderer = Graphics::Instance().GetDebugRenderer();

private:
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameracontroller = nullptr;
	Sprite* Gamesprite = nullptr;

	std::unique_ptr<AudioSource>bgm;

	std::unique_ptr<AudioSource>game;		//�Q�[��
	std::unique_ptr<AudioSource>start;		//�X�^�[�g


	int timer;					//�^�C�}�[

	bool startFlag = true;		//�X�^�[�g��؂������ǂ���

	Sprite* font = nullptr;


};
