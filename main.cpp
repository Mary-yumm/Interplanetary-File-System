#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include "hashing.cpp"
#include "RoutingMachine.cpp"
using namespace std;

int main()
{
    int option = 0;
    bool InputOptionValid = false;

    int total_machines = -1, identifier_space_bits = 0;
    char manually_Assign = ' ';

    cout << setw(90) << "WELCOME TO THE INTER-PLANETARY FILE SYSTEM!\n\n";

    //Options
    cout << "Choose an option: \n\n\n";

    cout << left << setw(60) << "To Set Total Number Of Machines : " << setw(25) << "Press 1\n";

    cout << left << setw(60) << "To Specify Identifier Space : " << setw(25) << "Press 2\n";

    cout << left << setw(60) << "To Add a New Machine : " << setw(25) << "Press 3\n";

    cout << left << setw(60) << "To Delete a Machine : " << setw(25) << "Press 4\n";

    cout << left << setw(60) << "To Print Routing Table of a Machine : " << setw(25) << "Press 5\n";

    cout << left << setw(60) << "To Remove Data From a Specific Machine : " << setw(25) << "Press 6\n";

    cout << left << setw(60) << "To Insert Data From a Specific Machine : " << setw(25) << "Press 7\n";

    cout << left << setw(60) << "To Manually Assign IDs to each Machine : " << setw(25) << "Press 8\n";

    cout << "Enter option: ";
    //Keep taking input if invalid option is choosen.
    while (InputOptionValid != true)
    {
        cin >> option;
        if (option >= 1 && option <= 8)
        {
            InputOptionValid = true;
        }
        else
        {
            InputOptionValid = false;
            cout << "ERROR: Please choose an option between 1 and 8.\n";
        }
    }

    //Actions based on option choosen
    switch (option)
    {
    case 1: //Set total machines
        while (total_machines < 0)
        {
            cout << "Please enter total number of machines: ";
            cin >> total_machines;
            if (total_machines < 0)
            {
                "Invalid Input: Total Machines cannot be negative\n";
            }
        }
        //Ftn call to set total machines here/////////////////////////
        break;
    case 2:
        while (identifier_space_bits <= 0)
        {
            cout << "Enter total bits for Identifier Space: ";
            cin >> identifier_space_bits;
            if (identifier_space_bits < 0)
            {
                "\nInvalid Input: Total Bits must be greater than 0\n";
            }
        }
        break;
    case 3:
        cout << "Do you want to manually assign machine ID? (y/n)  ";
        cin >> manually_Assign;
        cin.ignore();

        if (manually_Assign == 'y' || manually_Assign == 'Y')
        {
            string machine_id = "";
            InputOptionValid = false;
            while (InputOptionValid != true)
            {
                cout << "Enter ID: ";
                getline(cin, machine_id);
                if (machine_id.length()) //==identifierspace variable here)
                {
                    InputOptionValid = true;
                    break;
                }
                else
                {
                    cout << "INVALID SIZE!\n\n";
                }
            }
            //call ftns for add machine waghera etc
        }
        else
        {
            string machine_name = "";
            cout << "Enter machine name:  ";
            getline(cin, machine_name);
            //Call ftns for hash waghera and wtever u wanna do here
        }
        break;
    case 4:
        cout << "Would you like to enter the machine name or machine ID? (n/i) ";
        cin >> manually_Assign;
        cin.ignore();

        if (manually_Assign == 'n' || manually_Assign == 'N')
        {
            string machine_name;
            cout << "Enter name:  ";
            getline(cin, machine_name);
            //call ftns for deletion here
        }
        else
        {
            string machine_id = "";
            while (InputOptionValid != true)
            {
                cout << "Enter ID: ";
                getline(cin, machine_id);
                if (machine_id.length()) //==identifierspace variable here)
                {
                    InputOptionValid = true;
                    break;
                }
                else
                {
                    cout << "INVALID SIZE!\n\n";
                }
            }
            //call ftns for del machine waghera etc
        }
        break;
    case 5:
        cout << "Routing Table of Machine: \n\n";
        //call displayRoutingTable()
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    default:
        break;

    }
}
