#include <iostream>
#include <string>
#include <fstream>

class Person {
	std::string name;
	char gender;
	int age;
public:
	Person() { name = ""; gender = NULL; age = NULL; }
	Person(std::string name, char gender, int age)
	{
		this->name = name; this->gender = gender; this->age = age;
	}
	std::string get_name() { return name; }
	char get_gender() { return gender; }
	int get_age() { return age; }
};

class Node {
	Person person;
	char color;
	Node *right;
	Node *left;
	Node *parent;
	int number_of_man;
	int number_of_woman;
public:
	Node() { right = NULL; left = NULL; parent = NULL; color = 'B'; number_of_man = number_of_woman = 0; }
	Node(Person person)
	{
		this->person = person;
		right = left = parent = NULL;
		number_of_woman = number_of_man = 0;
	}
	//Geters and Setters to edit node connections of a Node
	Node *get_right() { return right; }
	Node *get_left() { return left; }
	Node *get_parent() { return parent; }
	Person get_person() { return person; }
	char get_color() { return color; }
	void set_right(Node *right) { this->right = right; }
	void set_left(Node *left) { this->left = left; }
	void set_parent(Node *parent) { this->parent = parent; }
	void set_color(char color) { this->color = color; }
	/*This function gets name of the node's person. I created this function
	because when doing insertion, I need to compare people's names.*/
	std::string get_person_name() { return person.get_name(); }
	void increase_number_of_man() { number_of_man++; }
	void increase_number_of_woman() { number_of_woman++; }
};

class RB_Tree {
	Node *root;
	int node_count;
	std::string blank;
public:
	RB_Tree() { node_count = 0; blank = ""; }
	void insert(Node *);
	Node *insert_binary(Node *, Node *);
	Node *get_parent(Node *, Node *, Node*);
	Node *get_uncle(Node*);
	void right_rotation(Node *);
	void left_rotation(Node *);
	void swap_colors(Node *, Node *);
	void fix_tree(Node *);
	void print(Node * ,int);
	Node *get_root() { return root; }
	Node *nth_woman(int);
	Node *nth_man(int);
	void update_numbers(Node *, char);
};

/*Decleration of the Binary insert function. Find an empty suitable leaf and 
put node in there. Ignore rb operations.*/
Node *RB_Tree::insert_binary(Node *subroot, Node *in_node)
{
	if (subroot == NULL)
		return  in_node;
	if (in_node->get_person_name() < subroot->get_person_name())
		subroot->set_left(insert_binary(subroot->get_left(), in_node));
	else
		subroot->set_right(insert_binary(subroot->get_right(), in_node));
}
//Decleration of the insert which is the member function of RB_Tree class.
void RB_Tree::insert(Node *in_node)
{
	//If there is no node in the tree, inserted node will be the root.
	if (node_count == 0)
	{
		//Root node is always black.
		in_node->set_color('B');
		root = in_node;
		node_count++;
	}
	else
	{
		//First insert element into the right place.
		insert_binary(root, in_node);
		//Set its parent node.
		in_node->set_parent(get_parent(root, in_node, NULL));
		//Set the color of the new element to RED
		in_node->set_color('R');
		fix_tree(in_node);
		node_count++;
	}
}

/*I want to keep parent information inside the node. So I create this function
to learn a node's parent node.*/

void RB_Tree::fix_tree(Node* node)
{
	if (node == root) node->set_color('B');
	if (root != node && node->get_parent()->get_color() != 'B')
	{
		if (get_uncle(node) != NULL && get_uncle(node)->get_color() == 'R')
		{
			node->get_parent()->set_color('B');
			get_uncle(node)->set_color('B');
			node->get_parent()->get_parent()->set_color('R');
			fix_tree(node->get_parent()->get_parent());
		}
		else
		{
			if (node->get_parent() == node->get_parent()->get_parent()->get_left() &&
				node == node->get_parent()->get_left())
			{
				swap_colors(node->get_parent()->get_parent(), node->get_parent());
				right_rotation(node->get_parent()->get_parent());
			}
			else if (node->get_parent() == node->get_parent()->get_parent()->get_left() &&
				node == node->get_parent()->get_right())
			{
				left_rotation(node->get_parent());
				right_rotation(node->get_parent());
				swap_colors(node, node->get_right());
			}
			else if (node->get_parent() == node->get_parent()->get_parent()->get_right() &&
				node == node->get_parent()->get_right())
			{
				swap_colors(node->get_parent()->get_parent(), node->get_parent());
				left_rotation(node->get_parent()->get_parent());
			}
			else if (node->get_parent() == node->get_parent()->get_parent()->get_right() &&
				node == node->get_parent()->get_left())
			{
				right_rotation(node->get_parent());
				left_rotation(node->get_parent());
				swap_colors(node, node->get_left());
			}
			else
				std::cout << "Imposible situation..." << std::endl;
		}
	}
}

