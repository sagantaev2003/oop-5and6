#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

// структура узла дерева
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

// построение дерева
Node* buildTree(const string& expr, int start, int end) {
    if (start > end) return nullptr;

    bool isNumber = true;
    for (int i = start; i <= end; i++) {
        if (!isdigit(expr[i])) { isNumber = false; break; }
    }
    if (isNumber) return new Node(expr.substr(start, end - start + 1));

    int minPriority = INT_MAX;
    int opPos = -1;
    int brackets = 0;

    for (int i = start; i <= end; i++) {
        if (expr[i] == '(') brackets++;
        else if (expr[i] == ')') brackets--;
        else if (brackets == 0 && (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')) {
            int p = priority(expr[i]);
            if (p <= minPriority) {
                minPriority = p;
                opPos = i;
            }
        }
    }

    if (opPos == -1 && expr[start] == '(' && expr[end] == ')') {
        return buildTree(expr, start + 1, end - 1);
    }

    Node* root = new Node(string(1, expr[opPos]));
    root->left = buildTree(expr, start, opPos - 1);
    root->right = buildTree(expr, opPos + 1, end);
    return root;
}

// вывод дерева с уровнями
void printTree(Node* root, int level = 1) {
    if (!root) return;
    cout << "<Уровень " << level << "> " << root->value << endl;
    if (root->left) printTree(root->left, level + 1);
    if (root->right) printTree(root->right, level + 1);
}

// обход
void preOrder(Node* root, string& out) {
    if (!root) return;
    out += root->value + " ";
    preOrder(root->left, out);
    preOrder(root->right, out);
}

void inOrder(Node* root, string& out) {
    if (!root) return;
    inOrder(root->left, out);
    out += root->value;
    inOrder(root->right, out);
}

void postOrder(Node* root, string& out) {
    if (!root) return;
    postOrder(root->left, out);
    postOrder(root->right, out);
    out += root->value + " ";
}

// вычисление
double evaluate(Node* root) {
    if (!root->left && !root->right) return stod(root->value);
    double left = evaluate(root->left);
    double right = evaluate(root->right);
    if (root->value == "+") return left + right;
    if (root->value == "-") return left - right;
    if (root->value == "*") return left * right;
    if (root->value == "/") return left / right;
    return 0;
}

// запуск
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

    double result = evaluate(root);

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

    return 0;
}
