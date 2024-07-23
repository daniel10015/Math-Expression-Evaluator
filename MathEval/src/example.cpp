#include "../include/ExpressionEvaluation.h"
#include <unordered_map>
#include <iostream>
#include <array>

int main(int argc, char** argv)
{
	std::unordered_map<std::string, size_t> definition;
	definition["a"] = 0;
	MathEvaluator<1> compute("69*a + 4.0-4/2 + sin(3.14) - cos(2)", definition);
	compute.Setup();
	//MathEvaluator<1>::Setup();

	std::array<float, 1> inputs{};
	inputs[0] = 1.0f;

	std::cout << compute.Evaluate(inputs) << std::endl;

	return 0;
}