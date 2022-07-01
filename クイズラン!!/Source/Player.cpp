#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"

//�R���X�g���N�^
Player::Player()
{
	model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;
}

//�f�X�g���N�^
Player::~Player() { delete model; }

//�X�V����
void Player::Update(float elapsedTime)
{
	////�i�s�x�N�g�����x�N�g���擾
	//DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//
	////�ړ�����
	//float moveSpeed = this->moveSpeed * elapsedTime;
	//position.x += moveVec.x * moveSpeed;
	//position.z += moveVec.z * moveSpeed;

	InputMove(elapsedTime);

	////���͏����擾
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisLX();//���X�e�B�b�N�̓��͏��A�E�����͂�����+1�A����-1
	//float ay = gamePad.GetAxisLY();
	//
	////�ړ�����
	//float moveSpeed = 5.0f * elapsedTime;
	//{
	//	//���X�e�B�b�N�̓��͏������Ƃ�XZ���ʂւ̈ړ�����
	//	position.x += ax * moveSpeed;
	//	position.z += ay * moveSpeed;
	//}
	//
	////��]����
	//float rotateSpeed = DirectX::XMConvertToRadians(360) * elapsedTime;
	//if (gamePad.GetButton() & GamePad::BTN_A)
	//{
	//	//X����]����
	//	angle.x += rotateSpeed;
	//}
	//if (gamePad.GetButton() & GamePad::BTN_B)
	//{
	//	//Y����]����
	//	angle.y += rotateSpeed;
	//}
	//if (gamePad.GetButton() & GamePad::BTN_X)
	//{
	//	//Z����]����
	//	angle.z += rotateSpeed;
	//}

	// �W�����v���͏���
	InputJump();

	// �e�ۓ��͏���
	InputProjectile();

	// ���x(����)�����X�V
	UpdateVelocity(elapsedTime);

	// �e�ۍX�V����
	projectileManager.Update(elapsedTime);

	// �v���C���[�ƓG�Ƃ̏Փˏ���
	CollisionPlayerVsEnemies();

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	model->UpdateTransform(transform);
}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
	// �e�ە`�揈��
	projectileManager.Render(dc, shader);
}

void Player::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player",nullptr,ImGuiBackendFlags_None))
	{
		//�g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�ʒu
			ImGui::InputFloat3("Position", &position.x);
			//��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//�X�P�[��
			ImGui::InputFloat3("Scale",&scale.x);
		}
	}
	ImGui::End();
}

//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();//���X�e�B�b�N�̓��͏��A�E�����͂�����+1�A����-1
	float ay = gamePad.GetAxisLY();

	//�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	//�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX      = cameraRight.x;
	float cameraRightZ      = cameraRight.z;
	float cameraRightLength = sqrt((cameraRightX * cameraRightX) + (cameraRightZ * cameraRightZ));
	//�P�ʃx�N�g����
	if (cameraRightLength>0.0f)
	{
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX      = cameraFront.x;
	float cameraFrontZ      = cameraFront.z;
	float cameraFrontLength = sqrt((cameraFrontX * cameraFrontX) + (cameraFrontZ * cameraFrontZ));
	//�P�ʃx�N�g����
	if (cameraFrontLength > 0.0f)
	{
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	//�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f�A
	//�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f�A
	//�i�s�x�N�g�����v�Z
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
	//Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;
}

//�ړ����͏���
void Player::InputMove(float elapsedTime)
{
	//�i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	//�ړ�����
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);

	// ���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

// �f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	
	// �Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	// �e�ۃf�o�b�O�v���~�e�B�u�`��
	projectileManager.DrawDebugPrimitive();
}

// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	// �S�Ă̓G�Ƒ�������ŏՓˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		// �Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if(Collision::IntersectCylinderVsCylinder(position,radius,height,
		                                          enemy->GetPosition(),
			                                      enemy->GetRadius(),
			                                      enemy->GetHeight(),
			                                      outPosition)) // �����o����̈ʒu�ݒ�
			                                                    //enemy->SetPosition(outPosition);
		{
			// �G�̐^��t�߂ɓ����������𔻒�
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			// �ォ�瓥��Â����ꍇ�͏��W�����v����
			if (normal.y > 0.8f)
			{
				// ���W�����v����
				Jump(jumpSpeed * 0.5f);
			}
			else
			{
				// �����o����̈ʒu�ݒ�
				enemy->SetPosition(outPosition);
			}
		}
																
		//if (Collision::IntersectSphereVsSphere(
		//	position, radius,
		//	enemy->GetPosition(),
		//	enemy->GetRadius(),
		//	outPosition)) enemy->SetPosition(outPosition);// �����o����̈ʒu�ݒ�

	}
}

void Player::OnLanding() { jumpCount = 0; }

// �W�����v���͏���
void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
	{
		//Jump(jumpSpeed);
		// �W�����v�񐔐���
		if (jumpCount < jumpLimit)
		{
			// �W�����v
			jumpCount++;
			Jump(jumpSpeed);
		}
	}
}

// �e�ۓ��͏���
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// ���i�e�۔���
	if (gamePad.GetButtonDown()&GamePad::BTN_X)
	{
		// �O����
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0.0f;
		dir.z = cosf(angle.y);
		// ���ˈʒu�i�v���C���[�̍�������j
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.x = position.y + height * 0.5;
		pos.x = position.z;
		// ����
		ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
		projectile->Launch(dir, pos);
		//projectileManager.Register(projectile);
	}
}