#include "Skydome.h"
using namespace KamataEngine;
//Initializeの関数定義
void Skydome::Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera) 
{
	assert(model);
	model_ = model;
	//textureHandle_ = textureHandle;
	camera_ = camera;
	worldTransform_.Initialize();
	Model::CreateFromOBJ("skydome", true);
}
//Updateの関数定義
void Skydome::Update() 
{
	worldTransform_.TransferMatrix(); 
}

//Drawの関数定義
void Skydome::Draw() 
{
	// 3Dモデル描画
	model_->Draw(worldTransform_,*camera_); 
}