Node *RB_Tree::get_parent(Node *subroot, Node *in_node, Node *parent)
{
	if (subroot->get_person_name() == in_node->get_person_name())
	{
		return parent;
	}
	if (subroot->get_person_name() > in_node->get_person_name())
	{
		get_parent(subroot->get_left(), in_node, subroot);
	}
	else
	{
		get_parent(subroot->get_right(), in_node, subroot);
	}
}

Node *RB_Tree::get_uncle(Node *in_node)
{//Get node and find his uncle
	//If value of the parent smaller than grandparent, uncle is grandparent's right
	if (in_node->get_parent()->get_person_name() <
		in_node->get_parent()->get_parent()->get_person_name())
		return in_node->get_parent()->get_parent()->get_right();
	else //uncle is grandparent's left
		return in_node->get_parent()->get_parent()->get_left();
}

void RB_Tree::right_rotation(Node *node)
{
	Node *p = node->get_left();
	node->set_left(p->get_right());
	if (node->get_left() != NULL)
		node->get_left()->set_parent(node);
	p->set_parent(node->get_parent());
	if (node == node->get_parent()->get_left())
		node->get_parent()->set_left(p);
	else
		node->get_parent()->set_right(p);
	p->set_right(node);
	node->set_parent(p);
}

void RB_Tree::left_rotation(Node *node)
{
	Node *q = node->get_right();
	node->set_right(q->get_left());
	if (node->get_right() != NULL)
		node->get_right()->set_parent(node);
	q->set_parent(node->get_parent());
	if (node == node->get_parent()->get_left())
		node->get_parent()->set_left(q);
	else
		node->get_parent()->set_right(q);
	q->set_left(node);
	node->set_parent(q);
}

void RB_Tree::swap_colors(Node *node1, Node *node2)
{
	char temp = node1->get_color();
	node1->set_color(node2->get_color());
	node2->set_color(temp);
}

void RB_Tree::print(Node *node, int a)
{
	if (node->get_left() != NULL)
	{	
		blank = blank + "         ";
		print(node->get_left(), 1);
		blank.erase(blank.size() - 9, 9);
	}
	if (a == 0)
		std::cout << blank << "(" << node->get_color() << ")" << node->get_person_name()
		<< "-" << node->get_person().get_age() << "-" << node->get_person().get_gender() << std::endl;
	else if (a == 1)
		std::cout << blank << "┌───" << "(" << node->get_color() << ")" << node->get_person_name() 
		<< "-" << node->get_person().get_age() << "-" << node->get_person().get_gender() <<std::endl;
	else
		std::cout << blank << "└───" << "(" << node->get_color() << ")" << node->get_person_name()
		<< "-" << node->get_person().get_age() << "-" << node->get_person().get_gender() << std::endl;
	if (node->get_right() != NULL)
	{
		blank = blank + "         ";
		print(node->get_right(), 2);
		blank.erase(blank.size() - 9, 9);
	}
		
}

void RB_Tree::update_numbers(Node *node, char gender)
{
	if (node->get_parent() == NULL)
	{
		if (gender == 'F')
			node->increase_number_of_woman();
		else
			node->increase_number_of_woman();
	}
	if (node->get_person().get_gender() == 'F')
	{
		node->increase_number_of_woman;
		update_numbers(node->get_parent(), 'F');
	}
}

int main(int argc ,char* argv[])
{
	//Get argument text from console and open the proper file
	//std::string filename = argv[1];
	std::ifstream txt_file(argv[1]);
	//Create the red black tree.
	RB_Tree tree;
	//Temporary variables to hold information from file.
	std::string name;
	char gender;
	int age;
	//Read from file if the file is opened.
	if (txt_file.is_open())
	{
		//Do until end of the file.
		while (!txt_file.eof())
		{
			//Read line elements one by one, initilize proper variables.
			txt_file >> name;
			txt_file >> gender;
			txt_file >> age;
			//Create a person with the information from the file.
			Person person(name, gender, age);
			//Create a node with the person information.
			Node *node = new Node(person);
			//Insert node into the red black tree.
			tree.insert(node);
		}
	}
	else //Throw error message and close the program.
	{	
		std::cout << "File cannot be opened." << std::endl;
		return 1;
	}
	tree.print(tree.get_root(), 0);
	return 0;
}