#include "GameScene.h"

using namespace KamataEngine;
// デストラクタ
GameScene::~GameScene()
{
	delete player_;
	delete model_;
}
void GameScene::Initialize()
{
	// カメラ
	camera_.Initialize();
	worldTransform_.Initialize();
	camera_.Initialize();
	model_ = Model::Create();
	textureHandle_ = TextureManager::Load("uvChecker.png");
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &camera_);
}
void GameScene::Update() 
{
	player_->Update();
}
void GameScene::Draw()
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	player_->Draw();
	//model_->Draw(worldTransform_, camera_, textureHandle_);
	Model::PostDraw();
}