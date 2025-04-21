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


    
    int getPropertyID();
    string getName();
    string getLocation();
    double getPrice();
    double getSize();
    PropertyType getType();
    vector<string> getFeatures();
    int getSellerID();


    
    void setName(string& newName);
    void setLocation(string& newLocation);
    void setPrice(double newPrice);
    void setSize(double newSize);
    void setType(PropertyType newType);
    void setFeatures(vector<string>& newFeatures);
    void setSellerID(int newSellerID);
};



