#include "Enemy.h"
#include"base/TextureManager.h"
#include"myMath.h"

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection)
{
	model_ = model;
	viewProjection_ = viewProjection;
	texture_= KamataEngine::TextureManager::Load("uvChecker.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0,0,100 };
	phase_ = Phase::Approach;
}

void Enemy::Update()
{
	switch (phase_)
	{
	case Phase::Approach:
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
}
