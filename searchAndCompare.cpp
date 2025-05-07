#include<iostream>
#include"SearchAndCompare.h"
#include"Property.h"
#include <unordered_set>

using namespace std;

vector<PropertyListing> searchAndCompare::searchByType( unordered_map<int, PropertyListing>& properties, string type) {

    vector<PropertyListing> result;

    for ( auto pair : properties) {
         PropertyListing& property = pair.second;
        if (propertyTypeToString(property.getType()) == type) {
            result.push_back(property);
        }
    }
    return result;
}

vector<PropertyListing> searchAndCompare::searchBypriceRange( unordered_map<int, PropertyListing>& properties, double minPrice, double maxPrice) {

    vector<PropertyListing> result;

    for ( auto pair : properties) {
         PropertyListing& property = pair.second;
        if (property.getPrice() >= minPrice || property.getPrice() <= maxPrice) {
            result.push_back(property);
        }
    }
    return result;
}


vector<PropertyListing> searchAndCompare::searchByAreaRange( unordered_map<int, PropertyListing>& properties, double minArea, double maxArea) {

    vector<PropertyListing> result;

    for ( auto pair : properties) {
         PropertyListing& property = pair.second;
        if (property.getPrice() >= minArea || property.getPrice() <= maxArea) {
            result.push_back(property);
        }
    }
    return result;
}

vector<PropertyListing> searchAndCompare::searchByLocation( unordered_map<int, PropertyListing>& properties, string government, string city)
{
    vector<PropertyListing> result;

    for ( auto pair : properties) {
         PropertyListing& property = pair.second;
        if (strcmp(property.getLocation().city, city.c_str()) == 0) {                //chat GPT مش فاهم ده ايه
            result.push_back(property);
        }
    }
    return result;
}

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

    bool anotherFilter = true;

    while (anotherFilter) {
        cout << "Choose a filter option:" << endl;
        cout << "[1] Search by Property Type" << endl;;
        cout << "[2] Search by Location" << endl;;
        cout << "[3] Search by Price Range" << endl;;
        cout << "[4] Search by Area Range" << endl;
        cout << "[5] Finish Search" << endl;;

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
        case 5:
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
    
        // Display the results
        if (finalResults.empty()) {
            cout << "No matching properties found" << endl;
        }
        else {
            cout << "Matching Properties " << endl;;
            for (   auto property : finalResults) {
                property.displayInfo();
            }
        }
    }
