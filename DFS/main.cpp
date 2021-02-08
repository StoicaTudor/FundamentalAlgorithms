/*
	Stoica Tudor Mihai
	Group 30423
	Task: You are required to implement correctly and efficiently the căutare în depthfirst
search
algorithm (DFS) (Chapter 22.3 from the book). For graph representation, you should use
adjacency lists. You also have to:
Implement
the Tarjan algorithm for detecting strongly connected components
(https://en.wikipedia.org/wiki/Tarjan's_strongly_connected_components_algorithm)
Implement
topological sorting (described in chapter 22.4)


	how I implemented:
	dfs: classic dfs: take the first node -> apply dfs to its adjList; mark every node as visited; if a node we want to visit is visited, do not visit it
	tarzan: I implemented it, by looking at the video https://www.youtube.com/watch?v=wUgWX0nc4NY&ab_channel=WilliamFiset, no code taken from anywhere
	topological sort: do dfs for every node (if it is not visited already, place every node on a stack); the sorting order is the reversed order on the stack
	I also used no code, I implemented all by myself, watching this video: https://youtu.be/Q9PIxaNGnig

	data structures used:

	public: struct Node {

	enum NodeStatus {
		UNVISITED,
		VISITING_NEIGHBOURS,
		VISITED_ALL_NEIGHBOURS
	};

	int value;
	vector <Node> adjList;
	bool visited;

	// for Tarzan
	NodeStatus status = NodeStatus::UNVISITED;
	int lowLinkValue = -1;

	void initNode(int value) {
		this->value = value;
		this->visited = false;
		this->status = NodeStatus::UNVISITED;
	}

	bool isDummyNode() {
		return (this->value == -1);
	}
};

public: struct Graph {

	int nrNodes;
	vector <Node> nodes;
	}

*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include "Profiler.h"

using namespace std;

Profiler profiler("DFS");

class DFS {

public: struct Node {

	enum NodeStatus {
		UNVISITED,
		VISITING_NEIGHBOURS,
		VISITED_ALL_NEIGHBOURS
	};

	int value;
	vector <Node> adjList;
	bool visited;

	// for Tarzan
	NodeStatus status = NodeStatus::UNVISITED;
	int lowLinkValue = -1;

	void initNode(int value) {
		this->value = value;
		this->visited = false;
		this->status = NodeStatus::UNVISITED;
	}

	bool isDummyNode() {
		return (this->value == -1);
	}
};

public: struct Graph {

	int nrNodes;
	vector <Node> nodes;

	void refreshGraph() {

		for (Node& currentNode : nodes) {

			currentNode.lowLinkValue = -1;
			currentNode.visited = false;
			currentNode.status = Node::NodeStatus::UNVISITED;
		}
	}

	// int getNext

	void initDemoGraphDFS() {

		this->nrNodes = 11;

		for (int i = 0; i < nrNodes; i++) {

			Node newNode;
			newNode.initNode(i);

			nodes.push_back(newNode);
		}

		nodes.at(0).adjList.push_back(nodes.at(1));
		nodes.at(1).adjList.push_back(nodes.at(0));

		nodes.at(0).adjList.push_back(nodes.at(2));
		nodes.at(2).adjList.push_back(nodes.at(0));

		nodes.at(1).adjList.push_back(nodes.at(5));
		nodes.at(5).adjList.push_back(nodes.at(1));

		nodes.at(1).adjList.push_back(nodes.at(6));
		nodes.at(6).adjList.push_back(nodes.at(1));

		nodes.at(2).adjList.push_back(nodes.at(3));
		nodes.at(3).adjList.push_back(nodes.at(2));

		nodes.at(2).adjList.push_back(nodes.at(4));
		nodes.at(4).adjList.push_back(nodes.at(2));

		nodes.at(2).adjList.push_back(nodes.at(9));
		nodes.at(9).adjList.push_back(nodes.at(2));

		nodes.at(3).adjList.push_back(nodes.at(9));
		nodes.at(9).adjList.push_back(nodes.at(3));

		nodes.at(3).adjList.push_back(nodes.at(10));
		nodes.at(10).adjList.push_back(nodes.at(3));

		nodes.at(6).adjList.push_back(nodes.at(7));
		nodes.at(7).adjList.push_back(nodes.at(6));

		nodes.at(6).adjList.push_back(nodes.at(8));
		nodes.at(8).adjList.push_back(nodes.at(6));

		nodes.at(7).adjList.push_back(nodes.at(8));
		nodes.at(8).adjList.push_back(nodes.at(7));

		nodes.at(9).adjList.push_back(nodes.at(10));
		nodes.at(10).adjList.push_back(nodes.at(9));
	}

	void initDemoGraphTarzan() {

		this->nrNodes = 8;

		for (int i = 0; i < nrNodes; i++) {

			Node newNode;
			newNode.initNode(i);

			nodes.push_back(newNode);
		}

		// demo nr 0

		nodes.at(0).adjList.push_back(nodes.at(1));
		nodes.at(1).adjList.push_back(nodes.at(2));
		nodes.at(2).adjList.push_back(nodes.at(0));
		nodes.at(3).adjList.push_back(nodes.at(4));
		nodes.at(3).adjList.push_back(nodes.at(7));
		nodes.at(4).adjList.push_back(nodes.at(5));
		nodes.at(5).adjList.push_back(nodes.at(6));
		nodes.at(5).adjList.push_back(nodes.at(0));
		nodes.at(6).adjList.push_back(nodes.at(0));
		nodes.at(6).adjList.push_back(nodes.at(2));
		nodes.at(6).adjList.push_back(nodes.at(4));
		nodes.at(7).adjList.push_back(nodes.at(3));
		nodes.at(7).adjList.push_back(nodes.at(5));


		// demo nr 1
		/*
		nodes.at(0).adjList.push_back(nodes.at(1));
		nodes.at(1).adjList.push_back(nodes.at(2));
		nodes.at(2).adjList.push_back(nodes.at(0));
		nodes.at(2).adjList.push_back(nodes.at(3));
		nodes.at(3).adjList.push_back(nodes.at(4));
		nodes.at(2).adjList.push_back(nodes.at(5));
		nodes.at(5).adjList.push_back(nodes.at(6));
		nodes.at(7).adjList.push_back(nodes.at(5));
		nodes.at(6).adjList.push_back(nodes.at(7));
		*/
	}

	Node getNextUnvisitedNodeTarzan() {

		for (Node currentNode : this->nodes) {

			if (currentNode.status == Node::NodeStatus::UNVISITED) {

				return currentNode;
			}
		}

		Node dummyNode;
		dummyNode.initNode(-1);

		return dummyNode;
	}

	Node getNextUnvisitedNodeTopological() {

		for (Node currentNode : this->nodes) {

			if (currentNode.visited == false) {

				return currentNode;
			}
		}

		Node dummyNode;
		dummyNode.initNode(-1);

		return dummyNode;
	}

	void initDemoGraphTopo() {

		// demo nr 0
		// this->nrNodes = 5;

		// demo nr 1
		this->nrNodes = 10;

		for (int i = 0; i < nrNodes; i++) {

			Node newNode;
			newNode.initNode(i);

			nodes.push_back(newNode);
		}

		// demo nr 0
		//nodes.at(0).adjList.push_back(nodes.at(1));
		//nodes.at(0).adjList.push_back(nodes.at(2));
		//nodes.at(1).adjList.push_back(nodes.at(3));
		//nodes.at(2).adjList.push_back(nodes.at(3));
		//nodes.at(3).adjList.push_back(nodes.at(4));

		// demo nr 1
		nodes.at(0).adjList.push_back(nodes.at(1));
		nodes.at(0).adjList.push_back(nodes.at(5));
		nodes.at(1).adjList.push_back(nodes.at(7));
		nodes.at(3).adjList.push_back(nodes.at(2));
		nodes.at(3).adjList.push_back(nodes.at(4));
		nodes.at(3).adjList.push_back(nodes.at(8));
		nodes.at(4).adjList.push_back(nodes.at(8));
		nodes.at(6).adjList.push_back(nodes.at(0));
		nodes.at(6).adjList.push_back(nodes.at(1));
		nodes.at(6).adjList.push_back(nodes.at(2));
		nodes.at(8).adjList.push_back(nodes.at(2));
		nodes.at(8).adjList.push_back(nodes.at(4));
		nodes.at(9).adjList.push_back(nodes.at(4));
	}

	void printAdjList() {

		for (int i = 0; i < this->nrNodes; i++) {

			Node currentNode = this->nodes.at(i);

			cout << "parent " << currentNode.value << " : ";

			for (Node currentNodeAdjNode : currentNode.adjList) {

				cout << currentNodeAdjNode.value << " ";
			}

			cout << endl;
		}
	}

	void initPerformanceDFS(int nrNodes, int nrEdges) {

		this->nrNodes = nrNodes;

		int** adjMat = new int* [this->nrNodes];

		for (int i = 0; i < nrNodes; i++) {
			adjMat[i] = new int[this->nrNodes];
		}

		for (int i = 0; i < this->nrNodes; i++) {
			for (int j = 0; j < this->nrNodes; j++) {
				adjMat[i][j] = 0;
			}
		}
		// initialize a utility adjMatrix
		// it is only used to generate random edges

		for (int i = 0; i < this->nrNodes; i++) {

			Node newNode;
			newNode.initNode(i);

			nodes.push_back(newNode);
		}

		int node0 = rand() % this->nrNodes;
		int node1 = rand() % this->nrNodes;

		while (nrEdges > 0) {// for every edge left to complete

			nrEdges--;
			// cout << nrEdges << endl;

			while (adjMat[node0][node1] == 1) {

				node0 = rand() % this->nrNodes;
				node1 = rand() % this->nrNodes;
			} // generate random edges while we found an edge which does not exist

			 // when we finally found it:
			adjMat[node0][node1] = 1;
			nodes.at(node0).adjList.push_back(nodes.at(node1));
		}

		free(adjMat);
	}
};

