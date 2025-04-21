#include "Property.h"
#include <iostream>

using namespace std;

string propertyTypeToString(PropertyType type) {
    switch (type) {
    case PropertyType::Apartment: return "Apartment";
    case PropertyType::Villa: return "Villa";
    case PropertyType::Studio: return "Studio";
    case PropertyType::Townhouse: return "Townhouse";
    case PropertyType::Land: return "Land";
    default: return "Unknown";
    }
}

Property::Property(int id, string& name, string& location, double price,
    double size, PropertyType type, vector<string>& features, int sellerID)
    : propertyID(id), name(name), location(location), price(price), size(size),
    propertyType(type), features(features), sellerID(sellerID) {
}

void Property::displayInfo() {
    cout << "Property ID: " << propertyID << "\n"
        << "Name: " << name << "\n"
        << "Location: " << location << "\n"
        << "Price: $" << price << "\n"
        << "Size: " << size << " sqm\n"
        << "Type: " << propertyTypeToString(propertyType) << "\n"
        << "Features: ";
    for (auto& feature : features) {
        cout << feature << ", ";
    }
    cout << "\nSeller ID: " << sellerID << "\n";
}

bool Property::matchesFilters(string& loc, double minPrice, double maxPrice, PropertyType type) {
    if (!loc.empty() && location != loc)
        return false;
    if (price < minPrice || price > maxPrice)
        return false;
    if (type != PropertyType::Unknown && propertyType != type)
        return false;
    return true;
}

// Getters 
PropertyType Property::getType() { return propertyType; }
int Property::getSellerID() { return sellerID; }
double Property::getPrice() { return price; }
double Property::getSize() { return size; }

void Property::getfeatures()
{
    
    for (auto& feature : features) {
        cout << feature << ", ";
    }
    
}



