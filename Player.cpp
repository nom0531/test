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

	p_animationClips[player_AnimationClip_Idle].Load("Assets/animData/TinyHero/Idle_Normal.tka");		// 待機
	p_animationClips[player_AnimationClip_Idle].SetLoopFlag(true);										// ループあり
	p_animationClips[player_AnimationClip_Walk].Load("Assets/animData/TinyHero/MoveBWD_Battle.tka");	// 歩く
	p_animationClips[player_AnimationClip_Walk].SetLoopFlag(true);										// ループあり
	p_animationClips[player_AnimationClip_Attack].Load("Assets/animData/TinyHero/Attack2.tka");			// 攻撃
	p_animationClips[player_AnimationClip_Attack].SetLoopFlag(false);									// ループなし
	p_animationClips[player_AnimationClip_Damege].Load("Assets/animData/TinyHero/Damage.tka");			// やられ
	p_animationClips[player_AnimationClip_Damege].SetLoopFlag(false);									// ループなし
	//p_animationClips[player_AnimationClip_Dead].Load("Assets/animData/TinyHero/Dead.tka");				// 死亡
	//p_animationClips[player_AnimationClip_Dead].SetLoopFlag(false);										// ループなし
	p_animationClips[player_AnimationClip_Clear].Load("Assets/animData/TinyHero/Victory.tka");			// ステージクリア
	p_animationClips[player_AnimationClip_Clear].SetLoopFlag(false);									// ループなし
}

void Player::Playermodel(bool second)
{
	if (second == false) {
		// レイトレースを切る
		modelRender.SetRaytracingWorld(false);

		// 読み込み
		modelRender.Init("Assets/modelData/model/character/Hero.tkm", p_animationClips, player_AnimationClip_Num, enModelUpAxisZ);
		modelRender.SetScale(0.8f, 0.8f, 0.8f);
	}

	// 位置座標
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

	// 向き
	rotation.SetRotationDegY(0.0f);
	modelRender.SetRotation(rotation);

	// 画像処理 現在使用予定なし
	//spriteRender.Init("Assets/sprite/mycharacter_1.dds", 100, 100);
}

void Player::SetPointer() {
	// インスタンスを探す
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
	Move();				// 移動
	Rotation();			// 回転
	PlayAnimation();	// アニメーション

	// 会話イベント外の時
	if (num->stopFlag == false) {
		// 攻撃していない　かつ　Aボタンを押したとき攻撃
		if (g_pad[0]->IsTrigger(enButtonA) && AttackFlag == false) {

			Attack();
		}
	}

	// 攻撃アニメーションが終了したとき
	if (modelRender.IsPlayingAnimation() == false && animState == 2) {
		num->turn = 2;	// 敵のターンにする
		animState = 0;	// 待機モーションに戻す
		AttackFlag = false;
	}

	// 被弾モーションが終了したとき
	if (modelRender.IsPlayingAnimation() == false && animState == 3) {
		animState = 0;	// 待機モーションに戻す
	}

	// クリアした時
	if (stageClear == true) {
		animState = 5;
	}

	modelRender.Update();
}

void Player::Move()
{
	// 被弾アニメーション中はこれより下は実行しない
	if (modelRender.IsPlayingAnimation() == true && animState == 3) {
		return;
	}
	// 勝利モーション時にはこれ以下の処理は実行しない
	if (animState == 5) {
		return;
	}

	if (num->turn == 1) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	//ヘルプを表示しているとき
	if (num->stopFlag == true) {
		moveFlag = false;
	}

	float moveSpeed = 50.0f;	//動くスピード

	if (moveFlag == true) {

		// マス目上に動く
		// 上
		if (g_pad[0]->IsTrigger(enButtonUp)) {

			Vector2 move = position2D;
			move.y -= 1.0f;			

			if (IsMove(move) == true) {
				position.z += moveSpeed;
				num->movePlayer = true;	// イベント用フラグ
				num->turn = 2;			// MOBのターン

				MoveState = 1;
				Position();			// 二次元配列上の座標

				// 効果音を再生
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(22);					// player_Foots
				se->Play(false);				// ループしない
				se->SetVolume(3.5f);			// 音量の設定をする
			}
		}
		// 下
		else if (g_pad[0]->IsTrigger(enButtonDown)) {

			Vector2 move = position2D;
			move.y += 1.0f;

			if (IsMove(move) == true) {
				position.z -= moveSpeed;

				// 効果音を再生
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(22);					// player_Foots
				se->Play(false);				// ループしない
				se->SetVolume(3.5f);			// 音量の設定をする

				num->movePlayer = true;	// イベント用フラグ
				num->turn = 2;			// MOBのターン

				MoveState = 4;
				Position();			// 二次元配列上の座標
			}
		}
		// 右
		else if (g_pad[0]->IsTrigger(enButtonRight)) {

			Vector2 move = position2D;
			move.x += 1.0f;

			if (IsMove(move) == true) {
				position.x += moveSpeed;

				// 効果音を再生
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(22);					// player_Foots
				se->Play(false);				// ループしない
				se->SetVolume(3.5f);			// 音量の設定をする

				num->movePlayer = true;	// イベント用フラグ
				num->turn = 2;			// MOBのターン

				MoveState = 2;
				Position();			// 二次元配列上の座標
			}
		}
		// 左
		else if (g_pad[0]->IsTrigger(enButtonLeft)) {

			Vector2 move = position2D;
			move.x -= 1.0f;

			if (IsMove(move) == true) {
				position.x -= moveSpeed;

				// 効果音を再生
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(22);					// player_Foots
				se->Play(false);				// ループしない
				se->SetVolume(3.5f);			// 音量の設定をする

				num->movePlayer = true;	// イベント用フラグ
				num->turn = 2;			// MOBのターン

				MoveState = 3;
				Position();			// 二次元配列上の座標
			}
		}
	}
	else {
		animState = 0;
	}

	// 絵描きさんに座標を教える。
	modelRender.SetPosition(position);
}

