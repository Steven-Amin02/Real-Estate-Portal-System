#include<iostream>
#include"SearchAndCompare.h"
#include"Property.h"
#include <unordered_set>
#include<algorithm>
#include<iomanip>

using namespace std;

vector<PropertyListing> searchAndCompare::searchByType(unordered_map<int, PropertyListing>& properties, string type) {

    vector<PropertyListing> result;

    for (auto pair : properties) {
        PropertyListing& property = pair.second;
        if (propertyTypeToString(property.getType()) == type) {
            result.push_back(property);
        }
    }
    return result;
}

vector<PropertyListing> searchAndCompare::searchBypriceRange(unordered_map<int, PropertyListing>& properties, double minPrice, double maxPrice) {

    vector<PropertyListing> result;

    for (auto pair : properties) {
        PropertyListing& property = pair.second;
        if (property.getPrice() >= minPrice &&  property.getPrice() <= maxPrice) {
            result.push_back(property);
        }
    }
    return result;
}


vector<PropertyListing> searchAndCompare::searchByAreaRange(unordered_map<int, PropertyListing>& properties, double minArea, double maxArea) {

    vector<PropertyListing> result;

    for (auto pair : properties) {
        PropertyListing& property = pair.second;
        if (property.getPrice() >= minArea && property.getPrice() <= maxArea) {
            result.push_back(property);
        }
    }
    return result;
}

vector<PropertyListing> searchAndCompare::searchByBedrooms(unordered_map<int, PropertyListing>& properties, int bedrooms) {
    vector<PropertyListing> result;
    for (auto pair : properties) {
        PropertyListing& property = pair.second;
        if (property.getBedrooms() == bedrooms) {
            result.push_back(property);
        }
    }
    return result;
}

vector<PropertyListing> searchAndCompare::searchByLocation(unordered_map<int, PropertyListing>& properties, string governorate, string city) {
    vector<PropertyListing> result;
    for (auto pair : properties) {
        PropertyListing& property = pair.second;
        if (property.getLocation().governorate == governorate && property.getLocation().city == city) {
            result.push_back(property);
        }
    }
    return result;
}


//vector<PropertyListing> searchAndCompare::searchByLocation(unordered_map<int, PropertyListing>& properties, string government, string city)
//{
//    vector<PropertyListing> result;
//
//    for (auto pair : properties) {
//        PropertyListing& property = pair.second;
//        if (strcmp(property.getLocation().city, city.c_str()) == 0) {                //chat GPT مش فاهم ده ايه
//            result.push_back(property);
//        }
//    }
//    return result;
//}

std::unordered_set<PropertyListing, PropertyHash, PropertyEqual> searchAndCompare::searchMerge(
    std::vector<PropertyListing> type,
    std::vector<PropertyListing> price,
    std::vector<PropertyListing> area,
    std::vector<PropertyListing> location
) {
    std::unordered_set<PropertyListing, PropertyHash, PropertyEqual> search;
    for (const PropertyListing& p : type)
        search.insert(p);
    for (const PropertyListing& p : price)
        search.insert(p);
    for (const PropertyListing& p : area)
        search.insert(p);
    for (const PropertyListing& p : location)
        search.insert(p);
    return search;
}


void searchAndCompare::searchMenu() {
    unordered_map<int, PropertyListing> allProperties;

    vector<PropertyListing> typeResults;
    vector<PropertyListing> priceResults;
    vector<PropertyListing> areaResults;
    vector<PropertyListing> locationResults;
    vector<PropertyListing> bedroomsResult;

    bool anotherFilter = true;

    while (anotherFilter) {
        cout << "Choose a filter option:" << endl;
        cout << "[1] Search by Property Type" << endl;;
        cout << "[2] Search by Location" << endl;;
        cout << "[3] Search by Price Range" << endl;;
        cout << "[4] Search by Area Range" << endl;
        cout << "[5] Search by Area Range" << endl;
        cout << "[6] Finish Search" << endl;;

        int choice1;
        cin >> choice1;

        switch (choice1) {
        case 1: {
            string type;
            cout << "Enter property type: ";
            cin >> type;
            typeResults = searchByType(allProperties, type);
            break;
        }
        case 2: {
            string gov, city;
            cout << "Enter government: ";
            cin >> gov;
            cout << "Enter city: ";
            cin >> city;
            locationResults = searchByLocation(allProperties, gov, city);
            break;
        }
        case 3: {
            double minPrice, maxPrice;
            cout << "Enter minimum price: ";
            cin >> minPrice;
            cout << "Enter maximum price: ";
            cin >> maxPrice;
            priceResults = searchBypriceRange(allProperties, minPrice, maxPrice);
            break;
        }
        case 4: {
            double minArea, maxArea;
            cout << "Enter minimum area: ";
            cin >> minArea;
            cout << "Enter maximum area: ";
            cin >> maxArea;
            areaResults = searchByAreaRange(allProperties, minArea, maxArea);
            break;
        }
        case 5: {
            int bedrooms;
            cout << "Enter minimum price: ";
            cin >> bedrooms;
            bedroomsResult = searchByBedrooms(allProperties , bedrooms);
            break;
        }
        case 6:
            anotherFilter = false;
            break;
        default:
            cout << "Invalid choice. Try again.\n";
            continue;
        }

        if (anotherFilter) {
            cout << "nDo you want to apply another filter ?" << endl;
            cout << "[1] Yes" << endl;
            cout << "[2] No, show results" << endl;
            int choice2;
            cin >> choice2;
            if (choice2 == 2) {
                anotherFilter = false;
            }
        }
    }

    unordered_set<PropertyListing, PropertyHash, PropertyEqual> finalResults = searchMerge(typeResults, priceResults, areaResults, locationResults);

    if (finalResults.empty()) {
        cout << "No matching properties found" << endl;
    }
    else {
        cout << "Matching Properties " << endl;;
        for (auto property : finalResults) {
            property.displayInfo();
        }
    }
}


