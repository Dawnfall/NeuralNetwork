#pragma once

#ifdef NEURAL_EXPORT
	#define NEURAL_API __declspec(dllexport)
#else
	#define NEURAL_API __declspec(dllimport)
#endif