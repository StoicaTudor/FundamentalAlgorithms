/*
	Stoica Tudor Mihai
	Group 30423

	Task:

	You are required to implement correctly and efficiently the base operations for disjoint set
(chapter 21.1 from book ) and the Kruskal’s algorithm (searching for the minimum spanning 1
tree - chapter 23.2) using disjoint sets.
You have to use a tree as the representation of a disjoint set. Each tree holds, besides the
necessary information, also the rank field (i.e. the height of the tree).
The base operations on disjoints sets are:
● MAKE_SET (x)
○ creates a set with the element x
● UNION (x, y)
○ makes the union between the set that contains the element x and the set that
contains the element y
○ the heuristic union by rank takes into account the height of the two trees so as to
make the union
○ the pseudo-code can be found in the chapter 21.3 from the book 1
● FIND_SET (x)
○ searches for the set that contains the element x
○ the heuristic path compression links all nodes that were found on the path to x to
the root node

	
	Implementation:

	I used the vector class for my disjoint sets implementation;

	I created a vector of parents:
	vector<Node> supremeParents

	struct Node {

		int parent;
		int supremeParent;
		int key;
		vector <Node> adjacencyList;

		void init(int key, int supremeParent, int parent) {

			this->key = key;
			this->supremeParent = supremeParent;
			this->parent = parent;
		}

		bool isDummy() {

			return (this->key == -1 && this->supremeParent == -1 && this->parent == -1);
		}

		void addNodeInAdjacencyList(int key) {

			Node newNeighbour;
			newNeighbour.init(key, this->supremeParent, this->key);
			this->adjacencyList.push_back(newNeighbour);
		}
	};

	Every supremeParent has a key and a list of children;
	Every child has a list of children as well, since parents as well as children, are of type Node

	void makeSet(int key, Operation as, Operation co)
	creates a supremeParent and has the complexity O(1)

	int findSet(int key, Operation as, Operation co)
	has complexity O(1) in my implementation, since if we identify the node with the key key, we can automatically call node.supremeParent
	but in order to find the node with the key key, I have to iterate through all sets, through all nodes; this has the complexity O(n)
	however, in practice, the complexity is better, reaching O(log n) in average

	in order to iterate through all sets, I use


	Node findNodeWithKey(int key, Operation as, Operation co) {

		// iterate through all supreme parents i.e., through all sets
		for (Node parent : supremeParent) {
			co.count();
			if (parent.key == key) {

				return parent;
			}
			// else try to identify in that specific set the key we are looking for
			Node searchResult = find(parent, key, as, co);

			// if we found nothing, continue the search in next set
			if (searchResult.isDummy()) {
				continue;
			} else {
				// if we found it,  we return it
				return searchResult;
			}
		}

		// if we found nothing, we return a dummy result, which signifies that we found nothing

		Node notFound;
		notFound.init(-1, -1, -1); // init dummy result

		return notFound;
	}

	Node find(Node parent, int key, Operation as, Operation co) {

		for (Node child : parent.adjacencyList) {
			co.count();
			if (child.key == key) {

				return child;
			}

			Node searchResult =
				find(child, key, as, co);

			if (searchResult.isDummy()) {

			} else {
				return searchResult;
			}
		}


		// if we found nothing, we return a dummy result, which sigifies that we found nothing

		Node notFound;
		notFound.init(-1, -1, -1); // init dummy result

		return notFound;
		}

		void makeUnion(int key0, int key1, Operation as, Operation co) ->  void link(Node supremeParent0, Node supremeParent1, Operation as, Operation co)
		checks which set has a bigger rank; the set with the smaller rank gets pasted as a child in the set with the bigger rank
		if their ranks are equal, the rank of the set++
		this has the complexity O(log n) (from findSet); makeUnion has complexity O(1), but link has complexity O(log n) (from findSet)

		Kruskal algorithm has the complexity O(nlog n)
*/

#include <iostream>
#include <vector>
#include "Profiler.h"
#define NR_TESTS 5
#define STEP_SIZE 100
#define MAX_SIZE 10000

using namespace std;

Profiler profiler("DisjointSets");

class DisjointSets {

public:

	struct Node {

		int parent;
		int supremeParent;
		int key;
		vector <Node> adjacencyList;

		void init(int key, int supremeParent, int parent) {

			this->key = key;
			this->supremeParent = supremeParent;
			this->parent = parent;
		}

