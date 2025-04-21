
#include <string>
#include <vector>

using namespace std;

enum class PropertyType {
    Apartment,
    Villa,
    Studio,
    Townhouse,
    Land,
    Unknown
};

string propertyTypeToString(PropertyType type);

class Property {
private:
    int propertyID;
    string name;
    string location;
    double price;
    double size;
    PropertyType propertyType;
    vector<string> features;
    int sellerID;

public:
    Property(int id, string& name, string& location, double price,
        double size, PropertyType type, vector<string>& features, int sellerID);

    void displayInfo(); 

    bool matchesFilters(string& loc, double minPrice, double maxPrice, PropertyType type); 

    // Getters
    PropertyType getType();
    int getSellerID();
    double getPrice();
    double getSize();
    void getfeatures();
};



