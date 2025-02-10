#include "Enemy.h"
#include"base/TextureManager.h"
#include"myMath.h"
#include"Player.h"
#include"GameScene.h"


Enemy::~Enemy()
{
}

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection, KamataEngine::Vector3 position)
{
	model_ = model;
	viewProjection_ = viewProjection;
	texture_= KamataEngine::TextureManager::Load("uvChecker.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	phase_ = Phase::Approach;
	
	ApproachInitialize();
}

void Enemy::Update()
{
	
	if (!isDead) {
		switch (phase_)
		{

		case Phase::Approach:
			//デスフラグが立った弾を削除

			Approach();

			break;
		case Phase::Leave:

			Leave();
			break;
		default:

			break;
		}

		worldTransform_.UpdateMatrix();
	}
}

void Enemy::Draw()
{
	model_->Draw(worldTransform_, *viewProjection_, texture_);

}

void Enemy::Approach()
{
	//移動
	const float kSpeed = 0.5f;
	KamataEngine::Vector3 v = { 0.0f,0.0f,kSpeed };
	worldTransform_.translation_ -= v;
	
	//発射タイマーカウントダウン
	shotTimer_--;
	//指定時間に達した
	if (shotTimer_ < 0) {
		Fire();
		shotTimer_ = kFireInterval;
	}
	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::Leave()
{
	//移動
	const float kLSpeed = 0.5f;
	KamataEngine::Vector3 v1 = { 0.0f,0.0f,kLSpeed };
	worldTransform_.translation_ += v1;

	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z >50.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Fire()
{
	assert(player_);

		//弾の速度
		const float kBulletSpeed =0.7f;
		KamataEngine::Vector3 velocity(0, 0, kBulletSpeed);
		//自キャラの座標
		KamataEngine::Vector3 playerWorldPos = player_->GetWorldPosition();
		//敵キャラの座標
		KamataEngine::Vector3 enemyWorldPos = { worldTransform_.matWorld_.m[3][0] ,
			                                    worldTransform_.matWorld_.m[3][1] ,
		                                        worldTransform_.matWorld_.m[3][2] };

		KamataEngine::Vector3 differentialV = playerWorldPos - enemyWorldPos;
		
		
		velocity = Normalize(differentialV);
		velocity* kBulletSpeed;
		//弾を生成し初期
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		
			//弾を登録する
		gameScene_->AddEnemyBullet(newBullet);
		
}

void Enemy::ApproachInitialize()
{
	//発射タイマー初期化
	shotTimer_ = kFireInterval;
}

void Enemy::OnCollision()
{
	isDead = true;
}

KamataEngine::Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	//ワールド座標を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