void Player::Rotation()
{
	if (moveFlag == true) {
		// マス目上に動く
		// 上
		if (g_pad[0]->IsTrigger(enButtonUp)) {
			rotation.SetRotationDegY(0.0f);
			forward.Set(0, -1);
		}
		// 下
		else if (g_pad[0]->IsTrigger(enButtonDown)) {
			rotation.SetRotationDegY(-180.0f);
			forward.Set(0, 1);
		}
		// 右
		else if (g_pad[0]->IsTrigger(enButtonRight)) {
			rotation.SetRotationDegY(90.0f);
			forward.Set(1, 0);
		}
		// 左
		else if (g_pad[0]->IsTrigger(enButtonLeft)) {
			rotation.SetRotationDegY(-90.0f);
			forward.Set(-1, 0);
		}
	}
	// 回転を教える
	modelRender.SetRotation(rotation);
}

void Player::Damege(int attackPower)
{
	animState = 3;

	// 効果音を再生
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(21);					// player_Attack
	se->Play(false);				// ループしない
	se->SetVolume(3.5f);			// 音量の設定をする

	// ダメージ計算
	HP -= attackPower;
}

void Player::Attack()
{
	animState = 2;

	// 攻撃場所を計算
	Vector2 attackPos = position2D;		//プレイヤーの前方を計算する
	attackPos.x += forward.x;
	attackPos.y += forward.y;

	// 効果音を再生
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(20);					// enemy_Attack
	se->Play(false);				// ループしない
	se->SetVolume(3.5f);			// 音量の設定をする

	// エネミー
	QueryGOs<Enemy>("slime", [&](Enemy* slime) {
		if (slime->s_position2D.x == attackPos.x && slime->s_position2D.y == attackPos.y) {
			// 被弾
			slime->s_Damege();
		}
		return true;
	});
	QueryGOs<Enemy2>("mush", [&](Enemy2* mush) {
		if (mush->m_position2D.x == attackPos.x && mush->m_position2D.y == attackPos.y) {
			// 被弾
			mush->m_Damege();
		}
		return true;
		});

	AttackFlag = true;
}

void Player::Position()
{
	// チュートリアル
	if (num->stageState == 0) {
		// 座標を更新
		switch (MoveState) {
		case 1:
			// 上
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
			// 右
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
			// 左
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
			// 下
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
			// 上記以外の時は何もしない
			break;
		}
	}
	// 本編
	else if (num->stageState != 0) {
		// 座標を更新
		switch (MoveState) {
		case 1:
			// 上
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
			// 右
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
			// 左
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
			// 下
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
			// 上記以外の時は何もしない
			break;
		}
	}
}

bool Player::IsMove(Vector2 movePos) {

	bool isMove = true;	// 移動できる

	// エネミー
	QueryGOs<Enemy>("slime", [&](Enemy* slime) {
		// 移動できる？
		Vector2 enemyNextPos = slime->s_position2D;
		// 移動先がエネミーと同じ
		if (enemyNextPos.x == movePos.x && enemyNextPos.y == movePos.y) {
			isMove = false;
		}

		enemyNextPos.x += slime->s_NextMove[slime->s_moveCount].x;
		enemyNextPos.y += slime->s_NextMove[slime->s_moveCount].y;
		// 移動先がエネミーの移動先と同じ
		if (enemyNextPos.x == movePos.x && enemyNextPos.y == movePos.y) {
			isMove = false;
		}

		return true;
	});

	QueryGOs<Enemy2>("mush", [&](Enemy2* mush) {
		// 移動できる？
		Vector2 enemyNextPos = mush->m_position2D;
		// 移動先がエネミーと同じ
		if (enemyNextPos.x == movePos.x && enemyNextPos.y == movePos.y) {
			isMove = false;
		}

		//enemyNextPos.x += mush->m_NextMove[mush->m_moveCount].x;
		//enemyNextPos.y += mush->m_NextMove[mush->m_moveCount].y;
		// 移動先がエネミーの移動先と同じ
		if (enemyNextPos.x == movePos.x && enemyNextPos.y == movePos.y) {
			isMove = false;
		}

		return true;
	});

	// 地形判定
	// 範囲外だったら false を返す
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
		// 地形
	if (stage->stageFlag[(int)movePos.y][(int)movePos.x] == 1) {
		isMove = false;
		}
		break;
	case 1:
	case 2:
		// 地形
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
		modelRender.PlayAnimation(player_AnimationClip_Idle,0.1f);		// 待機
		break;
	case 1:
		modelRender.PlayAnimation(player_AnimationClip_Walk, 0.1f);		// 歩く
		break;
	case 2:
		modelRender.PlayAnimation(player_AnimationClip_Attack, 0.1f);		// 攻撃
		break;
	case 3:
		modelRender.PlayAnimation(player_AnimationClip_Damege, 0.1f);		// ダメージ
		break;
	//case 4:
	//	modelRender.PlayAnimation(player_AnimationClip_Dead);		// 死亡
	//	break;
	case 5:
		modelRender.PlayAnimation(player_AnimationClip_Clear);		// ステージクリア
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	// 描画
	modelRender.Draw(rc);
}