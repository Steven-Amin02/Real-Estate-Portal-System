#pragma once
#include "Property.h"
#include "Admin.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>


struct PropertyHash {
    size_t operator()(const PropertyListing& property) const {
        return std::hash<int>()(property.getPropertyID());
    }
};
struct PropertyEqual {
    bool operator()(const PropertyListing& a, const PropertyListing& b) const {
        return a.getPropertyID() == b.getPropertyID();
    }
};
class searchAndCompare {
public:
    vector<PropertyListing> searchByType(unordered_map<int, PropertyListing>& properties, string type);
    vector<PropertyListing> searchBypriceRange(unordered_map<int, PropertyListing>& properties, double minPrice, double maxPrice);
    vector<PropertyListing> searchByAreaRange(unordered_map<int, PropertyListing>& properties, double minArea, double maxArea);
    vector<PropertyListing> searchByLocation(unordered_map<int, PropertyListing>& properties, string government, string city);
    vector<PropertyListing> searchByBedrooms(unordered_map<int, PropertyListing>& properties,int bedrooms);
    unordered_set<PropertyListing, PropertyHash, PropertyEqual> searchMerge(vector<PropertyListing> type, vector<PropertyListing> price, vector<PropertyListing> area, vector<PropertyListing> location);
    void searchMenu();

    static bool comparePriceAsc(const PropertyListing& a, const PropertyListing& b);
    static bool comparePriceDesc(const PropertyListing& a, const PropertyListing& b);
    static bool compareSizeAsc(const PropertyListing& a, const PropertyListing& b);
    static bool compareSizeDesc(const PropertyListing& a, const PropertyListing& b);

    vector<PropertyListing> sortByPriceAsc(unordered_map<int, PropertyListing>& properties);
    vector<PropertyListing> sortByPriceDesc(unordered_map<int, PropertyListing>& properties);

    vector<PropertyListing> sortBySizeAsc(unordered_map<int, PropertyListing>& properties);
    vector<PropertyListing> sortBySizeDesc(unordered_map<int, PropertyListing>& properties);

    void compareProperties(unordered_map<int, PropertyListing>& properties);
};
