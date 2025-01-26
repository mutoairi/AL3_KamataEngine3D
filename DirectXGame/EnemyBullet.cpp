#include "EnemyBullet.h"


void EnemyBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity)
{
	//NULLポインタチェック
	//assert(model);

	model_ = model;
	//テクスチャ読み込み
	texture_ = KamataEngine::TextureManager::Load("sample.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	//座標を移動させる
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const KamataEngine::Camera& camera)
{
	model_->Draw(worldTransform_, camera, texture_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

KamataEngine::Vector3 EnemyBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	//ワールド座標を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
