// comment out the below definition if using elsewhere
// uncomment out below definition if testing
//#define MATH_EVAL_EXAMPLE_MAIN 
#ifdef MATH_EVAL_EXAMPLE_MAIN
#include "../include/ExpressionEvaluation.h"
#include <unordered_map>
#include <iostream>
#include <array>
#include <chrono>


int main(int argc, char** argv)
{
	std::unordered_map<std::string, size_t> definition;
	definition["a"] = 0;
	MathEvaluator<1> compute("a * sin(3.14) - cos(2)", definition);
	compute.Setup();
	//MathEvaluator<1>::Setup();

	std::array<float, 1> inputs{};
	inputs[0] = 1.0f;

	double avg_ns_noCache;
	double avg_ns_cache;

	std::chrono::time_point< std::chrono::high_resolution_clock> time = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 50000; i++)
	{
		inputs[0] = (i % 20);
		//std::cout << compute.Evaluate(inputs) << std::endl
		//inputs[0] = i;
		compute.Evaluate(inputs);
		auto currentTime = std::chrono::high_resolution_clock::now(); // current time
		auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - time); // time diff
		time = currentTime; // update current time
		avg_ns_noCache = elapsed_time.count(); // print time diff
	}

	time = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 50000; i++)
	{
		inputs[0] = (i % 20);
		//inputs[0] = i;
		//std::cout << compute.Evaluate(inputs, true) << std::endl;
		compute.Evaluate(inputs, true);
		auto currentTime = std::chrono::high_resolution_clock::now(); // current time
		auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - time); // time diff
		time = currentTime; // update current time
		avg_ns_cache = elapsed_time.count(); // print time diff
	}

	std::cout << "results (50000 iterations)\n" << "avg cache: " << avg_ns_cache / 50000.0 << "ns" << std::endl << "avg no cache : " << avg_ns_noCache / 50000.0 << "ns" << std::endl;
	std::cin.get();
	

	return 0;
}

#endif /* MATH_EVAL_EXAMPLE_MAIN */