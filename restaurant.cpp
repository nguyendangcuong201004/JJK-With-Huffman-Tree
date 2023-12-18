#include "main.h"

class JJK_Restaurant;
class Gojo_Restaurant;
class Sukuna_Restaurant;
class HuffNode;
class InternalNode;
class HuffTree;

int MAXSIZE;


class HuffNode  {
private:
    char c;
    int wgt;
    int hgt;
    int order;
    HuffNode* left;
    HuffNode* right;
public:
    HuffNode(){}
    HuffNode(char x, int w, int h, int n){
        this->c = x;
        this->wgt = w;
        this->hgt = h;
        this->order = n;
        this->left = NULL;
        this->right = NULL;
    }
    HuffNode(HuffNode* a, HuffNode* b, int n){
        this->c = '\0';
        this->wgt = a->getWeight() + b->getWeight();
        this->hgt = max(a->getHeight(), b->getHeight()) + 1;
        this->left = a;
        this->right = b;
        this->order = n;
    }
    ~HuffNode(){}
    char getChar(){
        return this->c;
    }
    int getOrder(){
        return this->order;
    }
    void setOrder(int newOrder){
        this->order = newOrder;
    }
    int getHeight()  {
        return this->hgt;
    }
    void setHeight(int newHeight){
        this->hgt = newHeight;
    }
    int getWeight() {
        return this->wgt;
    }
    bool isLeaf() {
        if (this->left == NULL && this->right == NULL){
            return true;
        }
        return false;
    }
    HuffNode*& getLeft(){
        return this->left;
    }
    void setLeft(HuffNode* newNode){
        this->left = newNode;
    }
    HuffNode*& getRight(){
        return this->right;
    }
    void setRight(HuffNode* newNode){
        this->right = newNode;
    }
};

class HuffTree {
private:
    HuffNode* root;
public:
    HuffTree(){}

    HuffTree(HuffTree* left, HuffTree* right, int n){
        root = new HuffNode(left->root, right->root, n);
    }
    HuffTree(char c, int w, int h, int n){
        root = new HuffNode(c, w, h, n);
    }
    ~HuffTree(){}
    HuffNode*& getRoot(){
        return this->root;
    }
    void print(HuffNode* root){
        if (root == NULL) return;
        else if(root->isLeaf()){
            solution << root->getChar() << endl;
        }
        else {
            print(root->getLeft());
            solution << root->getWeight() << endl;
            print(root->getRight());
        }
    }
};

struct compare
{
    bool operator() (HuffTree *a, HuffTree *b) {
        if (a->getRoot()->getWeight() > b->getRoot()->getWeight()) {
            return true;
        }
        else if (a->getRoot()->getWeight() == b->getRoot()->getWeight()) {
            return a->getRoot()->getOrder() > b->getRoot()->getOrder();
        }
        return false;
    }
};

void print_queue(priority_queue<HuffTree*, vector<HuffTree*>, compare> Q){
    while (!Q.empty()){
        HuffTree* top = Q.top(); Q.pop();
        solution << top->getRoot()->getWeight() << "(" << top->getRoot()->getChar() << ")" << "\t";
    }
}

void rotateLeft (HuffNode *&root)
{
    HuffNode *rightRoot = root->getRight();
    root->setHeight(max(root->getLeft()->getHeight(), rightRoot->getLeft()->getHeight()) + 1);
    rightRoot->setHeight(max(root->getHeight(), rightRoot->getRight()->getHeight()) + 1);
    root->setRight(rightRoot->getLeft());
    rightRoot->setLeft(root);
    root = rightRoot;
}

void rotateRight(HuffNode *&root)
{
    HuffNode *leftRoot = root->getLeft();
    root->setHeight(max(root->getRight()->getHeight(), leftRoot->getRight()->getHeight()) + 1);
    leftRoot->setHeight(max(root->getHeight(), leftRoot->getLeft()->getHeight()) + 1);
    root->setLeft(leftRoot->getRight());
    leftRoot->setRight(root);
    root = leftRoot;
}

void leftBalance (HuffNode *&root)
{
    HuffNode *&leftRoot = root->getLeft();
    int difference = leftRoot->getLeft()->getHeight() - leftRoot->getRight()->getHeight();
    if (difference >= 0) {
        rotateRight(root);
    }
    else {
        rotateLeft(leftRoot);
        rotateRight(root);
    }
}

void rightBalance (HuffNode *&root)
{
    HuffNode *&rightRoot = root->getRight();
    int difference = rightRoot->getLeft()->getHeight() - rightRoot->getRight()->getHeight();
    if (difference > 0) {
        rotateRight(rightRoot);
        rotateLeft(root);
    }
    else {
        rotateLeft(root);
    }
}

