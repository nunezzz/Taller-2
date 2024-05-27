#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Category.h"
#include <algorithm> 
#include <cctype>
#include "Pharmacy.h"
#include "Product.h"
using namespace std;

/*
Función que lee el archivo y carga las categorías, subcategorías y productos a un vector de categorías.
*/
vector<Category> readText(const std:: string& filename) 
{
    vector<Category> bodega;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return bodega;
    }
    string line;
    Category currentCategory;
    Subcategory currentSubcategory;

    while (getline(file, line)) 
    {
        if (line.empty()) continue; // Ignorar líneas vacías

        if (line.find("#") != string::npos) { // lee las lineas que son Categoría
            
            if(!currentSubcategory.name.empty())
            {
                currentCategory.subcategories.push_back(currentSubcategory);
                currentSubcategory = Subcategory();
            }
            
            if (!currentCategory.name.empty()) 
            {
                bodega.push_back(currentCategory);
            }
            currentCategory = Category(); // Reinicia la categoría actual
            currentCategory.name = line.substr(0, line.find("#"));
            currentCategory.subcategories.clear();
            //cout<<"cate: "<<currentCategory.name<<endl; // verifica si se esta leyendo bien el txt
        } 
        else if (line.find("@") != string::npos) { // lee las lineas que son Subcategoría
            if (!currentSubcategory.name.empty()) {
                currentCategory.subcategories.push_back(currentSubcategory);
            }
            currentSubcategory = Subcategory(); // Reinicia la subcategoría actual
            currentSubcategory.name = line.substr(0,line.find("@")); 
            currentSubcategory.products.clear();
            //cout<<"sub: "<<currentSubcategory.name<<endl; // verifica si se esta leyendo bien el txt
        } else if (line.find("&") != string::npos) { // lee las lineas que son Producto
            istringstream iss(line);
            Product product;
            getline(iss, product.name, '-');
            getline(iss, product.id, '-');

            // Leer el precio como un entero
            string priceString;
            getline(iss, priceString, '-');
            product.price = stoi(priceString); 

            // Leer el stock como un entero
            string stockString;
            getline(iss, stockString, '-');
            product.stock = stoi(stockString);

            currentSubcategory.products.push_back(product);
            //cout<<"productos: "<<product.name<<endl; // verifica si se esta leyendo bien el txt
        }
    }
     if (!currentSubcategory.name.empty()) {
        currentCategory.subcategories.push_back(currentSubcategory);
    }
    if (!currentCategory.name.empty()) {
        bodega.push_back(currentCategory);
    }
    file.close();
    return bodega;
}

