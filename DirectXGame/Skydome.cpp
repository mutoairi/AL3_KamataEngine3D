#include "Skydome.h"

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection) {

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// objカラーの初期化
	objColor.Initialize();

	// モデルの受け取り
	model_ = model;

	// カメラの受け取り
	viewProjection_ = viewProjection;
}

void Skydome::Update() {
	
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_, &objColor);
}