bool preOrder_balance(HuffNode*& root){
    if (root == NULL || root->isLeaf()){
        return false;
    }
    int difference = root->getLeft()->getHeight() - root->getRight()->getHeight();
    if (difference < -1){
        rightBalance(root);
        return true;
    }
    else if (difference > 1){
        leftBalance(root);
        return true;
    }
    if (preOrder_balance(root->getLeft())){
        root->setHeight(max(root->getLeft()->getHeight(), root->getRight()->getHeight()) + 1);
        return true;
    }
    if (preOrder_balance(root->getRight())){
        root->setHeight(max(root->getLeft()->getHeight(), root->getRight()->getHeight()) + 1);
        return true;
    }
    return false;
}

void make_balance(HuffNode*& node, int num){
    while (num > 0){
        if (!preOrder_balance(node)){
            return;
        }
        node->setHeight(max(node->getLeft()->getHeight(), node->getRight()->getHeight()) + 1);
        num--;
    }
}

HuffTree* BuidHuff (priority_queue<HuffTree*, vector<HuffTree*>, compare> Q){
    int order = Q.size();
    if (Q.size() <= 1) return NULL;
    HuffTree* tmp1 = NULL;
    HuffTree* tmp3 = NULL;
    HuffTree* tmp2 = NULL;
    while (Q.size() >= 2){
        order++;
        tmp1 = Q.top(); Q.pop();
        tmp2 = Q.top(); Q.pop();
        tmp3 = new HuffTree(tmp1, tmp2, order);
        COUNTDELETE++;
        int num = 3;
        make_balance(tmp3->getRoot(), num);
        tmp3->getRoot()->setOrder(order);
        Q.push(tmp3);
        delete tmp1; tmp1 = NULL;
        COUNTDELETE--;
        delete tmp2; tmp2 = NULL;
        COUNTDELETE--;
    }
    return tmp3;
}

void dfs(HuffNode* root, map<char, string> &mp, string tmp){
    if (root == NULL) return;
    if (root->isLeaf()){
        mp[root->getChar()] = tmp;
    }
    else {
        dfs(root->getLeft(), mp, tmp + "0");
        dfs(root->getRight(), mp, tmp + "1");
    }
}

map<char, string> encode(HuffTree* huff_tree){
    map<char, string> mp;
    dfs(huff_tree->getRoot(), mp, "");
    return mp;
}

string decode(string d, map<char, string> mp){
    string res = "";
    for (char c : d){
        res += mp[c];
    }
    return res;
}

bool cmp(pair<char, int> p1, pair<char, int> p2){
    if (p1.second != p2.second){
        return p1.second < p2.second;
    }
    if ((p1.first >= 65 && p1.first <= 90 && p2.first >= 65 && p2.first <= 90) || 
        (p1.first >= 97 && p1.first <= 122 && p2.first >= 97 && p2.first <= 122)){
        return p1.first < p2.first;
    }
    if (p1.first >= 97 && p1.first <= 122 && p2.first >= 65 && p2.first <= 90){
        return true;
    }
    return false;
}

int binaryToDecimal(string binaryStr) {
    int decimalValue = 0;

    for (char bit : binaryStr) {
        decimalValue = decimalValue * 2 + (bit - '0');
    }

    return decimalValue;
}

class Gojo_Restaurant {
    class BST; // Tránh xung đột của bảng băm
private:
    vector <BST> areaTable;
public:
    Gojo_Restaurant() : areaTable(MAXSIZE + 1) {} // Khởi tạo cí MAXSIZE khu vực 

