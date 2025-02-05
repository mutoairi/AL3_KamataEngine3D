#include "GameScene.h"
#include <cassert>



GameScene::GameScene() {};
GameScene::~GameScene() {
	delete model_;
	delete modelSkyDome_;
	delete player_;
	delete debugCamera_;
	delete skyDome_;
};


void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//texture = TextureManager::Load("white1x1.png");
	model_ = Model::CreateFromOBJ("block", true);
	modelSkyDome_ = Model::CreateFromOBJ("sphere", true);
	viewProjection_.Initialize();
	worldTransform_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	player_->Initialize(model_, &viewProjection_);
	//敵の生成
	enemy_ = new Enemy();
	enemy_->Initialize(model_, &viewProjection_);
	/*敵キャラに自キャラのアドレスを渡す*/
	enemy_->SetPlayer(player_);
	//天球の生成
	skyDome_ = new Skydome();
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//レールカメラの生成
	railCamera = new RailCamera();
	railCamera->Initialize(railPos,railRot,&viewProjection_);


	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&viewProjection_);
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		if (isDebugCameraActive_ == true)
			isDebugCameraActive_ = false;
		else
			isDebugCameraActive_ = true;

	}

#endif
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetCamera().matView;
		viewProjection_.matProjection = debugCamera_->GetCamera().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		//ビュープロジェクション行列の更新と転送	
		viewProjection_.translation_ = railCamera->GetWorldPosition();
		viewProjection_.rotation_ = railCamera->GetWorldRotate();
		viewProjection_.UpdateMatrix();
		

		

	}
	CheckAllCollision();
	
	
	//========プレイヤー===============

	player_->Update();

	//========エネミー =================

	enemy_->Update();
	
	//========  天球　==================

	skyDome_->Update();

	//========レールカメラ=============

	railCamera->Update();
	
};

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// 
	skyDome_->Draw();
	player_->Draw();
	enemy_->Draw();
	/// </summary>

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	//=========================
	// 
	// 自キャラと敵弾
	// 
    // ========================
	
	//自キャラの座標
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();
		//posAとposBの距離
		float posC = (posB.x - posA.x) * (posB.x - posA.x) +
			(posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z);
	
		//半径の差
		float L = (player_->GetRadius() + bullet->GetRadius()) * (player_->GetRadius() + bullet->GetRadius());
		//球と球の交差判定
		if (posC<=L) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	//===========================
	//
	//自弾と敵キャラ
	//
	//============================
	
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		//posAとposBの距離
		float posD = (posB.x - posA.x) * (posB.x - posA.x) +
			(posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z);

		//半径の差
		float M = (enemy_->GetRadius() + bullet->GetRadius()) * (enemy_->GetRadius() + bullet->GetRadius());
		//球と球の交差判定
		if (posD <= M) {
			//自キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}


	//===========================
	//
	//自弾と敵弾
	//
	//============================

	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			posA = playerBullet->GetWorldPosition();
			posB = enemyBullet->GetWorldPosition();
			//posAとposBの距離
			float posC = (posB.x - posA.x) * (posB.x - posA.x) +
				(posB.y - posA.y) * (posB.y - posA.y) +
				(posB.z - posA.z) * (posB.z - posA.z);

			//半径の差
			float L = (playerBullet->GetRadius() + enemyBullet->GetRadius()) * (playerBullet->GetRadius() + enemyBullet->GetRadius());
			//球と球の交差判定
			if (posC <= L) {
				//自キャラの衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}

		}
	}
}
