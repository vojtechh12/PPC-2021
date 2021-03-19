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
    usedconfig_t usedconfig;
    
    // setting default config parameters
    usedconfig.min = -99;
    usedconfig.max = 100;
    usedconfig.width = 3;
    usedconfig.align = "left";
    usedconfig.header = 1;
    usedconfig.stretch = 1;
    usedconfig.bonus_s = 0;
    usedconfig.bonus_h = 0;

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
        else if (config_c[i].type == "stretch"){
            usedconfig.stretch = std::stoi(config_c[i].value);
            usedconfig.bonus_s = 1;
            }
        else if (config_c[i].type == "header") {
            usedconfig.header = std::stoi(config_c[i].value);
            usedconfig.bonus_h = 1;
        }
    }
    //checking if users wishes arent tooo stupid
    if ((usedconfig.width < 0) || (usedconfig.min > usedconfig.max)) {
        std::cerr << "Invalid configuration\n";
        return 102;
        }
    
    std::vector<std::vector<int>> values;
    std::size_t longest = 0;
    std::size_t widest = 0;

    // loading numbers
    while(std::getline(std::cin, line)){
        std::stringstream ss(line);
        std::vector<int> row;
        std::string cell;
        sum_t number;
        std::size_t row_length = 0;
        std::string sum_str;
        
        while (std::getline(ss,cell, ';')) {
            
            try {   // if loaded cell is number
                number.value = std::stoi(cell);
                if (cell.length() > widest) widest = cell.length();
                if  ((number.value < usedconfig.min) || (number.value > usedconfig.max)) {
                    std::cerr << "Out of range\n";
                    return 100;
                    }
				if (((int)cell.length() > usedconfig.width) && !usedconfig.bonus_s) {
                    std::cerr << "Cell is too short\n";
                    return 103;
                    }
                row.push_back(number.value);
                row_length ++;
            }
            catch(const std::exception& e){ // if there is a text (SUM?)
                // algorothm for computing SUMt
                sum_t sum;
                sum = getSum(cell, row);
                if (sum.valid){
                    row.push_back(sum.value);
                    row_length ++;
                    // find out the wideness of a valid sum
                    sum_str = std::to_string(sum.value);
                    if (sum_str.length() > widest) widest = sum_str.length();
                } 
                // invlaid sum / invalid input
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

    // now changing width parameter if stretch=1
    if (usedconfig.bonus_s && usedconfig.stretch) usedconfig.width = (int)widest;
    
    // print out the config
    std::cout << "config.min=" << usedconfig.min << std::endl;
    std::cout << "config.max=" << usedconfig.max << std::endl;
    std::cout << "config.width=" << usedconfig.width << std::endl;
    std::cout << "config.align=" << usedconfig.align << std::endl;
    //print the bonus parametres if set
    if (usedconfig.bonus_s)
    std::cout << "config.stretch=" << usedconfig.stretch << std::endl;
    if (usedconfig.bonus_h)
    std::cout << "config.header=" << usedconfig.header << std::endl;

    std::cout << std::endl;
    
    // string for measuring the nuber of ciphers, if stretch=0
    std::string tmp;

	// print table
    // line separator
    if (usedconfig.header){
        for (std::size_t i = 0; i <= longest; i++)
        {
            if (i==0) std::cout << "+";
            std::cout << std::right << std::setw(usedconfig.width +3) << std::setfill('-') << '+';
        }
        std::cout << std::endl; }
    // column identifiers, in case header=1
    if (usedconfig.header) {
        for (size_t i = 0; i <= longest; i++) {
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
        // line identifiers and cells with numbers
        std::size_t j = 0;
        // line separator
        {for (std::size_t i = 0; i <= longest; i++)
            {
            if (i==0) std::cout << "+";
            if (i == longest && !usedconfig.header) continue;
            std::cout << std::right << std::setw(usedconfig.width +3) << std::setfill('-') << '+';
            }
        std::cout << std::endl; }
        // columns on this line
        for (; j <= values[i].size(); j++){
            if (j == 0 && usedconfig.header) { 
                // line identifier
                if (usedconfig.align == "left") std::cout << std::left;
                std::cout << "| " << std::setw(usedconfig.width) << std::setfill(' ') << i+1 << " |";
            }
            else {
                if (j == 0) {
                    std::cout << '|';
                    if (usedconfig.align == "left") std::cout << std::left;
                    j++;
                    }
                // from second column on, prints out the numbers
                std::cout << ' ';
                // if cell is wide enough or stretch=1
                if (usedconfig.stretch)
                std::cout << std::setw(usedconfig.width) << std::setfill(' ')<< values[i][j-1] << " |";
                // if this cell is too short && sretch=0, print the #'s
                else {
                    tmp = std::to_string(values[i][j-1]);
                    if ((int)tmp.length() > usedconfig.width) {
                    for (int i = 0; i < usedconfig.width; i++) {
                        std::cout << '#'; }
                        std::cout << " |";
                        }
                    else { 
                        std::cout << std::setw(usedconfig.width) << std::setfill(' ') << values[i][j-1] << " |"; }
                }
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
        if (i == longest && !usedconfig.header) continue;
        std::cout << std::right << std::setw(usedconfig.width +3) << std::setfill('-') << '+';
        }
    std::cout << std::endl; }

    return 0;
}
