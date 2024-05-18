#pragma once
#include <sha.h>
#include <filters.h>
#include <hex.h>
#include <iostream>
#include <string>
#include <fstream>

#include <cstdlib>
#include <iomanip>

#include "RoutingMachine.cpp"

using namespace CryptoPP;
using namespace std;

string hexToBinary(const string& hexString)
{
    string binaryString;
    for (int i = 0; i < hexString.length(); i++)
    {
        int value;
        char digit = hexString[i];
        if (digit >= '0' && digit <= '9')
            value = digit - '0';

        else if (digit >= 'A' && digit <= 'F')
            value = digit - 'A' + 10;

        else if (digit >= 'a' && digit <= 'f')
            value = digit - 'a' + 10;

        // Convert the hex digit to binary (4 bits)
        for (int i = 3; i >= 0; --i)
            binaryString.push_back((value & (1 << i)) ? '1' : '0');
    }

    return binaryString;
}

// Reads the file to pass it into SHA 1 to get the hash
// returns the entire content as a string
string fileOpen(string filePath) {
    // Create an input file stream
    ifstream inputFile(filePath);

    // Check if the file is opened successfully
    if (!inputFile.is_open())
        cout << "Error opening file: " << filePath << endl;

    // Read data from the file
    string line;
    string total = "";
    while (getline(inputFile, line))
        total += line;

    // Close the file
    inputFile.close();

    return total;
}

// narrowing down the data to fit the identifier space
string editing_hash(string original, int bits)
{
    return original.substr(0, bits);
}

