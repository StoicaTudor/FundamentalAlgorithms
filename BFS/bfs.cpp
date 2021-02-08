/*
	Stoica Tudor Mihai
	Group 30423

	bfs has complexity(V+E)
	for the shortest path I used bfs (lee algorithm)
	in order to find the path, we has to go backwards, from end to start and look for the parent
	for bfs_tree, I used to code from lab 7
*/

#include <stdlib.h>
#include <string.h>
#include <queue>
#include <vector>
#include "bfs.h"

#include<iostream>
#include <cstdlib>

struct MultiWayTree {
	int father;
	std::vector<int> childrenNodes;
	int key;

	MultiWayTree(int key) { this->key = key; }

	void addChild(int childKey) { this->childrenNodes.push_back(childKey); }

	void setFather(int fatherKey) { this->father = fatherKey; }

	MultiWayTree() {}
};

bool coordsInsideGrid(int row, int col, const Grid* grid) {
	return ((row > 0 && row < grid->rows) && (col > 0 && col < grid->cols));
}


int get_neighbors(const Grid* grid, Point p, Point neighb[]) {
	// TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
	// the point p will have at most 4 neighbors (up, down, left, right)
	// avoid the neighbors that are outside the grid limits or fall into a wall
	// note: the size of the array neighb is guaranteed to be at least 4

	int nrNeighb = 0;

	if (coordsInsideGrid(p.row, p.col + 1, grid) && grid->mat[p.row][p.col + 1] == 0) {

		neighb[nrNeighb].row = p.row;
		neighb[nrNeighb].col = p.col + 1;
		nrNeighb++;
	}

	if (coordsInsideGrid(p.row, p.col - 1, grid) && grid->mat[p.row][p.col - 1] == 0) {

		neighb[nrNeighb].row = p.row;
		neighb[nrNeighb].col = p.col - 1;
		nrNeighb++;
	}

	if (coordsInsideGrid(p.row + 1, p.col, grid) && grid->mat[p.row + 1][p.col] == 0) {

		neighb[nrNeighb].row = p.row + 1;
		neighb[nrNeighb].col = p.col;
		nrNeighb++;
	}

	if (coordsInsideGrid(p.row - 1, p.col, grid) && grid->mat[p.row - 1][p.col] == 0) {

		neighb[nrNeighb].row = p.row - 1;
		neighb[nrNeighb].col = p.col;
		nrNeighb++;
	}
	return nrNeighb;
}

void grid_to_graph(const Grid* grid, Graph* graph) {
	//we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
	Node* nodes[MAX_ROWS][MAX_COLS];
	int i, j, k;
	Point neighb[4];

	//compute how many nodes we have and allocate each node
	graph->nrNodes = 0;
	for (i = 0; i < grid->rows; ++i) {
		for (j = 0; j < grid->cols; ++j) {
			if (grid->mat[i][j] == 0) {
				nodes[i][j] = (Node*)malloc(sizeof(Node));
				memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
				nodes[i][j]->position.row = i;
				nodes[i][j]->position.col = j;
				++graph->nrNodes;
			} else {
				nodes[i][j] = NULL;
			}
		}
	}
	graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
	k = 0;
	for (i = 0; i < grid->rows; ++i) {
		for (j = 0; j < grid->cols; ++j) {
			if (nodes[i][j] != NULL) {
				graph->v[k++] = nodes[i][j];
			}
		}
	}

	//compute the adjacency list for each node
	for (i = 0; i < graph->nrNodes; ++i) {
		graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
		if (graph->v[i]->adjSize != 0) {
			graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
			k = 0;
			for (j = 0; j < graph->v[i]->adjSize; ++j) {
				if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
					neighb[j].col >= 0 && neighb[j].col < grid->cols &&
					grid->mat[neighb[j].row][neighb[j].col] == 0) {
					graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
				}
			}
			if (k < graph->v[i]->adjSize) {
				//get_neighbors returned some invalid neighbors
				graph->v[i]->adjSize = k;
				graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
			}
		}
	}
}

