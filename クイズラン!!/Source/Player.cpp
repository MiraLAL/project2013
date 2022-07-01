#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"

//コンストラクタ
Player::Player()
{
	model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;
}

//デストラクタ
Player::~Player() { delete model; }

//更新処理
void Player::Update(float elapsedTime)
{
	////進行ベクトルをベクトル取得
	//DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//
	////移動処理
	//float moveSpeed = this->moveSpeed * elapsedTime;
	//position.x += moveVec.x * moveSpeed;
	//position.z += moveVec.z * moveSpeed;

	InputMove(elapsedTime);

	////入力情報を取得
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisLX();//左スティックの入力情報、右が入力されると+1、左は-1
	//float ay = gamePad.GetAxisLY();
	//
	////移動操作
	//float moveSpeed = 5.0f * elapsedTime;
	//{
	//	//左スティックの入力情報をもとにXZ平面への移動処理
	//	position.x += ax * moveSpeed;
	//	position.z += ay * moveSpeed;
	//}
	//
	////回転操作
	//float rotateSpeed = DirectX::XMConvertToRadians(360) * elapsedTime;
	//if (gamePad.GetButton() & GamePad::BTN_A)
	//{
	//	//X軸回転操作
	//	angle.x += rotateSpeed;
	//}
	//if (gamePad.GetButton() & GamePad::BTN_B)
	//{
	//	//Y軸回転操作
	//	angle.y += rotateSpeed;
	//}
	//if (gamePad.GetButton() & GamePad::BTN_X)
	//{
	//	//Z軸回転操作
	//	angle.z += rotateSpeed;
	//}

	// ジャンプ入力処理
	InputJump();

	// 弾丸入力処理
	InputProjectile();

	// 速度(速力)処理更新
	UpdateVelocity(elapsedTime);

	// 弾丸更新処理
	projectileManager.Update(elapsedTime);

	// プレイヤーと敵との衝突処理
	CollisionPlayerVsEnemies();

	//オブジェクト行列を更新
	UpdateTransform();

	model->UpdateTransform(transform);
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
	// 弾丸描画処理
	projectileManager.Render(dc, shader);
}

void Player::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player",nullptr,ImGuiBackendFlags_None))
	{
		//トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Position", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			ImGui::InputFloat3("Scale",&scale.x);
		}
	}
	ImGui::End();
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();//左スティックの入力情報、右が入力されると+1、左は-1
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティックの入力値によって進行方向を計算
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはXZ平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX      = cameraRight.x;
	float cameraRightZ      = cameraRight.z;
	float cameraRightLength = sqrt((cameraRightX * cameraRightX) + (cameraRightZ * cameraRightZ));
	//単位ベクトル化
	if (cameraRightLength>0.0f)
	{
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX      = cameraFront.x;
	float cameraFrontZ      = cameraFront.z;
	float cameraFrontLength = sqrt((cameraFrontX * cameraFrontX) + (cameraFrontZ * cameraFrontZ));
	//単位ベクトル化
	if (cameraFrontLength > 0.0f)
	{
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	//スティックの水平入力値をカメラ右方向に反映、
	//スティックの垂直入力値をカメラ前方向に反映、
	//進行ベクトルを計算
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
	//Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

//移動入力処理
void Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	//移動処理
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);

	// 旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

// デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	
	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	// 弾丸デバッグプリミティブ描画
	projectileManager.DrawDebugPrimitive();
}

// プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	// 全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		// 衝突処理
		DirectX::XMFLOAT3 outPosition;
		if(Collision::IntersectCylinderVsCylinder(position,radius,height,
		                                          enemy->GetPosition(),
			                                      enemy->GetRadius(),
			                                      enemy->GetHeight(),
			                                      outPosition)) // 押し出し後の位置設定
			                                                    //enemy->SetPosition(outPosition);
		{
			// 敵の真上付近に当たったかを判定
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			// 上から踏んづけた場合は小ジャンプする
			if (normal.y > 0.8f)
			{
				// 小ジャンプする
				Jump(jumpSpeed * 0.5f);
			}
			else
			{
				// 押し出し後の位置設定
				enemy->SetPosition(outPosition);
			}
		}
																
		//if (Collision::IntersectSphereVsSphere(
		//	position, radius,
		//	enemy->GetPosition(),
		//	enemy->GetRadius(),
		//	outPosition)) enemy->SetPosition(outPosition);// 押し出し後の位置設定

	}
}

void Player::OnLanding() { jumpCount = 0; }

// ジャンプ入力処理
void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
	{
		//Jump(jumpSpeed);
		// ジャンプ回数制限
		if (jumpCount < jumpLimit)
		{
			// ジャンプ
			jumpCount++;
			Jump(jumpSpeed);
		}
	}
}

// 弾丸入力処理
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// 直進弾丸発射
	if (gamePad.GetButtonDown()&GamePad::BTN_X)
	{
		// 前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0.0f;
		dir.z = cosf(angle.y);
		// 発射位置（プレイヤーの腰あたり）
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.x = position.y + height * 0.5;
		pos.x = position.z;
		// 発射
		ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
		projectile->Launch(dir, pos);
		//projectileManager.Register(projectile);
	}
}