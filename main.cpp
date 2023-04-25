#include <iostream>
#include <vector>
#include <regex>
#include <queue>

using namespace std;

class Node
{
    string name;
    int id;
public:
    Node *right;
    Node *left;
    // Default constructor.
    Node();
    // Parameterized constructor.
    Node(string name, int id);
    ~Node();
    //get private data
    string GetName();
    int GetID();
    void Transfer(Node* endNode);
};
string Node::GetName(){
    return name;

};
int Node::GetID(){
    return id;
};
void Node::Transfer(Node* copy){
    this->name = copy->name;
    this->id = copy->id;
}
Node::Node() {
    left = nullptr;
    right = nullptr;
    id = 0;
    name = "";
}
Node::Node(string name, int fid) {
    this->name  = name;
    id  = fid;
    left = nullptr;
    right = nullptr;
}
Node::~Node() {
    delete right;
    delete left;
}
int checkHeight(Node*Node){
    if (Node == NULL)
        return 0;
    return 1 + max(checkHeight(Node->left), checkHeight(Node->right));
}
int BalanceFactor(Node* root) {
    // if negative balance factor Right side is too long
    // if postive balance factor Left side is too long
    int lheight, rheight;
    if (root == NULL) {
        return 0;
    } else {
        lheight = checkHeight(root->left);
        rheight = checkHeight(root->right);
        return (lheight - rheight);
    }
}
//(step by step)
Node* LeftRotation(Node* node){
    //value we return (will be the new root)
    Node* newParent = node-> right;
    //saving node of any potential value in the Left child of the NEW Parent
    Node* temp2 = newParent->left;
    //moving old root value to the left child of the NEW parent
    newParent->left = node;
    //setting TEMP2 as right child of the NEW parent's left child
    node->right  = temp2;
    return newParent;
}
Node* RightRotation(Node* node){
    //value we return (will be the new root)
    Node* newParent = node->left;
    //saving node of any potential value in the Right child of NEW Parent
    Node* temp2 = newParent->right;
    //moving old root value to the right child of the NEW parent
    newParent->right = node;
    //setting TEMP2 as the left child of the NEW parent's right child
    node->left = temp2;
    return newParent;
}

//Right-Left case
Node* RightLeftRotation(Node* node){
    node->right = RightRotation(node->right);
    return LeftRotation(node);

}
//Left-Right case
Node* LeftRightRotation(Node* node){
    node->left = LeftRotation(node->left);
    return RightRotation(node);
}
Node* balanceTree(Node* Node){
    if (BalanceFactor(Node) >= 2) {
        if (BalanceFactor(Node->left) >= 1){
            return RightRotation(Node);
        }
        else if (BalanceFactor(Node->left) <= -1){
            return LeftRightRotation(Node);
        }
    } else if (BalanceFactor(Node) <= -2) {
        if (BalanceFactor(Node->right) <= -1){
            return LeftRotation(Node);
        }
        else if (BalanceFactor(Node->right) >= 1){
            return RightLeftRotation(Node);
        }
    }
    return Node;
}
Node* insert(Node *root, string name, int fid) {
    Node *temp = root;
    if (root == NULL) {
        return new Node(name, fid);
    } else if (fid < root->GetID()) {
        root->left = insert(temp->left, name, fid);
        root = balanceTree(root);
    } else if (fid > root->GetID()) {
        root->right = insert(temp->right, name, fid);
        root = balanceTree(root);
    }

    return root;
}
bool OnlyLetters(string &input) {
    return regex_match(input, regex("^[a-zA-Z ]*$"));
}
string extractName(string Line){
    bool start = false,end = false;
    string name;
    for(char x : Line){
        if(x == '"' && start){
            end=true;
        }
        else if(x == '"' && !end){
            start=true;
        }
        else if(start && !end){
            name.push_back(x);
        }
    }
    return name;
}
void SearchName (Node* root, string searchName){
    bool found = false;
    if(root == NULL){
        cout<< "unsuccessful" << endl;
        return;
    }
    queue<Node*> q;
    q.push(root);
    while (!q.empty()){
        Node* temp = q.front();
        if(temp->left != NULL){
            q.push(temp->left);
        }
        if(temp->right != NULL){
            q.push(temp->right);
        }
        if(temp->GetName() != searchName){
            q.pop();
        }
        if(temp->GetName()  == searchName){
            found = true;
            cout<< temp->GetID()<<endl;
            q.pop();
        }
    }
    if(!found){
        cout<< "unsuccessful" << endl;
    }
}
Node* SearchID (Node* root, int id){
    if(root == NULL){
        return NULL;
    }

    if(root->GetID() == id){
        return root;
    }

    if (id > root->GetID()){
        return SearchID(root->right, id);
    }

    if (id < root->GetID()< id){
        return SearchID(root->left, id);
    }
    if(root->left == NULL && root->right == NULL){
        return NULL;
    }

}
Node* Succesor(Node* root){
    while(root->left != NULL){
        root = Succesor(root->left);
    }
    if(root->left == NULL){
        return root;
    }
}
Node* removeNode(Node*root, int id){
    if(id < root->GetID()){
        root->left = removeNode(root->left, id);
    }
    else if(id > root->GetID()){
        root->right =  removeNode(root->right, id);
    }
    else if (id == root->GetID()){
        //no children
        if(!root->left && !root->right){
            return NULL;
        }
            //Right Node is empty
        else if(!root->left){
            Node *temp = root->right;
            free(root);
            return temp;
        }
            //Left Node is empty
        else if(!root->right) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
            //2 children
        else{
            //finding succesor
            Node* temp = Succesor(root->right);
            //copy over data
            root->Transfer(temp);
            //remove previous duplicate successor
            root->right = removeNode(root->right, temp->GetID());
            return root;
        }
    }
    return root;
}
void PrintInorder(Node* root, bool lastNode){
    if(root != NULL){

        PrintInorder(root->left, false);
        cout<< root->GetName();
        if(!lastNode || root->right != NULL){
            cout<< ", ";
        }
        PrintInorder(root->right,lastNode);
    }
    return;
}
void PrintPreorder(Node* root){
    if(root==NULL){
        return;
    }
    cout<<root->GetName();
    //if not end of Substring line
    if(root->left || root->right){
        cout << ", ";
    }
    PrintPreorder(root->left);
    //if not end of right subtring
    if(root->left && root->right){
        cout << ", ";
    }
    PrintPreorder(root->right);
}
void PrintPostorder(Node* root, bool lastLeftNode, bool lastRightNode){
    if(root != NULL){

        PrintPostorder(root->left, false, lastRightNode);
        PrintPostorder(root->right,lastLeftNode, false);

        if(lastLeftNode && lastRightNode){
            cout<< root->GetName();
        }
        else{
            cout<< root->GetName() << ", ";
        }

    }
    return;
}
Node* findInOrder(Node* root, int& nthNode, Node* NthNode){
    if(root != NULL){
        NthNode = findInOrder(root->left, nthNode, NthNode);
        nthNode-=1;
        if(nthNode == 0){
            NthNode = root;
            return root;
        }
        NthNode = findInOrder(root->right,nthNode, NthNode);
    }
    return NthNode;
}

