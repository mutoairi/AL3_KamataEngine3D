#pragma once
#include<3d/WorldTransform.h>
#include<3d/ObjectColor.h>
#include<3d/Model.h>
#include<3d/Camera.h>
#include<input/Input.h>
#include"math/Vector3.h"
#include"myMath.h"

class PlayerBullet
{

public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);
	void Update();
	void Draw(const KamataEngine::Camera& camera);

private:
	KamataEngine::Camera* viewProjection_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	uint32_t texture_ = 0u;
};

