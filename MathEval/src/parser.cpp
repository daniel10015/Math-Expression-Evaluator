#include <iostream>
#include "parser.h"
#include "lexer.h"

using std::cout;
using std::endl;

namespace Lexer
{

	void parser::syntax_error()
	{
		cout << "syntax TOKEN_TYPE_ERROR\n";
		exit(1);
	}

	Token parser::expect(TokenType tt)
	{
		Token t1 = lex->GetToken();
		if (t1.token_type != tt)
		{
#ifdef DEBUG_PARSER
			cout << "expected " << tt << ", but got " << t1.token_type << endl;
#endif
			syntax_error(); // add calls here
			return { 0 };
		}
		return t1;
	}

	int parser::GetLineNo() { return this->line_no; }

	tree_node* parser::GetRoot() { return this->root_node; }

	parser::~parser()
	{
		if (lex)
			delete lex;
		if (tree_node_memory)
			delete[] tree_node_memory;
	}

	parser::parser(std::string input)
	{
		lex = new LexicalAnalyzer(input);

		number_of_tree_nodes = lex->GetNumOfToks();
		tree_node_memory = new tree_node[number_of_tree_nodes];

		infix_precedence[TokenType::END_OF_FILE] = -1;
		infix_precedence[TokenType::VAR] = 0;
		infix_precedence[TokenType::NUM] = 0;
		infix_precedence[TokenType::PLUS] = 2;
		infix_precedence[TokenType::MINUS] = 2;
		infix_precedence[TokenType::MULT] = 4;
		infix_precedence[TokenType::DIV] = 4;
		infix_precedence[TokenType::EXP] = 5;
		infix_precedence[TokenType::LPAREN] = 6;
		infix_precedence[TokenType::RPAREN] = 0;
		infix_precedence[TokenType::LBRAC] = 6;
		infix_precedence[TokenType::RBRAC] = 0;
		infix_precedence[TokenType::SIN] = 0;
		infix_precedence[TokenType::COS] = 0;
		infix_precedence[TokenType::TAN] = 0;
		infix_precedence[TokenType::ARCSIN] = 0;
		infix_precedence[TokenType::ARCCOS] = 0;
		infix_precedence[TokenType::ARCTAN] = 0;
		// END_OF_FILE = 0, 
		// EXP, SIN, COS, TAN, ARCSIN, ARCCOS, ARCTAN, 
		// NUM, ID, VAR, EQUAL, NOT_EQUAL, PLUS, MINUS, 
		// MULT, DIV, COMMA, LPAREN, RPAREN, LBRAC, RBRAC, LESS, GREATER, 
		// TOKEN_TYPE_ERROR
		prefix_precedence[TokenType::END_OF_FILE] = -1;
		prefix_precedence[TokenType::VAR] = 0;
		prefix_precedence[TokenType::NUM] = 0;
		prefix_precedence[TokenType::PLUS] = 2;
		prefix_precedence[TokenType::MINUS] = 2;
		prefix_precedence[TokenType::MULT] = 4;
		prefix_precedence[TokenType::DIV] = 4;
		prefix_precedence[TokenType::EXP] = 5;
		prefix_precedence[TokenType::LPAREN] = prefix_precedence.at(TokenType::MULT);
		prefix_precedence[TokenType::RPAREN] = 0;
		prefix_precedence[TokenType::LBRAC] = prefix_precedence.at(TokenType::MULT);
		prefix_precedence[TokenType::RBRAC] = 0;
		prefix_precedence[TokenType::SIN] = 0;
		prefix_precedence[TokenType::COS] = 0;
		prefix_precedence[TokenType::TAN] = 0;
		prefix_precedence[TokenType::ARCSIN] = 0;
		prefix_precedence[TokenType::ARCCOS] = 0;
		prefix_precedence[TokenType::ARCTAN] = 0;
	}

	void parser::PrintTokens()
	{
		Token t;
		t = lex->GetToken();
		while (t.token_type != TokenType::END_OF_FILE)
		{
			t.Print();
			t = lex->GetToken();
		}
	}

	int8_t parser::getInfixPrecedence(TokenType t)
	{
		if (infix_precedence.find(t) != infix_precedence.end())
			return  infix_precedence.at(t);
		return -1;
	}

	int8_t parser::getPrefixPrecedence(TokenType t)
	{
		if (prefix_precedence.find(t) != prefix_precedence.end())
			return  prefix_precedence.at(t);
		return -1;
	}

