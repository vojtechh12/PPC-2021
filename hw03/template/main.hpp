#include <vector>
#include <string>

#ifndef __MAIN_HPP__
#define __MAIN_HPP__

struct sort_name;

class Item;

// declaration of the sort function according to name
struct sort_name {
	inline bool operator() (const Item *a, const Item * b);
};

class Database {
	private:
		std::vector<Item *> db;
	public:
		Database ();
		~Database();
		void add (Item * a);
		void list ();
		void find (std::string & tofind);
		void erase (std::string & toerase);
		void remove (std::string & theid);
		void sort (std::string & crit);
		void sort (std::string & crit, std::string & ord);
		void sort_id ();
		void sort_str(std::string & crit, bool is_asc);
};

class Item {
	protected:
		int ID;
		std::string name, year;
	public:
		Item (std::string a, std::string b) : name(a), year(b) {}
		virtual ~Item ();
		friend sort_name;
		friend void Database::add(Item * a);
		virtual void print();
		virtual bool search(std::string & tofind);
		bool search_id(std::string & theid);
		int getid();
		std::string get_name();
		std::string get_year();
};

// Book related to the Database
class Book : public Item
{
private:
	std::string author;
public:
	Book (std::string a, std::string auth, std::string b) : Item (a, b)
	{
		author = auth;
	}
	void print();
	bool search(std::string & tofind);
	~Book();

};

// Journal related to the Database
class Journal : public Item
{
private:
	std::string volume, issue;
public:
	Journal(std::string a, std::string vol, std::string iss, std::string b) : Item (a, b)
	{
		volume = vol;
		issue = iss;
	}
	void print ();
	bool search(std::string & tofind);
	~Journal();

};



#endif // __MAIN_HPP__