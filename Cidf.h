#ifndef _CIDF_H_
#define _CIDF_H_

#include <vector>
#include <string>
#include <fstream>
#include <tuple>

/**
 *	freaking love tuples!
 *	this should be in a libary.
 */

class idfNode {
public:
	std::string name;
	std::vector<idfNode> categories;
	std::vector<std::tuple<std::string, std::string>> variables;

	void print();
};

class Cidf {
public:
	Cidf() {};
	Cidf(std::string filename);
	~Cidf();

	idfNode node;

	//TEST FUNCTIONS
	void tokenizePrint(std::string content);

private:
	enum class TokenType {
		NUMBER,
		WORD,
		STRING,
		WHITESPACE,
		NEWLINE,
		OTHER
	};

	std::vector<std::tuple<std::string, TokenType>> tokenize(std::string content);
		std::tuple<std::string, Cidf::TokenType> tokenizeNumber(std::string, unsigned int*);
		std::tuple<std::string, Cidf::TokenType> tokenizeWord(std::string, unsigned int*);
		std::tuple<std::string, Cidf::TokenType> tokenizeString(std::string, unsigned int*);
		std::tuple<std::string, Cidf::TokenType> tokenizeWhitespace(std::string, unsigned int*);
		std::tuple<std::string, Cidf::TokenType> tokenizeNewline();

	idfNode parse(std::vector<std::tuple<std::string, TokenType>>);
		idfNode parse2(std::vector<std::tuple<std::string, TokenType>>, std::string, std::string, unsigned int*);
		bool parse2CatOrVar(std::vector<std::tuple<std::string, TokenType>>, std::string, unsigned int*);
		idfNode parse2Cat(std::vector<std::tuple<std::string, TokenType>>, std::string, unsigned int*);
		std::tuple<std::string, std::string> parse2Var(std::vector<std::tuple<std::string, TokenType>>, std::string, unsigned int*);
		void parseNext(std::vector<std::tuple<std::string, TokenType>> tokens, unsigned int* i);
};

#endif //_CIDF_H_