void free_graph(Graph* graph) {
	if (graph->v != NULL) {
		for (int i = 0; i < graph->nrNodes; ++i) {
			if (graph->v[i] != NULL) {
				if (graph->v[i]->adj != NULL) {
					free(graph->v[i]->adj);
					graph->v[i]->adj = NULL;
				}
				graph->v[i]->adjSize = 0;
				free(graph->v[i]);
				graph->v[i] = NULL;
			}
		}
		free(graph->v);
		graph->v = NULL;
	}
	graph->nrNodes = 0;
}

Operation* countOp(Operation* op, int countNr) {

	if (op != NULL) {
		op->count(countNr);
		return op;
	}

	return NULL;
}

bool visited(Node* nodeToTest, bool* freqVector) {
	// in order to save space and time, I created a frequence vector which checks if we stepped on a certain cel in the past
	// for this, I had to encapsulate 2 variables in a single variable
	// so, rows are multiplied by 10 and added to collumns

	// constant time O(1)
	int cel = nodeToTest->position.row * 10 + nodeToTest->position.col;
	return (freqVector[cel]);
}

MultiWayTree* parentToMultiWayTree(int* parentsArr, MultiWayTree* multiWayArr, int size) {

	int rootKey = -1;

	for (unsigned int i = 0; i < size; i++) { // only 1 iteration O(n)

		if (parentsArr[i] >= 0) { // if we do not speak about the root

			multiWayArr[parentsArr[i]].key = parentsArr[i];
			multiWayArr[i].key = i;
			multiWayArr[parentsArr[i]].addChild(i);
			multiWayArr[i].setFather(parentsArr[i]);

		} else if (parentsArr[i] == -1) {  // if we speak about the root
			rootKey = i;
		}
	}

	return multiWayArr;
}

void pp(int curr, MultiWayTree* multiWayArr, Point* repr, int level) {

	for (int i = 0; i < level; i++) {
		std::cout << "      ";
	}

	std::cout << repr[curr].row << "  " << repr[curr].col << std::endl;

	for (int child : multiWayArr[curr].childrenNodes) {
		pp(child, multiWayArr, repr, level + 1);
	}
}

void printMultiWayArr(int size, MultiWayTree* multiWayArr, Point* repr) {

	for (int i = 0; i < size; i++) {
		std::cout << "key: " << multiWayArr[i].key << "     ";
		std::cout << "father: " << ((multiWayArr[i].father > size) ? -1 : multiWayArr[i].father) << "     ";
		std::cout << "children:  ";

		for (int child : multiWayArr[i].childrenNodes) {
			std::cout << child << " ";
		}

		std::cout << std::endl;
	}
	std::cout << std::endl;

	pp(0, multiWayArr, repr, 0);
}

bool* markCurrentNodeAsVisited(bool* freqVector, Node* currentNode) {

	freqVector[currentNode->position.row * 10 + currentNode->position.col] = true;
	return freqVector;
}

void bfs(Graph* graph, Node* s, Operation* op) {
	// TOOD: implement the BFS algorithm on the graph, starting from the node s
	// at the end of the algorithm, every node reachable from s should have the color BLACK
	// for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
	// for counting the number of operations, the optional op parameter is received
	// since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
	// if(op != NULL) op->count();

	bool* freqVector = new bool[graph->nrNodes * 10];

	std::queue<Node*> bfsQueue;
	s->dist = 0;
	bfsQueue.push(s);
	op = countOp(op, 2);
	Node* currentNode = NULL;

	op = countOp(op, 1);
	while (!bfsQueue.empty()) {

		op = countOp(op, 1);

		currentNode = bfsQueue.front();
		freqVector = markCurrentNodeAsVisited(freqVector, currentNode);
		currentNode->color = COLOR_BLACK;
		bfsQueue.pop(); // O(1) complexity
		op = countOp(op, 4);

		for (int i = 0; i < currentNode->adjSize; i++) {
			op = countOp(op, 1);
			if (!visited(currentNode->adj[i], freqVector)) {

				currentNode->adj[i]->parent = currentNode;
				currentNode->adj[i]->dist = currentNode->dist + 1;
				bfsQueue.push(currentNode->adj[i]); // O(1) complexity
				op = countOp(op, 3);
			}
		}
	}

	free(currentNode);
	free(freqVector);
}