    void insert(int result){
        int ID = result % MAXSIZE + 1; // Số thứ tự khu vực
        areaTable[ID].insert(result);
    }
    void remove_with_kokuen(){
        for (int i = 1; i <= MAXSIZE; i++){
            areaTable[i].remove();
        }
    }
    void print_wiht_limitless(int num){
        if (num <= 0 || num > MAXSIZE){
            return;
        }
        areaTable[num].print();
    }
private:
    class BST {
        class Node;
    private:
        Node* root;
        queue<int> Order;
    public:
        BST() : root(NULL) {}
        ~BST(){
            deleteBinaryTree(root);
        }
        void deleteBinaryTree(Node* root) {
            if (root == nullptr) {
                return;
            }
            deleteBinaryTree(root->left);
            deleteBinaryTree(root->right);
            delete root;
            COUNTDELETE--;
        }
        int size(){ // Số lượng khách hàng = Số Node = kích thước hàng đợi (FIFO)
            return Order.size();
        } 
        Node* insert_recursive(Node* node, int result){ // Hàm thêm Node vào BST bằng đệ quy 
            if (node == NULL){
                Order.push(result); // có newNode thì sẽ thêm vào hàng đợi thứ tự (Order)
                COUNTDELETE++;
                return new Node(result);
            }
            else if (node->result > result){
                node->left = insert_recursive(node->left, result);
            }
            else node->right = insert_recursive(node->right, result);
            return node;
        }
        void insert(int result){ // insert ban đầu chỉ có result chứ không có root của BST
            root = insert_recursive(root, result);
        }
        Node* minNode(Node* node){ // Hàm tìm Node nhỏ nhất của cây bên phải 
            Node* tmp = node;
            while (tmp != NULL && tmp->left != NULL){
                tmp = tmp->left;
            }
            return tmp;
        }
        Node* remove_recursive(Node* node, int result){ // Hàm xóa khách khỏi khu vực bằng đệ quy 
            if (node == NULL) return node;
            if (node->result > result){
                node->left = remove_recursive(node->left, result);
            }
            else if (node->result < result){
                node->right = remove_recursive(node->right, result);
            }
            else {
                if (node->left == NULL){
                    Node* tmp = node->right;
                    delete node;
                    COUNTDELETE--;
                    return tmp;
                }
                else if (node->right == NULL){
                    Node* tmp = node->left;
                    delete node;
                    COUNTDELETE--;
                    return tmp;
                }
                else {
                    Node* deleteNode = minNode(node->right); // Node nhỏ nhất của Node bên phải để thay thế 
                    node->result = deleteNode->result;
                    node->right = remove_recursive(node->right, deleteNode->result);
                }
            }
            return node;
        }
        void preOrder(Node* node, vector<int>&v){
            if (node == NULL) return;
            v.push_back(node->result);
            preOrder(node->left, v);
            preOrder(node->right, v);
        }
        long long Combination(int n, int r){ // Tính hoán vị dựa trên công thức Logarit 
            long long p = 1, k = 1;
            if (n - r < r)
                r = n - r;
        
            if (r != 0) {
                while (r) {
                    p *= n;
                    k *= r;
                    long long m = __gcd(p, k);
                    p /= m;
                    k /= m;
                    n--;
                    r--;
                    p %= MAXSIZE;
                }
            }
            else{
                p = 1;
            }
            return p;
        }
        long long Count_Permutation(vector<int> v){  // Đếm số Hoán vị 
            if (v.size() == 0 || v.size() == 1) return 1;
            vector<int> n1, n2;
            int root = v[0];
            for (unsigned int i = 1; i < v.size(); i++){
                if (v[i] < root){
                    n1.push_back(v[i]);
                }
                else n2.push_back(v[i]);
            }
            return ((Combination(n1.size() + n2.size(), n1.size()) % MAXSIZE) * (Count_Permutation(n1) % MAXSIZE) * (Count_Permutation(n2)) % MAXSIZE) % MAXSIZE;
        }
        void remove() {
            if (this->size() == 0) return;
            vector<int> v;
            preOrder(root, v);
            long long Y = Count_Permutation(v);
            Y = Y % MAXSIZE;
            while (Y != 0 && !Order.empty()){
                int tmp = Order.front();
                Order.pop();
                root = remove_recursive(root, tmp);
                Y--;
            }
        }
        void inOrder(Node* node){
            if (node == NULL){
                return;
            }
            inOrder(node->left);
            solution << node->result << endl;
            inOrder(node->right);
        }
        void print(){
            if (this->size() == 0){
                return;
            }
            inOrder(root);
        }
    private:
        class Node { // Node của Cây nhị phân tìm kiếm 
        private:
            int result;
            Node* left;
            Node* right;
            friend class BST;
        public:
            Node(int res) : result(res), left(NULL), right(NULL) {}
        };
    };
};

