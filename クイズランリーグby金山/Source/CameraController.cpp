#include"CameraController.h"
#include"Camera.h"
#include"Input/Input.h"

//�X�V����
void CameraController::Update(float elapsendTime)
{
    GamePad& gamepad = Input::Instance().GetGamePad();
    float ax = gamepad.GetAxisRX();
    float ay = gamepad.GetAxisRY();
    //�J�����̉�]���x
    float speed = rollSpeed * elapsendTime;


#if false
    //�X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.x += ay * speed;
    angle.y += ax * speed;

    //X���̃J������]�𐧌�
    if (angle.y > maxAngleX) angle.y = maxAngleX;
    if (angle.y < minAngleX)angle.y = minAngleX;

    //Y���̉�]�l���|3.14�`3.14�Ɏ��܂�悤�ɂ���
    if (angle.y < -DirectX::XM_PI)angle.y += DirectX::XM_2PI;
    if (angle.y > DirectX::XM_PI)angle.y -= DirectX::XM_2PI;
#endif

    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Tranceform = DirectX::XMMatrixRotationRollPitchYaw(angle.y, angle.x, angle.z);

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Tranceform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;

    //�{�ɂ���ƃJ������O����Ƃ邱�Ƃ��ł���
    eye.x = target.x - front.x * range;
    eye.y = cameraPositionY;
    eye.z = target.z - front.z * range;

        //�J�����̎��_�ƒ����_��ݒ�
        Camera::Incetance().SetLookAT(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}