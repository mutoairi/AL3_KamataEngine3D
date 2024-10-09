#include "Player.h"

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection) {
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();

	objColor.Initialize();
	
}
void Player::Update(){

};

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_,&objColor); 
}