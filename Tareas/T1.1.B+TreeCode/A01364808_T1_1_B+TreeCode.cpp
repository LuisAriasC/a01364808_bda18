#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>

using namespace std;
//
//
//
struct Node {
    bool isLeaf;
    bool isRoot;
    Node *parent;
    vector<int> values;
    vector<Node *> childs;
    Node *next;
};
//
//
//
struct final_Node{
    Node *parent_Node;
    bool n_leaf;
    
    final_Node(){}
    final_Node(Node *Node, bool b): parent_Node(Node), n_leaf(b){}
};
//
//
//
class BplusTree {
public:
    int num_values;
    int num_pointers;
    
    Node *Root;
    queue<final_Node> q;

    BplusTree(): num_values(0), num_pointers(0), Root(NULL){}
    
    Node* newNode(bool isLeaf, bool isRoot);
    Node* getNode(Node * actual_Node, int val);
    
    void insert(int value, Node * parent);
    void insert_to_Parent(Node *n, int kprime, Node *nprime);
    void insert_to_Leaf(Node *leaf_Node, int value, Node *new_Child);
    
    void delete_value(int k);
    
    void delete_in_Node(Node* N, int k);
    bool tooFewEntry(Node *N);
    void deleteEntry(Node* N, int k);
    bool fusionable(Node *N, Node *Nprime);
    
