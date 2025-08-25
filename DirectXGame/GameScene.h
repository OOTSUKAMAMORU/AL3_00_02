#pragma once
#include "KamataEngine.h"
#include "player.h"
#include "Enemy.h"
#include<vector>
#include "Skydome.h"
#include"MapChipField.h"
#include "CameraController.h"
#include "DeathParticles.h"
// ゲームシーン
class GameScene
{
	void GenerateBlocks();
	MapChipField* mapChipField_; 
	//3Dモデル
	KamataEngine::Model* modelSkaydome_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

private:
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;
	std::list<Enemy*> enemies_;
	Skydome* skydome_ = nullptr;
	CameraController* cameraController_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	// 3Dモデルデータ
	KamataEngine::Model* model_Block = nullptr;
	KamataEngine::Model* model_Skydome = nullptr;
	KamataEngine::Model* model_Enemy = nullptr;
	KamataEngine::Model* model_DeathParticles = nullptr;

public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	~GameScene();
	void CheckAllCollisions();
	DeathParticles* deathParticles_ = nullptr;

};