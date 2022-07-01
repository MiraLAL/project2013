#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

void CameraController::Update(float elapsedTime)
{
	//���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();//�E�X�e�B�b�N�̓��͏��A�������͂�����+1�A����-1
	float ay = gamePad.GetAxisRY();
	//�J�����̉�]���x
	float speed = rollSpeed * elapsedTime;

	//�E�X�e�B�b�N�̓��͏��(�l)�����Ƃ�X����Y������]
	{
		angle.y += ax * speed;
		angle.x += ay * speed;
	}

	//X���̃J������]�𐧌�
	if (angle.x < minAngleX) angle.x = minAngleX;
	if (angle.x > maxAngleX) angle.x = maxAngleX;

	//Y���̉�]�l��-3.14�`3.14�Ɏ��܂�
	if (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
	if (angle.y >  DirectX::XM_PI) angle.y -= DirectX::XM_2PI;

	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	//�J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLookAt(eye,target,DirectX::XMFLOAT3(0,1,0));
}
