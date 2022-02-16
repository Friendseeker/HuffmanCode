#include <iostream>
#include <vector>
#include <queue>
// Note: this is merely a demo, instead of a performance-oriented implementation. Certain details, such as string concatenation are performance inefficient, but are kept to not distract from the main purpose.

using namespace std;
struct Node {
    char letter;
    int freq;
    Node *left;
    Node *right;
    Node(char x, int f) : letter(x), freq(f), left(NULL), right(NULL) {}
    Node(Node* a, Node* b) : letter(0), freq(a->freq + b->freq), left(a), right(b) {}
};

class Huffman {
    Node* root;
    vector<string> encoding;
    vector<pair<char, int>> frequency;
    // Helper function to generate encoding from Huffman Tree
    void dfs(Node* root, string path) {
        if (not root->left and not root->right) {
            encoding[root->letter] = path;
        }
        if (root->left and root->right) {
            dfs(root->left, path + "0");
            dfs(root->right, path + "1");
        }
    }
public:
    // Given letters and their frequency
    // Generates the Huffman encoding and initializes the class
    Huffman(vector<pair<char, int>>& freq) {
        auto comp = [](Node* a, Node* b) {return a->freq > b->freq;};
        priority_queue<Node*, vector<Node*>, decltype(comp)> pq(comp);
        // Not the most efficient, but this is just a demo
        for (auto [letter, f]: freq) {
            pq.push(new Node(letter, f));
        }
        // Start building the Huffman Tree
        while (pq.size() >= 2) {
            Node* first = pq.top(); pq.pop();
            Node* second = pq.top(); pq.pop();
            pq.push(new Node(first, second));
        }
        root = pq.top(); pq.pop();

        // Call DFS to generate encoding from the Tree
        encoding.resize(128);
        dfs(root, "");

        // Save freq for nice print of huffman codes
        frequency = freq;
    }

    // Given a message, decode the string
    string decode(const string& msg) {
        Node* state = root;
        string decoded_msg;
        int i = 0;
        while (i < msg.size()) {
            while (state->left and state->right) {
                if (msg[i] == '0') {state = state->left;}
                if (msg[i] == '1') {state = state->right;}
                ++i;
            }
            decoded_msg.push_back(state->letter);
            state = root;
        }
        return decoded_msg;
    }

    // Given a message, encode it using Huffman
    string encode(const string& msg) {
        string encoded_msg;
        for (char c: msg) {
            encoded_msg += encoding[c];
        }
        return encoded_msg;
    }

    // Prints the encoding for each letter
    void print_scheme() {
        for (auto [letter, f]: frequency) {
            cout << letter << " " << encode(string(1, letter)) << endl;
        }
    }
};

int main() {
    // Format:
    // cin start with number of letters k
    // then rest k lines, in the format [letter] [frequency] is fetched
    // then a number p is fetched resembling number of commands
    // Example input:
    // 5
    // a 32
    // b 25
    // c 20
    // d 18
    // e 5
    // 2
    // print
    // encode abc

    int count;
    vector<pair<char, int>> input;
    char letter; int freq;
    cin >> count;
    while (count--) {
        cin >> letter >> freq;
        input.emplace_back(letter, freq);
    }
    cin >> count;
    string command;
    string msg;
    Huffman huff(input);
    while (count--) {
        cin >> command;
        if (command == "encode") {
            cin >> msg;
            cout << huff.encode(msg) << endl;
        } else if (command == "decode") {
            cin >> msg;
            cout << huff.decode(msg) << endl;
        } else if (command == "print") {
            huff.print_scheme();
        }
    }
    return 0;
}