class Sukuna_Restaurant {
    class Node;
private:
    vector<Node*> areaTable;
    list<Node*> LRU;
private:
    bool CompareOrder(int index1, int index2){ // index1 là cha, index2 là con 
        for (Node* tmp : LRU){
            if (tmp->ID == areaTable[index1]->ID) {
                return true;
            }
            if (tmp->ID == areaTable[index2]->ID){
                return false;
            }
        }
        return false;
    }
    void reHeapDown(int index){
        unsigned int left = index * 2 + 1;
        unsigned int right = index * 2 + 2;
        int largest = index;
        if (left < areaTable.size() && areaTable[largest]->size() > areaTable[left]->size()){
            largest = left;
        }
        else if (left < areaTable.size() && areaTable[largest]->size() == areaTable[left]->size()){
            if (CompareOrder(largest, left)){
                largest = left;
            }
        }
        if (right < areaTable.size() && areaTable[largest]->size() > areaTable[right]->size()){
            largest = right;
        }
        else if (right < areaTable.size() && areaTable[largest]->size() == areaTable[right]->size()){
            if (CompareOrder(largest, right)){
                largest = right;
            }
        }
        if (largest != index){
            swap(areaTable[largest], areaTable[index]);
            reHeapDown(largest);
        }
    }
    void reHeapUp(int index){
        int n = areaTable.size();
        if (index <= 0 || index >= n) return;
        int parent = (index - 1) / 2;
        if (parent >= 0 && areaTable[parent]->size() > areaTable[index]->size()){
            swap(areaTable[parent], areaTable[index]);
            reHeapUp(parent);
        }
        else if (parent >= 0 && areaTable[parent]->size() == areaTable[index]->size()){
            if (CompareOrder(parent, index)){
                swap(areaTable[parent], areaTable[index]);
                reHeapUp(parent);
            }
        }
    }
    void moveTop(Node* node){ // Chuyển Node bị thay đổi gần nhất lên đầu danh sách
        bool check = false;
        for (Node* x : LRU){
            if (x->ID == node->ID){
                check = true;
                break;
            }
        }
        if (check){
            LRU.remove(node);
        }
        LRU.push_front(node);
    }
    void removeNode(Node* node){
        LRU.remove(node);
    }
public:
    ~Sukuna_Restaurant(){
        while (LRU.size() > 0){
            Node* tmp = LRU.front();
            LRU.pop_front();
            delete tmp;
            COUNTDELETE--;
        }
    }
    void insertHeap(int result){
        int ID = result % MAXSIZE + 1;
        int index = -1;
        for (unsigned int i = 0; i < areaTable.size(); i++){
            if (areaTable[i]->ID == ID){
                index = i;
                break;
            }
        }
        if (index == -1){
            areaTable.push_back(new Node(ID));
            COUNTDELETE++;
            index = areaTable.size() - 1;
            areaTable[index]->insert(result);
            this->moveTop(areaTable[index]);
            this->reHeapUp(index);

        }
        else {
            areaTable[index]->insert(result);
            this->moveTop(areaTable[index]);
            this->reHeapDown(index);
        }
    }
    void remove_with_keiteien(int num){
        if (areaTable.size() <= 0) return; // Trường hợp không có khu vực nào có khách
        int number = num;
        vector <pair<Node*, int>> tmp;
        for (Node* x : LRU){
            tmp.push_back({x, tmp.size()});
        }
        sort(tmp.begin(), tmp.end(), [](pair<Node*, int> a, pair<Node*, int> b){
            if (a.first->size() != b.first->size()){
                return a.first->size() < b.first->size();
            }
            return a.second > b.second;
        });
        unsigned int index = 0;
        while (index < tmp.size() && num != 0){  // Có khu vực có khách 
            tmp[index].first->remove(number); // xóa number khách trong khu vực 
            if (tmp[index].first->size() == 0){ // nếu như không còn khách trong khu vực thì xóa khỏi cây heap 
                int pos = 0;
                for (unsigned int i = 0; i < areaTable.size(); i++){
                    if (areaTable[i]->ID == tmp[index].first->ID){
                        pos = i;
                    }
                }
                swap(areaTable[pos], areaTable[areaTable.size() - 1]);
                this->removeNode(areaTable[areaTable.size() - 1]);
                delete areaTable[areaTable.size() - 1];
                COUNTDELETE--;
                areaTable.pop_back();
            }
            index += 1;
            this->reHeapDown(0); // set lại min heap 
            num--;
        }
    }
    void preOrder(int index, int num){
        int n = this->areaTable.size();
        if (index >= n) return;
        this->areaTable[index]->print(num);
        preOrder(index * 2 + 1, num);
        preOrder(index * 2 + 2, num);
    }
    void print_with_cleave(int num){
        if (num <= 0) return;
        preOrder(0, num);
    }
private:
    class Node {
    private:
        int ID;
        list <int> head;
        friend class Sukuna_Restaurant;
    public:
        Node(int id) : ID(id) {}
        int size() {
            return head.size();
        }
        void insert(int result){
            head.push_front(result);
        }
        void remove(int num) {
            while (num != 0 && !head.empty()){
                solution << head.back() << "-" << ID << endl;
                head.pop_back();
                num--;
            }
        }
        void print(int num){
            for (auto it = head.begin(); it != head.end() && num > 0; it++, num--){
                solution << ID << "-" << *it << endl;
            }
        }
    };
};

