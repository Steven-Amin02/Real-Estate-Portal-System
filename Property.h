#pragma once
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

ostream& operator<<(ostream&, PropertyLocation&);

class PropertyListing {
private:
    static int maxPropertyID;
    int propertyID;
    double price, size;
    PropertyType propertyType;
    PropertyLocation location;
    string name, features, userHandle;
    int bedrooms;
public:
    PropertyListing();
    PropertyListing(string& name, PropertyLocation location, double price, double size, PropertyType type, string& features, string userHandle);
    void displayInfo() const;
    PropertyListing createProperty();

    bool matchesFilters(string& loc, double minPrice, double maxPrice, PropertyType type);
    int getPropertyID() const ;
    string getName() const ;
    PropertyLocation getLocation() const;
    double getPrice() const;
    double getSize() const;
    PropertyType getType() const;
    int getBedrooms() const;
    string getFeatures();
    string getUserHandle();
    void setName(string& newName);
    void setLocation(PropertyLocation newLocation);
    void setPrice(double newPrice) ;
    void setSize(double newSize);
    void setType(PropertyType newType);
    void setFeatures(string& newFeatures);
    void setBedrooms(int newBedrooms);
};

#endif
