#include <string>
#include <vector>
#include "Subcategory.h"

class Category {
public:
    string name;
    vector<Subcategory> subcategories;

    Category() = default; // Constructor predeterminado

    Category(const std::string& cate_name);
};