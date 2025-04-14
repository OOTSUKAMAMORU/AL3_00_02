#pragma once
#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR,int) 
{
	//エンジンの初期化
	KamataEngine::Initialize(L"LE2A_07_オオツカ_マモル_AL3");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	
	// ゲームシーンのインスタンス生成
	GameScene* gameScene = new GameScene();
	// ゲームシーンの初期化
	gameScene->Initialize();

	
	//メインループ
	while (true) 
	{
		//エンジンの更新
		if (KamataEngine::Update()) {
			break;

		}
		gameScene->Update();
		//描画開始
		dxCommon->PreDraw();
		//描画終了
		gameScene->Draw();
		dxCommon->PostDraw();

	}
	delete gameScene;
	//エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}