class JJK_Restaurant {
private:
    list <HuffTree*> customer;
    Gojo_Restaurant Gojo;
    Sukuna_Restaurant Sukuna;

public:
    ~JJK_Restaurant(){
        while (customer.size() > 0){
           HuffTree* tmp = customer.front();
           customer.pop_front();
           delete tmp;
           COUNTDELETE--; 
        }
    }

	void LAPSE (string name){ // LAPSE -> xử lý để ra được result của khách hàng 
		int cnt[256] = {0};
        string s = name;
        for (char x : s){
            cnt[int(x)]++;
        }
        vector<pair<char, int>> v;
        for (int i = 0; i < 256; i++){
            if (cnt[i]){
                v.push_back({char(i), cnt[i]});
            }
        }
        if (v.size() < 3) return;
        map<char, char> mp;
        for (unsigned int i = 0; i < v.size(); i++){
            if (v[i].first >= 65 && v[i].first <= 90){
                char tmp = v[i].first;
                v[i].first = ((v[i].first - 65) + v[i].second) % 26 + 65;
                mp[tmp] = v[i].first;
            }
            if (v[i].first >= 97 && v[i].first <= 122){
                char tmp = v[i].first;
                v[i].first = ((v[i].first - 97) + v[i].second) % 26 + 97;
                mp[tmp] = v[i].first;
            }
        }
        memset(cnt, 0, sizeof(cnt));
        for (auto it : v){
            cnt[int(it.first)] += it.second;
        }
        v.clear();
        for (int i = 0; i < 256; i++){
            if (cnt[i]){
                v.push_back({char(i), cnt[i]});
            }
        }
        sort(v.begin(), v.end(), cmp);
        priority_queue<HuffTree*, vector<HuffTree*>, compare> Q;
        for (unsigned int i = 0; i < v.size(); i++){
            Q.push(new HuffTree(v[i].first, v[i].second, 0, i + 1));
            COUNTDELETE++;
        }
        int result = 0;
        HuffTree* huff_man = NULL;
        if (Q.size() > 1){
            huff_man = BuidHuff(Q);
            map<char, string> mahoa = encode(huff_man);
            for (char &x : s){
                x = mp[x];
            }
            string res = decode(s, mahoa);
            string ans = "";
            if (res.size() <= 10){
                ans = res;
            }
            else ans = res.substr(res.size() - 10 ,10);
            reverse(ans.begin(), ans.end());
            result = binaryToDecimal(ans);
        }
        else huff_man = Q.top();
        customer.push_front(huff_man);
        if (result % 2 == 1){
            Gojo.insert(result);
        }
        else {
            Sukuna.insertHeap(result);
        }
 	}

	void KOKUSEN(){
        Gojo.remove_with_kokuen();
    }
	void KEITEIKEN(int num){
        Sukuna.remove_with_keiteien(num);
    }
	void HAND(){
        if (customer.size() > 0){
            customer.front()->print(customer.front()->getRoot());
        }
    }
	void LIMITLESS(int num){
        Gojo.print_wiht_limitless(num);
    }
	void CLEAVE(int num){
        Sukuna.print_with_cleave(num);
    }
};

void simulate(string filename)
{
	JJK_Restaurant* JJK = NULL;
	ifstream ss(filename);
	if (ss.fail()){
		cout << "ERROR: " << filename << endl;
		return;
	}
	string str, maxsize, name, num;
	while (ss >> str){
		if (str == "MAXSIZE"){
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
			JJK = new JJK_Restaurant();
            COUNTDELETE++;
		}
		else if (str == "LAPSE"){
			ss >> name;
			JJK->LAPSE(name);
		}
		else if (str == "KOKUSEN"){
			JJK->KOKUSEN();
		}
		else if (str == "KEITEIKEN"){
			ss >> num;
			JJK->KEITEIKEN(stoi(num));
		}
		else if (str == "HAND"){
			JJK->HAND();
		}
		else if (str == "LIMITLESS"){
			ss >> num;
			JJK->LIMITLESS(stoi(num));
		}
		else if (str == "CLEAVE"){
			ss >> num;
			JJK->CLEAVE(stoi(num));
		}
	}
	delete JJK;
    COUNTDELETE--;
}