# MathEval
Evaluates a function at points with user-defined variable names 

# How to run
- Clone the repo by running `clone https://github.com/daniel10015/Math-Expression-Evaluator.git`
- Example code is in `MathEval/src/example.cpp`. Uncomment `#define MATH_EVAL_EXAMPLE_MAIN` to use the main function, otherwise don't include it, or remove the file, to use as a submodule.

# Features
- Grammar defined in `parser.h` https://github.com/daniel10015/Math-Expression-Evaluator/blob/master/MathEval/src/parser.h?plain=1#L16
- Supports single-precision floating point operations only, it will convert integers to float
- Arbitrary function input size, and user-defined variable names
- Currently only parses explicitly (e.g. `2tan(x)` must be `2*tan(x)`)
- Optional caching
  - NOTE: From testing, caching is 2x slower than not caching if it always misses, however, cache hits can be up to 10x faster.   

# How it works
- Lexer will tokenize input string for parser to read
- Parser will construct a tree with operator precedence using a pratt parser
- Math Evaluator does a dfs on the tree to compute the output
