#include <iostream>
#include <fstream>
#include <string>
#include <climits>
using namespace std;

// узел дерево
struct Node {
    string value;
    Node* left;
    Node* right;
    Node(string val) : value(val), left(nullptr), right(nullptr) {}
};

// приоритет операций
int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// проверка, что подстрока — число
bool isNumber(const string& expr, int start, int end) {
    for (int i = start; i <= end; i++)
        if (!isdigit(expr[i])) return false;
    return true;
}

// построение дерева с учётом скобок
Node* buildTree(const string& expr, int start, int end) {
    if (start > end) return nullptr;

    // если вся подстрока — число
    if (isNumber(expr, start, end))
        return new Node(expr.substr(start, end - start + 1));

    int minPriority = INT_MAX;
    int opPos = -1;
    int bracketCount = 0;

    for (int i = start; i <= end; i++) {
        if (expr[i] == '(') bracketCount++;
        else if (expr[i] == ')') bracketCount--;
        else if (bracketCount == 0 && (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')) {
            int p = priority(expr[i]);
            if (p <= minPriority) {
                minPriority = p;
                opPos = i;
            }
        }
    }

    // если все выражение в скобках, отбрасываем их
    if (opPos == -1 && expr[start] == '(' && expr[end] == ')') {
        return buildTree(expr, start + 1, end - 1);
    }

    Node* root = new Node(string(1, expr[opPos]));
    root->left = buildTree(expr, start, opPos - 1);
    root->right = buildTree(expr, opPos + 1, end);
    return root;
}

// инфиксная запись
void inOrder(Node* root, string& out) {
    if (!root) return;

    // если узел не лист, оборачиваем скобками
    bool isOperator = root->left || root->right;
    if (isOperator) out += "(";

    inOrder(root->left, out);
    out += root->value;
    inOrder(root->right, out);

    if (isOperator) out += ")";
}

// префикс
void preOrder(Node* root, string& out) {
    if (!root) return;
    out += root->value + " ";
    preOrder(root->left, out);
    preOrder(root->right, out);
}
//постфикс
void postOrder(Node* root, string& out) {
    if (!root) return;
    postOrder(root->left, out);
    postOrder(root->right, out);
    out += root->value + " ";
}

// вычисление 
int evaluate(Node* root) {
    if (!root->left && !root->right) return stoi(root->value);
    int left = evaluate(root->left);
    int right = evaluate(root->right);
    if (root->value == "+") return left + right;
    if (root->value == "-") return left - right;
    if (root->value == "*") return left * right;
    if (root->value == "/") return left / right;
    return 0;
}


void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// дерево с уровнями
void printTree(Node* root, int level = 1) {
    if (!root) return;
    cout << "<Уровень " << level << "> " << root->value << endl;
    if (root->left) printTree(root->left, level + 1);
    if (root->right) printTree(root->right, level + 1);
}

int main() {
    setlocale(LC_ALL, "Russian");

    ifstream fin("input.txt");
    if (!fin.is_open()) {
        cerr << "Не удалось открыть input.txt\n";
        return 1;
    }

    string expr;
    getline(fin, expr);
    fin.close();

    cout << "Исходное выражение: " << expr << endl;

    Node* root = buildTree(expr, 0, expr.length() - 1);

    cout << "\nДерево с уровнями:\n";
    printTree(root);

    string infix = "", prefix = "", postfix = "";
    inOrder(root, infix);
    preOrder(root, prefix);
    postOrder(root, postfix);

    int result = evaluate(root);

    cout << "\nInfix: " << infix << " = " << result << endl;
    cout << "Prefix: " << prefix << " = " << result << endl;
    cout << "Postfix: " << postfix << " = " << result << endl;

    ofstream fout("output.txt");
    if (fout.is_open()) {
        fout << "Infix: " << infix << " = " << result << endl;
        fout << "Prefix: " << prefix << " = " << result << endl;
        fout << "Postfix: " << postfix << " = " << result << endl;
        fout.close();
    }

    deleteTree(root);
    return 0;
}
