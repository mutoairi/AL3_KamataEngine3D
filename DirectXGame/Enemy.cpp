#include "Enemy.h"
#include"base/TextureManager.h"

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection)
{
	model_ = model;
	viewProjection_ = viewProjection;
	texture_= KamataEngine::TextureManager::Load("uvChecker.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = {10,0,100 };
}

void Enemy::Update()
{
	const float kSpeed = 0.1f;
	worldTransform_.translation_.z -= kSpeed;
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw()
{
	model_->Draw(worldTransform_, *viewProjection_, texture_);
}
