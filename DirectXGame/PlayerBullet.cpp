#include "PlayerBullet.h"
#include"base/TextureManager.h"

void PlayerBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	texture_ = KamataEngine::TextureManager::Load("uvChecker.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update()
{
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const KamataEngine::Camera& camera)
{
	model_->Draw(worldTransform_, camera, texture_);
}