	unary_op parser::GetUnaryOp(TokenType t)
	{
		switch (t)
		{
		case TokenType::EXP:
			return unary_op::EXP_OP;
		case TokenType::SIN:
			return unary_op::SIN_OP;
		case TokenType::COS:
			return unary_op::COS_OP;
		case TokenType::TAN:
			return unary_op::TAN_OP;
		case TokenType::ARCSIN:
			return unary_op::ARCSIN_OP;
		case TokenType::ARCCOS:
			return unary_op::ARCCOS_OP;
		case TokenType::ARCTAN:
			return unary_op::ARCTAN_OP;
		default:
			return unary_op::ERROR_UN_OP;
		}
	}

	bool parser::is_identifier(TokenType t)
	{
		switch (t)
		{
			//case EXP:
			//case SIN:
			//case COS:
			//case TAN:
			//case ARCSIN:
			//case ARCCOS:
			//case ARCTAN:
		case TokenType::NUM:
		case TokenType::ID:
			return true;
		default:
			return false;
		}
	}

	unary_op parser::GetPrefixOp(TokenType t)
	{
		switch (t)
		{
		case TokenType::EXP:
			return unary_op::EXP_OP;
		case TokenType::SIN:
			return unary_op::SIN_OP;
		case TokenType::COS:
			return unary_op::COS_OP;
		case TokenType::TAN:
			return unary_op::TAN_OP;
		case TokenType::ARCSIN:
			return unary_op::ARCSIN_OP;
		case TokenType::ARCCOS:
			return unary_op::ARCCOS_OP;
		case TokenType::ARCTAN:
			return unary_op::ARCTAN_OP;
		case TokenType::MINUS:
			return unary_op::MINUS_OP;
		case TokenType::NUM:
			return unary_op::NUM_OP;
		case TokenType::ID:
			return unary_op::ID_OP;
		default:
			return unary_op::ERROR_UN_OP;
		}
	}

	bin_op parser::GetBinOp(TokenType t)
	{
		switch (t)
		{
		case TokenType::PLUS:
			return bin_op::ADD_OP;
		case TokenType::MINUS:
			return bin_op::SUB_OP;
		case TokenType::MULT:
			return bin_op::MULT_OP;
		case TokenType::DIV:
			return bin_op::DIV_OP;
		default:
			return bin_op::ERROR_BIN_OP;
		}
	}

	tree_node* parser::parse()
	{
		root_node = parse_block();
		return root_node;
	}

	tree_node* parser::parse_block()
	{
		TokenType t1 = lex->peek(1).token_type; // ID
		TokenType t2 = lex->peek(2).token_type; // LPAREN
		TokenType t3 = lex->peek(3).token_type; // VAR
		TokenType t4 = lex->peek(4).token_type; // COMMA (or) EQUAL 
		TokenType t5 = lex->peek(5).token_type;
		if (t1 == TokenType::ID && t2 == TokenType::LPAREN && t3 == TokenType::ID)
		{
			if (t4 == TokenType::COMMA || (t4 == TokenType::RPAREN && t5 == TokenType::EQUAL))
			{
				parse_decl();
				expect(TokenType::EQUAL);
			}
		}
		return parse_expr(0);
	}

	// function declaration section
	tree_node* parser::parse_decl()
	{
		function_name.push_back(expect(TokenType::ID).lexeme); // function name
		expect(TokenType::LPAREN);
		parse_varList();
		expect(TokenType::RPAREN);
		return nullptr;
	}

	tree_node* parser::parse_varList()
	{
		function_name.push_back(expect(TokenType::ID).lexeme);
		if (lex->peek(1).token_type == TokenType::COMMA)
			parse_varList();
		return nullptr;
	}

	// pratt parsing
	// first call should terminate on EOF
	tree_node* parser::parse_expr(int8_t precedence)
	{
		tree_node* left = parse_prefix(); // becomes possible LHS
		tree_node* temp;
		Token op;
#ifdef DEBUG_PARSER
		cout << "parse expr: " << lex->peek(1).lexeme << endl;
#endif
		while (getInfixPrecedence(lex->peek(1).token_type) > precedence)
		{
#ifdef DEBUG_PARSER
			cout << "higher precedence, recursively call expr" << endl;
#endif
			op = lex->GetToken();
			temp = left;
			left = &(tree_node_memory[tree_node_idx++]); // new tree_node{}; //{ BINARY_OP, left, parse_expr( getInfixPrecedence(op.token_type) ), GetBinOp(op.token_type) }; // lhs, rhs, op
			left->type = node_type::BINARY_OP;
			left->binary_op.lhs = temp; // previous left
			left->binary_op.rhs = parse_expr(getInfixPrecedence(op.token_type));
			left->binary_op.op_type = GetBinOp(op.token_type);
		}
		return left;
	}

