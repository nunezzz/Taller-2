# pragma once 
# include <iostream>

using namespace std;

class Product
{
    public:
        string name; 
        string id;
        int price;
        int stock;
    public:
    Product() = default;
    Product (string, string, int, int);
        string getName();
        void setName(string);
        string getId();
        void setId(string);
        int getPrice();
        void setPrice(int);
        int getStock();
        void setStock(int);
    
};