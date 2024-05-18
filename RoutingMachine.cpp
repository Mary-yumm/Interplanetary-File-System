#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include "B-Trees.h"
#include <fstream>

using namespace std;

class RoutingNode;
class Machine;
class CircularLinkedListOfMachines;

const string folder_path = "C:\\Users\\maryam\\Desktop\\DS Folder"; // DEPENDING ON USER

/*
The class Routing Node is a doubly linked class which acts as the routing table.
It has a pointer routed_machine which points to the machine it points to.
Other than that it has next and prev which points to the predecessor and successor nodes of the routing table.
*/

class RoutingNode
{
private:
	Machine* routed_machine;
	RoutingNode* next;
	RoutingNode* prev;
public:
	RoutingNode(Machine* m)
	{
		routed_machine = m;
		next = prev = NULL;
	}
	void setNext(RoutingNode* n) { this->next = n; }
	RoutingNode* getNext() { return this->next; }
	RoutingNode* getPrev() { return this->prev; }
	void setPrev(RoutingNode* r) { this->prev = r; }
	void setMachine(Machine* m) { routed_machine = m; }
	Machine* getMachine() const { return routed_machine; }
};


// acts as folders
class Machine
{
private:
	string name; // unique identifier
	RoutingNode* headRT; // head of the Routing Table
	bTree* head_btree; // head of the B Tree
	Machine* next; // next machine in the Circular Linked List
public:
	void setRThead(RoutingNode* head = NULL) { this->headRT = head; }
	bTree* getBtree() { return this->head_btree; }
	void setBtree(bTree* bt) { this->head_btree = bt; }
	Machine(string n, int order)
	{
		this->name = n;
		this->head_btree = new bTree(order);
		this->headRT = NULL;
		//headBT = NULL;
		this->next = NULL;
	}
	void adding_routing_table(Machine* new_m)
	{
		RoutingNode* new_node = new RoutingNode(new_m);
		if (this->headRT == NULL)
			this->headRT = new_node;
		else
		{
			new_node->setNext(this->headRT);
			this->headRT->setPrev(new_node);
			this->headRT = new_node;
		}
	}
	Machine(const Machine& m)
	{
		this->name = m.name;
		this->head_btree = m.head_btree;
		this->headRT = m.headRT;
		this->next = m.next;
	}
	void setName(string n) { name = n; }
	string getName() const { return name; }
	RoutingNode* getTable() const { return headRT; }
	Machine* getNext() { return next; }
	void setNext(Machine* m) { next = m; }

	// not in use for demo, helps in displaying routing table display_routing_table_inmachine()
	int bintodec(string binary)
	{
		int decimal = 0;
		int power = 0;

		for (int i = binary.length() - 1; i >= 0; i--)
		{
			if (binary[i] == '1')
				decimal += pow(2, power);

			power++;
		}
		return decimal;
	}

	// displays the only specific routing table
	void display_routing_table_inmachine()
	{
		RoutingNode* copy_head = headRT;
		while (copy_head != NULL)
		{
			//cout << bintodec(copy_head->getMachine()->getName()) << "  ";
			cout << "->  " << copy_head->getMachine()->getName() << "  "; //TO BE USED IN DEMO
			copy_head = copy_head->getNext();
		}
	}
};

class CircularLinkedListOfMachines
{
private:
	Machine* headMachine;
	string totalMachines;
	int identifier_bits;
	int order;
	string machine_entered_count;

	void folder_deletion(string machine_name)
	{
		string path = folder_path + "\\" + machine_name;
		LPCSTR path_final = path.c_str();
		LPCWSTR path_final2 = ConvertLPCSTRtoLPCWSTR(path_final);
		RemoveDirectory(path_final2);
		delete[] path_final2;
	}

	// path "C:\Users\jakha\OneDrive\Desktop\DS_PROJECT_DEMO"
	void folder_creation(string machine_name)
	{
		string path = folder_path + "\\" + machine_name;
		LPCSTR path_final = path.c_str();
		LPCWSTR path_final2 = ConvertLPCSTRtoLPCWSTR(path_final);
		CreateDirectory(path_final2, NULL);
		delete[] path_final2;
	}

