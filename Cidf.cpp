#include "Cidf.h"
#include "extra_std.h"

#include <cctype>
#include <iostream>

using namespace std;
using namespace estd;

void idfNode::print() {
	std::cout << this->name << std::endl;

	if(! this->categories.empty()) {
		std::cout << "Categories: " << std::endl;
		for(unsigned int index = 0; index < this->categories.size(); ++index) {
			categories[index].print();
			std::cout << std::endl;
		}
	}

	if(! this->variables.empty()) {
		std::cout << "Variables: " << std::endl;
		for(unsigned int index = 0; index < this->variables.size(); ++index) {
			std::string name = get<0>(variables[index]);
			std::string value = get<1>(variables[index]);
			std::cout << "(" << name << "," << value << ")" << std::endl;
		}
	}
}

Cidf::Cidf(string filename) {
	std::string content = loadFile(filename);
	this->node = parse(tokenize(content));
}

Cidf::~Cidf() {

}

void Cidf::tokenizePrint(std::string filename) {
	std::vector<tuple<string, Cidf::TokenType>> tokens = tokenize( loadFile(filename) );

	std::cout << "[";
	for(unsigned int index = 0; index < tokens.size()-1; ++index) {
		std::cout << get<0>(tokens[index]) << ",";
	}
	std::cout << get<0>(tokens[tokens.size()-1]);
	std::cout << "]" << std::endl;
}

vector<tuple<string, Cidf::TokenType>> Cidf::tokenize(string content) {
	vector<tuple<string, TokenType>> result;

	for(unsigned int index = 0; index < content.size(); index++) {
		if(content[index] == '\n')  {
			result.push_back( tokenizeNewline() );
		}
		else if(content[index] == '.' || isdigit(content[index])) {
			result.push_back( tokenizeNumber(content, &index) );
		} 
		else if(content[index] == '\"') {
			result.push_back( tokenizeString(content, &index) );
		}
		else if(isalpha(content[index])) {
			result.push_back( tokenizeWord(content, &index) );
		}
		else if(content[index] == ' ' || content[index] == '\t') {
			result.push_back( tokenizeWhitespace(content, &index) );
		}
		else throw exception("In tokenize: parsing gone wrong!" + content[index]);
	}
	return result;
}

tuple<string, Cidf::TokenType> Cidf::tokenizeNewline() {
	return make_tuple("\n", TokenType::NEWLINE);
}

tuple<string, Cidf::TokenType> Cidf::tokenizeNumber(string str, unsigned int* i) {
	unsigned int &index = (*i);
	bool point = false;
	std::string result;

	for(; index < str.size(); index++) {
		if(!point && str[index] == '.') { 
			result.push_back(str[index]);
			point = true;
		}
		else if(isdigit(str[index])) {
			result.push_back(str[index]);
		}
		else { 
			index--; 
			break;
		}
	}

	return make_tuple(result, TokenType::NUMBER);
}

tuple<string, Cidf::TokenType> Cidf::tokenizeWord(string str, unsigned int* i) {
	unsigned int &index = (*i);
	std::string result;

	for(; index < str.size(); index++) {
		if(isalnum(str[index]) || str[index] == '_' || str[index] == '-') result.push_back(str[index]);
		else { 
			index--; 
			break;
		}
	}

	return make_tuple(result, TokenType::WORD);
}

tuple<string, Cidf::TokenType> Cidf::tokenizeString(string str, unsigned int* i) {
	unsigned int &index = (*i);
	std::string result;

	result.push_back(str[index]);
	index++;

	for(; index < str.size(); index++) {
		if(str[index] == '\"') { 
			result.push_back(str[index]);
			break;
		}
		else result.push_back(str[index]);
	}

	return make_tuple(result, TokenType::STRING);
}

tuple<string, Cidf::TokenType> Cidf::tokenizeWhitespace(string str, unsigned int* i) {
	unsigned int &index = (*i);
	std::string result;

	for(; index < str.size(); index++) {
		if(str[index] == ' ' || str[index] == '\t') result.push_back(str[index]);
		else { 
			index--; 
			break;
		}
	}

	return make_tuple(result, TokenType::WHITESPACE);
}

idfNode Cidf::parse(vector<tuple<string, TokenType>> tokens) {
	unsigned int index = 0;

	if(get<1>(tokens[0]) == TokenType::WHITESPACE) return parse2(tokens, "root", get<0>(tokens[0]), &index);	
	return parse2(tokens, "root", "", &index);
}

idfNode Cidf::parse2(vector<tuple<string, TokenType>> tokens, string name, string level, unsigned int* i) {
	unsigned int &index = (*i);
	idfNode result;
	result.name = name;

	while(index < tokens.size()) {
		TokenType type = get<1>(tokens[index]);
		std::string str = get<0>(tokens[index]);

		if(type == TokenType::WHITESPACE) {
			if(str.size() < level.size()) return result;
			if(str.size() > level.size()) throw exception("Cidf parse2: unexpected level increase");
			
			++index;
			if(parse2CatOrVar(tokens, level, i)) result.categories.push_back( parse2Cat(tokens, level, i) );
			else result.variables.push_back( parse2Var(tokens, level, i) );
		}
		else if(get<1>(tokens[index]) == TokenType::WORD && level.empty()) {
			if(parse2CatOrVar(tokens, level, i)) result.categories.push_back( parse2Cat(tokens, level, i) );
			else result.variables.push_back( parse2Var(tokens, level, i) );
		}
		else if(get<1>(tokens[index]) == TokenType::WORD) {
			return result;
		}
		else throw exception(("Cidf parse2: unexpected token found: " + str).c_str());
	}

	return result;
}

bool Cidf::parse2CatOrVar(vector<tuple<string, TokenType>> tokens, string level, unsigned int* i) {
	unsigned int &index = (*i);

	if(index + 2 < tokens.size()) {
		if(get<1>(tokens[index+1]) == TokenType::NEWLINE || get<1>(tokens[index+2]) == TokenType::NEWLINE) {
			return true;
		}
		return false;
		
	}
	else return true;
}

idfNode Cidf::parse2Cat(vector<tuple<string, TokenType>> tokens, string level, unsigned int* i) {
	idfNode result;
	unsigned int &index = (*i);
	result.name = get<0>(tokens[index]);

	parseNext(tokens, i);

	if(index < tokens.size() && get<1>(tokens[index]) == TokenType::WHITESPACE && get<0>(tokens[index]).size() > level.size())
		result = parse2(tokens, result.name, get<0>(tokens[index]), i);
	else
		return result;

	if(get<1>(tokens[index]) != TokenType::WORD) {
		parseNext(tokens, i);
	}

	return result;
}

tuple<string, string> Cidf::parse2Var(vector<tuple<string, TokenType>> tokens, string level, unsigned int* i) {
	std::string name;
	std::string value;
	unsigned int &index = (*i);

	TokenType valueType = get<1>(tokens[index+2]);
	if(valueType != TokenType::STRING && valueType != TokenType::NUMBER) throw exception("Cidf parse2Var: A variable isn't a string or number!");

	name = get<0>(tokens[index]);
	value = get<0>(tokens[index+2]);

	index += 3;

	parseNext(tokens, i);

	return make_tuple(name, value);
}

void Cidf::parseNext(vector<tuple<string, TokenType>> tokens, unsigned int* i) {
	unsigned int &index = (*i);

	while(index < tokens.size()-1 && get<1>(tokens[index+1]) != TokenType::WORD)
		++index;

	if(index < tokens.size() && get<1>(tokens[index]) == TokenType::NEWLINE)
		++index;
}