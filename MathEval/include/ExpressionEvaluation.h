#pragma once
#ifndef EXPRESSION_EVALUATION_H
#define EXPRESSION_EVALUATION_H

#include "../src/parser.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>


// untested
#include <numeric>    // for std::accumulate
// Custom hash function for std::array<float, S>
template<std::size_t S>
struct std::hash<std::array<float, S>> {
    std::size_t operator()(const std::array<float, S>& arr) const {
        std::size_t seed = 0;
        for (const float& val : arr) {
            // Combine the hash of each element with the seed using a prime number
            seed ^= std::hash<float>{}(val)+0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

// overload computation funcs
float add(float t1, float t2);
float sub(float t1, float t2);
float mult(float t1, float t2);
float divide(float t1, float t2);
float func_exp(float t1);
float func_sin(float t1);
float func_cos(float t1);

// cache the computation 
// should test if caching is faster or not
template <size_t S>
class MathEvaluator
{
public:
	MathEvaluator() = delete;
	// function_inputs corresponds string -> idx, idx element of (0, S-1)
	MathEvaluator(const std::string& math_expr_input, std::unordered_map<std::string, size_t>& function_inputs);
	float Evaluate(const std::array<float, S>& inputs, bool store = false);
	static void Setup(void);
private:
	parser* m_pParser;
	tree_node* m_root;;
	std::unordered_map<std::array<float, S>, float> m_cache;
    std::unordered_map<std::string, float> m_cache_constants;
	std::unordered_map<std::string, size_t> *m_function_inputs;
    // perform bfs on root and add map to m_cache_constants
    void ConvertConstantsToFloats();
	// function pointer array
	// 2-parameter functions
	static std::vector<std::function<float(float, float)>> s_twoParameterFunctions;
	// 1-parameter functions
	static std::vector<std::function<float(float)>> s_oneParameterFunctions;

private: // herlper functions
	float Evaluate_recursive(const std::array<float, S>&, tree_node* curr);

private: // functions
};

// alloc static class vars
template <size_t S>
std::vector<std::function<float(float, float)>> MathEvaluator<S>::s_twoParameterFunctions;
template <size_t S>
std::vector<std::function<float(float)>> MathEvaluator<S>::s_oneParameterFunctions;




#include "../include/ExpressionEvaluation.h"
#include <math.h>

template <size_t S>
MathEvaluator<S>::MathEvaluator(const std::string& math_expr_input, std::unordered_map<std::string, size_t>& function_inputs)
{
    m_pParser = new parser(math_expr_input);
    m_root = m_pParser->parse();
    m_function_inputs = &function_inputs;
    ConvertConstantsToFloats();
}


template <size_t S>
float MathEvaluator<S>::Evaluate(const std::array<float, S>& inputs, bool store)
{
    // check cache
    if (m_cache.find(inputs) != m_cache.end())
    {
        return m_cache.at(inputs);
    }

    // compute
    float result = Evaluate_recursive(inputs, m_root);

    // cache if store
    if (store)
    {
        m_cache[inputs] = result;
    }
}

template <size_t S>
float MathEvaluator<S>::Evaluate_recursive(const std::array<float, S>& inputs, tree_node* curr)
{
    float result;
    // either binary operation or function operation on single variable input
    if (curr->type == BINARY_OP)
    {
        float L = Evaluate_recursive(inputs, curr->binary_op.lhs);
        float R = Evaluate_recursive(inputs, curr->binary_op.rhs);
        // L op R
        result = s_twoParameterFunctions.at(curr->binary_op.op_type)(L, R);
    }
    else
    {
        // either NUM or single operation
        unary_op operation = curr->prefix_op.op;
        if (operation == NUM_OP || operation == ID_OP)
        {
            // plug in number if ID, else float NUM 
            if (operation == NUM_OP)
            {
                result = m_cache_constants.at(*(curr->prefix_op.lexeme));
            }
            else
            {
                result = inputs[m_function_inputs->at(*(curr->prefix_op.lexeme))];
            }
        }
        else
        {
            // compute child then op(CHILD)
            float child = Evaluate_recursive(inputs, curr->prefix_op.next);
            // op(child)
            result = s_oneParameterFunctions.at(curr->prefix_op.op)(child);
        }
    }

    return result;
}

template <size_t S>
void  MathEvaluator<S>::ConvertConstantsToFloats()
{
    std::queue<tree_node*> q;
    q.push(m_root);
    while (!q.empty())
    {
        // check top of queue
        auto n = q.front();
        q.pop();

        // recursive call
        if (n->type == BINARY_OP)
        {
            q.push(n->binary_op.lhs);
            q.push(n->binary_op.rhs);
        }
        else if (n->prefix_op.next != nullptr)
        {
            q.push(n->prefix_op.next);
        }
        else if (n->prefix_op.op == NUM_OP)
        {
            m_cache_constants[*(n->prefix_op.lexeme)] = stof(*(n->prefix_op.lexeme));
        }
    }
}



// overload computation funcs
float add(float t1, float t2)
{
    return t1 + t2;
}

float sub(float t1, float t2)
{
    return t1 - t2;
}

float mult(float t1, float t2)
{
    return t1 * t2;
}

float divide(float t1, float t2)
{
    return t1 / t2;
}

float func_exp(float t1)
{
    return expf(t1);
}

float func_sin(float t1)
{
    return sinf(t1);
}

float func_cos(float t1)
{
    return cosf(t1);
}



template <size_t S>
void MathEvaluator<S>::Setup(void)
{
    /*
      EXP_OP, SIN_OP, COS_OP, TAN_OP,
      ARCSIN_OP, ARCCOS_OP, ARCTAN_OP, MINUS_OP,
      NUM_OP, ID_OP,
    */
    // TODO
    s_oneParameterFunctions.push_back(func_exp);
    s_oneParameterFunctions.push_back(func_sin);
    s_oneParameterFunctions.push_back(func_cos);
    //m_oneParameterFunctions.push_back(func_tan);
    //m_oneParameterFunctions.push_back(func_arcsin);
    //m_oneParameterFunctions.push_back(func_arccos);
    //m_oneParameterFunctions.push_back(func_arctan);
    //m_oneParameterFunctions.push_back(func_minus);

    s_twoParameterFunctions.push_back(add);
    s_twoParameterFunctions.push_back(sub);
    s_twoParameterFunctions.push_back(mult);
    s_twoParameterFunctions.push_back(divide);
}


#endif /* EXPRESSION_EVALUATION_H */