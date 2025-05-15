#include "Property.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Property::Property(int id, std::string n, std::string loc, std::string t, double p, double s, std::string f, std::string owner, bool highlight, bool approve)
    : id(id), name(n), location(loc), type(t), price(p), size(s), features(f), ownerUsername(owner), highlighted(highlight), approved(approve)
{
}

void Property::display() const
{
    std::cout << (highlighted ? "* " : "  ");
    std::cout << "ID: " << id << " | Name: " << name << " | Location: " << location
              << " | Type: " << type << " | Size: " << std::fixed << std::setprecision(1) << size << " sq ft"
              << " | Price: $" << std::fixed << std::setprecision(1) << price << " | Features: " << features
              << " | Owner: " << ownerUsername << std::endl;
}

int Property::getId() const { return id; }
std::string Property::getLocation() const { return location; }
double Property::getPrice() const { return price; }
double Property::getSize() const { return size; }
std::string Property::getType() const { return type; }
std::string Property::getName() const { return name; }
std::string Property::getFeatures() const { return features; }
std::string Property::getOwnerUsername() const { return ownerUsername; }
bool Property::isHighlighted() const { return highlighted; }
void Property::setHighlighted(bool h) { highlighted = h; }
bool Property::isApproved() const { return approved; }
void Property::setApproved(bool a) { approved = a; }
