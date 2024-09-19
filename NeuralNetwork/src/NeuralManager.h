#pragma once

#include "NeuralNetwork.h"
#include "Core.h"

#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>

namespace dawn
{
	struct NeuralRequest
	{
		int Id;
		float* Inputs;
	};

	struct NeuralResult
	{
		int Id;
		float* Outputs;
	};

	class NeuralManager
	{
	public:
		NeuralManager();
		~NeuralManager();

		void AddRequest(const NeuralRequest& request);
		std::vector<NeuralResult> PopResults();


	private:
		void Run();

		std::thread m_runThread;
		std::mt19937 m_randGen;
		std::vector<std::unique_ptr<NeuralNetwork>> m_allNNs;

		std::queue<NeuralRequest> m_requests;
		std::queue<NeuralResult> m_results;
		std::mutex m_mutex;
		std::condition_variable m_conditionVar;
	};
}