		bool isDummy() {

			return (this->key == -1 && this->supremeParent == -1 && this->parent == -1);
		}

		void addNodeInAdjacencyList(int key) {

			Node newNeighbour;
			newNeighbour.init(key, this->supremeParent, this->key);
			this->adjacencyList.push_back(newNeighbour);
		}
	};

public:
	vector <Node> supremeParent;
	vector <int> supremeParentRank;

private:
	static const int MAX_NR_SETS = 10000;

public:

	DisjointSets() {

	}

	void makeSet(int key, Operation as, Operation co) {

		co.count();
		if (supremeParent.size() == MAX_NR_SETS) {

			cout << "ERROR: MAXIMUM NUMBER OF SETS ALREADY REACHED\n";
			return;
		}

		as.count(5);
		Node newParent;
		newParent.init(key, key, key);
		supremeParent.push_back(newParent);
		supremeParentRank.push_back(0);
	}

	void makeUnion(int key0, int key1, Operation as, Operation co) {

		link(findSetNode(key0, as, co), findSetNode(key1, as, co), as, co);
		// link(key0, key1);
	}

private:
	void link(Node supremeParent0, Node supremeParent1, Operation as, Operation co) {

		int supremeParent0Index = getIndexOfSupremeParent(supremeParent0, as, co);
		int supremeParent1Index = getIndexOfSupremeParent(supremeParent1, as, co);

		//cout << "supremeParent0   " << supremeParent0.key << "  --->>>>   supremeParent0Index    " << supremeParent0Index << endl;
		//cout << "supremeParent1   " << supremeParent1.key << "  --->>>>   supremeParent1Index    " << supremeParent1Index << endl;

		// cout << supremeParentRank.at(supremeParent0Index) << "   " << supremeParentRank.at(supremeParent1Index);
		co.count();
		if (supremeParentRank.at(supremeParent0Index) > supremeParentRank.at(supremeParent1Index)) {
			// cout << "here\n";

			as.count();
			// the smaller ranked set will gain as supreme parent, the bigger ranked set
			supremeParent.at(supremeParent1Index).supremeParent = supremeParent0.supremeParent;

			//  every node in the smaller ranked set will gain as supreme parent, the bigger ranked set
			updateSupremeParent(supremeParent.at(supremeParent1Index), supremeParent0.supremeParent, as, co);

			as.count();
			// the smaller ranked set will become a child of the bigger ranked set
			supremeParent.at(supremeParent0Index).adjacencyList.push_back(supremeParent.at(supremeParent1Index));

			// remove the smaller ranked entities
			supremeParent.erase(supremeParent.begin() + supremeParent1Index);
			supremeParentRank.erase(supremeParentRank.begin() + supremeParent1Index);

		} else {
			// the smaller ranked set will gain as supreme parent, the bigger ranked set
			supremeParent.at(supremeParent0Index).supremeParent = supremeParent1.supremeParent;

			//  every node in the smaller ranked set will gain as supreme parent, the bigger ranked set
			updateSupremeParent(supremeParent.at(supremeParent0Index), supremeParent1.supremeParent, as, co);

			// the smaller ranked set will become a child of the bigger ranked set
			supremeParent.at(supremeParent1Index).adjacencyList.push_back(supremeParent.at(supremeParent0Index));

			co.count();
			if (supremeParentRank.at(supremeParent0Index) == supremeParentRank.at(supremeParent1Index)) {

				as.count();
				supremeParentRank.at(supremeParent1Index)++;

			} //else {

				// remove the smaller ranked entities
			supremeParent.erase(supremeParent.begin() + supremeParent0Index);

			supremeParentRank.erase(supremeParentRank.begin() + supremeParent0Index);
			//}
		}


		// cout << supremeParent0.key << " " << supremeParent1.key << endl;
	   // cout << supremeParent0 << " " << supremeParent1 << endl;
	}

private:

	void updateSupremeParent(Node& supremeParent0, int supremeParentKey, Operation as, Operation co) {

		for (auto& child : supremeParent0.adjacencyList) {
			as.count();
			child.supremeParent = supremeParentKey;
			updateSupremeParent(child, supremeParentKey, as, co);
		}
	}

