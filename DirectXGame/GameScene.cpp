#include "GameScene.h"
#include <cassert>
#include<fstream>




GameScene::GameScene() {};
GameScene::~GameScene() {
	delete model_;
	delete modelSkyDome_;
	delete player_;
	delete debugCamera_;
	delete skyDome_;
	for (EnemyBullet* bullet_ : bullets_) {
		delete bullet_;
	}
	for (Enemy* enemy_ : enemys_) {
		delete enemy_;
	}
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
	
	//天球の生成
	skyDome_ = new Skydome();
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//レールカメラの生成
	railCamera = new RailCamera();
	railCamera->Initialize(railPos,railRot,&viewProjection_);
	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera->GetWorldTransform());

	//CSV読み込み
	LoadEnemyPopData();

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
	//===uprodeEnemy==========

	UpdateEnemyCommands();

	//=====================
	CheckAllCollision();
	
	//========レールカメラ=============

	railCamera->Update();
	
	//========プレイヤー===============

	player_->Update();

	//========エネミー =================

	

	/*更新*/
	for (Enemy* enemy_ : enemys_) {
		enemy_->Update();
	}

	/*敵の弾*/
	//弾更新
	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Update();
	}

	/*解放*/
	enemys_.remove_if([](Enemy* enemy_) {
		if (enemy_->IsDead()) {
			delete enemy_;
			return true;
		}
		return false;
		});
	bullets_.remove_if([](EnemyBullet* bullet_) {
		if (bullet_->IsDead()) {
			delete bullet_;
			return true;
		}
		return false;
		});
	
	//========  天球　==================

	skyDome_->Update();

	
	
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
	//弾更新
	for (Enemy* enemy_ : enemys_) {
		enemy_->Draw();
	}

	//弾更新
	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection_);
	}
	
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

		const std::list<EnemyBullet*>& enemyBullets =bullets_ ;

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
			if (posC <= L) {
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


		for (Enemy* enemy : enemys_) {
			for (PlayerBullet* bullet : playerBullets) {


				//敵キャラの座標
				posA = enemy->GetWorldPosition();
				posB = bullet->GetWorldPosition();

				//posAとposBの距離
				float posD = (posB.x - posA.x) * (posB.x - posA.x) +
					(posB.y - posA.y) * (posB.y - posA.y) +
					(posB.z - posA.z) * (posB.z - posA.z);

				//半径の差
				float M = (enemy->GetRadius() + bullet->GetRadius()) * (enemy->GetRadius() + bullet->GetRadius());
				//球と球の交差判定
				if (posD <= M) {
					//自キャラの衝突時コールバックを呼び出す
					enemy->OnCollision();
					//敵弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
				}

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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet)
{
	bullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/EnemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyCommands()
{
	//待機処理
	if(waitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			//待機完了
			waitFlag = false;
		}
		return;
	}
	//1行分の文字列を入れる
	std::string line;
	//コマンド実行ループ
	while (std::getline(enemyPopCommands, line)) {
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		//POPコマンド
		if (word.find("POP") == 0) {
			//X
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//Y
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//Z
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			EnemyPop(Vector3(x, y, z));
		}
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			waitFlag = true;
			waitTimer = waitTime;
			//コマンドループを抜ける
			break;
		}


	}
}

void GameScene::EnemyPop(Vector3 position)
{
	Enemy* enemy = new Enemy();
	enemy->SetGameScene(this);
	/*敵キャラに自キャラのアドレスを渡す*/
	enemy->SetPlayer(player_);
	enemy->Initialize(model_, &viewProjection_,position);
	enemys_.push_back(enemy);
	
}
