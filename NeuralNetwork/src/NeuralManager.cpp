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

	std::vector<int> NeuralManager::CreateNNs(const std::vector<int>& layers, int count)
	{
		std::vector<int> ids;

		for (int i = 0; i < count; i++)
		{
			int newId = GetValidKey();
			m_allNNs.insert(m_allNNs.begin() + newId, std::make_unique<NeuralNetwork>(layers, &m_randGen));
			ids.push_back(newId);
		}

		return ids;
	}
	std::vector<int> NeuralManager::CreateNNsFromData(const std::vector<float>& data, int count)
	{
		std::vector<int> ids;

		for (int i = 0; i < count - 1; i++)
		{
			int newId = GetValidKey();
			m_allNNs.insert(m_allNNs.begin() + newId, std::make_unique<NeuralNetwork>(data));
			ids.push_back(newId);
		}

		return ids;
	}
	std::vector<int> NeuralManager::DuplicateNN(int id, int count)
	{
		std::vector<int> ids;

		for (int i = 0; i < count - 1; i++)
		{
			int newId = GetValidKey();
			m_allNNs.insert(m_allNNs.begin() + newId, std::make_unique<NeuralNetwork>(*m_allNNs[id]));
			ids.push_back(newId);
		}

		return ids;
	}
	void NeuralManager::DestroyNNs(const std::vector<int>& ids)
	{
		for (int id : ids)
		{
			if (m_allNNs[id] == nullptr)
				continue;
			m_allNNs[id] = nullptr;
			RemoveKey(id);
		}
	}

	std::vector<std::vector<float>> NeuralManager::GetNNDatas(const std::vector<int>& ids)
	{
		std::vector<std::vector<float>> results;

		for (int id : ids)
		{
			results.emplace_back(m_allNNs[id]->ExportData());
		}

		return results;
	}
	void NeuralManager::Mutate(const std::vector<int>& ids, float maxChange, float chance)
	{
		for (const auto& nn : m_allNNs)
		{
			nn->Mutate(chance, maxChange, m_randGen);
		}
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







	NeuralResult* NeuralManager::Calculate(NeuralRequest* requests, size_t requestsSize)
	{
		//TODO:....

		return nullptr;
	}
	std::vector<int> NeuralManager::Cross(int idA, int idB, int count)
	{
		//TODO:.....
		return std::vector<int>();
	}






	int NeuralManager::GetValidKey()
	{
		if (!m_availableIndices.empty())
		{
			int index = m_availableIndices.back();
			m_availableIndices.pop_back();
			return index;
		}
		else
		{
			return (int)m_allNNs.size();
		}
	}
	void NeuralManager::RemoveKey(int key)
	{
		if (key == m_allNNs.size() - 1)
			return;
		m_availableIndices.push_back(key);
	}
}