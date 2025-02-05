#include "RailCamera.h"
#include"myMath.h"

void RailCamera::Initialize(KamataEngine::Vector3 position, KamataEngine::Vector3 rotation, KamataEngine::Camera* view)
{
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.Initialize();
	
	viewProjection_ = view;
	viewProjection_->farZ = 400.0f;


}

KamataEngine::Vector3 RailCamera::GetWorldPosition()
{

	//ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	//ワールド座標を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

KamataEngine::Vector3 RailCamera::GetWorldRotate()
{
	return worldTransform_.rotation_;
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
