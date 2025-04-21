#pragma once
#include "KamataEngine.h"
#include "player.h"
// ゲームシーン
class GameScene
{
private:
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;

public:
	~GameScene();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
};