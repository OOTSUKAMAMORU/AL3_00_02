#include "player.h"
#include "MyMath.h"
using namespace KamataEngine;
// Initializeの関数定義
void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) 
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	camera_ = camera;
}

// Updateの関数定義
void Player::Update() 
{ 
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_. translation_);
	worldTransform_.TransferMatrix();

}

// Drawの関数定義
void Player::Draw() 
{
	// 3Dモデル描画

	model_->Draw(worldTransform_, *camera_);
}