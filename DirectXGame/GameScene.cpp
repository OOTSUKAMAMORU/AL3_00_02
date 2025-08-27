#include "GameScene.h"
#include "CameraController.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include "TitleScene.h"
#include <Windows.h>
using namespace KamataEngine;
// デストラクタ
GameScene::~GameScene() {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();
	// 3Dモデルデータの解放

	delete player_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete model_;
	delete debugCamera_;
	delete modelSkaydome_;
	delete skydome_;
	delete model_Enemy;
	delete model_DeathParticles;
	delete fade_;
}
void GameScene::CheckAllCollisions() {
	AABB aabb1, aabb2;
	aabb1 = player_->GetAABB();
	for (Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	}
}
void GameScene::Initialize() {
	// フェードインから開始
	phase_ = Phase::kFadeIn;
	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	// 要素数を変更する
	// 列数を設定
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockVirtical; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
	// 3Dモデルの生成
	modelSkaydome_ = Model::CreateFromOBJ("skydome", true);
	debugCamera_ = new DebugCamera(1280, 720);

	// 3Dモデルデータの生成
	model_Block = Model::CreateFromOBJ("Block");
	model_Skydome = Model::CreateFromOBJ("Skydome");
	model_Enemy = Model::CreateFromOBJ("Enemy");
	model_DeathParticles = Model::CreateFromOBJ("DeathParticle");

	// カメラ
	camera_.Initialize();
	worldTransform_.Initialize();
	camera_.Initialize();
	model_ = Model::CreateFromOBJ("player");
	textureHandle_ = TextureManager::Load("uvChecker.png");
	player_ = new Player();
	skydome_ = new Skydome();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(model_, &camera_, playerPosition);

	for (int32_t i = 0; i < 5; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10 + i, 18);
		newEnemy->Initialize(model_Enemy, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}
	skydome_->Initialize(modelSkaydome_, &camera_);
	GenerateBlocks();
	cameraController_ = new CameraController();
	cameraController_->Intialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);
	player_->SetMapChipField(mapChipField_);
	// 仮の生成処理。後で消す。

	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}
// 表示ブロックの生成
void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	// 要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
void GameScene::Update() {
	ChangePhase();
	switch (phase_) {
	case Phase::kPlay:
		CheckAllCollisions();
		break;
	case Phase::kDeath:
		deathParticles_->Update();
		break;
	case Phase::kFadeIn:
		fade_->Update();
		break;
	case Phase::kFadeOut:
		fade_->Update();
		break;
	}
	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
			debugCamera_->Update();
		}
	}
	player_->Update();
	skydome_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = camera_.matProjection = debugCamera_->GetCamera().matProjection;

		// ビュープロジェクション行列の更新と転送
		camera_.TransferMatrix();
	} else {
		cameraController_->Update();
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	}
#endif
	CheckAllCollisions();
	if (deathParticles_) {
		deathParticles_->Update();
	}
}
void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_Block->Draw(*worldTransformBlock, camera_);
		}
	}
	player_->Draw();

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (deathParticles_) {
		deathParticles_->Draw();
	}
	skydome_->Draw();
	model_->Draw(worldTransform_, camera_, textureHandle_);
	Model::PostDraw();
}
void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead() == true) {
			phase_ = Phase::kDeath;
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(model_DeathParticles, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		if (deathParticles_->IsFinished()) {
			finished_ = true;
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case Phase::kFadeIn:
		if (fade_->IsFinished()) 
		{
			phase_ = Phase::kPlay;
		}
		break;
	case Phase::kFadeOut:
		if (fade_->IsFinished())
	{
		 finished_=true;
	}
		break;
	}
}