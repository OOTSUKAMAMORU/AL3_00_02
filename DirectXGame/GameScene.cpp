#include "GameScene.h"

using namespace KamataEngine;
// デストラクタ
GameScene::~GameScene()
{
	delete model_;
}
void GameScene::Initialize() 
{ 
	// カメラ
	camera_.Initialize();
	worldTransform_.Initialize();
	camera_.Initialize();
	model_ = Model::Create();
	textureHandle_ = TextureManager::Load("uvChecker.png"); }
void GameScene::Update() {

}
void GameScene::Draw() 
{ 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance(); 
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, camera_, textureHandle_);
	Model::PostDraw();
}