#include "stdafx.h"
#include "Player.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "Game.h"
#include "Stage.h"
#include "Stage2.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Num.h"
#include "Sum.h"
#include "Chat.h"

Player::Player()
{
	

	SetPointer();

	p_animationClips[player_AnimationClip_Idle].Load("Assets/animData/TinyHero/Idle_Normal.tka");		// �ҋ@
	p_animationClips[player_AnimationClip_Idle].SetLoopFlag(true);										// ���[�v����
	p_animationClips[player_AnimationClip_Walk].Load("Assets/animData/TinyHero/MoveBWD_Battle.tka");	// ����
	p_animationClips[player_AnimationClip_Walk].SetLoopFlag(true);										// ���[�v����
	p_animationClips[player_AnimationClip_Attack].Load("Assets/animData/TinyHero/Attack2.tka");			// �U��
	p_animationClips[player_AnimationClip_Attack].SetLoopFlag(false);									// ���[�v�Ȃ�
	p_animationClips[player_AnimationClip_Damege].Load("Assets/animData/TinyHero/Damage.tka");			// ����
	p_animationClips[player_AnimationClip_Damege].SetLoopFlag(false);									// ���[�v�Ȃ�
	//p_animationClips[player_AnimationClip_Dead].Load("Assets/animData/TinyHero/Dead.tka");				// ���S
	//p_animationClips[player_AnimationClip_Dead].SetLoopFlag(false);										// ���[�v�Ȃ�
	p_animationClips[player_AnimationClip_Clear].Load("Assets/animData/TinyHero/Victory.tka");			// �X�e�[�W�N���A
	p_animationClips[player_AnimationClip_Clear].SetLoopFlag(false);									// ���[�v�Ȃ�
}

void Player::Playermodel(bool second)
{
	if (second == false) {
		// ���C�g���[�X��؂�
		modelRender.SetRaytracingWorld(false);

		// �ǂݍ���
		modelRender.Init("Assets/modelData/model/character/Hero.tkm", p_animationClips, player_AnimationClip_Num, enModelUpAxisZ);
		modelRender.SetScale(0.8f, 0.8f, 0.8f);
	}

	// �ʒu���W
	switch (p_stageState) {
	case 0:
		position = { 100.0f,11.0f,2.0f };
		position_stage[4][2] = 1;
		position2D = { 2.0f,4.0f };
		break;
	case 1:
	case 2:
		position = { 200.0f,11.0f,2.0f };
		position_stage2[8][4] = 1;
		position2D = { 4.0f,8.0f };
		break;
	}
	modelRender.SetPosition(position);

	// ����
	rotation.SetRotationDegY(0.0f);
	modelRender.SetRotation(rotation);

	// �摜���� ���ݎg�p�\��Ȃ�
	//spriteRender.Init("Assets/sprite/mycharacter_1.dds", 100, 100);
}

void Player::SetPointer() {
	// �C���X�^���X��T��
	stage = FindGO<Stage>("stage");
	stage2 = FindGO<Stage2>("stage2");

	slime = FindGO<Enemy>("slime");
	mush = FindGO<Enemy2>("mush");

	num = FindGO<Num>("num");
	sum = FindGO<Sum>("sum");
}

Player::~Player()
{
}

void Player::Update()
{
	Move();				// �ړ�
	Rotation();			// ��]
	PlayAnimation();	// �A�j���[�V����

	// ��b�C�x���g�O�̎�
	if (num->stopFlag == false) {
		// �U�����Ă��Ȃ��@���@A�{�^�����������Ƃ��U��
		if (g_pad[0]->IsTrigger(enButtonA) && AttackFlag == false) {

			Attack();
		}
	}

	// �U���A�j���[�V�������I�������Ƃ�
	if (modelRender.IsPlayingAnimation() == false && animState == 2) {
		num->turn = 2;	// �G�̃^�[���ɂ���
		animState = 0;	// �ҋ@���[�V�����ɖ߂�
		AttackFlag = false;
	}

	// ��e���[�V�������I�������Ƃ�
	if (modelRender.IsPlayingAnimation() == false && animState == 3) {
		animState = 0;	// �ҋ@���[�V�����ɖ߂�
	}

	// �N���A������
	if (stageClear == true) {
		animState = 5;
	}

	modelRender.Update();
}

