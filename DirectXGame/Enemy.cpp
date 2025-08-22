#include "Enemy.h"
#include "MyMath.h"
#include <numbers>
using namespace KamataEngine;
using namespace MathUtility;
void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = - std::numbers::pi_v<float> / 2.0f;
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;

}
void Enemy::Update() 
{
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	walkTimer_ += 1.0f / 60.0f;
	worldTransform_.translation_ += velocity_;
	worldTransform_.rotation_.x = std::sin(walkTimer_ * 5.0f);
}
void Enemy::Draw() {
	// 3Dモデル描画

	model_->Draw(worldTransform_, *camera_);
}