    void printNode(Node* tNode);
    void bfs_travel(Node *tNode);
};
//
//
//
Node* BplusTree::newNode(bool isLeaf,bool isRoot){
    Node* new_Node = new Node;
    new_Node->isRoot = isRoot;
    new_Node->isLeaf = isLeaf;
    new_Node->next = NULL;
    return new_Node;
}
//
//
//
Node* BplusTree::getNode(Node *actual_Node, int val){
    if(actual_Node->isLeaf){
        return actual_Node;
    }
    
    int i;
    
    for(i=0;i<actual_Node->values.size();i++){
        if(actual_Node->values[i]>val) break;
    }
    
    return getNode(actual_Node->childs[i],val);
}
//
//
//
void BplusTree::insert_to_Leaf(Node *leaf_Node, int value, Node *Child){
    int i;
    
    for(i=0;i<leaf_Node->values.size();i++){
        if(value < leaf_Node->values[i]) break;
    }
    
    int aux_value;
    Node *aux_Child;
    
    for(int j = i; j<leaf_Node->values.size(); j++){
        aux_Child = leaf_Node->childs[j];
        aux_value = leaf_Node->values[j];
        
        leaf_Node->childs[j] = Child;
        leaf_Node->values[j] = value;
        
        Child = aux_Child;
        value = aux_value;
    }
    
    leaf_Node->childs.push_back(Child);
    leaf_Node->values.push_back(value);
}
//
//
//
void BplusTree::insert_to_Parent(Node *leaf_Node, int up_value, Node *rest_Node){
    
    if (leaf_Node->isRoot) {
        Root = newNode(false,true);
        leaf_Node->isRoot=false;
        
        Root->childs.push_back(leaf_Node);
        Root->childs.push_back(rest_Node);
        Root->values.push_back(up_value);
        leaf_Node->parent = Root;
        rest_Node->parent = Root;
    } else {
        Node *new_Parent = leaf_Node->parent;
        
        int i;
        for( i=0; i < new_Parent->values.size(); i++){
            if (new_Parent->values[i] > up_value) break;
        }
        
        int aux_up_value;
        Node *aux_rest_Node;
        
        for(int j = i; j < new_Parent->values.size(); j++){
            aux_up_value = new_Parent->values[j];
            aux_rest_Node = new_Parent->childs[j+1];
            
            new_Parent->values[j] = up_value;
            new_Parent->childs[j+1] = rest_Node;
            
            up_value = aux_up_value;
            rest_Node = aux_rest_Node;
        }
        
        new_Parent->values.push_back(up_value);
        new_Parent->childs.push_back(rest_Node);
        rest_Node->parent = new_Parent;
        
        if (new_Parent->childs.size() > num_pointers) {
            
            Node *aux_Parent = newNode(false,false);
            int middle_index = (num_pointers+1) / 2;
            int aux_upper_value = new_Parent->values[middle_index-1];
            
            for(i = middle_index; i < new_Parent->values.size(); i++){
                aux_Parent->childs.push_back(new_Parent->childs[i]);
                new_Parent->childs[i]->parent = aux_Parent;
                aux_Parent->values.push_back(new_Parent->values[i]);
            }
            aux_Parent->childs.push_back(new_Parent->childs[i]);
            new_Parent->childs[i]->parent = aux_Parent;
            
            new_Parent->values.erase(new_Parent->values.begin()+middle_index-1, new_Parent->values.end());
            new_Parent->childs.erase(new_Parent->childs.begin()+middle_index, new_Parent->childs.end());
            
            insert_to_Parent(new_Parent,aux_upper_value,aux_Parent);
        }
    }
}
//
//
//
void BplusTree::delete_in_Node(Node* actual_Node, int delete_value){
    int i;
    for(i=0; i < actual_Node->values.size(); i++){
        if(actual_Node->values[i]==delete_value) break;
    }
    actual_Node->values.erase(actual_Node->values.begin()+i);
    
    if(!actual_Node->isLeaf)
        actual_Node->childs.erase(actual_Node->childs.begin()+i+1);
}
//
//
//
bool BplusTree::tooFewEntry(Node *N){
    
    if(N->isLeaf){
        int minV = num_pointers/2;
        if(N->values.size()<minV) return true;
    }else{
        int minC = (num_pointers+1)/2;
        if(N->childs.size()<minC) return true;
    }
    return false;
}
//
//
//
void BplusTree::deleteEntry(Node* actual_Node, int delete_value){
    
    delete_in_Node(actual_Node,delete_value);
    if(actual_Node->isRoot && actual_Node->childs.size()==1){
        Root = actual_Node->childs[0];
        Root->parent = NULL;
        Root->isRoot = true;
    }
    if(tooFewEntry(actual_Node) && !actual_Node->isRoot){
        
        Node *parent_Node = actual_Node->parent;
        
        int i;
        for(i=0;i<parent_Node->childs.size();i++){
            if(parent_Node->childs[i]==actual_Node) break;
        }
        
        Node *leftSib, *rightSib, *aux_actual_Node;
        bool left;
        
        if(i>0)
            leftSib = parent_Node->childs[i-1];
        else
            leftSib = NULL;
        
        if(i < parent_Node->childs.size()-1)
            rightSib = parent_Node->childs[i+1];
        else
            rightSib = NULL;
        
        if(leftSib==NULL)
            left=false;
        else if(rightSib==NULL)
            left = true;
        else if(leftSib->values.size()>rightSib->values.size())
            left=false;
        else
            left = true;
        
        int aux_delete_value;
        
        if(left){
            aux_actual_Node = leftSib;
            aux_delete_value = parent_Node->values[i-1];
        }
        else {
            aux_actual_Node = rightSib;
            aux_delete_value = parent_Node->values[i];
        }
        
        if(fusionable(actual_Node,aux_actual_Node)){
            if(!left){
                Node *tmp = actual_Node;
                actual_Node = aux_actual_Node;
                aux_actual_Node = tmp;
            }
            if(!actual_Node->isLeaf){
                aux_actual_Node->values.push_back(aux_delete_value);
                int j;
                for(j = 0; j < actual_Node->values.size();j++){
                    aux_actual_Node->childs.push_back(actual_Node->childs[j]);
                    aux_actual_Node->values.push_back(actual_Node->values[j]);
                    actual_Node->childs[j]->parent = aux_actual_Node;
                }
                aux_actual_Node->childs.push_back(actual_Node->childs[j]);
                actual_Node->childs[j]->parent = aux_actual_Node;
            }
            else {
                
                for(int j = 0; j<actual_Node->values.size();j++){
                    aux_actual_Node->values.push_back(actual_Node->values[j]);
                }
                aux_actual_Node->next = actual_Node->next;
            }
            deleteEntry(parent_Node,aux_delete_value);
            
        }
        else {
            if(left){
                
                if(!actual_Node->isLeaf){
                    int index = (int)aux_actual_Node->childs.size()-1;
                    int temp_val = aux_actual_Node->values[index-1];
                    Node *temp_parent = aux_actual_Node->childs[index];
                    aux_actual_Node->childs.erase(aux_actual_Node->childs.begin() + index);
                    aux_actual_Node->values.erase(aux_actual_Node->values.begin() + index - 1);
                    actual_Node->childs.insert(actual_Node->childs.begin(),temp_parent);
                    actual_Node->values.insert(actual_Node->values.begin(),aux_delete_value);
                    parent_Node->values[i-1] = temp_val;
                }
                else {
                    int index = (int)aux_actual_Node->values.size()-1;
                    int temp_val = aux_actual_Node->values[index];
                    aux_actual_Node->values.erase(aux_actual_Node->values.begin() + index);
                    actual_Node->values.insert(actual_Node->values.begin(), temp_val);
                    parent_Node->values[i-1] = temp_val;
                }
            }
            else {
                
                if(!actual_Node->isLeaf){
                    int index = 0;
                    int temp_val = aux_actual_Node->values[index];
                    Node *temp_parent = aux_actual_Node->childs[index];
                    
                    aux_actual_Node->childs.erase(aux_actual_Node->childs.begin() + index);
                    aux_actual_Node->values.erase(aux_actual_Node->values.begin() + index);
                    
                    actual_Node->childs.push_back(temp_parent);
                    actual_Node->values.push_back(aux_delete_value);
                    
                    parent_Node->values[i-1] = temp_val;
                }
                else {
                    int index = 0;
                    int temp_val = aux_actual_Node->values[index];
                    aux_actual_Node->values.erase(aux_actual_Node->values.begin());
                    actual_Node->values.push_back(temp_val);
                    parent_Node->values[i] = aux_actual_Node->values[0];
                }
            }
        }
        
    }
    else {
        printf("Delete finished\n");
    }
    
}
//
//
//
void BplusTree::delete_value(int value){
    Node *L = getNode(Root,value);
    printf("content: \n");
    bfs_travel(L);
    deleteEntry(L,value);
    
}
//
//
//
bool BplusTree::fusionable(Node *N, Node *Nprime){
    if(N->isLeaf){
        if(num_values>=(N->values.size()+Nprime->values.size())){
            return true;
        }
        return false;
    }
    if(num_pointers>=(N->childs.size()+Nprime->childs.size())){
        return true;
    }
    return false;
}
//
//
//
void BplusTree::insert(int value, Node *parent){
    
    Node *leafNode;
    
    if (Root == NULL) {
        Root = newNode(true, true);
        leafNode = Root;
    }
    else {
        leafNode = getNode(Root,value);
    }
    
    int keyValueCount = (int)leafNode->values.size();
    
    if(keyValueCount<num_values){
        insert_to_Leaf(leafNode,value,parent);
    }
    else{
        Node* leafNode2 = newNode(true,false);
        insert_to_Leaf(leafNode,value,parent);
        
        leafNode2->next = leafNode->next;
        leafNode2->parent = leafNode->parent;
        
        leafNode->next=leafNode2;
        int mid_index = (num_pointers+1)/2;
        
        for(int i = mid_index; i<num_pointers ; i++){
            leafNode2->childs.push_back(leafNode->childs[i]);
            leafNode2->values.push_back(leafNode->values[i]);
        }
        
        leafNode->values.erase(leafNode->values.begin() + mid_index, leafNode->values.end());
        leafNode->childs.erase(leafNode->childs.begin() + mid_index, leafNode->childs.end());
        
        int aux_val = leafNode2->values[0];
        insert_to_Parent(leafNode,aux_val,leafNode2);
    }
    
}
//
//
//
void BplusTree::printNode(Node* tNode){
    printf(" [");
    int i ;
    for(i=0; i<tNode->values.size()-1;i++){
        printf("%d|",tNode->values[i]);
    }
    if(tNode->values.size()>0) printf("%d]",tNode->values[i]);
}
//
//
//
void BplusTree::bfs_travel(Node *actual_Node){
    
    q.push(final_Node(actual_Node,true));
    while(!q.empty()){
        final_Node p = q.front();
        Node *temp = p.parent_Node;
        q.pop();
        printNode(temp);
        
        if(p.n_leaf)
            cout << endl;
        
        int i;
        if(!temp->isLeaf){
            for(i=0;i<temp->childs.size()-1;i++){
                q.push(final_Node(temp->childs[i],false));
            }
            if(p.n_leaf) q.push(final_Node(temp->childs[i],true));
            else q.push(final_Node(temp->childs[i],false));
        }
    }
}
/*
 *
 */
