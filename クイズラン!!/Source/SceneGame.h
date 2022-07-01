#pragma once

#include "Stage.h"
#include "Player.h"
#include "CameraController.h"

#include "Audio/Audio.h"

// �Q�[���V�[��
class SceneGame
{
public:
	SceneGame() {}
	~SceneGame() {}

	// ������
	void Initialize();

	// �I����
	void Finalize();

	//�֗���delete�����@�c���
	template<class T>
	inline void SafeDelete(T*& a_T);

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render();

	//���y����
	void Sound();

private:
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;

	std::unique_ptr<AudioSource> bgm;
};

