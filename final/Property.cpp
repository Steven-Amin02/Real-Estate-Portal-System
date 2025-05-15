#include "Property.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Helper to format price with commas and 1 decimal
static std::string formatPrice(double price)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << price;
    std::string s = ss.str();
    size_t dot = s.find('.');
    size_t start = (s[0] == '-') ? 1 : 0;
    for (size_t i = dot - 3; i > start; i -= 3)
        s.insert(i, ",");
    return s;
}

Property::Property(int id, std::string n, std::string loc, std::string t, double p, std::string f, std::string owner, bool highlight, bool approve)
    : id(id), name(n), location(loc), type(t), price(p), features(f), ownerUsername(owner), highlighted(highlight), approved(approve) {
}

void Property::display() const
{
    std::cout << (highlighted ? "* " : "  ");
    std::cout << "ID: " << id << " | Name: " << name << " | Location: " << location
        << " | Type: " << type << " | Price: $" << formatPrice(price) << " | Features: " << features
        << " | Owner: " << ownerUsername << std::endl;
}

int Property::getId() const { return id; }
std::string Property::getLocation() const { return location; }
double Property::getPrice() const { return price; }
std::string Property::getType() const { return type; }
std::string Property::getName() const { return name; }
std::string Property::getFeatures() const { return features; }
std::string Property::getOwnerUsername() const { return ownerUsername; }
bool Property::isHighlighted() const { return highlighted; }
void Property::setHighlighted(bool h) { highlighted = h; }
bool Property::isApproved() const { return approved; }
void Property::setApproved(bool a) { approved = a; }
