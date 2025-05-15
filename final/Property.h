#pragma once
#include <string>

class Property
{
	int id;
	std::string name, location, type;
	double price;
	std::string features;
	std::string ownerUsername;
	bool highlighted = false;
	bool approved = false;

public:
	// Default constructor
	Property() : id(0), price(0.0) {}

	Property(int id, std::string n, std::string loc, std::string t, double p, std::string f, std::string owner, bool highlight = false, bool approve = false);
	void display() const;
	int getId() const;
	std::string getLocation() const;
	double getPrice() const;
	std::string getType() const;
	std::string getName() const;
	std::string getFeatures() const;
	std::string getOwnerUsername() const;
	bool isHighlighted() const;
	void setHighlighted(bool h);
	bool isApproved() const;
	void setApproved(bool a);
};