	int getIndexOfSupremeParent(Node parent, Operation as, Operation co) {

		for (int i = 0; i < supremeParent.size(); i++) {
			co.count();
			if (supremeParent[i].key == parent.key) {

				return i;
			}
		}

		return -1;
	}
public:
	int findSet(int key, Operation as, Operation co) {

		Node searchResult = findNodeWithKey(key, as, co);

		if (searchResult.isDummy()) {
			cout << "findSet(" << key << ") was not succesful\n";
		}

		return searchResult.supremeParent;
	}

	Node findSetNode(int key, Operation as, Operation co) {

		Node searchResult = findNodeWithKey(key, as, co);

		if (searchResult.isDummy()) {
			cout << "findSet(" << key << ") was not succesful\n";
		}

		return findNodeWithKey(searchResult.supremeParent, as, co);
	}

private:
	Node findNodeWithKey(int key, Operation as, Operation co) {

		// iterate through all supreme parents i.e., through all sets
		for (Node parent : supremeParent) {
			co.count();
			if (parent.key == key) {

				return parent;
			}
			// else try to identify in that specific set the key we are looking for
			Node searchResult = find(parent, key, as, co);

			// if we found nothing, continue the search in next set
			if (searchResult.isDummy()) {
				continue;
			} else {
				// if we found it,  we return it
				return searchResult;
			}
		}

		// if we found nothing, we return a dummy result, which signifies that we found nothing

		Node notFound;
		notFound.init(-1, -1, -1); // init dummy result

		return notFound;
	}

	Node find(Node parent, int key, Operation as, Operation co) {

		for (Node child : parent.adjacencyList) {
			co.count();
			if (child.key == key) {

				return child;
			}

			Node searchResult =
				find(child, key, as, co);

			if (searchResult.isDummy()) {

			} else {
				return searchResult;
			}
		}

		/*
		* if we found nothing, we return a dummy result, which sigifies that we found nothing
		*/
		Node notFound;
		notFound.init(-1, -1, -1); // init dummy result

		return notFound;
	}

public:

	static struct Edge {

		int edge0;
		int edge1;
		int weight;

		void initEdge(int edge0, int edge1, int weight) {
			this->edge0 = edge0;
			this->edge1 = edge1;
			this->weight = weight;
		}

		static bool compare(Edge edge0, Edge edge1) {
			return(edge0.weight > edge1.weight);
		}
	};

	vector <Edge> kruskal(vector <Edge> edge, int nrEdges, Operation as, Operation co) {

		vector <Edge> minimumSpanningTree;
		initVerteces(nrEdges, as, co);
		sortEdges(edge);

		for (Edge currentEdge : edge) {

			if (this->findSet(currentEdge.edge0, as, co) != this->findSet(currentEdge.edge1, as, co)) {

				as.count();
				minimumSpanningTree.push_back(currentEdge);
				this->makeUnion(currentEdge.edge0, currentEdge.edge1, as, co);
			}
		}

		return minimumSpanningTree;
	}


	int getTotalWeightKruskal(vector <Edge> edge) {

		int totalWeight = 0;

		for (Edge currentEdge : edge) {

			totalWeight += currentEdge.weight;
		}

		return totalWeight;
	}

private:
	void initVerteces(int nrEdges, Operation as, Operation co) {

		for (int i = 0; i < nrEdges; i++) {
			this->makeSet(i, as, co);
		}
	}

	void sortEdges(vector<Edge>& edge) {

		for (int i = 0; i < edge.size() - 1; i++) {

			for (int j = i + 1; j < edge.size(); j++) {

				if (DisjointSets::Edge::compare(edge.at(i), edge.at(j))) {
					swap(edge.at(i), edge.at(j));
				}
			}
		}
	}
};

void demo() {

	Operation as = profiler.createOperation("Assignements", 10);
	Operation co = profiler.createOperation("Comparisons", 10);

	DisjointSets disjointSet;
	disjointSet.makeSet(0, as, co);
	disjointSet.makeSet(1, as, co);
	disjointSet.makeSet(2, as, co);
	disjointSet.makeSet(3, as, co);
	disjointSet.makeSet(4, as, co);
	disjointSet.makeSet(5, as, co);
	disjointSet.makeSet(6, as, co);
	disjointSet.makeSet(7, as, co);
	disjointSet.makeSet(8, as, co);


	disjointSet.makeUnion(1, 0, as, co);
	disjointSet.makeUnion(3, 2, as, co);
	disjointSet.makeUnion(3, 1, as, co);
	disjointSet.makeUnion(4, 5, as, co);

	disjointSet.makeUnion(6, 7, as, co);
	disjointSet.makeUnion(8, 7, as, co);
	disjointSet.makeUnion(4, 2, as, co);
	disjointSet.makeUnion(8, 1, as, co);

	cout << disjointSet.findSet(0, as, co) << endl;
	cout << disjointSet.findSet(1, as, co) << endl;
	cout << disjointSet.findSet(2, as, co) << endl;
	cout << disjointSet.findSet(3, as, co) << endl;
	cout << disjointSet.findSet(4, as, co) << endl;
	cout << disjointSet.findSet(5, as, co) << endl;
	cout << disjointSet.findSet(6, as, co) << endl;
	cout << disjointSet.findSet(7, as, co) << endl;
	cout << disjointSet.findSet(8, as, co) << endl;
}

