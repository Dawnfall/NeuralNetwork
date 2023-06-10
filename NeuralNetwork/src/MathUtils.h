#pragma once

#include <math.h>
#include <vector>

class MathUtils
{
public:
	static float Default(float x) {
		return x;
	}

	float DefaultDerivative(float x) {
		return x; //TODO: maybe wrong??
	}

	inline static float Sigmoid(float x) {
		if (x > 0) {
			return 1.0f / (1.0f + exp(-x));
		}
		else {
			float exp_x = exp(x);
			return exp_x / (1.0f + exp_x);
		}
	}

	inline static float SigmoidDerivative(float x) {
		float sigmoid_x = Sigmoid(x);
		return sigmoid_x * (1.0f - sigmoid_x);
	}

	inline static float SigmoidDerivativeFromSigmoidInput(float sigmoidX)
	{
		return sigmoidX * (1 - sigmoidX);
	}

	inline static Eigen::VectorXf SigmoidDerivativeFromSigmoidInputVec(const Eigen::VectorXf& sigmoidX)
	{
		Eigen::VectorXf res(sigmoidX.size());
		for (int i = 0; i < sigmoidX.size(); i++)
			res[i] = MathUtils::SigmoidDerivativeFromSigmoidInput(sigmoidX[i]);
		return std::move(res);
	}

	static float SumSquaredVector(const Eigen::VectorXf& vec)
	{
		float sum = 0;
		for (float v : vec)
			sum += v * v;

		return sum;
	}

	static void SubstractVec(const std::vector<float>& A, const std::vector<float>& B, Eigen::VectorXf& result)
	{
		for (int i = 0; i < A.size(); i++)
			result[i] = A[i] - B[i];
	}

	static bool AreEqual(float a, float b, float tolerance = 0.0001) {
		return std::abs(a - b) < tolerance;
	}
	static bool AreEqual(const std::vector<float>& a, const std::vector<float>& b, float tolerance = 0.001){
		if (a.size() != b.size())
			return false;

		for (int i = 0; i < a.size(); i++)
			if (!AreEqual(a[i], b[i], tolerance))
				return false;
		return true;
	}

	static void Print(const std::string& text)
	{
		printf(text.c_str());
	}

	static void Print(int i)

	{
		Print(std::to_string(i));
	}

	static void Print(float i)
	{
		Print(std::to_string(i));
	}

	static void Print(const std::vector<float>& vec)
	{
		Print(VecToString(vec));
	}

	static std::string VecToString(const std::vector<float>& vec)
	{
		std::string res = "[";
		for (float i : vec)
			res += "," + std::to_string(i);
		res += "]";
		return res;
	}
};