// Función para transformar una cadena a minúsculas y eliminar espacios adicionales
string toLowerTrim(const string& str) 
{
    string result;
    transform(str.begin(), str.end(), back_inserter(result), ::tolower);
    result.erase(remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}
// Función para buscar un producto por nombre en la bodega 
Product* findProduct(vector<Category>& bodega,string& productName) {
    string searchName = toLowerTrim(productName);
    for (Category& category : bodega) {
        for (Subcategory& subcategory : category.subcategories) {
            for (Product& product : subcategory.products) {
                if (toLowerTrim(product.name) == searchName) {
                    return &product;
                }
            }
        }
    }
    return nullptr;
}

// Función para sobreescribir los datos de un archivo 
void replaceText(vector<Category>& bodega)
{
    ofstream file ("bodega.txt");
    if(!file.is_open())
    {
        cout << "Error en el sistema" << endl;
    }

    for(const Category& category : bodega)
    {
        file << category.name << "#" << endl;
        for(const Subcategory& sub : category.subcategories)
        {
            file<< sub.name<<"@"<<endl;
            for(const Product& pro : sub.products)
            {
                file<<pro.name<< "-" << pro.id << "-" << pro.price << "-" << pro.stock << "&" <<endl; 
            }
        }   
    }
    file.close();
}

// Función para agregar un nuevo cliente y reistrar sus compras 
void addClient(vector<Category>bodega,Pharmacy& farmacia)
{
    string nombreCliente, tipoCliente;
    cout << "Ingrese el nombre del cliente: ";
    getline(cin, nombreCliente);
    cout << "Ingrese el tipo de cliente (NORMAL, TERCERA_EDAD, DISCAPACITADO, EMBARAZADA): ";
    getline(cin, tipoCliente);
    transform(tipoCliente.begin(), tipoCliente.end(), tipoCliente.begin(), :: toupper); //Transforma todo a la mayusculas 

    Client nuevoCliente(tipoCliente, nombreCliente, farmacia.getNextNumAtencion());
            
             
    while (true) {
        string nombreProducto;
        int cantidad; 
        cout << "Ingrese el nombre del producto que desea comprar (o 'fin' para terminar): ";
        getline(cin, nombreProducto);
        if (nombreProducto == "fin")
        {
        break;
        }
        Product* producto = findProduct(bodega, nombreProducto);
        if (producto == nullptr) 
        {
            cout << "Producto no encontrado." << endl;
            continue;
        }
        cout << "Ingrese la cantidad del producto: ";
        cin >> cantidad;
        cin.ignore(); // Ignorar el salto de línea anterior
            
        if (producto->stock < cantidad) 
        {
            cout << "No hay suficiente stock del producto. Stock disponible: " << producto->stock << endl;
        } else 
        {
            producto->stock -= cantidad;
            Product productoComprado = *producto;
            productoComprado.stock = cantidad; // Usar 'stock' para representar la cantidad comprada
            nuevoCliente.addProduct(productoComprado);        
            //replaceText(bodega,cantidad); // REVISAR SI FUNCIONA CON ESTO DENTRO DEL WHILE 
        }
    }
    farmacia.agregarCliente(nuevoCliente);
    replaceText(bodega); 
}
// Función para atender a un cliente y generar la boleta de compra.
void serveClient(vector<Category>& bodega, Pharmacy& farmacia)
{
    Client cliente = farmacia.atenderCliente();
    if (!cliente.getType().empty()) {
        int total = 0;
        cout<<"___________________________________________"<< endl;
        cout<<"          ** FARMACIA PUPUSITA **"<< endl;
        cout<<"                 - Boleta -"<<endl;
        cout<<"___________________________________________"<< endl;
        cout<<"Nombre comprador: " << cliente.getName() << endl; // Cambiado a getName()
        cout<<"-------------------------------------------"<< endl;
        cout<<"Cant.       Producto              Valor"<< endl;
        cout<<"-------------------------------------------"<< endl;
        for (const auto& producto : cliente.shoppingList) 
        {
            int subtotal = producto.price * producto.stock;
            cout<<producto.stock<<"        "<< producto.name <<"           $"<< subtotal <<endl;
            //cout << producto.name << " - " << producto.stock << " unidades a $" << producto.price << " cada uno. Subtotal: $" << subtotal << "\n";
            total += subtotal; 
        }
        cout<<"-------------------------------------------"<< endl;
        cout<<"TOTAL: $"<< total<<endl;
        cout<<"-------------------------------------------"<< endl;
    }
}   
 
// Función para buscar un producto por su ID en la bodega 
Product* findId(vector<Category>& bodega,string& idProduct) {
    string searchId= toLowerTrim(idProduct);
    for (Category& category : bodega) {
        for (Subcategory& subcategory : category.subcategories) {
            for (Product& product : subcategory.products) {
                if (toLowerTrim(product.id) == searchId) {
                    return &product;
                }
            }
        }
    }
    return nullptr;
}         
// Función para restablecer el stock de un producto ya existente en la bodega 
void reStock(vector<Category>bodega, Pharmacy& farmacia)
{
    string idProduct;
    int newStock;
    cout<<"Ingrese le ID del producto: "<<endl;
    getline(cin, idProduct);
    cout<<"Ingrese el stock nuevo: "<<endl;
    cin>>newStock;

    Product* product = findId(bodega,idProduct);
    if(product == nullptr)
    {
        cout<<"El producto no existe"<<endl;
    }
    else
    {
        product->stock += newStock;
        replaceText(bodega);
        cout<< "Se ha restablecido el stock del producto: "<< product->name <<endl;

    }
}
// Función para buscar una subcategoría en la bodega 
Subcategory* findSubcategory(vector<Category>& bodega,string& subCategory) {
    string searnamesub = toLowerTrim(subCategory);
    for (Category& category : bodega) {
        for (Subcategory& subcategory : category.subcategories) {
            if (toLowerTrim(subcategory.name) == searnamesub) 
            {
               return &subcategory;
            }   
        }
    }
    return nullptr;
} 
// Función para agregar un nuevo producto a una subcategoría 
void enterNewProduct(vector<Category>& bodega, Pharmacy& farmacia)
{
    string subCategory;
    string nameNewProduct;
    string idNewProduct;
    int priceNewProduct;
    int stockNewProduct;

    cout<<"Ingrese la subcategoria del producto nuevo: "<< endl;
    getline(cin,subCategory);
    Subcategory* subnewProduct = findSubcategory(bodega,subCategory);

    if(subnewProduct == nullptr)
    {
        cout<<"La subcategoria indicada no existe"<<endl;
    }
    else
    {
        cout<<"Ingrese el nombre del producto nuevo: "<<endl;
        getline(cin,nameNewProduct);
        cout<<"Ingrese el ID del producto nuevo: "<<endl;
        getline(cin,idNewProduct);
        cout<<"Ingrese el precio del producto nuevo: "<<endl;
        cin>>priceNewProduct;
        cout<<"Ingrese el stock del produto nuevo: "<<endl;
        cin>>stockNewProduct;
        Product newProduct;
        newProduct.name = nameNewProduct;
        newProduct.id = idNewProduct;
        newProduct.price = priceNewProduct;
        newProduct.stock = stockNewProduct;

        subnewProduct -> products.push_back(newProduct);
        replaceText(bodega);
        cout<<"** El producto se ha agregado con exito **"<<endl;
    }
}
// Interfaz de usuario que realiza distintas operaciones 
void userInterface(vector<Category>& bodega, Pharmacy& farmacia) 
{
    
    int opcion;
    cout << "-- ¿Qué desea realizar? -- \n";
    cout << "1. Agregar cliente.\n";
    cout << "2. Atender cliente.\n";
    cout << "3. Restock Productos.\n";
    cout << "4. Ingresar nuevo Prodcuto.\n";
    cout << "5. Salir.\n";

    cout << "Seleccione opcion: "; 
    cin >> opcion;
    cin.ignore(); // Ignorar el salto de línea anterior
    
    while(opcion != 5)
    {
        switch(opcion)
        {
            case 1:
            addClient(bodega,farmacia);
            break;
            case 2:
            serveClient(bodega,farmacia);
            break;
            case 3:
            reStock(bodega, farmacia);
            break;
            case 4:
            enterNewProduct(bodega,farmacia);
            break;
        }
        cout<<""<<endl;
        cout << "-- ¿Qué desea realizar? -- \n";
        cout << "1. Agregar cliente.\n";
        cout << "2. Atender cliente.\n";
        cout << "3. restock productos.\n";
        cout << "4. Ingresar Prodcutos.\n";
        cout << "5. Salir.\n";

        cout << "Seleccione opcion: ";
        cin >> opcion;
        cin.ignore();
    }
}
    
    
int main() 
{
    cout<<""<<endl;
    cout << "** BIENVENIDO A LA FARMACIA PUPUSITAS ** \n";
    cout<< ""<< endl;
    vector<Category> bodega = readText("bodega.txt");

    // Crear instancia de Pharmacy
    Pharmacy farmacia;

    // Interfaz de usuario
    userInterface(bodega, farmacia);
    
    
    return 0;
}