int main() {
    Node *root = nullptr;
    int numOfCommands;
    string input;
    getline(cin, input);
    numOfCommands = stoi(input);
    int id;
    bool validId, validName, removed;

    for (int x = 0; x < numOfCommands; x++) {
        string Line, name;
        getline(cin, Line);
        //inserting
        if (Line.substr(0, 7) == "insert ") {
            name = extractName(Line);
            if (OnlyLetters(name)) {
                validName = true;
            }
            if (regex_match(&Line[(Line.size() - 8)], regex("^[0-9]{8}$")) && Line[Line.size() - 9] == ' ') {
                id = stoi(Line.substr(Line.size() - 8));
                if (SearchID(root, id) == NULL) {
                    validId = true;
                } else {
                    validId = false;
                }
            }
            else {
                validId = false;
            }

            //insert first Node
            if (validName && validId && root == NULL) {
                root = insert(root, name, id);
                cout << "successful" << endl;
            }
                //insert other Nodes
            else if (validName && validId) {
                //root will always be the top value
                root = insert(root, name, id);
                cout << "successful" << endl;
            } else {
                cout << "unsuccessful" << endl;
            }
        }

            //searching for ID/NAME
        else if (Line.substr(0, 7) == "search ") {
            Line.erase(0, 7);
            //finding ID
            if (regex_match(&Line[0], regex("^[0-9]{8}$"))) {
                id = stoi(Line.substr(Line.size() - 8));
                Node *Result = SearchID(root, id);
                if (Result != NULL) {
                    cout << Result->GetName() << endl;
                } else {
                    cout << "unsuccessful" << endl;
                }
            }
                //finding name
            else {
                name = extractName(Line);
                if (OnlyLetters(name)) {
                    SearchName(root, name);
                } else {
                    cout << "unsuccessful" << endl;
                }
            }
        }

            //Printing data
        else if (Line == "printInorder") {
            PrintInorder(root, true);
            cout << endl;
        }
        else if (Line == "printPreorder") {
            PrintPreorder(root);
            cout << endl;
        }
        else if (Line == "printPostorder") {
            PrintPostorder(root, true, true);
            cout << endl;
        }
        else if (Line == "printLevelCount") {
            if(root == NULL){
                cout<< 0 <<endl;
                break;
            }
            cout<<checkHeight(root)<< endl;
        }

            // Removing
        else if (Line.substr(0, 7) == "remove ")
        {
            Line.erase(0, 6);
            if (Line[Line.size() - 9] == ' ') {
                Line.erase(0, 1);
                if (regex_match(&Line[0], regex("^[0-9]{8}$"))) {
                    id = stoi(Line.substr(Line.size() - 8));
                    if (SearchID(root, id)) {
                        validId = true;
                    }
                    else {
                        validId = false;
                        cout << "unsuccessful" << endl;
                    }
                }
            }
            if (validId) {
                root = removeNode(root, id);
                cout << "successful" << endl;
            }
        }
        else if (Line.substr(0, 14) == "removeInorder ") {
            Line.erase(0, 14);
            int nthNode = stoi(Line);
            nthNode++;
            Node *temp = findInOrder(root, nthNode, NULL);
            if (temp) {
                root = removeNode(root, temp->GetID());
                cout << "successful" << endl;
            } else {
                cout << "unsuccessful" << endl;
            }
        }
        validId = false;
        validName =false;
    }
    delete root;
    return 0;

}