	tree_node* parser::parse_prefix()
	{
		Token t1 = lex->GetToken();
#ifdef DEBUG_PARSER
		cout << "parse prefix: " << t1.lexeme << " " << t1.token_type << endl;
#endif
		if (t1.token_type == TokenType::END_OF_FILE) return nullptr;
		if (is_identifier(t1.token_type))
		{
#ifdef DEBUG_PARSER
			cout << "is identifier\n";
#endif
			tree_node* node = &(tree_node_memory[tree_node_idx++]); //new tree_node{};// { PREFIX_OP, nullptr, nullptr, static_cast<bin_op>(0)}; // union cheese jazz club
			node->type = node_type::PREFIX_OP;
			node->prefix_op.lexeme = new std::string(); // static initialization (added line)
			*(node->prefix_op.lexeme) = t1.lexeme;
			node->prefix_op.op = GetPrefixOp(t1.token_type);
			node->prefix_op.next = nullptr;
#ifdef DEBUG_PARSER
			cout << "next token type: " << lex->peek(1).token_type << "\n";
#endif
			return node;
		}
		else
		{
#ifdef DEBUG_PARSER
			cout << "is NOT identifier\n";
#endif
			int8_t precedence = getPrefixPrecedence(t1.token_type);
			if (precedence != -1)
			{
				if (t1.token_type == TokenType::LPAREN || t1.token_type == TokenType::RPAREN)
					return parse_expr(0); // reset precedence
				else
				{
					tree_node* temp_tn = &(tree_node_memory[tree_node_idx++]); //new tree_node{}; // {PREFIX_OP, nullptr, nullptr, static_cast<bin_op>(0)};
					temp_tn->type = node_type::PREFIX_OP;
					temp_tn->prefix_op.op = GetUnaryOp(t1.token_type);
					temp_tn->prefix_op.next = parse_expr(0);
					return temp_tn;
				}
			}
			else
			{
#ifdef DEBUG_PARSER
				cout << "parse_prefix syntax TOKEN_TYPE_ERROR\n" << t1.token_type << endl << (int)precedence << endl;
#endif
				syntax_error();
			}
		}
		return nullptr; // program never reaches here, it just stops the compiler warning
	}

#ifdef DEBUG_PARSER
	void parser::PrintBFS(tree_node* node)
	{
		std::queue<tree_node*> q;
		q.push(node);
		tree_node* n = nullptr;

		cout << "printbfs\n";


		while (q.front() != nullptr)
		{
			// print top of queue
			n = q.front();
			if (n->type == node_type::BINARY_OP)
				cout << n->binary_op.op_type << endl;
			else
				cout << *(n->prefix_op.lexeme) << " " << n->prefix_op.op << endl;
			q.pop();

			// recursive call
			if (n->type == node_type::BINARY_OP)
			{
				q.push(n->binary_op.lhs);
				q.push(n->binary_op.rhs);
			}
			else if (n->prefix_op.next != nullptr)
			{
				q.push(n->prefix_op.next);
			}
		}

		/*
			while(!q.empty())
			{
				if(q.front()->type == BINARY_OP)
					cout << q.front()->binary_op.op_type << endl;
				else
					cout << *(q.front()->prefix_op.lexeme) << " " << q.front()->prefix_op.op << endl;
				q.pop();
			}*/
	}
#endif /*DEBUG_PARSER*/

	// make this std::optional
	const std::vector<string>& parser::GetFunctionName()
	{
		if (function_name.empty())
			return std::vector<string>();
		return function_name;
	}

#ifdef MAIN_H

	// for testing purposes
	int main(int argc, char** argv)
	{
		cout << "usage: ./a.out < input.txt\n\n";

		parser p("5.0+a");
		//parser p("sin(sin(sin(sin(sin(sin(5*5))))))");
		//p.parse();
		p.PrintBFS(p.parse());
		//p.PrintTokens();
		return 0;
	}

#endif /* MAIN_H */

	// type checking class
	type_check::type_check(tree_node* node)
		: root(node)
	{}

	std::pair<bool, string> type_check::isFunction()
	{
		//if(root->type == BINARY_OP && root->)
		return std::make_pair(false, "");
	}

};