bool searchAndCompare::comparePriceAsc(const PropertyListing& a, const PropertyListing& b) {
    return a.getPrice() < b.getPrice();
}

vector<PropertyListing> searchAndCompare::sortByPriceAsc(unordered_map<int, PropertyListing>& properties) {
    vector<PropertyListing> sorted;
    for (const auto& pair : properties) {
        sorted.push_back(pair.second);
    }
    sort(sorted.begin(), sorted.end(), &searchAndCompare::comparePriceAsc);
    return sorted;
}

bool searchAndCompare::comparePriceDesc(const PropertyListing& a, const PropertyListing& b) {
    return a.getPrice() > b.getPrice();
}

vector<PropertyListing> searchAndCompare::sortByPriceDesc(unordered_map<int, PropertyListing>& properties) {
    vector<PropertyListing> sorted;
    for (const auto& pair : properties) {
        sorted.push_back(pair.second);
    }
        sort(sorted.begin(), sorted.end(), &searchAndCompare::comparePriceDesc);
        return sorted;
}

bool searchAndCompare::compareSizeAsc(const PropertyListing& a, const PropertyListing& b) {
    return a.getSize() < b.getSize();
}

vector<PropertyListing> searchAndCompare::sortBySizeAsc(unordered_map<int, PropertyListing>& properties) {
    vector<PropertyListing> sorted;
    for (const auto& pair : properties) {
        sorted.push_back(pair.second);
    }
    sort(sorted.begin(), sorted.end(), &searchAndCompare::compareSizeAsc);
    return sorted;
}

bool searchAndCompare::compareSizeDesc(const PropertyListing& a, const PropertyListing& b) {
    return a.getSize() > b.getSize();
}

vector<PropertyListing> searchAndCompare::sortBySizeDesc(unordered_map<int, PropertyListing>& properties) {
    vector<PropertyListing> sorted;
    for (const auto& pair : properties) {
        sorted.push_back(pair.second);
    }
    sort(sorted.begin(), sorted.end(), &searchAndCompare::compareSizeDesc);
    return sorted;
}

void searchAndCompare::compareProperties(unordered_map<int, PropertyListing>& properties) {
    if (properties.empty() || properties.size() > 4) {
        cout << "you can only compare 1 to 4 properties" << endl;
    }
    else {
        cout << left << setw(20) << "Key information";
        for (const auto& property : properties) {
            cout << setw(25) << "Property " + to_string(property.second.getPropertyID());
        }
        cout << endl;
        cout << string(100, '-') << endl;

        cout << setw(20) << "Name";
        for (const auto& p : properties) {
            cout << setw(25) << p.second.getName();
        }
        cout << endl;

        cout << setw(20) << "Type";
        for (const auto& p : properties) {
            cout << setw(25) << propertyTypeToString(p.second.getType());
        }
        cout << endl;

        cout << setw(20) << "Price";
        for (const auto& p : properties) {
            cout << setw(25) << p.second.getPrice();
        }
        cout << endl;

        cout << setw(20) << "Size";
        for (const auto& p : properties) {
            cout << setw(25) << p.second.getSize();
        }
        cout << endl;

        cout << setw(20) << "Governorate";
        for (const auto& p : properties) {
            cout << setw(25) << p.second.getLocation().governorate;
        }
        cout << endl;

        cout << setw(20) << "City";
        for (const auto& p : properties) {
            cout << setw(25) << p.second.getLocation().city;
        }
        cout << endl;

        cout << setw(20) << "City";
        for (const auto& p : properties) {
            cout << setw(25) << p.second.getLocation().street;
        }
        cout << endl;
    }
}
