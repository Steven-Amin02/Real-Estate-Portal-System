#include "Property.h"
#include <iostream>

using namespace std;

int PropertyListing::maxPropertyID = 0;




PropertyListing::PropertyListing()
    : propertyID(++maxPropertyID), price(0), size(0), propertyType(PropertyType::Unknown), location(0, "", "") {
}


const char* PropertyLocation::getCities(int index) {
    return PropertyLocation::cities[index];
}

const char* PropertyLocation::getStreets(int index) {
    return PropertyLocation::streets[index];
}

const char* PropertyLocation::governorates[3] = {
    "Cairo",
    "Alexandria",
    "Giza"
};

const char* PropertyLocation::cities[9] = {
    "Nasr City",
    "Heliopolis",
    "Zamalek",
    "Roushdy",
    "Smouha",
    "Mandara",
    "Dokki",
    "Mohandessin",
    "Faisal"
};

const char* PropertyLocation::streets[27] = {
    "Abbas El Akkad Street",
    "Makram Ebeid Street",
    "Mostafa_El_Nahas_Street",
    "Al Mirghani Street",
    "Baghdad Street",
    "Orouba Street",
    "Brazil Street",
    "Mohamed Mazhar Street",
    "26th of July Street",
    "Mohamed Ahmed Street",
    "Syria Street",
    "El Geish Road",
    "Victor Emmanue Street",
    "Fawzi Moaz Street",
    "Transport Engineering Road",
    "Tahrir Street",
    "Mossadak Street",
    "El Batal Ahmed Abdel Aziz Street",
    "Syria Street",
    "Shehab Street",
    "Gameat El_Dowal El_Arabiya Street",
    "Faisal Street",
    "Talbeya Street",
    "Khatem El Morsaleen Street"
};


PropertyLocation::PropertyLocation(int governorateIndex, const char* city, const char* street)
    : governorate(this->governorates[governorateIndex]), city(city), street(street)
{
}

std::ostream& operator<<(std::ostream& os, PropertyLocation& location) {
    os << "Govrnorate: " << location.governorate << "\nCity: " << location.city << "\nStreet: " << location.street << '\n';
    return os;
}

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

PropertyListing::PropertyListing(string& name, PropertyLocation location, double price,
    double size, PropertyType type, string& features, string userHandle)
    : propertyID(PropertyListing::maxPropertyID++), name(name), location(location), price(price), size(size),
    propertyType(type), features(features), userHandle(userHandle) {
}
//PropertyListing::PropertyListing(){}

void PropertyListing::displayInfo() const {
    cout << "Property ID: " << propertyID << "\n"
        << "Name: " << name << "\n"
        << "Location: " << location.getCities << "\n"
        << "Price: $" << price << "\n"
        << "Size: " << size << " sqm\n"
        << "Type: " << propertyTypeToString(propertyType) << "\n"
        << "Features: " << features;
    cout << "\nUser Handle: " << userHandle << "\n";
}

//bool PropertyListing::matchesFilters(string& loc, double minPrice, double maxPrice, PropertyType type) {
//    if (!loc.empty() && location != loc)
//        return false;
//    if (price < minPrice || price > maxPrice)
//        return false;
//    if (type != PropertyType::Unknown && propertyType != type)
//        return false;
//    return true;
//}
PropertyListing PropertyListing::createProperty() {
    string name, features, userHandle;
    double price, size;
    int propertyTypeChoice, governorateChoice, cityChoice, streetChoice;

    // Input property details
    cout << "Enter Property Name: ";
    cin.ignore(); // Clear input buffer
    getline(cin, name);

    cout << "Enter Property Size (in sqm): ";
    cin >> size;

    cout << "Enter Property Price: ";
    cin >> price;

    cout << "Pick Property Type:\n";
    cout << "1. Apartment\n2. Villa\n3. Studio\n4. Townhouse\n5. Land\n6. Unknown\n";
    cin >> propertyTypeChoice;

    cout << "Enter Property Features: ";
    cin.ignore(); // Clear input buffer
    getline(cin, features);

    cout << "Enter User Handle: ";
    cin >> userHandle;

    // Input location details
    cout << "Pick Governorate:\n1. Cairo\n2. Alexandria\n3. Giza\n";
    cin >> governorateChoice;

    cout << "Pick City:\n";
    const char* cities = PropertyLocation::getCities(3 * (governorateChoice - 1));
    for (int i = 0; i < 3; ++i) {
        cout << i + 1 << ". " << cities[i] << '\n';
    }
    cin >> cityChoice;

    cout << "Pick Street:\n";
    const char* streets = PropertyLocation::getStreets(3 * (cityChoice - 1));
    for (int i = 0; i < 3; ++i) {
        cout << i + 1 << ". " << streets[i] << '\n';
    }
    cin >> streetChoice;

    // Create PropertyLocation object
    PropertyLocation location(governorateChoice - 1, &cities[cityChoice - 1], &streets[streetChoice - 1]);

    // Create PropertyListing object
    PropertyListing property(name, location, price, size, static_cast<PropertyType>(propertyTypeChoice - 1), features, userHandle);

    return property;

}
// Getters 
int PropertyListing::getPropertyID() const { return propertyID; }

string PropertyListing::getName() const { return name; }

PropertyLocation PropertyListing::getLocation() const { return location; }

double PropertyListing::getPrice() const { return price; }

double PropertyListing::getSize() const { return size; }

PropertyType PropertyListing::getType() const { return propertyType; }

int PropertyListing::getBedrooms() const {return bedrooms;}

string PropertyListing::getFeatures() { return features; }

string PropertyListing::getUserHandle() { return userHandle; }

void PropertyListing::setName(string& newName) { name = newName; }

void PropertyListing::setLocation(PropertyLocation newLocation) { location = newLocation; }

void PropertyListing::setPrice(double newPrice)  {
    if (newPrice < 0 || newPrice > 100000000) {
        throw invalid_argument("Price must be between 0 and 1,000,000,000.");
    }
    price = newPrice;
}
void PropertyListing::setSize(double newSize) { size = newSize; }

void PropertyListing::setType(PropertyType newType) { propertyType = newType; }

void PropertyListing::setBedrooms(int newBedromms) { bedrooms = newBedromms; }

void PropertyListing::setFeatures(string& newFeatures) { features = newFeatures; }
