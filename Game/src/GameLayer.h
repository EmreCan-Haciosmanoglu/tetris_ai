#pragma once
#include "Can.h"
#include "imgui.h"

#include "../temps/NeuralNetwork.h"
#include "Player.h"

class GameLayer : public Can::Layer::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override {}
	virtual void OnDetach() override {}

	virtual void OnUpdate(Can::TimeStep ts) override;
	virtual void OnEvent(Can::Event::Event& event) override;
	virtual void OnImGuiRender() override;

	void PlayerDied() { m_PlayerLeft--; }

	inline std::vector<std::vector<bool>> GetBlock(int index) { return m_Blocks[index]; }
	void AddData(std::array<float, STATE_SIZE> data);
private:
	void UpdateGame(float ts);
	void DrawGame();
	void DrawToLabel();
	void ModifyTheData(std::array<float, 5> label);
	void LabelTheData(std::array<float, 5> label);
	void NextGeneration();
	void Test();
	void Train();
	void NewBrain(float learningRate, int layerCount, int* nodes, bool default);
	void ShuffleData();
	void DataFromFile();
	void DataFromFile2();
	void DataToFile();
	void DataToFile2();
	void DataToFile3();
	float* GetRandomState(std::map<std::array<float, STATE_SIZE>, std::array<float, 5>>::iterator it);

public:
	float m_LastPerc = 0.0f;
	int m_Epoch = 100;
	int m_CurrentEpoch = 0;
	bool b_IsTraining = false;

	int m_LabelIndex = 0;
	int m_UnlearnedDataIndex = 0;
	int m_GameWidth = 10;
	int m_GameHeight = 20;

	bool b_CustomNN = false;
	bool b_Modify = false;

	std::vector<std::map<std::array<float, STATE_SIZE>, std::array<float, 5>>::iterator> m_TrainingDataIndexVector;
	std::map<std::array<float, STATE_SIZE>, std::array<float, 5>> m_LabeledData;
	std::vector<std::array<float, STATE_SIZE>> m_UnlabeledData;
	std::vector<std::array<float, STATE_SIZE>> m_UnlearnedData;
	NeuralNetwork* m_Brain;

private:
	int m_LabelCounts[5] = { 0, 0, 0, 0, 0 };
	bool b_First = true;
	int m_CurrenPlayerIndex = 0;
	bool label = false;
	Can::Camera::OrthographicCameraController m_CameraController;
	std::vector< std::vector<std::vector<bool>>> m_Blocks = { {
		{ 1 },
		{ 1 },
		{ 1 },
		{ 1 }
		},{
		{ 1 , 0 },
		{ 1 , 0 },
		{ 1 , 1 }
		},{
		{ 1 , 1 },
		{ 1 , 0 },
		{ 1 , 0 }
		},{
		{ 1 , 0 },
		{ 1 , 1 },
		{ 0 , 1 }
		},{
		{ 0 , 1 },
		{ 1 , 1 },
		{ 1 , 0 }
		},{
		{ 1, 1 },
		{ 1, 1 }
		},{
		{ 1, 0 },
		{ 1, 1 },
		{ 1, 0 }
	} };


	int m_PlayerCount;
	int m_PlayerLeft;

	int m_MaxPoint = 0;
	int m_CurrentMaxPoint = 0;
	int m_Generation = 0;


	Player** m_Players;
};