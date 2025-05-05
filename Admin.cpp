#include <iostream>
#include "Admin.h"
#include"Property.h"


Admin::Admin(string username, string password, string email, string phone)
{
    username, password, email, phone = "";
}
void Admin::addPropertyListing(PropertyListing property) {
    Property_Listing[property.getPropertyID()] = property;
}
void Admin::removePropertyListings(int propertyid) {
    Property_Listing.erase(propertyid);
}
void Admin::editPropertyListing(int propertyID, PropertyListing updatedProperty) {
    if (Property_Listing.find(propertyID) != Property_Listing.end()) {
        Property_Listing[propertyID] = updatedProperty;
    }
    else {
        throw invalid_argument("Property ID not found ");
    }
}
void Admin::highlightPropertyListing(int propertyid) {
    if (Property_Listing.find(propertyid) != Property_Listing.end()) {
        highlightedProperty.insert(propertyid);
        cout << "Property ID " << propertyid << " is now highlighted" << endl;
    }
    else {
        throw invalid_argument("Property ID not found");
    }
}


void Admin::ManageListings() {
    int choice;
    int id;

    while (true) {
        cout << "--------------------Manage Property Listing--------------------" << endl;
        cout << "[1] Add Property\n"
            << "[2] Edit Property\n"
            << "[3] Remove Property\n"
            << "[4] Highlight Property\n"
            << "[5] Exit\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { // Add Property
            PropertyListing newProperty = PropertyListing();
            newProperty = newProperty.createProperty();
            addPropertyListing(newProperty); // Create property internally
            cout << "Property added successfully!\n";
            break;
        }
        case 2: { // Edit Property
            cout << "Enter Property ID: ";
            cin >> id;
            if (Property_Listing.find(id) != Property_Listing.end()) {
                PropertyListing& propertyToEdit = Property_Listing[id];
                cout << "Editing Property:\n";
                propertyToEdit.displayInfo();

                // Allow admin to update property details
                string newName, newFeatures;
                double newPrice, newSize;
                int newTypeChoice;

                cout << "Enter New Name (or press Enter to keep current): ";
                cin.ignore();
                getline(cin, newName);
                if (!newName.empty()) propertyToEdit.setName(newName);

                cout << "Enter New Price (or -1 to keep current): ";
                cin >> newPrice;
                if (newPrice >= 0) propertyToEdit.setPrice(newPrice);

                cout << "Enter New Size (or -1 to keep current): ";
                cin >> newSize;
                if (newSize >= 0) propertyToEdit.setSize(newSize);

                cout << "Enter New Features (or press Enter to keep current): ";
                cin.ignore();
                getline(cin, newFeatures);
                if (!newFeatures.empty()) propertyToEdit.setFeatures(newFeatures);

                cout << "Pick New Property Type (or 0 to keep current):\n";
                cout << "1. Apartment\n2. Villa\n3. Studio\n4. Townhouse\n5. Land\n6. Unknown\n";
                cin >> newTypeChoice;
                if (newTypeChoice > 0) {
                    propertyToEdit.setType(static_cast<PropertyType>(newTypeChoice - 1));
                }

                cout << "Property updated successfully!\n";
            }
            else {
                cout << "Property ID not found.\n";
            }
            break;
        }
        case 3: { // Remove Property
            cout << "Enter Property ID: ";
            cin >> id;
            try {
                removePropertyListings(id);
                cout << "Property removed successfully!\n";
            }
            catch (const invalid_argument& e) {
                cout << e.what() << '\n';
            }
            break;
        }
        case 4: { // Highlight Property
            // Display all properties
            cout << "--------------------Available Properties--------------------\n";
            cout << "ID\tName\t\tPrice\t\tType\n";
            cout << "-----------------------------------------------------------\n";
            for (const auto& entry : Property_Listing) {
                PropertyListing temp_prop = entry.second;
                cout << temp_prop.getPropertyID() << "\t" << temp_prop.getName() << "\t\t$"
                    << temp_prop.getPrice() << "\t\t" << propertyTypeToString(temp_prop.getType()) << '\n';
            }



            cout << "-----------------------------------------------------------\n";

            // Prompt user to highlight a property
            cout << "Enter Property ID to Highlight: ";
            cin >> id;
            try {
                highlightPropertyListing(id);
                cout << "Property highlighted successfully!\n";
            }
            catch (const invalid_argument& e) {
                cout << e.what() << '\n';
            }
            break;
        }
        case 5: { // Exit
            return;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
void Admin::UserManagement() {
    int choice;
    int propertyID;

    while (true) {
        cout << "--------------------User Management--------------------" << endl;
        cout << "[1] View Pending Property Listings\n"
            << "[2] Approve Next Property Listing\n"
            << "[3] Reject Next Property Listing\n"
            << "[4] Moderate User Activities (Remove Property)\n"
            << "[5] Exit\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { // View Pending Property Listings
            if (pendingApprovals.empty()) {
                cout << "No pending properties for approval.\n";
            }
            else {
                cout << "--------------------Pending Properties--------------------\n";
                queue<PropertyListing> tempQueue = pendingApprovals; 
                while (!tempQueue.empty()) {
                    PropertyListing property = tempQueue.front();
                    tempQueue.pop();
                    cout << "Property ID: " << property.getPropertyID()
                        << " | Name: " << property.getName()
                        << " | Price: $" << property.getPrice()
                        << " | Type: " << propertyTypeToString(property.getType()) << '\n';
                }
                cout << "---------------------------------------------------------\n";
            }
            break;
        }
        case 2: {
            approveNextProperty();
            break;
        }
        case 3: { 
            rejectNextProperty();
            break;
        }
        case 4: { 
            cout << "Enter Property ID to Remove: ";
            cin >> propertyID;

            if (Property_Listing.find(propertyID) != Property_Listing.end()) {
                removePropertyListings(propertyID);
                cout << "Property ID " << propertyID << " has been removed.\n";
            }
            else {
                cout << "Property ID not found.\n";
            }
            break;
        }
        case 5: {
            return;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

void Admin::viewPendingUserApprovals() {
    cout << "--------------------Pending User Approvals--------------------" << endl;
    bool hasPendingApprovals = false;

    for ( auto userPair : Users) {
        BaseAccount* account = userPair.second;
        if (account->type == AccountType::AdminAccount) {
            AdminAccount* adminAccount = static_cast<AdminAccount*>(account);
            if (!adminAccount->authorization) {
                cout << "User Handle: @" << adminAccount->userHandle
                    << " | Name: " << adminAccount->firstName << " " << adminAccount->lastName << endl;
                hasPendingApprovals = true;
            }
        }
    }

    if (!hasPendingApprovals) {
        cout << "No pending user approvals.\n";
    }
    cout << "-------------------------------------------------------------" << endl;
}




DashboardStats Admin::getDashboardStats() {
    DashboardStats stats = { 0, 0, 0, 0 };

    // Count total users and pending user approvals
    for (auto userPair : Users) {
        stats.totalUsers++;
        BaseAccount* account = userPair.second;
        if (account->type == AccountType::AdminAccount) {
            AdminAccount* adminAccount = static_cast<AdminAccount*>(account);
            if (!adminAccount->authorization) {
                stats.pendingUserApprovals++;
            }
        }
    }

    
    stats.totalProperties = Property_Listing.size();
    stats.pendingPropertyApprovals = pendingApprovals.size();

    return stats;
}


void Admin::accessDashboard() {
    DashboardStats stats = getDashboardStats();

    cout << "--------------------Admin Dashboard--------------------" << endl;
    cout << "Total Users: " << stats.totalUsers << endl;
    cout << "Total Properties: " << stats.totalProperties << endl;
    cout << "Pending User Approvals: " << stats.pendingUserApprovals << endl;
    cout << "Pending Property Approvals: " << stats.pendingPropertyApprovals << endl;
    cout << "------------------------------------------------------" << endl;
}


void Admin::addPropertyToApprovalQueue(PropertyListing property) {
    pendingApprovals.push(property);
    cout << "Property submitted for admin approval.\n";
}
void Admin::approveNextProperty() {
    if (pendingApprovals.empty()) {
        cout << "No pending properties for approval.\n";
        return;
    }

    PropertyListing property = pendingApprovals.front();
    pendingApprovals.pop();

    addPropertyListing(property);
    cout << "Property ID " << property.getPropertyID() << " approved and added to the system.\n";
}
void Admin::rejectNextProperty() {
    if (pendingApprovals.empty()) {
        cout << "No pending properties for rejection.\n";
        return;
    }

    PropertyListing property = pendingApprovals.front();
    pendingApprovals.pop();

    cout << "Property ID " << property.getPropertyID() << " rejected and removed from the queue.\n";
}