	// converter function to make correct data types
	LPCWSTR ConvertLPCSTRtoLPCWSTR(const LPCSTR input)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, input, -1, NULL, 0); // Assuming UTF-8 encoding
		wchar_t* answer = new wchar_t[size];
		MultiByteToWideChar(CP_UTF8, 0, input, -1, answer, size);
		return answer;
	}

	bool insertMachine(Machine* m)
	{
		if (headMachine == NULL)
		{
			headMachine = m;
			m->setNext(m);
		}
		else if (m->getName() < headMachine->getName())
		{
			Machine* temp = headMachine;
			while (temp->getNext() != headMachine)
			{
				temp = temp->getNext();
			}
			temp->setNext(m);
			m->setNext(headMachine);
			headMachine = m;
		}
		else
		{
			Machine* temp = headMachine;
			if (m->getName() == temp->getName())
				return false;
			while (temp->getNext() != headMachine)
			{
				if (m->getName() == temp->getNext()->getName())
					return false;
				if (m->getName() < temp->getNext()->getName())
				{
					break;
				}
				temp = temp->getNext();
			}
			m->setNext(temp->getNext());
			temp->setNext(m);
		}
		return true;
	}

	// --------------STRING MANIPULATION FTNS START------------------------
	string removing_initial_zeroes(string original)
	{
		while (original[0] == '0')
		{
			if (original.length() == 1)
				break;
			original = original.substr(1);
		}
		return original;
	}

	string adding(string str1, string str2)
	{
		string result;
		char carry = '0';

		int i = str1.length() - 1;
		int j = str2.length() - 1;

		// Continue the loop until both strings are processed and there's no carry left
		while (i >= 0 || j >= 0 || carry != '0')
		{
			char bit1 = '0';
			char bit2 = '0';

			if (i >= 0)
				bit1 = str1[i--];
			else
				bit1 = '0';

			if (j >= 0)
				bit2 = str2[j--];
			else
				bit2 = '0';

			char sum;
			if (bit1 == bit2)
			{
				sum = carry;
				if (bit1 == '1')
					carry = '1';
				else
					carry = '0';
			}
			else
			{
				if (carry == '1')
					sum = '0';
				else
					sum = '1';
			}
			// Prepend the sum to the result
			result = sum + result;
		}

		return result;
	}

	string subtracting(string str1, string str2)
	{
		for (string p = "0"; p != str2; p = adding(p, "1"))
		{
			for (int i = str1.length() - 1; i >= 0; i--)
			{
				if (str1[i] == '1')
				{
					str1[i] = '0';
					for (int j = i + 1; j < str1.length(); j++)
					{
						str1[j] = '1';
					}
					break;
				}
			}
		}
		return removing_initial_zeroes(str1);
	}

	string finding_possible_machines(int bits)
	{
		string ans = "1";
		for (int i = 0; i < bits - 1; i++)
		{
			ans += '1';
		}
		return ans;
	}

	string power(int bits)
	{
		string ans = "1";
		for (int i = 0; i < bits; i++)
		{
			ans += '0';
		}
		return ans;
	}

	// --------------STRING MANIPULATION FTNS ends------------------------

	void update_routing_table()
	{
		Machine* copy_head = headMachine;
		for (string i = "0"; i != this->machine_entered_count; i = adding(i, "1"))
		{
			copy_head->setRThead(NULL); // reset routing table
			for (int j = this->identifier_bits; j > 0; j--)
			{
				Machine* copy_copy_head = headMachine;
				string x = copy_head->getName();
				string y = power(j - 1);
				string pos = adding(copy_head->getName(), power(j - 1));
				if (pos.length() > identifier_bits)
				{
					pos = subtracting(pos, this->totalMachines);
					pos = subtracting(pos, "1");
				}
				if (pos.length() < this->identifier_bits)
				{
					pos = add_zero_bits(pos, identifier_bits);
				}
				if (copy_copy_head->getName() >= pos)
				{
					copy_head->adding_routing_table(headMachine);
					continue;
				}
				while (true)
				{
					if (copy_copy_head->getNext()->getName() == headMachine->getName())
					{
						copy_head->adding_routing_table(copy_copy_head->getNext());
						break;
					}
					else if (copy_copy_head->getNext()->getName() >= pos)
					{
						copy_head->adding_routing_table(copy_copy_head->getNext());
						break;
					}
					else
					{
						copy_copy_head = copy_copy_head->getNext();
					}
				}
			}
			copy_head = copy_head->getNext();
		}
	}

	bool deletemachine_helper(string name)
	{
		if (headMachine == NULL)
			return false;

		Machine* current = headMachine;
		Machine* prev = NULL;
		do
		{
			prev = current;
			current = current->getNext();
			if (current->getName() == name)
			{
				if (current == current->getNext())
				{
					deleting_all(current);
					this->headMachine = NULL;
					return true;
				}
				transferring_data_to_successor(current);
				Machine* delete_machine = current;
				prev->setNext(current->getNext());
				if (headMachine == delete_machine)
					headMachine = headMachine->getNext();
				delete delete_machine;
				return true;
			}

		} while (current != headMachine);
		return false;
	}

	// for last machine
	void deleting_all(Machine* last_machine)
	{
		while (last_machine->getBtree()->get_root() != NULL)
		{
			string top_hash = last_machine->getBtree()->get_top();
			string ans = "";
			Node* duplicate = last_machine->getBtree()->search(top_hash, ans);
			if (duplicate == NULL)
			{
				remove(ans.c_str());
				/*if (successor->getBtree()->get_root() == NULL)
				{
					cout << "STILL NULL" << endl;
				}*/
				last_machine->getBtree()->Delete(top_hash, ans);
			}
			else
			{
				last_machine->getBtree()->Delete(top_hash, duplicate->value);
				while (duplicate != NULL)
				{
					remove(duplicate->value.c_str());
					duplicate = duplicate->next;
				}
			}
		}
		//folder_deletion(last_machine->getName());
	}

	// file change cuz of machine change
	string updating_file_path(string original, string machine_name)
	{
		string general_path = "C:\\Users\\jakha\\OneDrive\\Desktop\\DS_PROJECT_DEMO\\";
		general_path += machine_name;
		general_path += "\\";
		string file_name = "";
		for (int j = original.length() - 1; j > 0; j--)
		{
			if (original[j] == '\\')
				break;
			else
				file_name.insert(0, 1, original[j]);
		}
		bool same = false;
		string copy_general_path = general_path;
		if (original == (general_path + file_name))
			copy_general_path += '.';

		copy_general_path += file_name;
		general_path += file_name;
		making_new_file(original, general_path, copy_general_path);
		return general_path;
	}

	void making_new_file(string source, string destination, string temp_destination)
	{
		std::ifstream sourceFile(source); // Path to the source file
		std::ofstream destFile(temp_destination); // Path to the destination file

		if (sourceFile.is_open() && destFile.is_open()) {
			// Transfer contents from source to destination
			destFile << sourceFile.rdbuf();

			sourceFile.close();
			destFile.close();
			std::cout << "File transferred successfully!\n";
		}
		else {
			std::cout << "Failed to transfer the file!\n";
		}
		remove(source.c_str());
		rename(temp_destination.c_str(), destination.c_str());
	}

	// used in deleting machine
	void transferring_data_to_successor(Machine* delete_machine_pointer)
	{
		Machine* successor = delete_machine_pointer->getNext();
		while (delete_machine_pointer->getBtree()->get_root() != NULL)
		{
			string top_hash = delete_machine_pointer->getBtree()->get_top();
			string ans = "";
			Node* duplicate = delete_machine_pointer->getBtree()->search(top_hash, ans);
			if (duplicate == NULL)
			{
				string file_path_updated = updating_file_path(ans, successor->getName());
				successor->getBtree()->insert(top_hash, file_path_updated);
				delete_machine_pointer->getBtree()->Delete(top_hash, ans);
			}
			else
			{
				delete_machine_pointer->getBtree()->Delete(top_hash, duplicate->value);
				while (duplicate != NULL)
				{
					string file_path_updated = updating_file_path(duplicate->value, successor->getName());
					successor->getBtree()->insert(top_hash, file_path_updated);
					duplicate = duplicate->next;
				}
			}
		}
	}

	// used in inserting machine
	void predecessor_transfer(Machine* new_machine, Machine* successor)
	{
		if (successor->getBtree()->get_root() == NULL)
			return;
		Machine* temp = new Machine(successor->getName(), this->order);
		this->folder_creation(temp->getName());
		while (successor->getBtree()->get_root() != NULL)
		{
			string top_hash = successor->getBtree()->get_top();
			string ans = "";
			Node* duplicate = successor->getBtree()->search(top_hash, ans);
			if ((top_hash > new_machine->getName() && new_machine != headMachine) || (new_machine == headMachine && (new_machine->getName() < top_hash) && (top_hash <= new_machine->getNext()->getName())) || (new_machine->getNext() == headMachine && (top_hash > new_machine->getName() || new_machine->getNext()->getName() >= top_hash)))
			{
				if (duplicate == NULL)
				{
					string updated_file = updating_file_path(ans, temp->getName());
					temp->getBtree()->insert(top_hash, updated_file);
					successor->getBtree()->Delete(top_hash, ans);
				}
				else
				{
					successor->getBtree()->Delete(top_hash, duplicate->value);
					while (duplicate != NULL)
					{
						temp->getBtree()->insert(top_hash, updating_file_path(duplicate->value, temp->getName()));
						duplicate = duplicate->next;
					}
				}
			}
			else
			{
				if (duplicate == NULL)
				{
					new_machine->getBtree()->insert(top_hash, updating_file_path(ans, new_machine->getName()));
					successor->getBtree()->Delete(top_hash, ans);
				}
				else
				{
					successor->getBtree()->Delete(top_hash, duplicate->value);
					while (duplicate != NULL)
					{
						new_machine->getBtree()->insert(top_hash, updating_file_path(duplicate->value, new_machine->getName()));
						duplicate = duplicate->next;
					}
				}
			}
		}

		Machine* curr = headMachine;
		do {
			if (curr->getName() == temp->getName())
				break;
		} while (true);

		curr->setBtree(temp->getBtree());
	}