void print_bfs_tree(Graph* graph) {

	//first, we will represent the BFS tree as a parent array
	int n = 0; //the number of nodes
	int* p = NULL; //the parent array
	Point* repr = NULL; //the representation for each element in p

	//some of the nodes in graph->v may not have been reached by BFS
	//p and repr will contain only the reachable nodes
	int* transf = (int*)malloc(graph->nrNodes * sizeof(int));
	for (int i = 0; i < graph->nrNodes; ++i) {
		if (graph->v[i]->color == COLOR_BLACK) {
			transf[i] = n;
			++n;
		} else {
			transf[i] = -1;
		}
	}
	if (n == 0) {
		//no BFS tree
		free(transf);
		return;
	}

	int err = 0;
	p = (int*)malloc(n * sizeof(int));
	repr = (Point*)malloc(n * sizeof(Node));
	for (int i = 0; i < graph->nrNodes && !err; ++i) {
		if (graph->v[i]->color == COLOR_BLACK) {
			if (transf[i] < 0 || transf[i] >= n) {
				err = 1;
			} else {
				repr[transf[i]] = graph->v[i]->position;
				if (graph->v[i]->parent == NULL) {
					p[transf[i]] = -1;
				} else {
					err = 1;
					for (int j = 0; j < graph->nrNodes; ++j) {
						if (graph->v[i]->parent == graph->v[j]) {
							if (transf[j] >= 0 && transf[j] < n) {
								p[transf[i]] = transf[j];
								err = 0;
							}
							break;
						}
					}
				}
			}
		}
	}
	free(transf);
	transf = NULL;

	if (!err) {
		// TODO: pretty print the BFS tree
		// the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
		// when printing the node k, print repr[k] (it contains the row and column for that point)
		// you can adapt the code for transforming and printing multi-way trees from the previous labs

		MultiWayTree* multiWayArr = new MultiWayTree[n];
		parentToMultiWayTree(p, multiWayArr, n);
		printMultiWayArr(n, multiWayArr, repr);
	}

	if (p != NULL) {
		free(p);
		p = NULL;
	}
	if (repr != NULL) {
		free(repr);
		repr = NULL;
	}
}

int shortest_path(Graph* graph, Node* start, Node* end, Node* path[]) {
	// TODO: compute the shortest path between the nodes start and end in the given graph
	// the nodes from the path, should be filled, in order, in the array path
	// the number of nodes filled in the path array should be returned
	// if end is not reachable from start, return -1
	// note: the size of the array path is guaranteed to be at least 1000

	bool weReachedEnd = false;

	bool* freqVector = new bool[graph->nrNodes * 10];
	path[0] = start;

	std::queue<Node*> bfsQueue;
	start->dist = 0;
	bfsQueue.push(start);

	Node* currentNode = NULL;

	while (!bfsQueue.empty() && !weReachedEnd) {

		currentNode = bfsQueue.front();
		freqVector = markCurrentNodeAsVisited(freqVector, currentNode);
		currentNode->color = COLOR_BLACK;
		bfsQueue.pop();

		for (int i = 0; i < currentNode->adjSize; i++) {

			if (!visited(currentNode->adj[i], freqVector)) {

				if (currentNode->adj[i] == end) {

					weReachedEnd = true;
					end->dist = currentNode->dist + 1;
					end->parent = currentNode;
					break;
				}
				currentNode->adj[i]->parent = currentNode;
				currentNode->adj[i]->dist = currentNode->dist + 1;
				bfsQueue.push(currentNode->adj[i]);
			}
		}
	}
	free(freqVector);

	// now, compute the path backwards, from the end to the start

	currentNode = end;
	int pathIndex = end->dist;

	while (currentNode != start) {

		path[--pathIndex] = currentNode;
		currentNode = currentNode->parent;
	}

	free(currentNode);

	return end->dist;
}

