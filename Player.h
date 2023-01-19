#pragma once

class Stage;
class Stage2;
class Game;
class Enemy;
class Enemy2;
class Num;
class Sum;

// プレイヤー
class Player:public IGameObject
{
public:
	Player();
	~Player();

	void Update();
	void Move();						// 移動
	void Rotation();					// 回転
	void Damege(int attackPower);						// ダメージ
	void Attack();						// 攻撃
	void Position();					// 二次元配列上の位置
	void PlayAnimation();				// アニメーション
	void Render(RenderContext& rc);
	void Playermodel(bool second);					// モデルの読み込み

	void SetPointer();

	// メンバ変数
	ModelRender modelRender;
	Vector3 position;					// 座標
	Vector2 position2D;					// 2D座標
	Vector2 forward;					// プレイヤーの正面
	Quaternion rotation;				// クォータニオン

	// アニメーション
	enum player_AnimationClip {
		player_AnimationClip_Idle,		// 待機
		player_AnimationClip_Walk,		// 歩く
		player_AnimationClip_Attack,	// 攻撃
		player_AnimationClip_Damege,	// やられ
		//player_AnimationClip_Dead,	// 死亡(未実装)
		player_AnimationClip_Clear,		// ステージクリア
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
	bool moveFlag = false;				// 移動できるかどうか
	bool posFlag = false;				// 移動できるオブジェクトかどうか
	bool stageClear = false;			// ステージをクリアしたかどうか
	bool AttackFlag = false;			// 攻撃したかどうか
	/// <summary>
	/// アニメーション
	/// 0 :	待機
	/// 1 :	歩く
	/// 2 : 攻撃
	/// 3 : 被弾
	/// 5 : 勝利
	/// </summary>
	int animState = 0;					
	int HP = 3;							// HP
	int player_Attack = 1;				// 攻撃力
	int p_stageState = 0;				// playerの初期座標
	int itemCount = 0;					// 獲得アイテム数

	int position_stage[5][5] = { 0 };	// 現在の座標
	int position_stage2[9][9] = { 0 };	// 現在の座標
	int MoveState = 0;					// 1:上 2:右 3:左 4:下


private:
	/// <summary>
	/// 移動できるときはtrueを できないときはfalseを返す
	/// </summary>
	/// <param name="movePos">移動予定の座標</param>
	bool IsMove(Vector2 movePos);
};

