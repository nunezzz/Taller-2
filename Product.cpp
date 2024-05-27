# include <iostream>
# include "Product.h"
using namespace std;

Product :: Product (string name,string id, int price, int stock)
{
    this -> name = name;
    this -> id = id;
    this -> price = price;
    this -> stock = stock;
}
string Product :: getName(){return this -> name;}
void Product :: setName(string name){this -> name = name;}
string Product :: getId(){return this-> id;}
void Product :: setId(string id){this -> id = id;}
int Product :: getPrice(){return this -> price;}
void Product ::setPrice(int price) { this->price = price;}
int Product :: getStock(){return this -> stock;}
void Product ::setStock(int stock) { this->stock = stock;}