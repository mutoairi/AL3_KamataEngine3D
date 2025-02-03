#include "RailCamera.h"
#include"myMath.h"

void RailCamera::Initialize(KamataEngine::Vector3 position, KamataEngine::Vector3 rotation, KamataEngine::Camera* view)
{
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.Initialize();
	viewProjection_ = view;

}

void RailCamera::Update()
{
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.UpdateMatrix();


	viewProjection_->matView = Inverse(worldTransform_.matWorld_);
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f, -10.0f, 10.0f);
	ImGui::End();
}