void Player::Move()
{
	// ��e�A�j���[�V�������͂����艺�͎��s���Ȃ�
	if (modelRender.IsPlayingAnimation() == true && animState == 3) {
		return;
	}
	// �������[�V�������ɂ͂���ȉ��̏����͎��s���Ȃ�
	if (animState == 5) {
		return;
	}

	if (num->turn == 1) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	//�w���v��\�����Ă���Ƃ�
	if (num->stopFlag == true) {
		moveFlag = false;
	}

	float moveSpeed = 50.0f;	//�����X�s�[�h

	if (moveFlag == true) {

		// �}�X�ڏ�ɓ���
		// ��
		if (g_pad[0]->IsTrigger(enButtonUp)) {

			Vector2 move = position2D;
			move.y -= 1.0f;			

			if (IsMove(move) == true) {
				position.z += moveSpeed;
				num->movePlayer = true;	// �C�x���g�p�t���O
				num->turn = 2;			// MOB�̃^�[��

				MoveState = 1;
				Position();			// �񎟌��z���̍��W

				// ���ʉ����Đ�
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(22);					// player_Foots
				se->Play(false);				// ���[�v���Ȃ�
				se->SetVolume(3.5f);			// ���ʂ̐ݒ������
			}
		}
		// ��
		else if (g_pad[0]->IsTrigger(enButtonDown)) {

			Vector2 move = position2D;
			move.y += 1.0f;

			if (IsMove(move) == true) {
				position.z -= moveSpeed;

				// ���ʉ����Đ�
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(22);					// player_Foots
				se->Play(false);				// ���[�v���Ȃ�
				se->SetVolume(3.5f);			// ���ʂ̐ݒ������

				num->movePlayer = true;	// �C�x���g�p�t���O
				num->turn = 2;			// MOB�̃^�[��

				MoveState = 4;
				Position();			// �񎟌��z���̍��W
			}
		}
		// �E
		else if (g_pad[0]->IsTrigger(enButtonRight)) {

			Vector2 move = position2D;
			move.x += 1.0f;

			if (IsMove(move) == true) {
				position.x += moveSpeed;

				// ���ʉ����Đ�
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(22);					// player_Foots
				se->Play(false);				// ���[�v���Ȃ�
				se->SetVolume(3.5f);			// ���ʂ̐ݒ������

				num->movePlayer = true;	// �C�x���g�p�t���O
				num->turn = 2;			// MOB�̃^�[��

				MoveState = 2;
				Position();			// �񎟌��z���̍��W
			}
		}
		// ��
		else if (g_pad[0]->IsTrigger(enButtonLeft)) {

			Vector2 move = position2D;
			move.x -= 1.0f;

			if (IsMove(move) == true) {
				position.x -= moveSpeed;

				// ���ʉ����Đ�
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(22);					// player_Foots
				se->Play(false);				// ���[�v���Ȃ�
				se->SetVolume(3.5f);			// ���ʂ̐ݒ������

				num->movePlayer = true;	// �C�x���g�p�t���O
				num->turn = 2;			// MOB�̃^�[��

				MoveState = 3;
				Position();			// �񎟌��z���̍��W
			}
		}
	}
	else {
		animState = 0;
	}

	// �G�`������ɍ��W��������B
	modelRender.SetPosition(position);
}

void Player::Rotation()
{
	if (moveFlag == true) {
		// �}�X�ڏ�ɓ���
		// ��
		if (g_pad[0]->IsTrigger(enButtonUp)) {
			rotation.SetRotationDegY(0.0f);
			forward.Set(0, -1);
		}
		// ��
		else if (g_pad[0]->IsTrigger(enButtonDown)) {
			rotation.SetRotationDegY(-180.0f);
			forward.Set(0, 1);
		}
		// �E
		else if (g_pad[0]->IsTrigger(enButtonRight)) {
			rotation.SetRotationDegY(90.0f);
			forward.Set(1, 0);
		}
		// ��
		else if (g_pad[0]->IsTrigger(enButtonLeft)) {
			rotation.SetRotationDegY(-90.0f);
			forward.Set(-1, 0);
		}
	}
	// ��]��������
	modelRender.SetRotation(rotation);
}

