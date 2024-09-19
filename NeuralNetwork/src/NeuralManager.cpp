#include <random>

#include "NeuralManager.h"
#include "NeuralNetwork.h"

#include <thread>

namespace dawn
{
	NeuralManager::NeuralManager() :
		m_randGen(std::random_device()()),
		m_runThread(& NeuralManager::Run, this)
	{}

	NeuralManager::~NeuralManager()
	{
		if (m_runThread.joinable())
		{
			//process anything before stopping
			m_runThread.join();
		}
	}

	void NeuralManager::AddRequest(const NeuralRequest& request)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_requests.push(request);
		m_conditionVar.notify_one();
	}

	std::vector<NeuralResult> NeuralManager::PopResults()
	{
		std::vector<NeuralResult> results;

		std::lock_guard<std::mutex> finishedLock(m_mutex);
		while (!results.empty())
		{
			results.push_back(m_results.front()); 
			m_results.pop();
		}

		return results;
	}



	void NeuralManager::Run()
	{
		while (true)
		{
			//// Wait until there is a request in the queue
			//std::unique_lock<std::mutex> lock(mutex);
			//cv.wait(lock, [] { return !requestQueue.empty(); });

			//// Pop the request from the queue
			//Request request = requestQueue.front();
			//requestQueue.pop();

			//// Unlock the mutex during request processing
			//lock.unlock();

			//// Process the request
			//// ...

			//// Add the finished request to the finished queue
			//std::lock_guard<std::mutex> finishedLock(mutex);
			//finishedQueue.push(request);
		}
	}

}