#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include "main.hpp"

Database::Database(){
	// Database constructor
}

Item::~Item() {}
void Item::print() {}
bool Item::search(std::string & tofind) {return 0;}
Book::~Book() {}
Journal::~Journal() {}

int id = 1;

// add item to the database
void Database::add(Item * a) {
	db.push_back(a);
	a->ID = id++;
}

void throwerr (std::string & command) {
	std::vector<std::string> M = {"list", "remove", "erase", "find", "sort"};
	for (auto cmd : M) {
		if (command == cmd) {
			// missing argument
			std::cout << '+' << std::string(58, '-') << "+\n";
			std::cout << "| Command "<<'"'<<command<<'"'<<" expects some argument"<<std::string(25-command.size(), ' ')<<"|\n";
			std::cout << '+' << std::string(58, '-') << "+\n";
			// zakoncit returnem
			return;
		}
	}
	// command does not exist
	std::cout << '+' << std::string(58, '-') << "+\n";
	std::cout << "| Unknown command "<<'"'<<command<< '"'<<std::string(39-command.size(), ' ')<<"|\n";
	std::cout << '+' << std::string(58, '-') << '+'<<std::endl;
}
void throwerr_id (std::string & argument) {
	std::cout << '+' << std::string(58, '-') << "+\n";
	std::cout << "| ID = " << argument << " is not in the database" << std::string(29-argument.size(), ' ') << "|\n";
	std::cout << '+' << std::string(58, '-') << "+\n";
}

void Database::sort(std::string & crit) {
	// uses default order = asc, bc user didnt specify
	// decides to which funct to delegate the task based on the received criteria, or calls error
	if (crit == "id") ;
	else if ((crit == "year") || (crit == "name")) sort_str(crit, 1);
	else {
		// unknown criterion
		std::cout << '+' << std::string(58, '-') << "+\n";
		std::cout << "| Unknown sorting order" << std::string(36,' ')<<"|\n";
		std::cout << '+' << std::string(58, '-') << "+\n";
	}
}
void Database::sort(std::string & crit, std::string & ord) {
	// uses order given by the user
	// decides to which fnct to delegate the task based on received criteria, or calls error
	if ((crit == "id") && (ord == "desc")) sort_id();
	else if ((crit == "year") || (crit == "name")) {
		// call sort_str and before decide on the order
		if (ord == "asc") sort_str(crit, 1);
		else sort_str(crit, 0);
	}
	else {
		// unknown criterion
		std::cout << '+' << std::string(58, '-') << "+\n";
		std::cout << "| Unknown sorting order2" << std::string(35,' ')<<"|\n";
		std::cout << '+' << std::string(58, '-') << "+\n";
	}
}

void Database::sort_id() {
	// descending
	Item* tmpI;
	for (size_t i = 0; i < (db.size()-1); i++) {
		for (size_t j = 0; j < (db.size()-1-i); j++) {
			if (db[j]->getid() < db[j+1]->getid()) {
				tmpI = db[j];
				db[j] = db[j+1];
				db[j+1] = tmpI;
			}
		}
	}
}
void Database::sort_str(std::string & crit, bool is_asc = 1) {
	
	bool is_desc = !is_asc;
	bool is_name = 0;
	bool is_year = 0;
	if (crit == "name") is_name = 1;
	is_year = !is_name;
	Item* tmpI;
	if (is_name && is_asc) {
		for (size_t i = 0; i < (db.size()-1); i++) {
			for (size_t j = 0; j < (db.size()-1-i); j++) {
				if (db[j]->get_name() > db[j+1]->get_name()) {
					tmpI = db[j];
					db[j] = db[j+1];
					db[j+1] = tmpI;
				}
				/*
				else if ((db[j]->get_name() == db[j+1]->get_name()) && db[j]->getid() > db[j+1]->getid() ){
					tmpI = db[j];
					db[j] = db[j+1];
					db[j+1] = tmpI;
				}
				*/
			}
		}
	}
	else if (is_name && is_desc) {
		for (size_t i = 0; i < (db.size()-1); i++) {
			for (size_t j = 0; j < (db.size()-1-i); j++) {
				if (db[j]->get_name() < db[j+1]->get_name()) {
					tmpI = db[j];
					db[j] = db[j+1];
					db[j+1] = tmpI;
				}
				/*
				else if ((db[j]->get_name() == db[j+1]->get_name()) && db[j]->getid() < db[j+1]->getid() ){
					tmpI = db[j];
					db[j] = db[j+1];
					db[j+1] = tmpI;
				}
				*/
			}
		}
	}
	// then criterion must be year
	else if (is_year && is_asc) {
		for (size_t i = 0; i < (db.size()-1); i++) {
			for (size_t j = 0; j < (db.size()-1-i); j++) {
				if (db[j]->get_year() > db[j+1]->get_year()) {
					tmpI = db[j];
					db[j] = db[j+1];
					db[j+1] = tmpI;
				}
				/*
				else if ((db[j]->get_year() == db[j+1]->get_year()) && db[j]->getid() > db[j+1]->getid() ){
					tmpI = db[j];
					db[j] = db[j+1];
					db[j+1] = tmpI;
				}
				*/
			}
		}
	}
	else if (is_year && is_desc) {
		for (size_t i = 0; i < (db.size()-1); i++) {
			for (size_t j = 0; j < (db.size()-1-i); j++) {
				if (db[j]->get_year() < db[j+1]->get_year()) {
					tmpI = db[j];
					db[j] = db[j+1];
					db[j+1] = tmpI;
				}
				/*
				else if ((db[j]->get_year() == db[j+1]->get_year()) && db[j]->getid() < db[j+1]->getid() ){
					tmpI = db[j];
					db[j] = db[j+1];
					db[j+1] = tmpI;
				}
				*/
			}
		}
	}
}
// definition of the sort function according to name
/*
bool sort_name::operator()(const Item *a, const Item * b){

}
*/

