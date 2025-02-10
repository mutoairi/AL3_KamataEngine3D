#pragma once
#include<3d/WorldTransform.h>
#include<3d/ObjectColor.h>
#include<3d/Model.h>
#include<3d/Camera.h>
#include<input/Input.h>


class RailCamera
{
public:
	void Initialize(KamataEngine::Vector3 position, KamataEngine::Vector3 rotation,KamataEngine::Camera*view);
	KamataEngine::Vector3 GetWorldPosition();
	KamataEngine::Vector3 GetWorldRotate();
	const KamataEngine::WorldTransform &GetWorldTransform() const{ return worldTransform_; }
	void Update();
private:
	KamataEngine::Camera* viewProjection_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	// 速度
	KamataEngine::Vector3 velocity_ = { 0.0f, 0.0f,0.001f };
};