void Player::Damege(int attackPower)
{
	animState = 3;

	// ���ʉ����Đ�
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(21);					// player_Attack
	se->Play(false);				// ���[�v���Ȃ�
	se->SetVolume(3.5f);			// ���ʂ̐ݒ������

	// �_���[�W�v�Z
	HP -= attackPower;
}

void Player::Attack()
{
	animState = 2;

	// �U���ꏊ���v�Z
	Vector2 attackPos = position2D;		//�v���C���[�̑O�����v�Z����
	attackPos.x += forward.x;
	attackPos.y += forward.y;

	// ���ʉ����Đ�
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(20);					// enemy_Attack
	se->Play(false);				// ���[�v���Ȃ�
	se->SetVolume(3.5f);			// ���ʂ̐ݒ������

	// �G�l�~�[
	QueryGOs<Enemy>("slime", [&](Enemy* slime) {
		if (slime->s_position2D.x == attackPos.x && slime->s_position2D.y == attackPos.y) {
			// ��e
			slime->s_Damege();
		}
		return true;
	});
	QueryGOs<Enemy2>("mush", [&](Enemy2* mush) {
		if (mush->m_position2D.x == attackPos.x && mush->m_position2D.y == attackPos.y) {
			// ��e
			mush->m_Damege();
		}
		return true;
		});

	AttackFlag = true;
}

void Player::Position()
{
	// �`���[�g���A��
	if (num->stageState == 0) {
		// ���W���X�V
		switch (MoveState) {
		case 1:
			// ��
			for (int i = 0; i < num->stageNum; i++) {
				for (int j = 0; j < num->stageNum; j++) {
					if (position_stage[i][j] == 1) {

						position_stage[i][j] = 0;
						position_stage[i - 1][j] = 1;

						position2D.y -= 1.0f;
						MoveState = 0;

						return;
					}
				}
			}
			break;
		case 2:
			// �E
			for (int i = 0; i < num->stageNum; i++) {
				for (int j = 0; j < num->stageNum; j++) {
					if (position_stage[i][j] == 1) {

						position_stage[i][j] = 0;
						position_stage[i][j + 1] = 1;

						position2D.x += 1.0f;
						MoveState = 0;

						return;
					}
				}
			}
			break;
		case 3:
			// ��
			for (int i = 0; i < num->stageNum; i++) {
				for (int j = 0; j < num->stageNum; j++) {
					if (position_stage[i][j] == 1) {

						position_stage[i][j] = 0;
						position_stage[i][j - 1] = 1;

						position2D.x -= 1.0f;
						MoveState = 0;

						return;
					}
				}
			}
			break;
		case 4:
			// ��
			for (int i = 0; i < num->stageNum; i++) {
				for (int j = 0; j < num->stageNum; j++) {
					if (position_stage[i][j] == 1) {

						position_stage[i][j] = 0;
						position_stage[i + 1][j] = 1;

						position2D.y += 1.0f;
						MoveState = 0;

						return;
					}
				}
			}
			break;
		default:
			// ��L�ȊO�̎��͉������Ȃ�
			break;
		}
	}
	// �{��
	else if (num->stageState != 0) {
		// ���W���X�V
		switch (MoveState) {
		case 1:
			// ��
			for (int i = 0; i < num->stageNum; i++) {
				for (int j = 0; j < num->stageNum; j++) {
					if (position_stage2[i][j] == 1) {

						position_stage2[i][j] = 0;
						position_stage2[i - 1][j] = 1;

						position2D.y -= 1.0f;
						MoveState = 0;

						return;
					}
				}
			}
			break;
		case 2:
			// �E
			for (int i = 0; i < num->stageNum; i++) {
				for (int j = 0; j < num->stageNum; j++) {
					if (position_stage2[i][j] == 1) {

						position_stage2[i][j] = 0;
						position_stage2[i][j + 1] = 1;

						position2D.x += 1.0f;
						MoveState = 0;

						return;
					}
				}
			}
			break;
		case 3:
			// ��
			for (int i = 0; i < num->stageNum; i++) {
				for (int j = 0; j < num->stageNum; j++) {
					if (position_stage2[i][j] == 1) {

						position_stage2[i][j] = 0;
						position_stage2[i][j - 1] = 1;

						position2D.x -= 1.0f;
						MoveState = 0;

						return;
					}
				}
			}
			break;
		case 4:
			// ��
			for (int i = 0; i < num->stageNum; i++) {
				for (int j = 0; j < num->stageNum; j++) {
					if (position_stage2[i][j] == 1) {

						position_stage2[i][j] = 0;
						position_stage2[i + 1][j] = 1;

						position2D.y += 1.0f;
						MoveState = 0;

						return;
					}
				}
			}
			break;
		default:
			// ��L�ȊO�̎��͉������Ȃ�
			break;
		}
	}
}

