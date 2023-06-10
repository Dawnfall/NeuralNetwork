#include <random>

#include "NeuralManager.h"
#include "NeuralNetwork.h"

namespace dawn
{
	NeuralManager::NeuralManager() :m_randGen(std::random_device()())
	{}

	std::vector<int> NeuralManager::CreateNNs(const std::vector<int>& layers, int count)
	{
		std::vector<int> ids;

		for (int i = 0; i < count - 1; i++)
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