#pragma once
#include <vector>
#include <sstream>
namespace LSystem
{
	/*function ParseLSystemRule(const std::string& ruleString)
	* parameters:
	*	ruleString: string containing a single L-system generational rule
	*		format = "A-FFFB"
	*		LHS(A) = symbol to expand into RHS(FFFB) every generation
	* purpose: to parse a string containing an L-system rule into a pair of char(symbol) and string(expansion)
	* returns: pair containing symbol and expansion from that symbol
	*/
	std::pair<char, std::string> ParseLSystemRule(const std::string& ruleString);
	/*function ParseLSystemRules(const std::string& ruleInput)
	*parameters:
	*	ruleInput: string containing all L-system generational rules for this system
	*		format = "A-FFFB,..."
	* purpose: to parse a string containing L-system rules into pairs of char(symbol) and string(expansion)
	* returns: pair containing symbol and expansion from that symbol
	*/
	std::vector<std::pair<char, std::string>> ParseLSystemRules(const std::string& ruleInput);
	/*function CalculateLSystemAtGeneration(std::string axiom,
											const std::string& ruleString,
											int generation,
											std::vector<std::string>* allGens = nullptr);
	*parameters:
	*	axiom: string containing beginning characters of the L-system i.e. generation 0 or n=0
	*	ruleString: string containing rules to be parsed by ParseLSystemRule()
	*	generation: return the string of L-System evaluation at this generation
	*	allGens: pointer to vector to be optionally used as storage for every single generation calculated by this function
	* purpose: To turn L-system starting parameters and rules and evaluate it to the specified generation
	* returns: string containing evaluated L-System at the generation passed
	*/
	std::string CalculateLSystemAtGeneration(std::string axiom,
											 const std::string& ruleString,
											 int generation,
											 std::vector<std::string>* allGens = nullptr);
};

