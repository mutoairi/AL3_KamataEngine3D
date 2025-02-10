#pragma once
#include <KamataEngine.h>
#include<list>
#include<sstream>
#include "Player.h"
#include"Enemy.h"
#include"Skydome.h"
#include"RailCamera.h"
#include"EnemyBullet.h"

using namespace KamataEngine;

class GameScene {

public:

	//ゲームシーン

	//コンストラクタ
	GameScene();

	//デストラクタ
	~GameScene();

	void Initialize();

	void Update();

	void Draw();

	//衝突判定と応答
	void CheckAllCollision();

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void LoadEnemyPopData();

	void UpdateEnemyCommands();

	void EnemyPop(Vector3);
private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//ゲームシーン用メンバ変数
	Player* player_ = nullptr;
	Model* model_ = nullptr;
	//Enemy* enemy_ = nullptr;
	Skydome* skyDome_ = nullptr;
	Model* modelSkyDome_ = nullptr;
	WorldTransform worldTransform_;
	Camera viewProjection_;
	RailCamera* railCamera = nullptr;
	// uint32_t texture = 0;
	Vector3 railPos = { 0.0f,0.0f,-50.0f };
	Vector3 railRot = { 0.0f,0.0f,0.0f };
	std::list<EnemyBullet* >bullets_;
	std::list<Enemy* >enemys_;

	bool waitFlag = false;
	int32_t waitTimer;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
};