bool Player::IsMove(Vector2 movePos) {

	bool isMove = true;	// �ړ��ł���

	// �G�l�~�[
	QueryGOs<Enemy>("slime", [&](Enemy* slime) {
		// �ړ��ł���H
		Vector2 enemyNextPos = slime->s_position2D;
		// �ړ��悪�G�l�~�[�Ɠ���
		if (enemyNextPos.x == movePos.x && enemyNextPos.y == movePos.y) {
			isMove = false;
		}

		enemyNextPos.x += slime->s_NextMove[slime->s_moveCount].x;
		enemyNextPos.y += slime->s_NextMove[slime->s_moveCount].y;
		// �ړ��悪�G�l�~�[�̈ړ���Ɠ���
		if (enemyNextPos.x == movePos.x && enemyNextPos.y == movePos.y) {
			isMove = false;
		}

		return true;
	});

	QueryGOs<Enemy2>("mush", [&](Enemy2* mush) {
		// �ړ��ł���H
		Vector2 enemyNextPos = mush->m_position2D;
		// �ړ��悪�G�l�~�[�Ɠ���
		if (enemyNextPos.x == movePos.x && enemyNextPos.y == movePos.y) {
			isMove = false;
		}

		//enemyNextPos.x += mush->m_NextMove[mush->m_moveCount].x;
		//enemyNextPos.y += mush->m_NextMove[mush->m_moveCount].y;
		// �ړ��悪�G�l�~�[�̈ړ���Ɠ���
		if (enemyNextPos.x == movePos.x && enemyNextPos.y == movePos.y) {
			isMove = false;
		}

		return true;
	});

	// �n�`����
	// �͈͊O�������� false ��Ԃ�
	if (movePos.x < 0.0f) {
		isMove = false;
	}
	if (movePos.x > num->stageNum-1) {
		isMove = false;
	}
	if (movePos.y < 0.0f) {
		isMove = false;
	}
	if (movePos.y > num->stageNum-1) {
		isMove = false;
	}


	switch (num->stageState){
	case 0:
		// �n�`
	if (stage->stageFlag[(int)movePos.y][(int)movePos.x] == 1) {
		isMove = false;
		}
		break;
	case 1:
	case 2:
		// �n�`
		if (stage2->stageFlag[(int)movePos.y][(int)movePos.x] == 1) {
			isMove = false;
		}
		break;
	}
	
	return isMove;
}

void Player::PlayAnimation()
{
	switch (animState) {
	case 0:
		modelRender.PlayAnimation(player_AnimationClip_Idle,0.1f);		// �ҋ@
		break;
	case 1:
		modelRender.PlayAnimation(player_AnimationClip_Walk, 0.1f);		// ����
		break;
	case 2:
		modelRender.PlayAnimation(player_AnimationClip_Attack, 0.1f);		// �U��
		break;
	case 3:
		modelRender.PlayAnimation(player_AnimationClip_Damege, 0.1f);		// �_���[�W
		break;
	//case 4:
	//	modelRender.PlayAnimation(player_AnimationClip_Dead);		// ���S
	//	break;
	case 5:
		modelRender.PlayAnimation(player_AnimationClip_Clear);		// �X�e�[�W�N���A
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	// �`��
	modelRender.Draw(rc);
}