void demoKruskal() {

	Operation as = profiler.createOperation("Assignements", 7);
	Operation co = profiler.createOperation("Comparisons", 7);

	vector<DisjointSets::Edge> edge;

	DisjointSets::Edge newEdge;

	newEdge.initEdge(0, 1, 2);
	edge.push_back(newEdge);

	newEdge.initEdge(0, 2, 3);
	edge.push_back(newEdge);

	newEdge.initEdge(0, 3, 3);
	edge.push_back(newEdge);

	newEdge.initEdge(1, 2, 4);
	edge.push_back(newEdge);

	newEdge.initEdge(1, 4, 3);
	edge.push_back(newEdge);

	newEdge.initEdge(2, 3, 5);
	edge.push_back(newEdge);

	newEdge.initEdge(2, 4, 1);
	edge.push_back(newEdge);

	newEdge.initEdge(3, 5, 7);
	edge.push_back(newEdge);

	newEdge.initEdge(4, 5, 8);
	edge.push_back(newEdge);

	newEdge.initEdge(5, 6, 9);
	edge.push_back(newEdge);

	DisjointSets disjointSet;
	cout << disjointSet.getTotalWeightKruskal(disjointSet.kruskal(edge, 7, as, co));
}

void doMainTask() {

	for (int i = 0; i < NR_TESTS; i++) {
		cout << "TEST  " << i << endl;
		for (int nrNodes = STEP_SIZE; nrNodes < MAX_SIZE; nrNodes += STEP_SIZE) {
			cout << "nrNodes  " << nrNodes << endl;
			Operation as = profiler.createOperation("Assigns", nrNodes);
			Operation co = profiler.createOperation("Comparisons", nrNodes);

			vector <DisjointSets::Edge> edge;
			DisjointSets::Edge newEdge;

			// first make sure that our graph is conex
			// create a circle ex:   0 - 1 - 2 - 3 -0
			for (int k = 0; k < nrNodes; k++) {

				if (k == nrNodes - 1) {
					newEdge.initEdge(0, k, rand() % 100);
				} else {
					newEdge.initEdge(k, k + 1, rand() % 100);
				}
				edge.push_back(newEdge);
			}

			// after that, we can add the rest n*4 - n = n*3 remaining edges
			/* we perform this in this way:
			* 0 - 2, 0 - 3, 0 - 4
			* 1 - 3, 1 - 4, 1 - 5
			* and so on
			* in this way, we will have >= n*4 edges;always
			*/

			for (int k = 0; k < nrNodes; k++) {

				if (k + 2 >= nrNodes) {

					newEdge.initEdge(k, k + 2 - nrNodes, rand() % 100);
				} else {
					newEdge.initEdge(k, k + 2, rand() % 100);
				}
				edge.push_back(newEdge);

				if (k + 3 >= nrNodes) {
					newEdge.initEdge(k, k + 3 - nrNodes, rand() % 100);
				} else {
					newEdge.initEdge(k, k + 3, rand() % 100);
				}
				edge.push_back(newEdge);

				if (k + 4 >= nrNodes) {
					newEdge.initEdge(k, k + 4 - nrNodes, rand() % 100);
				} else {
					newEdge.initEdge(k, k + 4, rand() % 100);
				}
				edge.push_back(newEdge);
			}

			DisjointSets disjointSet;
			disjointSet.kruskal(edge, nrNodes, as, co);
		}
	}

	profiler.createGroup("Total Operations","Assigns","Comparisons");
	profiler.showReport();
}

int main() {

	// demo();
	demoKruskal();
	// doMainTask();

	return 0;
}