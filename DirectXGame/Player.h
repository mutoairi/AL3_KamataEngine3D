#pragma once
#include<3d/WorldTransform.h>
#include<3d/ObjectColor.h>
#include<3d/Model.h>
#include<3d/Camera.h>
#include<input/Input.h>
#include"PlayerBullet.h"

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection);
	void Update();
	void Draw();
	void Rotate();
	void Attack();
	void Move();

private:

	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Camera* viewProjection_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objColor = {};
	PlayerBullet* bullet_ = nullptr;
};
