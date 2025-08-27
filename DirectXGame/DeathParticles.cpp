#include "DeathParticles.h"
#include <numbers>
#include <algorithm>
#include "MyMath.h"
using namespace KamataEngine;
using namespace MathUtility;
void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	// ワールド変換の初期化
	for (WorldTransform& worldTransform : worldTransforms_) 
	{
		worldTransform.Initialize();
		worldTransform. translation_ = position;
	}
	//assert(model);
	model_ = model;
	camera_ = camera;
}
void DeathParticles::Update() 
{
	for (uint32_t i = 0; i < kNumParticles; i++) 
	{
		Vector3 velocity = {kSpeed, 0, 0};
		float angle = kAngleUnit * i;
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		worldTransforms_[i].translation_ += velocity;
		worldTransforms_[i].matWorld_ = MakeAffineMatrix(worldTransforms_[i].scale_, worldTransforms_[i].rotation_, worldTransforms_[i].translation_);
		worldTransforms_[i].TransferMatrix();
	}
	counter_ += 1.0f/60.0f;
	if (counter_ >= kDuration) 
	{
		counter_ = kDuration;
		isFinished_ = true;
	}
	if (isFinished_)
	{
			return;
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
}
void DeathParticles::Draw() 
{
	if (isFinished_) 
	{
		return;
	}
	for (WorldTransform& worldTransform : worldTransforms_) 
	{
		// 3Dモデル描画
		model_->Draw(worldTransform, *camera_,&objectColor_);
	}
}