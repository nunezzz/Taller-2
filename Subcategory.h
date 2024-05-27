#include <string>
#include <vector>
#include "Product.h"

class Subcategory 
{
public:
    string name;
    vector<Product> products;

    Subcategory() = default;
    Subcategory(const std::string& subcate_name);
};