Graph createEdgeBetween(int node0, int node1, Graph graph) {

	// a commutative method - it creates a edge between node0 and node1, as well as between node1 and node0
	Node* a = graph.v[node0];
	graph.v[node1]->adj[graph.v[node1]->adjSize++] = a;

	Node* b = graph.v[node1];
	graph.v[node0]->adj[graph.v[node0]->adjSize++] = b;

	return graph;
}

Graph randomlyGenerateEdges(Graph graph, int nrEdges, int nrNodes) {

	// init adj array
	for (int i = 0; i < nrNodes; i++) {

		graph.v[i]->adj = new Node * [nrNodes];
	}

	// declare adjacency matrix
	bool** mat = new bool* [nrNodes];

	for (int i = 0; i < nrNodes; i++) {
		mat[i] = new bool[nrNodes];
	}

	// init adj matrix on false

	for (int i = 0; i < nrNodes; i++) {
		for (int j = 0; j < nrNodes; j++) {
			mat[i][j] = false;
		}
	}

	// in order to make sure that all our vertices ar connected, we will make a fence, by connecting 0 - 1, 1 - 2, ... , n-1 - 0
	for (int i = 0; i < nrNodes - 1; i++) {

		graph = createEdgeBetween(i, i + 1, graph);
		mat[i][i + 1] = mat[i + 1][i] = true;
	}

	// finally create an edge between n-1 and 0
	graph = createEdgeBetween(nrNodes - 1, 0, graph);
	mat[nrNodes - 1][0] = mat[0][nrNodes - 1] = true;

	nrEdges -= nrNodes;

	// in order to create the remaining edges
	while (nrEdges > 0) {

		int node0 = rand() % nrNodes;
		int node1 = rand() % nrNodes;
		// do random until you find a false combination of edges (until you find a false on adj matrix)
		while (mat[node0][node1] == true && mat[node1][node0] == true) {

			node0 = rand() % nrNodes;
			node1 = rand() % nrNodes;
		}

		// if we found an mat[node0][node1] == false
		mat[node0][node1] = mat[node1][node0] = true;
		graph = createEdgeBetween(node0, node1, graph); // create an edge between them
		nrEdges--;
	}

	// free(mat);
	return graph;
}

void performance() {
	int n, i;
	Profiler p("bfs");

	// vary the number of edges
	for (n = 1000; n <= 4500; n += 100) {
		Operation op = p.createOperation("bfs-edges", n);
		Graph graph;
		graph.nrNodes = 100;
		//initialize the nodes of the graph
		graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
		for (i = 0; i < graph.nrNodes; ++i) {
			graph.v[i] = (Node*)malloc(sizeof(Node));
			memset(graph.v[i], 0, sizeof(Node));
		}
		// TODO: generate n random edges
		// make sure the generated graph is connected

		graph = randomlyGenerateEdges(graph, n, graph.nrNodes);

		bfs(&graph, graph.v[0], &op);
		// free_graph(&graph);
	}

	// vary the number of vertices
	for (n = 100; n <= 200; n += 10) {
		Operation op = p.createOperation("bfs-vertices", n);
		Graph graph;
		graph.nrNodes = n;
		//initialize the nodes of the graph
		graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
		for (i = 0; i < graph.nrNodes; ++i) {
			graph.v[i] = (Node*)malloc(sizeof(Node));
			memset(graph.v[i], 0, sizeof(Node));
		}
		// TODO: generate 4500 random edges
		// make sure the generated graph is connected

		graph = randomlyGenerateEdges(graph, 4500, graph.nrNodes);

		bfs(&graph, graph.v[0], &op);
		// free_graph(&graph);
	}

	p.showReport();
}
