#include "Player.h"
#include"math/Vector3.h"
#include"myMath.h"
#include<imgui.h>


void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection) {
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	//シングルトンインスタンスを取得する
	input_ = KamataEngine::Input::GetInstance();

	objColor.Initialize();
	
}
void Player::Update() { 
	//キャラクターの移動ベクトル
	KamataEngine::Vector3 move = {0, 0, 0};

	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトル
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	//移動限界座標
	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 18.0f;

	

	worldTransform_.translation_ += move;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();


	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Window");
	ImGui::DragFloat3("Player", &worldTransform_.translation_.x, 0.1f);
	ImGui::End();


};

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_,&objColor); 
}