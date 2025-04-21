#include "player.h"
using namespace KamataEngine;

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) 
{
	assert(model);
	model_=model;
	textureHandle_ = textureHandle;
	camera_ = camera;
	worldTransform_.Initialize();
}

void Player::Update() 
{
	worldTransform_.TransferMatrix(); 
}

void Player::Draw() 
{ 
	model_->Draw(worldTransform_, *camera_, textureHandle_); 
}