public: static enum GraphType {
	DFSS,
	TARZAN,
	NO_CYCLES_TOPOLOGICAL
};

public: Graph graph;

public:

	DFS(GraphType graphType, int nrNodes, int nrEdges) {

		switch (nrNodes)
		{

		case 0:
			switch (graphType) {

			case DFS::DFSS:
				this->graph.initDemoGraphDFS();
				break;
			case DFS::TARZAN:
				this->graph.initDemoGraphTarzan();
				break;
			case DFS::NO_CYCLES_TOPOLOGICAL:
				this->graph.initDemoGraphTopo();
				break;
			default:
				this->graph.initDemoGraphDFS();
				break;
			}

			break;

		default:
			switch (graphType) {
			case DFS::DFSS:
				this->graph.initPerformanceDFS(nrNodes, nrEdges);
				break;
			case DFS::TARZAN:
				this->graph.initDemoGraphTarzan();
				break;
			case DFS::NO_CYCLES_TOPOLOGICAL:
				this->graph.initDemoGraphTopo();
				break;
			default:
				this->graph.initDemoGraphDFS();
				break;
			}
			break;
		}
	}

	void doDFS(Node currentNode, Operation op) {

		op.count();
		if (this->graph.nodes.at(currentNode.value).visited == true) {

			return;
		}

		// else
		cout << currentNode.value << " ";
		op.count();
		this->graph.nodes.at(currentNode.value).visited = true;

		for (Node currentAdjNode : this->graph.nodes.at(currentNode.value).adjList) {

			op.count();
			doDFS(currentAdjNode, op);
		}
	}

	void doTarzan() {

		stack <Node> tarzanStack;
		Node currentNode;

		currentNode = this->graph.getNextUnvisitedNodeTarzan();

		while (!currentNode.isDummyNode()) { // while there are still possible SCC 

			doTarzanDFS(currentNode, tarzanStack);
			while (!tarzanStack.empty()) { tarzanStack.pop(); }
			currentNode = this->graph.getNextUnvisitedNodeTarzan();
		}

		for (int i = 0; i < this->graph.nodes.size(); i++) {
			cout << "Node " << i << " is part of component " << this->graph.nodes.at(i).lowLinkValue << endl;
		}
	}

	void doTarzanDFS(Node& currentNode, stack <Node>& tarzanStack) {

		if (!(currentNode.status == Node::NodeStatus::UNVISITED)) {
			return;
		}

		// else
		this->graph.nodes.at(currentNode.value).status = Node::NodeStatus::VISITING_NEIGHBOURS;
		this->graph.nodes.at(currentNode.value).lowLinkValue = currentNode.value;
		tarzanStack.push(currentNode);

		for (Node& currentNodeNeighbour : currentNode.adjList) {

			doTarzanDFS(this->graph.nodes.at(currentNodeNeighbour.value), tarzanStack);
			// now be backtrack

			if (!(this->graph.nodes.at(currentNodeNeighbour.value).status == Node::NodeStatus::UNVISITED) &&
				isOnStack(tarzanStack, currentNodeNeighbour)) {

				this->graph.nodes.at(currentNode.value).lowLinkValue =
					min(this->graph.nodes.at(currentNode.value).lowLinkValue, this->graph.nodes.at(currentNodeNeighbour.value).lowLinkValue);
			}

			this->graph.nodes.at(currentNode.value).status = Node::NodeStatus::VISITED_ALL_NEIGHBOURS;
		}
	}

	bool isOnStack(stack <Node> tarzanStack, Node node) {

		stack <Node> copyStack = tarzanStack;

		while (!copyStack.empty()) {

			if (copyStack.top().value == node.value) {

				return true;
			}
			copyStack.pop();
		}

		return false;
	}

	void doTopologicalSorting() {

		Node currentNode;
		currentNode = this->graph.getNextUnvisitedNodeTopological();
		stack<Node> topologicStack;

		while (!currentNode.isDummyNode()) {

			doDFSTopological(topologicStack, currentNode);
			currentNode = this->graph.getNextUnvisitedNodeTopological();
		}

		while (!topologicStack.empty()) {
			cout << topologicStack.top().value << " ";
			topologicStack.pop();
		}
	}

	void doDFSTopological(stack<Node>& topologicStack, Node currentNode) {

		if (this->graph.nodes.at(currentNode.value).visited == true) {
			return;
		}

		// else
		this->graph.nodes.at(currentNode.value).visited = true;

		for (Node neighbour : this->graph.nodes.at(currentNode.value).adjList) {

			doDFSTopological(topologicStack, neighbour);
		}

		topologicStack.push(this->graph.nodes.at(currentNode.value));
	}

