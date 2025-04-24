#ifndef PROPERTY
#define PROPERTY

#include <string>
using namespace std;

enum class PropertyType {
    Apartment,
    Villa,
    Studio,
    Townhouse,
    Land,
    Unknown
};

string propertyTypeToString(PropertyType);

class PropertyLocation {
private:
    static const char* governorates[3], * cities[9], * streets[27];
public:
    static const char* getCities(int);
    static const char* getStreets(int);
    const char* governorate, * city, * street;
    PropertyLocation(int, const char*, const char*);
};

std::ostream& operator<<(std::ostream&, PropertyLocation&);

class PropertyListing {
private:
    static int maxPropertyID;
    int propertyID;
    double price, size;
    PropertyType propertyType;
    PropertyLocation location;
    string name, features, userHandle;
public:
    PropertyListing(string& name, PropertyLocation location, double price, double size, PropertyType type, string& features, string userHandle);
    void displayInfo();
    bool matchesFilters(string& loc, double minPrice, double maxPrice, PropertyType type);
    int getPropertyID();
    string getName();
    PropertyLocation getLocation();
    double getPrice();
    double getSize();
    PropertyType getType();
    string getFeatures();
    string getUserHandle();
    void setName(string& newName);
    void setLocation(PropertyLocation newLocation);
    void setPrice(double newPrice);
    void setSize(double newSize);
    void setType(PropertyType newType);
    void setFeatures(string& newFeatures);
};

#endif
