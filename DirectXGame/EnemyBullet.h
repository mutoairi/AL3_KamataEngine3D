#pragma once
#include<3d/WorldTransform.h>
#include<3d/ObjectColor.h>
#include<3d/Model.h>
#include<3d/Camera.h>
#include"math/Vector3.h"
#include"myMath.h"
class EnemyBullet
{


	

	public:
		void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);
		void Update();
		void Draw(const KamataEngine::Camera& camera);
		bool IsDead()const { return isDead_; }

	private:
		KamataEngine::Camera* viewProjection_ = nullptr;
		KamataEngine::Model* model_ = nullptr;
		KamataEngine::WorldTransform worldTransform_;
		KamataEngine::Vector3 velocity_;
		uint32_t texture_ = 0u;
		
		//寿命
		static const int32_t kLifeTime = 60 * 5;
		//デスタイマー
		int32_t deathTimer_ = kLifeTime;
		//デスフラグ
		bool isDead_ = false;



};