public: DFS() {

}
};

void analysePerformance() {

	cout << "Test 1\n";
	for (int nrEdges = 1000; nrEdges <= 5000; nrEdges += 100) {

		cout << nrEdges << "/ 5000" << endl;
		DFS dfs(DFS::GraphType::DFSS, 100, nrEdges);

		Operation op = profiler.createOperation("Total operations", nrEdges);
		dfs.doDFS(dfs.graph.nodes.at(0), op);
	}

	cout << "\nTest 2\n";
	for (int nrNodes = 100; nrNodes <= 200; nrNodes += 10) {

		cout << nrNodes << "/ 200" << endl;
		DFS dfs(DFS::GraphType::DFSS, nrNodes, 9000);

		Operation op = profiler.createOperation("Total operations", nrNodes);
		dfs.doDFS(dfs.graph.nodes.at(0), op);
	}

	profiler.showReport();
}

int main() {

	Operation dummyOP = profiler.createOperation("dummy", 0);
	// choose between DFS::GraphType::NO_CYCLES_TOPOLOGICAL, DFS::GraphType::DFSS, DFS::GraphType::TARZAN
	DFS dfs(DFS::GraphType::TARZAN, 0, 0);
	dfs.graph.printAdjList();
	cout << endl;
	dfs.doTarzan();
	// dfs.doTarzan();
	// dfs.doTopologicalSorting();

	// analysePerformance();

	return 0;
}