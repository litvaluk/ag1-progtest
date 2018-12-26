#include <iostream>
#include <vector>
#include <queue>
#include <bitset>

using namespace std;

class Node {

private:

    vector<Node*> neighbors;
    bitset<100> imported;
    uint64_t nodeCost;
    uint64_t numberOfImported;

public:

    Node() : imported(0), nodeCost(0), numberOfImported(0) {}

    Node* getNeighbor(int index) {
        return neighbors[index];
    }

    uint64_t getNumberOfNeighbors() const {
        return neighbors.size();
    }

    void addToImported(uint64_t type, uint64_t cost) {
        // cout << "typ: " << type << endl;
        imported.set(type);
        nodeCost += cost;
        numberOfImported++;
    }

    bool isInImported(uint64_t type) {
        return imported[type];
    }

    void addNeighbor(Node* node) {
        neighbors.push_back(node);
    }

    uint64_t getNumberOfImported() {
        return numberOfImported;
    }

    uint64_t getNodeCost() {
        return nodeCost;
    }
    
    void printNode(uint64_t numberOfTypes) {
        cout << nodeCost;
        for (uint64_t i = 0; i < numberOfTypes; i++) {
            if(imported[i]){
                cout << " " << i;
            }
        }
        cout << "\n";
    }

};

class Commodity {

private:
    Node* node;
    uint64_t cost;
    uint64_t type;

public:

    Commodity(Node *node, uint64_t cost, uint64_t type) : node(node), cost(cost), type(type) {}

    Node* getNode() const {
        return node;
    }

    uint64_t getCost() const {
        return cost;
    }

    uint64_t getType() const {
        return type;
    }

};

void connect(Node& a, Node& b) {
    a.addNeighbor(&b);
    b.addNeighbor(&a);
}

int main() {

    vector<Node> nodes;
    queue<Commodity> q;
    uint64_t numberOfNodes = 0, numberOfEdges = 0, numberOfTypes = 0, neededTypes = 0;

    cin >> numberOfNodes >> numberOfEdges >> numberOfTypes >> neededTypes;

    for (uint64_t i = 0; i < numberOfNodes; i++) {
        nodes.push_back(Node());
    }

    for (uint64_t i = 0; i < numberOfNodes; i++) {
        int type;
        cin >> type;
        nodes[i].addToImported(type, 0);
        q.push(Commodity(&nodes[i], 0, type));
    }

    for (uint64_t i = 0; i < numberOfEdges; i++) {
        int a, b;
        cin >> a >> b;
        connect(nodes[a], nodes[b]);
    }

    uint64_t totalCost = 0;

    while(!q.empty()) {

        // cout << "number of nb: " << q.front().getNode()->getNumberOfNeighbors() << endl;

        for (uint64_t i = 0; i < q.front().getNode()->getNumberOfNeighbors(); i++) {
            if (!q.front().getNode()->getNeighbor(i)->isInImported(q.front().getType())
            && q.front().getNode()->getNeighbor(i)->getNumberOfImported() < neededTypes) {
                q.front().getNode()->getNeighbor(i)->addToImported(q.front().getType(), q.front().getCost() + 1);
                q.push(Commodity(q.front().getNode()->getNeighbor(i), q.front().getCost() + 1, q.front().getType()));
            }
        }

        q.pop();

    }

    for (uint64_t i = 0; i < numberOfNodes; i++) {
        totalCost += nodes[i].getNodeCost();
    }

    cout << totalCost << "\n";

    for (uint64_t i = 0; i < numberOfNodes; i++) {
        nodes[i].printNode(numberOfTypes);
    }

    return 0;
}