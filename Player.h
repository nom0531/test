#pragma once

class Stage;
class Stage2;
class Game;
class Enemy;
class Enemy2;
class Num;
class Sum;

// �v���C���[
class Player:public IGameObject
{
public:
	Player();
	~Player();

	void Update();
	void Move();						// �ړ�
	void Rotation();					// ��]
	void Damege(int attackPower);						// �_���[�W
	void Attack();						// �U��
	void Position();					// �񎟌��z���̈ʒu
	void PlayAnimation();				// �A�j���[�V����
	void Render(RenderContext& rc);
	void Playermodel(bool second);					// ���f���̓ǂݍ���

	void SetPointer();

	// �����o�ϐ�
	ModelRender modelRender;
	Vector3 position;					// ���W
	Vector2 position2D;					// 2D���W
	Vector2 forward;					// �v���C���[�̐���
	Quaternion rotation;				// �N�H�[�^�j�I��

	// �A�j���[�V����
	enum player_AnimationClip {
		player_AnimationClip_Idle,		// �ҋ@
		player_AnimationClip_Walk,		// ����
		player_AnimationClip_Attack,	// �U��
		player_AnimationClip_Damege,	// ����
		//player_AnimationClip_Dead,	// ���S(������)
		player_AnimationClip_Clear,		// �X�e�[�W�N���A
		player_AnimationClip_Num,
	};
	AnimationClip p_animationClips[player_AnimationClip_Num];

	Stage* stage;
	Stage2* stage2;
	Game* game;
	Enemy* slime;
	Enemy2* mush;
	Num* num;
	Sum* sum;
	
	float Timer = 0.0f;
	bool moveFlag = false;				// �ړ��ł��邩�ǂ���
	bool posFlag = false;				// �ړ��ł���I�u�W�F�N�g���ǂ���
	bool stageClear = false;			// �X�e�[�W���N���A�������ǂ���
	bool AttackFlag = false;			// �U���������ǂ���
	/// <summary>
	/// �A�j���[�V����
	/// 0 :	�ҋ@
	/// 1 :	����
	/// 2 : �U��
	/// 3 : ��e
	/// 5 : ����
	/// </summary>
	int animState = 0;					
	int HP = 3;							// HP
	int player_Attack = 1;				// �U����
	int p_stageState = 0;				// player�̏������W
	int itemCount = 0;					// �l���A�C�e����

	int position_stage[5][5] = { 0 };	// ���݂̍��W
	int position_stage2[9][9] = { 0 };	// ���݂̍��W
	int MoveState = 0;					// 1:�� 2:�E 3:�� 4:��


private:
	/// <summary>
	/// �ړ��ł���Ƃ���true�� �ł��Ȃ��Ƃ���false��Ԃ�
	/// </summary>
	/// <param name="movePos">�ړ��\��̍��W</param>
	bool IsMove(Vector2 movePos);
};

