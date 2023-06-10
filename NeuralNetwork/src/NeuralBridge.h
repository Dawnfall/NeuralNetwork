//#pragma once
//
//
//#include "NeuralManager.h"
//#include "Core.h"
//
////***************
//// Bridge Data
//
//namespace dawn
//{
//		NEURAL_API void* Init()
//		{
//			return new NeuralManager();
//		}
//		NEURAL_API void Delete(void* instance)
//		{
//			delete static_cast<NeuralManager*>(instance);
//		}
//		NEURAL_API void CreateNNs(void* instance,const std::vector<int>& layers,int count,int* result,int* resultSize)
//		{
//			std::vector<int> ids = static_cast<NeuralManager*>(instance)->CreateNNs(layers, count);
//
//			*resultSize = (int)ids.size();
//			result = new int[*resultSize];
//			std::copy(ids.begin(), ids.end(), result); 
//			//TODO: std::memcpy(result, ids.data(), sizeof(int) * (*resultSize)); contiguous memory!!!
//		}
//		NEURAL_API void CreateNNsFromData(void* instance,const std::vector<float>& data, int count,int* result,int* resultSize)
//		{
//			std::vector<int> ids = static_cast<NeuralManager*>(instance)->CreateNNsFromData(data, count);
//
//			*resultSize = (int)ids.size();
//			result = new int[*resultSize];
//			std::copy(ids.begin(), ids.end(), result);
//		}
//		NEURAL_API void DuplicateNN(void* instance,int id, int count,int* result,int* resultSize)
//		{
//			std::vector<int> ids = static_cast<NeuralManager*>(instance)->DuplicateNN(id, count);
//
//			*resultSize = (int)ids.size();
//			result = new int[*resultSize];
//			std::copy(ids.begin(), ids.end(), result);
//		}
//		NEURAL_API void DestroyNNs(void* instance,const std::vector<int>& ids)
//		{
//			return static_cast<NeuralManager*>(instance)->DestroyNNs(ids);
//		}
//		NEURAL_API void Mutate(void* instance, std::vector<int>& ids, float maxChange, float chance)
//		{
//			static_cast<NeuralManager*>(instance)->Mutate(ids, maxChange, chance);
//		}
//		NEURAL_API NeuralResult* Calculate(void* instance,NeuralRequest* requests,size_t requestsSize)
//		{
//			return static_cast<NeuralManager*>(instance)->Calculate(requests,requestsSize);
//		}
//		NEURAL_API void Cross(void* instance,int idA, int idB, int count,int* result,int* resultSize)
//		{
//			std::vector<int> ids = static_cast<NeuralManager*>(instance)->Cross(idA, idB, count);
//
//			*resultSize = ids.size();
//			result = new int[*resultSize];
//			std::copy(ids.begin(), ids.end(), result);
//		}
//
//		//DLL_EXPORT NeuralNetData* GetNNDatas(void* instance,const std::vector<int>& ids,NeuralNetData* result,size_t* resultSize)
////{
////	std::vector<NeuralNetData> allData = static_cast<NeuralManager*>(instance)->GetNNDatas(ids);
//
////	*resultSize = allData.size();
////	result = new NeuralNetData[*resultSize];
////	std::copy(ids.begin(), ids.end(), result);
////}
//}
//