public:

	static string add_zero_bits(string str, int bits)
	{
		while (str.length() != bits)
		{
			str.insert(0, 1, '0');
		}
		return str;
	}

	CircularLinkedListOfMachines(int bits, int order = 3)
	{
		this->order = order;
		headMachine = NULL;
		this->identifier_bits = bits;
		this->totalMachines = finding_possible_machines(bits);
		machine_entered_count = "0";
	}

	bool insertMachine(string machine_name)
	{
		if (this->machine_entered_count == adding(this->totalMachines, "1"))
		{
			cout << endl << "The Ring DHT is already full" << endl << endl;
			return false;
		}

		Machine* new_machine = new Machine(machine_name, this->order);

		bool success = (this->insertMachine(new_machine));
		if (success)
		{
			this->machine_entered_count = adding(machine_entered_count, "1");
			this->update_routing_table();
			folder_creation(machine_name);
			predecessor_transfer(new_machine, new_machine->getNext());
		}
		else
			cout << endl << "Error. Please Re-enter. Machine with Hash already exists." << endl << endl;
		return success;
	}

	void displayMachines()
	{
		Machine* temp = headMachine;
		cout << endl << "DISPLAYING MACHINES IN THE IPFS SYSTEM" << endl << endl;
		cout << "The Total Possible Machines are:  " << adding(this->totalMachines, "1") << endl << endl;
		cout << "The Total Number of Machines are:  " << this->machine_entered_count << endl << endl;
		for (string i = "0"; i != machine_entered_count; i = adding(i, "1"))
		{
			cout << temp->getName() << endl;
			temp = temp->getNext();
		}
	}

	bool deleteMachine(string name)
	{
		if (this->deletemachine_helper(name) == false)
		{
			cout << "Machine does not exist." << endl;
			return false;
		}
		else
		{
			cout << "Machine deleted." << endl;
			this->machine_entered_count = subtracting(this->machine_entered_count, "1");
			this->update_routing_table();
			this->folder_deletion(name);
			return true;
		}
	}

	// Goes to machine
	// checking if available
	// routing table gets to correct machine
	// finds if present and deletes
	void deleting_file(string hash_name, string machine_name)
	{
		cout << "Going to the specific machine." << endl;
		Machine* copy_head = this->headMachine;
		do
		{
			if (copy_head->getName() == machine_name)
			{
				break;
			}
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
		cout << endl << "----------------------------------------------" << endl;
		if (copy_head->getName() != machine_name)
		{
			cout << "Error. Machine not found" << endl;
			return;
		}
		cout << "Searching at Machine: " << copy_head->getName() << endl;
		cout << endl << "----------------------------------------------" << endl;
		string ans = "";
		Node* duplicates_btree = copy_head->getBtree()->search(hash_name, ans);
		if (ans != "" || duplicates_btree != NULL)
		{
			if (duplicates_btree == NULL)
			{
				cout << "File deleted: " << ans;

				remove(ans.c_str());
				copy_head->getBtree()->Delete(hash_name, ans);

			}
			else
			{
				cout << "Multiple Files deleted are attached below: " << endl;
				//cout << "->   " << ans << endl;
				copy_head->getBtree()->Delete(hash_name, duplicates_btree->value);

				while (duplicates_btree != NULL)
				{
					cout << "->   " << duplicates_btree->value << endl;
					remove(duplicates_btree->value.c_str());
					duplicates_btree = duplicates_btree->next;
				}
			}
			cout << endl;
			copy_head->getBtree()->printTree();
			cout << endl;
			return;
		}
		else
		{
			cout << "Search un-successfull!!!" << endl;
			cout << "Searching in rest of the machines worldwide." << endl;
			bool full_pass = false;
			if (hash_name < copy_head->getName())
				full_pass = true;
			while (true)
			{
				if ((copy_head->getNext()->getName() >= hash_name && full_pass == false) || (copy_head->getNext() == headMachine && copy_head->getName() < hash_name) || (copy_head->getNext() == headMachine && copy_head->getNext()->getName() >= hash_name))
				{
					if (copy_head->getName() == hash_name)
					{
						Node* duplicates_btree = copy_head->getBtree()->search(hash_name, ans);
						if (ans != "" || duplicates_btree != NULL)
						{
							if (duplicates_btree == NULL)
							{
								cout << "File deleted: " << ans << endl;
								remove(ans.c_str());
								copy_head->getBtree()->Delete(hash_name, ans);

							}

							else
							{
								cout << "Multiple Files deleted are attached below: " << endl;
								//cout << "->   " << ans << endl;
								copy_head->getBtree()->Delete(hash_name, duplicates_btree->value);
								while (duplicates_btree != NULL)
								{
									cout << "->   " << duplicates_btree->value << endl;
									remove(duplicates_btree->value.c_str());
									duplicates_btree = duplicates_btree->next;
								}

							}
							cout << endl;
							copy_head->getNext()->getBtree()->printTree();
							cout << endl;
						}
						else
						{

							cout << endl << "File not found." << endl << endl;
						}
					}
					else
					{
						Node* duplicates_btree = copy_head->getNext()->getBtree()->search(hash_name, ans);
						if (ans != "" || duplicates_btree != NULL)
						{
							if (duplicates_btree == NULL)
							{
								cout << "File deleted: " << ans << endl;
								remove(ans.c_str());
								copy_head->getNext()->getBtree()->Delete(hash_name, ans);

							}

							else
							{
								cout << "Multiple Files deleted are attached below: " << endl;
								//cout << "->   " << ans << endl;
								copy_head->getNext()->getBtree()->Delete(hash_name, duplicates_btree->value);
								while (duplicates_btree != NULL)
								{
									cout << "->   " << duplicates_btree->value << endl;
									remove(duplicates_btree->value.c_str());
									duplicates_btree = duplicates_btree->next;
								}

							}
							cout << endl;
							copy_head->getNext()->getBtree()->printTree();
							cout << endl;
						}
						else
						{
							cout << "File not found." << endl;
						}
					}
					return;
				}

			}
			RoutingNode* routing_pos = copy_head->getTable();
			while (true) // for routing table
			{
				if (routing_pos->getNext() == NULL)
				{
					copy_head = routing_pos->getMachine();
					cout << "You just got routed to Machine:  " << copy_head->getName() << endl;
					break;
				}
				else if ((routing_pos->getNext()->getMachine()->getName() >= hash_name && full_pass == false) || (routing_pos->getMachine()->getNext() == headMachine && hash_name > routing_pos->getMachine()->getName()))
				{
					copy_head = routing_pos->getMachine();
					cout << "You just got routed to Machine:  " << copy_head->getName() << endl;
					break;
				}
				else
				{
					routing_pos = routing_pos->getNext();
					if (routing_pos->getMachine() == headMachine || (routing_pos->getMachine()->getName() < routing_pos->getPrev()->getMachine()->getName()))
					{
						if ((routing_pos->getMachine()->getName() < routing_pos->getPrev()->getMachine()->getName()))
							routing_pos = routing_pos->getPrev();
						full_pass = false;
					}
				}
			}



		}
	}

	// Goes to machine
	// checking if present
	// routing table gets to correct machine
	// finds if present and displays
	void Searching(string hash_name, string machine_name)
	{
		cout << "Going to the specific machine." << endl;
		Machine* copy_head = this->headMachine;
		do
		{
			if (copy_head->getName() == machine_name)
			{
				break;
			}
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
		cout << endl << "----------------------------------------------" << endl;
		if (copy_head->getName() != machine_name)
		{
			cout << endl << "Error. Machine not found" << endl << endl;
			return;
		}
		cout << "Searching at Machine: " << copy_head->getName() << endl;
		cout << endl << "----------------------------------------------" << endl;
		string ans = "";
		Node* duplicates_btree = copy_head->getBtree()->search(hash_name, ans);
		if (ans != "" || duplicates_btree != NULL)
		{
			cout << "Search successfull!!!" << endl;
			if (duplicates_btree == NULL)
			{
				cout << "File found: " << ans;
			}
			else
			{
				cout << "Multiple Files found are attached below: " << endl;
				//cout << "->   " << ans << endl;
				while (duplicates_btree != NULL)
				{
					cout << "->   " << duplicates_btree->value << endl;
					duplicates_btree = duplicates_btree->next;
				}
			}
			return;
		}
		else
		{
			cout << "Search un-successfull!!!" << endl;
			cout << "Searching in rest of the machines worldwide." << endl;
			bool full_pass = false;
			if (hash_name < copy_head->getName())
				full_pass = true;
			while (true)
			{
				if ((copy_head->getNext()->getName() >= hash_name && full_pass == false) || (copy_head->getNext() == headMachine && copy_head->getName() < hash_name) || (copy_head->getNext() == headMachine && copy_head->getNext()->getName() >= hash_name))
				{
					if (copy_head->getName() == hash_name)
					{
						//						cout << "You just got routed to Machine: " << copy_head->getNext()->getName() << endl;
						Node* duplicate = copy_head->getNext()->getBtree()->search(hash_name, ans);
						if (ans != "" || duplicates_btree != NULL)
						{
							cout << "Search successfull!!!" << endl;
							if (duplicates_btree == NULL)
								cout << "File found: " << ans;

							else
							{
								cout << "Multiple Files found are attached below: " << endl;
								//cout << "->   " << ans << endl;
								while (duplicates_btree != NULL)
								{
									cout << "->   " << duplicates_btree->value << endl;
									duplicates_btree = duplicates_btree->next;
								}
							}
						}
						else
						{
							cout << "File not found." << endl;
						}
					}
					else
					{
						cout << "You just got routed to Machine: " << copy_head->getNext()->getName() << endl;
						Node* duplicate = copy_head->getNext()->getBtree()->search(hash_name, ans);
						if (ans != "" || duplicates_btree != NULL)
						{
							cout << "Search successfull!!!" << endl;
							if (duplicates_btree == NULL)
								cout << "File found: " << ans;

							else
							{
								cout << "Multiple Files found are attached below: " << endl;
								//cout << "->   " << ans << endl;
								while (duplicates_btree != NULL)
								{
									cout << "->   " << duplicates_btree->value << endl;
									duplicates_btree = duplicates_btree->next;
								}
							}
						}
						else
						{
							cout << "File not found." << endl;
						}
					}
					return;
				}

				RoutingNode* routing_pos = copy_head->getTable();
				while (true) // for routing table
				{
					if (routing_pos->getNext() == NULL)
					{
						copy_head = routing_pos->getMachine();
						cout << "You just got routed to Machine:  " << copy_head->getName() << endl;
						break;
					}
					else if ((routing_pos->getNext()->getMachine()->getName() >= hash_name && full_pass == false) || (routing_pos->getMachine()->getNext() == headMachine && hash_name > routing_pos->getMachine()->getName()))
					{
						copy_head = routing_pos->getMachine();
						cout << "You just got routed to Machine:  " << copy_head->getName() << endl;
						break;
					}
					else
					{
						routing_pos = routing_pos->getNext();
						if (routing_pos->getMachine() == headMachine || (routing_pos->getMachine()->getName() < routing_pos->getPrev()->getMachine()->getName()))
						{
							if ((routing_pos->getMachine()->getName() < routing_pos->getPrev()->getMachine()->getName()))
								routing_pos = routing_pos->getPrev();
							full_pass = false;
						}
					}
				}


			}
		}
	}

	// Goes to machine
	// routing table gets to correct machine
	// inserts
	void inserting_file(string hash, string file_path, string machine_name)
	{
		cout << "File hash:  " << hash << endl;
		Machine* prev_copy_head = NULL;
		cout << "Going to specific machine" << endl;
		Machine* copy_head = this->headMachine;
		do
		{
			if (copy_head->getName() == machine_name)
			{
				break;
			}
			prev_copy_head = copy_head;
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
		cout << endl << "----------------------------------------------" << endl;
		if (copy_head->getName() != machine_name)
		{
			cout << "Error. Machine not found" << endl;
			return;
		}
		cout << "Machine Name:  " << copy_head->getName() << endl;
		if (prev_copy_head == NULL)
		{
			prev_copy_head = headMachine;
			while (prev_copy_head->getNext() != copy_head)
			{
				prev_copy_head = prev_copy_head->getNext();
			}
		}
		if ((prev_copy_head == prev_copy_head->getNext()) || (hash > prev_copy_head->getName() && hash <= copy_head->getName()) || (hash > prev_copy_head->getName() && prev_copy_head->getNext() == headMachine) || (copy_head == headMachine && copy_head->getName() >= hash))
		{
			string file_path_updated = updating_file_path(file_path, copy_head->getName());
			cout << "File is to be inserted here. No need for any routing." << endl;
			copy_head->getBtree()->insert(hash, file_path_updated);
			cout << "File inserted successfully at: " << copy_head->getName() << endl << endl;
			copy_head->getBtree()->printTree();
			cout << endl;
			return;
		}
		bool full_pass = false;
		if (hash < copy_head->getName())
			full_pass = true;
		while (true)
		{
			if ((copy_head->getNext()->getName() >= hash && full_pass == false) || (copy_head->getNext() == headMachine && copy_head->getName() < hash) || (copy_head->getNext() == headMachine && copy_head->getNext()->getName() >= hash))
			{
				if (copy_head->getName() == hash)
				{
					//cout << "You just got routed to Machine: " << copy_head->getName() << endl;
					string file_path_updated = updating_file_path(file_path, copy_head->getName());
					cout << "File is to be inserted here. No need for any further routing." << endl;
					copy_head->getBtree()->insert(hash, file_path_updated);
					cout << "File inserted successfully at: " << copy_head->getName() << endl << endl;
					copy_head->getBtree()->printTree();
					cout << endl;
				}
				else
				{
					cout << "You just got routed to Machine: " << copy_head->getNext()->getName() << endl;
					string file_path_updated = updating_file_path(file_path, copy_head->getNext()->getName());
					cout << "File is to be inserted here. No need for any further routing." << endl;
					copy_head->getNext()->getBtree()->insert(hash, file_path_updated);
					cout << "File inserted successfully at: " << copy_head->getNext()->getName() << endl << endl;
					copy_head->getNext()->getBtree()->printTree();
					cout << endl;
				}
				return;
			}

			/*else if (copy_head->getName() < hash && copy_head->getNext() == headMachine)
			{
				cout << "You just got routed to: " << copy_head->getNext()->getName() << endl;
				string file_path_updated = updating_file_path(file_path, copy_head->getNext()->getName());
				cout << "File is to be inserted here. No need for any further routing." << endl;
				copy_head->getBtree()->insert(hash, file_path_updated);
				cout << "File inserted successfully at: " << copy_head->getNext()->getName() << endl;
				return;
			}
			else if (copy_head->getNext()->getName() >= hash && copy_head->getNext() == headMachine)
			{
				cout << "You just got routed to: " << copy_head->getNext()->getName() << endl;
				string file_path_updated = updating_file_path(file_path, copy_head->getNext()->getName());
				cout << "File is to be inserted here. No need for any further routing." << endl;
				copy_head->getBtree()->insert(hash, file_path_updated);
				cout << "File inserted successfully at: " << copy_head->getNext()->getName() << endl;
				return;
			}*/
			RoutingNode* routing_pos = copy_head->getTable();
			while (true) // for routing table
			{
				if (routing_pos->getNext() == NULL)
				{
					copy_head = routing_pos->getMachine();
					cout << "You just got routed to Machine:  " << copy_head->getName() << endl;
					break;
				}
				else if ((routing_pos->getNext()->getMachine()->getName() >= hash && full_pass == false) || (routing_pos->getMachine()->getNext() == headMachine && hash > routing_pos->getMachine()->getName()))
				{
					copy_head = routing_pos->getMachine();
					cout << "You just got routed to Machine:  " << copy_head->getName() << endl;
					break;
				}
				else
				{
					routing_pos = routing_pos->getNext();
					if (routing_pos->getMachine() == headMachine || (routing_pos->getMachine()->getName() < routing_pos->getPrev()->getMachine()->getName()))
					{
						if ((routing_pos->getMachine()->getName() < routing_pos->getPrev()->getMachine()->getName()))
							routing_pos = routing_pos->getPrev();
						full_pass = false;
					}
				}
			}

		}
	}

	void print_btree_inorder(string machine_name)
	{
		Machine* copy_head = headMachine;
		do {
			if (copy_head->getName() == machine_name)
			{
				cout << "For Machine: " << copy_head->getName() << endl;
				copy_head->getBtree()->inOrder();
				cout << "--------------------------------------------------------------" << endl;
				return;
			}
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
		cout << "Machine by this name doesn't exist." << endl;
	}

	void print_btree_inorder()
	{
		cout << "-------------------------------------------------------------" << endl;
		cout << "INORDER TRAVERSAL" << endl;
		Machine* copy_head = headMachine;
		do {
			cout << "For Machine: " << copy_head->getName() << endl;
			copy_head->getBtree()->inOrder();
			cout << "--------------------------------------------------------------" << endl;
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
	}

	void print_btree_levelorder(string machine_name)
	{
		Machine* copy_head = headMachine;
		do {
			if (copy_head->getName() == machine_name)
			{
				cout << "For Machine: " << copy_head->getName() << endl;
				copy_head->getBtree()->levelOrderTraversal();
				cout << "--------------------------------------------------------------" << endl;
				return;
			}
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
		cout << "Machine by this name doesn't exist." << endl;
	}

	void print_btree_levelorder()
	{
		cout << "-------------------------------------------------------------" << endl;
		cout << "LEVEL TRAVERSAL" << endl;
		Machine* copy_head = headMachine;
		do {
			cout << "For Machine: " << copy_head->getName() << endl;
			copy_head->getBtree()->levelOrderTraversal();
			cout << "--------------------------------------------------------------" << endl;
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
	}

	void print_btree_expanded()
	{
		cout << "-------------------------------------------------------------" << endl;
		cout << "EXPANDED TRAVERSAL" << endl;
		Machine* copy_head = headMachine;
		do {
			cout << "For Machine: " << copy_head->getName() << endl;
			copy_head->getBtree()->printTree();
			cout << endl << "--------------------------------------------------------------" << endl;
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
	}

	void print_btree_expanded(string machine_name)
	{
		Machine* copy_head = headMachine;
		do {
			if (copy_head->getName() == machine_name)
			{
				cout << "For Machine: " << copy_head->getName() << endl;
				copy_head->getBtree()->printTree();
				cout << "--------------------------------------------------------------" << endl;
				return;
			}
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
		cout << "Machine by this name doesn't exist." << endl;
	}

	void display_routing_table()
	{
		Machine* copy_head = this->headMachine;
		for (string i = "0"; i != this->machine_entered_count; i = this->adding(i, "1"))
		{
			cout << "ROUTING TABLE FOR " << copy_head->getName() << ": " << endl;
			copy_head->display_routing_table_inmachine();
			copy_head = copy_head->getNext();
			cout << endl;
		}
	}

	void display_routing_table(string machine_name)
	{
		Machine* copy_head = this->headMachine;
		do
		{
			if (copy_head->getName() == machine_name)
			{
				cout << "ROUTING TABLE FOR " << copy_head->getName() << ": " << endl;
				copy_head->display_routing_table_inmachine();
				cout << endl;
				return;
			}
			copy_head = copy_head->getNext();
		} while (copy_head != headMachine);
		cout << "No Machine with name " << machine_name << " found." << endl;
	}

};