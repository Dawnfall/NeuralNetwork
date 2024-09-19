//#pragma once
//
//#include <math.h>
//#include <vector>
//
//namespace MathUtils
//{
//	std::string VecToString(const std::vector<float>& vec)
//	{
//		std::string res = "[";
//		for (float i : vec)
//			res += "," + std::to_string(i);
//		res += "]";
//		return res;
//	}
//
//	void Print(const std::string& text)
//	{
//		printf(text.c_str());
//	}
//
//	void Print(int i)
//	{
//		MathUtils::Print(std::to_string(i));
//	}
//
//	void Print(float i)
//	{
//		MathUtils::Print(std::to_string(i));
//	}
//
//	void Print(const std::vector<float>& vec)
//	{
//		MathUtils::Print(MathUtils::VecToString(vec));
//	}
//
//
//
//	float ReLU(float x) {
//		return std::max(0.0f, x);
//	}
//	Eigen::VectorXf ReLUDerivative(const Eigen::VectorXf& x) {
//		Eigen::VectorXf result = x;
//		for (int i = 0; i < x.size(); ++i) {
//			result[i] = (x[i] > 0) ? 1.0f : 0.0f;
//		}
//		return result;
//	}
//
//	float Sigmoid(float x) {
//		if (x > 0) {
//			return 1.0f / (1.0f + exp(-x));
//		}
//		else {
//			float exp_x = exp(x);
//			return exp_x / (1.0f + exp_x);
//		}
//	}
//	float SigmoidDerivative(float x) {
//		float sigmoid_x = Sigmoid(x);
//		return sigmoid_x * (1.0f - sigmoid_x);
//	}
//	float SigmoidDerivativeFromSigmoidInput(float sigmoidX)
//	{
//		return sigmoidX * (1 - sigmoidX);
//	}
//	Eigen::VectorXf SigmoidDerivativeFromSigmoidInputVec(const Eigen::VectorXf& sigmoidX)
//	{
//		Eigen::VectorXf res(sigmoidX.size());
//		for (int i = 0; i < sigmoidX.size(); i++)
//			res[i] = MathUtils::SigmoidDerivativeFromSigmoidInput(sigmoidX[i]);
//		return std::move(res);
//	}
//
//	float SumSquaredVector(const Eigen::VectorXf& vec)
//	{
//		float sum = 0;
//		for (float v : vec)
//			sum += v * v;
//
//		return sum;
//	}
//
//	void SubstractVec(const std::vector<float>& A, const std::vector<float>& B, Eigen::VectorXf& result)
//	{
//		for (int i = 0; i < A.size(); i++)
//			result[i] = A[i] - B[i];
//	}
//
//	bool AreEqual(float a, float b, float tolerance = 0.0001) {
//		return std::abs(a - b) < tolerance;
//	}
//	bool AreEqual(const std::vector<float>& a, const std::vector<float>& b, float tolerance = 0.001){
//		if (a.size() != b.size())
//			return false;
//
//		for (int i = 0; i < a.size(); i++)
//			if (!AreEqual(a[i], b[i], tolerance))
//				return false;
//		return true;
//	}
//
//
//
//
//
//
//
//};



#pragma once

#include <math.h>
#include <vector>
#include <Eigen/Dense> // Ensure this is included for Eigen::VectorXf

namespace MathUtils
{
	std::string VecToString(const std::vector<float>& vec);

	void Print(const std::string& text);
	void Print(int i);
	void Print(float i);
	void Print(const std::vector<float>& vec);

	float ReLU(float x);
	Eigen::VectorXf ReLUDerivative(const Eigen::VectorXf& x);

	float Sigmoid(float x);
	float SigmoidDerivative(float x);
	float SigmoidDerivativeFromSigmoidInput(float sigmoidX);
	Eigen::VectorXf SigmoidDerivativeFromSigmoidInputVec(const Eigen::VectorXf& sigmoidX);

	float SumSquaredVector(const Eigen::VectorXf& vec);
	void SubstractVec(const std::vector<float>& A, const std::vector<float>& B, Eigen::VectorXf& result);

	bool AreEqual(float a, float b, float tolerance = 0.0001);
	bool AreEqual(const std::vector<float>& a, const std::vector<float>& b, float tolerance = 0.001);
}