// Implements the SHA 1
string generate_hash(string file_path, int identifier_bits, bool file_present)
{
    string hash = "";
    SHA1 sha1;
    if (file_present)
    {
        string source = fileOpen(file_path);  //This will be randomly generated somehow
        StringSource(source, true, new HashFilter(sha1, new HexEncoder(new StringSink(hash))));
    }
    else
    {
        StringSource(file_path, true, new HashFilter(sha1, new HexEncoder(new StringSink(hash))));
    }
    cout << "Hash Value generated in Hexa: " << hash << endl;

    cout << "Hash Value converted in Binary: " << hexToBinary(hash) << endl;

    hash = editing_hash(hexToBinary(hash), identifier_bits);

    cout << "Hash generated according to identifier bits: " << hash << endl;
    return hash;
}
/*
int main()
{
    //cout << "HELLO" << endl;
    int identifier_bits = 5;
    CircularLinkedListOfMachines cm1(identifier_bits, 3);
    //  cm1.insertMachine("00001");
      //	cm1.display_routing_table();
    cout << endl;
    cm1.insertMachine("00100");
    cm1.display_routing_table();
    cm1.insertMachine("01001");
    cm1.display_routing_table();
    cm1.insertMachine("01011");
    cm1.display_routing_table();
    cm1.insertMachine("01110");

    //  cm1.deleteMachine("10111");

    cm1.display_routing_table();
    cm1.insertMachine("10010");
    cm1.display_routing_table();
    cm1.insertMachine("10100");
    cm1.display_routing_table();
    cm1.insertMachine("10101");
    cm1.display_routing_table();
    cm1.insertMachine("11100");
    cm1.display_routing_table();
    cout << "ROUTING TABLE\n";
    cm1.display_routing_table();
    cm1.print_btree_inorder();

    cm1.inserting_file("00100", "C:\\Users\\maryam\\Desktop\\TEMP1.txt", "11100");
    cm1.display_routing_table();
    cm1.print_btree_inorder();

    cm1.inserting_file("01111", "C:\\Users\\maryam\\Desktop\\TEMP1.txt", "01110");
    cm1.display_routing_table();
    cm1.print_btree_inorder();

    cm1.inserting_file("11111", "C:\\Users\\maryam\\Desktop\\TEMP2.txt", "01110");
    cm1.print_btree_inorder();

    cm1.inserting_file("00001", "C:\\Users\\maryam\\Desktop\\TEMP3.txt", "11100");
    cm1.print_btree_inorder();

    cm1.inserting_file("11110", "C:\\Users\\maryam\\Desktop\\TEMP4.txt", "11100");
    cm1.print_btree_inorder();

    cm1.inserting_file("11101", "C:\\Users\\maryam\\Desktop\\TEMP5.txt", "11100");
    cm1.print_btree_inorder();

    cm1.display_routing_table();
    cm1.Searching("11110", "10010");
    cm1.insertMachine("11110");
    cm1.print_btree_inorder();
    cm1.deleteMachine("11110");
    cm1.print_btree_inorder();
    cm1.deleteMachine("00001");
    cm1.print_btree_inorder();
    cm1.deleteMachine("00100");
    //  cm1.display_routing_table();

    cm1.print_btree_inorder();
    //cm1.displayMachines();
    return 0;
}
*/
int main()
{
    int option = 0, bits = -1, order = 0, total_machines = 0;

    // Output the box with text
    cout << setw(90) << ".................................................\n";
    cout << setw(90) << "|  WELCOME TO THE INTER-PLANETARY FILE SYSTEM!  |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << "|                                               |\n";
    cout << setw(90) << ".................................................\n";

    cout << "PRESS ENTER TO CONTINUE.";
    cin.ignore();  // Wait for the user to press Enter
    system("cls");

    //Identifier space
    while (bits <= 0)
    {
        cout << "Please Specify Identifier Space (in bits) :  ";
        cin >> bits;
        if (bits <= 0)
        {
            cout << "\n\nINVALID INPUT. BITS MUST BE A POS INT\n\n";
        }
    }

    while (order <= 2)
    {
        cout << "Specify Order for the B-Tree : ";
        cin >> order;
    }


    CircularLinkedListOfMachines cm1(bits, order);

    while (total_machines <= 0)
    {
        cout << "Specify how many machines you want to insert : ";
        cin >> total_machines;
        if (total_machines <= 0)
        {
            cout << "\n\nINVALID CHARACTER!\n\n";
        }
    }

    system("cls");

    for (int i = 0; i < total_machines; i++)
    {
        cout << "Data entry: " << i + 1 << endl;

        cout << "Do you wish to enter the Machine Hash or Machine name: \n\nPress 1 for manually entering the machine hash.\n\nPress 2 for entering the machine name.\n\n";

        do {
            cout << "Enter your choice: ";
            cin >> option;
        } while (option != 1 && option != 2);
        if (option == 1)
        {
            string machine_data;
            cout << "Enter machine hash: ";
            cin >> machine_data;
            if (machine_data.length() > bits)
            {
                cout << endl << "Entered Hash is larger than the required. Adjusting Hash to fit" << endl;
                machine_data = editing_hash(machine_data, bits);
                cout << "New Hash is:  " << machine_data << endl;
            }
            if (machine_data.length() < bits)
            {
                cout << endl << "Entered Hash is smaller than the required. Adjusting Hash to fit" << endl;
                machine_data = CircularLinkedListOfMachines::add_zero_bits(machine_data, bits);
                cout << "New Hash is:  " << machine_data << endl;
            }
            if (cm1.insertMachine(machine_data) == false)
                i--;
        }
        else
        {
            string machine_data = "";
            cout << "\n\nEnter machine name: ";
            cin >> machine_data;
            if (cm1.insertMachine(generate_hash(machine_data, bits, false)) == false)
                i--;
        }
    }
    //Options

    do {

        cout << left << setw(37) << "Choose an option: \n\n\n";

        cout << left << setw(60) << "To Add a New Machine : " << setw(25) << "Press 1\n\n";

        cout << left << setw(60) << "To Delete a Machine : " << setw(25) << "Press 2\n\n";

        cout << left << setw(60) << "To Print Routing Table : " << setw(25) << "Press 3\n\n";

        cout << left << setw(60) << "To Remove Files From a Specific Machine : " << setw(25) << "Press 4\n\n";

        cout << left << setw(60) << "To Insert Files From a Specific Machine : " << setw(25) << "Press 5\n\n";

        cout << left << setw(60) << "To Search for a File From a Specific Machine : " << setw(25) << "Press 6\n\n";

        cout << left << setw(60) << "To View the B-trees : " << setw(25) << "Press 7\n\n";

        cout << left << setw(60) << "To View the Machines in the IPFS : " << setw(25) << "Press 8\n\n";

        cout << left << setw(60) << "To Clear Screen : " << setw(25) << "Press 9\n\n";

        cout << left << setw(60) << "To Exit : " << setw(25) << "Press 10\n\n";

        do {
            cout << "Enter option: ";
            cin >> option;
        } while (option < 1 || option > 11);

        if (option == 1)
        {
            string machine_data = "";
            cout << "Do you wish to enter the Machine Hash or Machine name: \nPress 1 for manually entering the machine hash.\nPress 2 for entering the machine name.\n";
            do {
                cout << "Enter your choice: ";
                cin >> option;
            } while (option != 1 && option != 2);
            if (option == 1)
            {
                cout << "Enter machine hash: ";
                cin >> machine_data;
                if (machine_data.length() > bits)
                {
                    cout << endl << "Entered Hash is larger than the required. Adjusting Hash to fit" << endl;
                    machine_data = editing_hash(machine_data, bits);
                    cout << "New Hash is:  " << machine_data << endl;
                }
                if (machine_data.length() < bits)
                {
                    cout << endl << "Entered Hash is smaller than the required. Adjusting Hash to fit" << endl;
                    machine_data = CircularLinkedListOfMachines::add_zero_bits(machine_data, bits);
                    cout << "New Hash is:  " << machine_data << endl;
                }
                if (cm1.insertMachine(machine_data) == true)
                    break;
            }
            else
            {
                while (true) {
                    cout << "Enter machine name: ";
                    cin >> machine_data;
                    if (cm1.insertMachine(generate_hash(machine_data, bits, false)) == true)
                        break;
                }
            }
        }

        else if (option == 2)
        {
            string machine_data = "";
            cout << "Enter Machine hash: ";
            cin >> machine_data;
            cm1.deleteMachine(machine_data);
        }

        else if (option == 3)
        {
            cout << "Do you wish to view all routing tables or of a specific machine: \nPress 1 for all.\nPress 2 for a specific machine.\n";
            do {
                cout << "Enter your choice: ";
                cin >> option;
            } while (option != 1 && option != 2);
            if (option == 1)
            {
                cm1.display_routing_table();
            }
            else
            {
                string machine_name = "";
                cout << "Enter machine hash: ";
                cin >> machine_name;
                cm1.display_routing_table(machine_name);
            }
        }

        else if (option == 4)
        {
            string file_hash = "";
            cout << "Enter file hash: ";
            cin >> file_hash;
            string machine_name = "";
            cout << "Enter machine name: ";
            cin >> machine_name;
            cm1.deleting_file(file_hash, machine_name);
        }

        else if (option == 5)
        {
            string file_path = "";
            cout << "Enter file path: ";
            cin >> file_path;
            string machine_name;
            cout << "Enter machine name: ";
            cin >> machine_name;
            cm1.inserting_file(generate_hash(file_path, bits, true), file_path, machine_name);
        }

        else if (option == 6)
        {
            string hash_name = "";
            cout << "Enter hash name: ";
            cin >> hash_name;
            string machine_name;
            cout << "Enter machine name: ";
            cin >> machine_name;
            cm1.Searching(hash_name, machine_name);
        }

        else if (option == 7)
        {
            cout << "Do you wish to view all trees or of a specific machine: \nPress 1 for all.\nPress 2 for a specific machine.\n";
            do {
                cout << "Enter your choice: ";
                cin >> option;
            } while (option != 1 && option != 2);
            string machine_name = "";
            if (option == 2)
            {
                cout << "Enter machine name: ";
                cin >> machine_name;
            }
            int tree_choice;
            cout << "Choose which of the following trees you wish to see\nPress 1 for Expanded view of Tree\nPress 2 for In order\nPress 3 for Level Order\n";
            do {
                cout << "Enter your choice: ";
                cin >> tree_choice;
            } while (tree_choice != 1 && tree_choice != 2 && tree_choice != 3);
            if (option == 1)
            {
                if (tree_choice == 1)
                {
                    cm1.print_btree_expanded();
                }
                else if (tree_choice == 2)
                {
                    cm1.print_btree_inorder();
                }
                else
                {
                    cm1.print_btree_levelorder();
                }
            }
            else
            {
                if (tree_choice == 1)
                {
                    cm1.print_btree_expanded(machine_name);
                }
                else if (tree_choice == 2)
                {
                    cm1.print_btree_inorder(machine_name);
                }
                else
                {
                    cm1.print_btree_levelorder(machine_name);
                }
            }
        }

        else if (option == 8)
        {
            cout << endl;
            cm1.displayMachines();
            cout << endl;
        }


        else if (option == 9)
        {
            system("cls");
        }

        else if (option != 10)
        {
            cout << "Incorrect Data entry\n";
        }

    } while (option != 10);

    return 0;
}



