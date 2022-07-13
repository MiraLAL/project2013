#include"Charactor.h"
#include"Stage.h"

extern int choice;

//�s��X�V����
void Charactor::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;

    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);

}

//�ړ�����
void Charactor::Move( float vx, float vz, float speed)
{
    //�ړ��x�N�g����ݒ�
    moveVecX = vx;
    moveVecZ = vz;

    //�ő呬�x�ݒ�
    maxMoveSpeed = speed;
}


//���񏈗�
void Charactor::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f) return;

    //�i�s�x�N�g����P�ʃx�N�g����
    vx /= length;
    vz /= length;

    //���g�̉�]����O���������߂�
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (frontX * vx) + (frontZ * vz);

    //���ϒl��-1.0����1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɂ����Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    float rot = 1.0f - dot;
    if (rot > speed) rot = speed;


    //���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (frontX * vz) - (frontZ * vx);

    //2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    //���E������s�����Ƃɂ���č��E��]��I������
    if (cross < 0.0f)
    {
        angle.y += rot;
    }
    else {
        angle.y -= rot;
    }
}

//�W�����v����
void Charactor::Jump(float speed)
{
    //������̗͂�ݒ�
    velocity.y = speed;
}

bool Charactor::ApplyDamage(int damage, float invincbleTime)
{
    //�_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0) return false;

    //���S���Ă���ꍇ�ɂ͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0) return false;

    //���G���Ԓ��̓_���[�W��^���Ȃ�
    if (invincibleTimer > 0.0f) return false;

    //���G���Ԑݒ�
    invincibleTimer = invincbleTime;

    //�_���[�W����
    health -= damage;

    //���S�ʒm
    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }

    //���N��Ԃ��ύX�����ꍇ��true ��Ԃ�
    return true;
}



void Charactor::UpdateVeclocity(float elapsedTime)
{
    //�o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    //�������͍X�V����
    UpdateVerticalVeclocity(elapsedFrame);

    //�������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    //�����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    if (Delay(GAMESTART,timer))
    {
        //�����ړ��X�V����
        UpdateHorizontalMove(elapsedTime);
    }

    ++timer;
}

//�������͍X�V����
void Charactor::UpdateVerticalVeclocity(float elapsedFrame)
{
    //�d�͏���
    velocity.y += gravity * elapsedFrame;

}


//�������͍X�V����
void Charactor::UpdateHorizontalVelocity(float elapsedFrame)
{
    //XZ���ʂ̑��͂���������
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    if (length > 0.0f)
    {
        //���C��
        float fraction = this->friction * elapsedFrame;

        //�󒆂ɂ���Ƃ��͖��C�͂����炷
        //if (!isGround) friction *= airControl;

        //���C�ɂ�鉡�����̌�������
        if (length > fraction)
        {
            //�P�ʃx�N�g����
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * fraction;
            velocity.z -= vz * fraction;
        }
        //�������̑��͂����C�ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        //�ړ��x�N�g�����[���łȂ��Ȃ��������
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        if (moveVecLength > 0.0f)
        {
            //������
            float acceleration = this->acceleration * elapsedFrame;

            //if (!isGround) acceleration *= airControl;

            //�ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //�ő呬�x����
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }



        }

    }
    else
    {
        velocity.x = moveVecX * maxMoveSpeed;
        velocity.z = moveVecZ * maxMoveSpeed;
    }
    //�ړ��x�N�g��
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}




//�����ړ��X�V����
void Charactor::UpdateHorizontalMove(float elapsedTime)
{
    //�ړ�����
        position.x = choice * choiceMove;
  
    //position.x += velocity.x * elapsedTime;
    position.z += maxMoveSpeed * elapsedTime;
 }



//�����ړ��X�V����
void Charactor::UpdateVerticalMove(float elapsedTime)
{
    //���������̈ړ���
    float my = velocity.y * elapsedTime;

    //������
    if (my < 0.0f)
    {
        //���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset,position.z };
        //���C�̏I�_�ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

        //���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            //�n�ʂɐݒu���Ă���
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            //�󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
        }
    }
    //�㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }


}


void Charactor::UpdateInvincleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}





//�Ռ���^����
void Charactor::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //���͂ɗ͂�������
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;

}

//�ҋ@����
bool Charactor::Delay(int time,int timer)
{
     int i = timer;

    if (time > i)
    {
        ++i;
        return false;
    }
    else
    {
        return true;
    }
}