int main(){
    BplusTree tree = *new BplusTree();
    
    
    int op;
    int num;
    
    cout << "Ingrese el número de hijos de los nodos de su arbol:";
    cin >> tree.num_pointers;
    tree.num_values = tree.num_pointers - 1;
    
    while (true) {
        cout << "Seleccione: \n\t1.- Insertar \n\t2.- Borrar \n\t3.- Imprimir \n\t4.- Salir \n\tOpción: ";
        cin >> op;
        if (op == 1) {
            cout << "\tIngrese número a insertar:";
            cin >> num;
            tree.insert(num, tree.Root);
        }
        else if (op == 2) {
            cout << "\tIngrese número a eliminar:";
            cin >> num;
            tree.delete_value(num);
        }
        else if (op == 3) {
            tree.bfs_travel(tree.Root);
        }
        else if (op == 4){
            break;
        }
        else{
            cout << "Ingrese una opción válida" << endl;
        }
    }
    
    tree.insert(10, tree.Root);
    tree.bfs_travel(tree.Root);
    
    tree.insert(12, tree.Root);
    tree.bfs_travel(tree.Root);
    
    tree.insert(11, tree.Root);
    tree.bfs_travel(tree.Root);
    
    tree.insert(14, tree.Root);
    tree.bfs_travel(tree.Root);
    
    tree.insert(13, tree.Root);
    tree.bfs_travel(tree.Root);
    
    tree.delete_value(11);
    tree.bfs_travel(tree.Root);
    
    return 0;
}






