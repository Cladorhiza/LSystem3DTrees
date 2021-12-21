#include "LSystem.h"



namespace LSystem {

    std::pair<char, std::string> ParseLSystemRule(const std::string& ruleString) {

        std::pair<char, std::string> rule;

        rule.first = ruleString[0];
        rule.second = ruleString.substr(2, ruleString.size() - 2);

        return rule;
    }

    std::vector<std::pair<char, std::string>> ParseLSystemRules(const std::string& ruleInput) {

        //format: [A-ABB,B-A]
        std::vector<std::pair<char, std::string>> rules;
        std::stringstream ruleStream(ruleInput);
        std::string ruleString;

        while (std::getline(ruleStream, ruleString, ',')) {
            rules.push_back(ParseLSystemRule(ruleString));
        }

        return rules;
    }


    std::string CalculateLSystemAtGeneration(std::string axiom,
                                             const std::string& ruleString,
                                             int generation,
                                             std::vector<std::string>* allGens) {


        std::vector<std::pair<char, std::string>> rules = ParseLSystemRules(ruleString);

        if (allGens) allGens->push_back(axiom);

        for (int i = 1; i <= generation; i++) {

            std::string nextGenAxiom = "";

            for (char c : axiom) {
                for (const auto& r : rules) {
                    if (r.first == c) {
                        nextGenAxiom += r.second;
                        break;
                    }
                }
            }
            axiom = nextGenAxiom;
            if (allGens) allGens->push_back(axiom);
        }
        return axiom;
    }



}