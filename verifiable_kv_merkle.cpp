// Compile: g++ sha256.cpp verifiable_kv_merkle.cpp
// run: cat in.txt | ./a.out

#include <bits/stdc++.h>
#include "sha256.h"
#include <unordered_map>
using namespace std;

string get_reverse(string str)
{
    return string(str.rbegin(),str.rend());
}

string get_hash(string str1, string str2)
{
    return sha256(str1 + str2);
}

struct Node
{
    string key;
    string value;
    string hash;
    Node *left;
    Node *right;

    Node(string data);
};

Node::Node(string data) {
    this->hash = data;
}

struct MerkleTree {
    Node* root;
    MerkleTree(vector<Node*> blocks);
};

MerkleTree::MerkleTree(vector<Node*> blocks) {
    vector<Node*> nodes;
    while (blocks.size() != 1) {
        for (unsigned int l = 0, n = 0; l < blocks.size(); l = l + 2, n++) {
            if (l != blocks.size() - 1) {
                nodes.push_back(new Node(get_hash(blocks[l]->hash, blocks[l+1]->hash)));
                nodes[n]->left = blocks[l];
                nodes[n]->right = blocks[l+1];
            } else {
                nodes.push_back(blocks[l]);
            }
        }
        blocks = nodes;
        nodes.clear();

    }
    this->root = blocks[0];
}

string merkle(vector<string> arr)
{
    int len = arr.size();
    if(len == 1)
        return arr[0];
    vector<string> newArr;
    for(int i=0; i<len-1; i=i+2)
        newArr.push_back(get_hash(arr[i], arr[i+1]));
    if(arr.size()%2 == 1)
        newArr.push_back(get_hash(arr[len-1], arr[len-1]));
    return merkle(newArr);
}

int get_value(string k, vector<Node*> leaves){
    for(int i=0;i<leaves.size();i++)
        if(!k.compare(leaves[i]->key))
            return i;
    return -1;
}

vector<Node*> put_value(string k, string v, vector<Node*> leaves){
    int f = 0, i;
    for(i=0;i<leaves.size();i++)
        if(!k.compare(leaves[i]->key)){
            f = 1;
            cout << "Found key match, updating..." << "\n";
            leaves[i]->value = v;
            leaves[i]->hash = get_hash(k, v);
        }
    if(f==0){
        cout << "No key match, adding..." << "\n";
        leaves.push_back(new Node(get_hash(k, v)));
        leaves[i]->key = k;
        leaves[i]->value = v;
        leaves[i]->left = NULL;
        leaves[i]->right = NULL;
    }
    return leaves;
}

vector<Node*> delete_value(string k, vector<Node*> leaves){
    // for(int i=0;i<leaves.size();i++)
    for (auto it = leaves.begin(); it != leaves.end(); ++it)
        // cout << (*it)->key << "\n";
        if(!k.compare((*it)->key)){
            cout << "Found key match, deleting..." << "\n";
            leaves.erase(it);
        }
    return leaves;
}

int main(){

    vector<string> kv;
    int n;
    vector< vector<string> > tx;
    unordered_map<string, string> umap;
    string k,v;

    cin >> n;
    for(int i=0;i<n;i++){
        cin >> k >> v;
        // umap[k] = v;
        kv.push_back(k);
        kv.push_back(v);
        tx.push_back(kv);
        kv.clear();
    }

    // int len = sizeof(txHashes)/sizeof(txHashes[0]);

    // for (int i = 0; i < len; i++)
    // {
    //     kv.push_back(txHashes[i]);
    // }
    
    //cout << sizeof(txHashes)/sizeof(txHashes[0]) << "\n" << val << "\n\n" << get_hash(key, val) << "\n";
    // cout << merkle(kv) << "\n\n";

    // for (auto x : umap)
    //     cout << x.first << " " << x.second << endl;

    vector<Node*> leaves;
    for (int i = 0; i < n; i++)
    {
        // kv.push_back(txHashes[i]);
        leaves.push_back(new Node(get_hash(tx[i][0], tx[i][1])));
        leaves[i]->key = tx[i][0];
        leaves[i]->value = tx[i][1];
        leaves[i]->left = NULL;
        leaves[i]->right = NULL;
    }
    
    MerkleTree *hashTree = new MerkleTree(leaves);
    cout << "Root hash: " << hashTree->root->hash << "\n\n";

    int found = get_value("20211016", leaves);
    if(found >= 0)
        cout << "Found value: " << leaves[found]->value << "\n\n";
    else
        cout << "Not found" << "\n\n";

    leaves = put_value("20211015", "8de86a6ae65d3692e7305e2c58ac62eebd97d3d943e093f577da25c36988246bca358758f6d27e6cf45272937977a748fd88391db679ceda7dc7bf1f005ee879c3641f8544d7c02f3580b07c0f9887f0c6a27ff5ab1d4a3e29caf197cfc299aec00e7f889cfc9216ec818bf2e1682fc6af0d89939c91776669478caf27c9727c3e23e8160039594a33894f6564e1b1348bbd7a0088d42c4acb73eeaed59c009d478508483cbb05defd7dcdac355dadf06282a6f2e14342cccba99e840202f943b7d25296e7bc6a6bf66da09b1c5ed273d99e3a9fc6d43e46ccb8b358fa13d4e93ebe1b59762a1c8020c1efe3747dd07f0e30617ed60b4e6a5bee16b6ea421dd0", leaves);
    hashTree = new MerkleTree(leaves);
    cout << "Root hash: " << hashTree->root->hash << "\n\n";

    leaves = put_value("20211019", "8de86a6ae65d3692e7305e2c58ac62eebd97d3d943e093f577da25c36988246bca358758f6d27e6cf45272937977a748fd88391db679ceda7dc7bf1f005ee879c3641f8544d7c02f3580b07c0f9887f0c6a27ff5ab1d4a3e29caf197cfc299aec00e7f889cfc9216ec818bf2e1682fc6af0d89939c91776669478caf27c9727c3e23e8160039594a33894f6564e1b1348bbd7a0088d42c4acb73eeaed59c009d478508483cbb05defd7dcdac355dadf06282a6f2e14342cccba99e840202f943b7d25296e7bc6a6bf66da09b1c5ed273d99e3a9fc6d43e46ccb8b358fa13d4e93ebe1b59762a1c8020c1efe3747dd07f0e30617ed60b4e6a5bee16b6ea421dd0", leaves);
    hashTree = new MerkleTree(leaves);
    cout << "Size: " << leaves.size() << "\n";
    cout << "New value: " << leaves[leaves.size()-1]->value << "\n";
    cout << "Root hash: " << hashTree->root->hash << "\n\n";

    leaves = delete_value("20211015", leaves);
    cout << "Size: " << leaves.size() << "\n";
    hashTree = new MerkleTree(leaves);
    cout << "Root hash: " << hashTree->root->hash << "\n\n";
    
    return 0;
}