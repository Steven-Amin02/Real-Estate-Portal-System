#include <iostream>
#include "Admin.h"
#include"Property.h"


Admin::Admin(string username, string password, string email, string phone)
    : BaseAccount(AccountType::AdminAccount, username, password, email, phone) // Call BaseAccount constructor
{
    // Initialize Admin-specific members if needed
}
void Admin::addPropertyListing(PropertyListing property) {
    Property_Listing[property.getPropertyID()] = property;
}
void Admin::removePropertyListings(int propertyid) {
    if (Property_Listing.find(propertyid) != Property_Listing.end()) {
        Property_Listing.erase(propertyid);
    }
    else {
        cout << "Cannot Remove this Property Because it doesnot exist"<<endl;
    }
}
void Admin::editPropertyListing(int propertyID, PropertyListing& updatedProperty) {
    int choice;
    do {
        cout << "--------------------Edit Property--------------------\n";
        cout << "1. Edit Name\n";
        cout << "2. Edit Price\n";
        cout << "3. Edit Size\n";
        cout << "4. Edit Features\n";
        cout << "5. Edit Property Type\n";
        cout << "6. Exit Editing\n";
        cout << "----------------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { // Edit Name
            string newName;
            cout << "Enter New Name (or press Enter to keep current): ";
            cin.ignore();
            getline(cin, newName);
            if (!newName.empty()) {
                updatedProperty.setName(newName);
                cout << "Name updated successfully!\n";
            }
            else {
                cout << "Name not changed.\n";
            }
            break;
        }
        case 2: { // Edit Price
            double newPrice;
            cout << "Enter New Price (or -1 to keep current): ";
            cin >> newPrice;
            if (newPrice >= 0) {
                updatedProperty.setPrice(newPrice);
                cout << "Price updated successfully!\n";
            }
            else {
                cout << "Price not changed.\n";
            }
            break;
        }
        case 3: { // Edit Size
            double newSize;
            cout << "Enter New Size (or -1 to keep current): ";
            cin >> newSize;
            if (newSize >= 0) {
                updatedProperty.setSize(newSize);
                cout << "Size updated successfully!\n";
            }
            else {
                cout << "Size not changed.\n";
            }
            break;
        }
        case 4: { // Edit Features
            string newFeatures;
            cout << "Enter New Features (or press Enter to keep current): ";
            cin.ignore();
            getline(cin, newFeatures);
            if (!newFeatures.empty()) {
                updatedProperty.setFeatures(newFeatures);
                cout << "Features updated successfully!\n";
            }
            else {
                cout << "Features not changed.\n";
            }
            break;
        }
        case 5: { // Edit Property Type
            int newTypeChoice;
            cout << "Pick New Property Type (or 0 to keep current):\n";
            cout << "1. Apartment\n2. Villa\n3. Studio\n4. Townhouse\n5. Land\n6. Unknown\n";
            cin >> newTypeChoice;
            if (newTypeChoice > 0) {
                updatedProperty.setType(static_cast<PropertyType>(newTypeChoice - 1));
                cout << "Property type updated successfully!\n";
            }
            else {
                cout << "Property type not changed.\n";
            }
            break;
        }
        case 6: { // Exit
            cout << "Exiting property editing.\n";
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    cout << "Property updated successfully!\n";
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
void Admin::displayAllProperty(){
    cout << "--------------------Available Properties--------------------\n";
    cout << "ID\tName\t\tPrice\t\tType\n";
    cout << "-----------------------------------------------------------\n";
    for (const auto& entry : Property_Listing) {
        PropertyListing temp_prop = entry.second;
        cout << temp_prop.getPropertyID() << "\t"
            << temp_prop.getName() << "\t\t$"
            << temp_prop.getPrice() << "\t\t"
            << propertyTypeToString(temp_prop.getType()) << '\n';
    }
    cout << "-----------------------------------------------------------\n";
}





void Admin::ManageListings() {
    int choice;
    int id;

    while (true) {
        cout << "--------------------Manage Property Listing--------------------" << endl;
        cout << "[1] Submit Property Listing\n"
            << "[2] Edit Property\n"
            << "[3] Remove Property\n"
            << "[4] Highlight Property\n"
            << "[5] Exit\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { // Submit Property (using the unified submission function)
            // For an admin, submitPropertyListing will add the listing directly
            // while for a user it might add it to an approval queue.
            this->submitPropertyListing(*this);
            cout << "Property submitted successfully!\n";
            break;
        }
        case 2: { // Edit Property
            cout << "Enter Property ID: ";
            cin >> id;
            if (Property_Listing.find(id) != Property_Listing.end()) {
                PropertyListing& propertyToEdit = Property_Listing[id];
                editPropertyListing(id, propertyToEdit);
            }
            else {
                throw invalid_argument("Property ID not found ");
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
            displayAllProperty();
            // Prompt and process highlight
            cout << "Enter Property ID to Highlight: ";
            cin >> id;
            try {
                highlightPropertyListing(id);
                cout << "Property highlighted successfully!\n";
            }
            catch (invalid_argument& e) {
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
                queue<int> tempQueue = pendingApprovals; // Use int queue
                while (!tempQueue.empty()) {
                    int propertyID = tempQueue.front();
                    tempQueue.pop();

                    if (Property_Listing.find(propertyID) != Property_Listing.end()) {
                        PropertyListing property = Property_Listing[propertyID];
                        cout << "Property ID: " << property.getPropertyID()
                            << " | Name: " << property.getName()
                            << " | Price: $" << property.getPrice()
                            << " | Type: " << propertyTypeToString(property.getType()) << '\n';
                    }
                    else {
                        cout << "Property ID " << propertyID << " not found in the system.\n";
                    }
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


void Admin::approveUser( string userHandle) {
    if (Users.find(userHandle) != Users.end()) {
        BaseAccount* account = Users[userHandle];
        if (account->type == AccountType::AdminAccount) {
            AdminAccount* adminAccount = static_cast<AdminAccount*>(account);
            if (!adminAccount->authorization) {
                adminAccount->authorization = true;
                cout << "User @" << adminAccount->userHandle << " has been approved.\n";
                return;
            }
            else {
                cout << "User @" << adminAccount->userHandle << " is already authorized.\n";
                return;
            }
        }
    }
    cout << "User handle not found or not an AdminAccount.\n";
}














void Admin::addPropertyToApprovalQueue(PropertyListing property) {
    pendingApprovals.push(property.getPropertyID()); // Push property ID instead of the entire object
    cout << "Property submitted for admin approval.\n";
}

void Admin::approveNextProperty() {
    if (pendingApprovals.empty()) {
        cout << "No pending properties for approval.\n";
        return;
    }

    int propertyID = pendingApprovals.front(); // Get property ID
    pendingApprovals.pop();

    if (Property_Listing.find(propertyID) != Property_Listing.end()) {
        addPropertyListing(Property_Listing[propertyID]); // Add the property using its ID
        cout << "Property ID " << propertyID << " approved and added to the system.\n";
    }
    else {
        cout << "Property ID " << propertyID << " not found in the system.\n";
    }
}

void Admin::rejectNextProperty() {
    if (pendingApprovals.empty()) {
        cout << "No pending properties for rejection.\n";
        return;
    }

    int propertyID = pendingApprovals.front(); // Get property ID
    pendingApprovals.pop();

    if (Property_Listing.find(propertyID) != Property_Listing.end()) {
        cout << "Property ID " << propertyID << " rejected and removed from the queue.\n";
    }
    else {
        cout << "Property ID " << propertyID << " not found in the system.\n";
    }
}