int Item::getid() {
	return this->ID;
}
std::string Item::get_name() {
	return this->name;
}
std::string Item::get_year() {
	return this->year;
}


// definition of fnct void vypis for both classes Book & Journal
void Book::print() {
	std::cout << "| "<<std::setw(2)<<this->ID<<" | "<<this->name<<std::string(52-this->name.size(), ' ')<<"|\n";
	std::cout << "|    | "<<this->year<<", "<<this->author<<std::string(52-(this->year.size()+this->author.size()+2), ' ')<<"|\n";
	std::cout << "+----+" << std::string(53,'-') << '+' << std::endl;
}
void Journal::print() {
	std::cout << "| "<<std::setw(2)<<this->ID<<" | "<<this->name<<std::string(52-this->name.size(), ' ')<<"|\n";
	std::cout << "|    | "<<this->year<<", "<<this->volume<<'('<<this->issue<<')'<<std::string(52-(4+this->year.size()+this->issue.size()+this->volume.size()), ' ')<<"|\n";
	std::cout << "+----+" << std::string(53,'-') << '+' << std::endl;
}

// definition of fnct bool search for both he classes, Book & Journal
bool Book::search(std::string & tofind) {
	size_t position;
	// i will check just name and author, other attributes make little sense
	if (((position = this->name.find(tofind)) == std::string::npos) && (position = this->author.find(tofind)) == std::string::npos) return 0;
	else return 1;
}
bool Journal::search(std::string & tofind) {
	size_t position;
	// i will check just name and, other attributes make little sense, but can be easilly added for check if needed
	if ((position = this->name.find(tofind)) == std::string::npos)  return 0;
	else return 1;
}

// definition of fnct search_id that gives 1 if the objcets id is the one given as arg
bool Item::search_id(std::string & theid) {
	int given_id = std::stoi(theid);
	if (this->ID == given_id) return 1;
	else return 0;
}


void Database::list() {
	// header
	std::cout << '+' << std::string(58,'-') << "+\n";
	std::cout << "| List of all records" << std::string(38, ' ') << "|\n";
	std::cout << "+----+" << std::string(53,'-') << '+' << std::endl;
	// body
	for (auto i : db) {
		i->print();
	}
	// tail
	std::cout << "| Total: " << db.size() << std::string(50-(std::to_string(db.size()).size()), ' ') << '|' << std::endl;
	std::cout << '+' << std::string(58,'-') << "+\n";
}

void Database::find(std::string & tofind) {
	int tot = 0; // how many relevant records are there
	// header
	std::cout << '+'<<std::string(58,'-')<<"+\n";
	std::cout << "| Search for "<<'"'<<tofind<<'"'<<std::string(44-tofind.size(),' ')<<"|\n";
	std::cout << "+----+" << std::string(53,'-') << '+' << std::endl;
	// body
	for (auto i : db) {
		// if any attribute of i has substr tofind, print the item
		if (i->search(tofind)) { 
			i->print();
			tot++;
		}
	}
	// tail
	std::cout << "| Total: " << tot << std::string(50-(std::to_string(tot).size()), ' ') << '|' << std::endl;
	std::cout << '+' << std::string(58,'-') << '+' << std::endl;
}

void Database::erase(std::string & toerase) {
	for (size_t i = 0; i < db.size(); i++) {
		// if any attribute of i has substr tofind, print the item
		if (db[i]->search(toerase)) { 
			// erase
			delete db[i];
			db.erase(db.begin()+i);
			i--;
		}
	}
}

void Database::remove(std::string & theid) {
	bool found = 0;
	for (size_t i = 0; i < db.size(); i++) {
		// if this Items ID matches the one given, remove it
		
		if (db[i]->search_id(theid)) {
			// remove
			found = 1;
			delete db[i];
			db.erase(db.begin()+i);
			break;
		}
	}
	if (!found) throwerr_id(theid);
}

Database::~Database(){
	for (auto i = db.begin(); i != db.end(); i++)
	{
		delete *i;
	}
	db.clear();
}


int main() {
	Database db;
	db.add(new Journal("IEEE Transaction on Computers", "C-35", "10", "1986"));
	db.add(new Journal("IEEE Transaction on Computers", "C-35", "11", "1986"));
	db.add(new Journal("IEEE Transactions on Communications", "28", "8", "1980"));
	db.add(new Book("Dva roky prazdnin", "Jules Verne", "1888"));
	db.add(new Book("Tajuplny ostrov", "Jules Verne", "1874"));
	db.add(new Book("Ocelove mesto", "Jules Verne", "1879"));
	std::string a;
	while (std::getline(std::cin, a)) {
		size_t position;
		if ((position = a.find(":")) == std::string::npos) {
			if (a == "list") {
				db.list();
			}
			else if (a == "find") throwerr(a);
			else if (a == "erase") throwerr(a);
			else if (a == "remove") throwerr(a);
			else if (a == "sort") throwerr(a);
			else
				throwerr(a);
		}
		else {
			std::string command = a.substr(0, position);
			std::string argument = a.substr(position+1);
			
			if (command == "find")
				db.find (argument);
			else if (command == "erase")
				db.erase(argument);
			else if (command == "remove")
				db.remove(argument);
			else if (command == "sort") {
				size_t position2;
				if ((position2 = argument.find(":")) == std::string::npos) db.sort(argument);
				else {
				std::string criterion = argument.substr(0, position2);
				std::string order = argument.substr(position2+1);
				// std::cout << "argument =" << argument << " criterion =" <<criterion << " " << "order =" << order << "\n";
				db.sort(criterion, order);
				}
			}
		}
	}
	return 0;
}
