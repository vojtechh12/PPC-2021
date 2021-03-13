#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <iomanip>
#include "main.hpp"
#include "parse.hpp"

int main(){  
    // variable for decoded config
    config_t config;
    std::string line;
    std::vector<config_t> config_c;
    used_config usedconfig;
    
    // setting default config parameters
    usedconfig.min = -99;
    usedconfig.max = 100;
    usedconfig.width = 3;
    usedconfig.align = "left";

    do{
        std::getline(std::cin, line);

        config = getConfig(line);
        if(config.valid){
            // todo ex. save the config line for later printing, and to read parameters
            // take care of invalid config
            config_c.push_back(config);
        }
    }
    while(config.valid);

    // rewriting paramtertres based on users wishes
    for (std::size_t i = 0; i < config_c.size(); i++)
    {
        if (config_c[i].type == "min") usedconfig.min = std::stoi(config_c[i].value);
        else if (config_c[i].type == "max") usedconfig.max = std::stoi(config_c[i].value);
        else if (config_c[i].type == "width") usedconfig.width = std::stoi(config_c[i].value);
        else if (config_c[i].type == "align") usedconfig.align = config_c[i].value;
    }
    //checking if users wishes arent tooo stupid
    if ((usedconfig.width < 0) || (usedconfig.min > usedconfig.max)) {
        std::cerr << "Invalid configuration\n";
        return 102;
        }
    
    std::vector<std::vector<int>> values;
    std::size_t longest = 0;

    // loading numbers
    while(std::getline(std::cin, line)){
        std::stringstream ss(line);
        std::vector<int> row;
        std::string cell;
        sum_t number;
        
        std::size_t row_length = 0;
        while (std::getline(ss,cell, ';')) {
            
            try {   // if loaded cell is number
                number.value = std::stoi(cell);
                if  ((number.value < usedconfig.min) || (number.value > usedconfig.max)) {
                    std::cerr << "Out of range\n";
                    return 100;
                    }
				if ((int)cell.length() > usedconfig.width) {
                    std::cerr << "Cell is too short\n";
                    return 103;
                    }
                row.push_back(number.value);
                row_length ++;
            }
            catch(const std::exception& e){ // if there is a text (SUM?)
                // algorothm for computing SUM
                // might need a change to sum_t
                sum_t sum;
                sum = getSum(cell, row);
                if (sum.valid){
                    row.push_back(sum.value);
                    row_length ++;
                } 
                // TODO else (invlaid sum / errory)
                else {
                    std::cerr << "Invalid input\n";
                    return 101;
                }
            }
            if (row_length > longest) longest = row_length;
        }   
        values.push_back(row);
        row_length = 0;
    }

    // print out the config - TODO
    std::cout << "config.min=" << usedconfig.min << std::endl;
    std::cout << "config.max=" << usedconfig.max << std::endl;
    std::cout << "config.width=" << usedconfig.width << std::endl;
    std::cout << "config.align=" << usedconfig.align << std::endl << std::endl;
    
	// print table
    // line separator
    {for (std::size_t i = 0; i <= longest; i++)
    {
        if (i==0) std::cout << "+";
        std::cout << std::right << std::setw(usedconfig.width +3) << std::setfill('-') << '+';
    }
    std::cout << std::endl; }
    // column identifiers
    {for (size_t i = 0; i <= longest; i++) {
        if (i == 0) {
            // 1st cell = blank
            std::cout << "| ";
            std::cout << std::setfill(' ') << std::setw(usedconfig.width) << " ";
            std::cout << " |";
        }
        else {
            std::cout << " ";
            if (usedconfig.align == "left") std::cout << std::left;
            std::cout << std::setw(usedconfig.width) << (char)('A' + i - 1) << " |";
        }
    }
    std::cout << std::endl; }

    for (std::size_t i = 0; i < values.size(); i++){
        //cells with numbers
        std::size_t j = 0;
        // line separator
        {for (std::size_t i = 0; i <= longest; i++)
            {
            if (i==0) std::cout << "+";
            std::cout << std::right << std::setw(usedconfig.width +3) << std::setfill('-') << '+';
            }
        std::cout << std::endl; }
        // columns on this line
        for (; j <= values[i].size(); j++){
            if (j == 0) { 
                if (usedconfig.align == "left") std::cout << std::left;
                std::cout << "| " << std::setw(usedconfig.width) << std::setfill(' ') << i+1 << " |";
            }
            else {
                std::cout << ' ';
                std::cout << std::setw(usedconfig.width) << values[i][j-1] << " |";
            }
        }
        // print the blanks
            for (; j <= longest; j++)
            {
                std::cout << ' ';
                std::cout << std::setw(usedconfig.width) << " ";
                std::cout << " |";
            }
        std::cout << std::endl;
    }
    // line separator
    {for (std::size_t i = 0; i <= longest; i++)
        {
        if (i==0) std::cout << "+";
        std::cout << std::right << std::setw(usedconfig.width +3) << std::setfill('-') << '+';
        }
    std::cout << std::endl; }

    return 0;
}
