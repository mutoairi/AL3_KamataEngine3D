#pragma once
#include<3d/WorldTransform.h>
#include<3d/ObjectColor.h>
#include<3d/Model.h>
#include<3d/Camera.h>
#include<input/Input.h>
#include<list>
#include "EnemyBullet.h"


//行動フェーズ
enum class Phase {
	Approach,//接近する
	Leave,//離脱する
};
class Enemy
{
public:
	~Enemy();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection);
	void Update();
	void Draw();
	void Approach();
	void Leave();
	void Fire();
	void ApproachInitialize();
	//発射間隔
	static const int kFireInterval = 60;

private:

	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Camera* viewProjection_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	//KamataEngine::ObjectColor objColor = {};
	uint32_t texture_ = 0u;
	//フェーズ
	Phase phase_;
	std::list<EnemyBullet* >bullets_ ;
	//発射タイマー
	int32_t shotTimer_ = 0;
};

