#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "random.h"

using namespace std;


Vector<Loc> shortestPath(Loc start, Loc end,Grid<double>& world,	
				double costFunction(Loc one, Loc two, Grid<double>& world),
				double heuristic(Loc start, Loc end, Grid<double>& world)) {

	Vector<Loc> result;
	TrailblazerPQueue<directVertex* > toVisit; // Yellow Vertexes

	Grid<directVertex* > graph(world.nRows, world.nCols);
	fillWith(NULL, graph); // fill graph with Gray Vertexes

	directVertex* currVertex = getVertex(start, NULL, 0, YELLOW);  //   Enqueue
	toVisit.enqueue(currVertex, heuristic(start, end, world));    // Starting Vertex

	while(!toVisit.isEmpty()) {
		dequeueMinTo(currVertex, toVisit, world);
		if (currVertex -> loc == end) break; // FOUND
		//else find neighbour vertexes
		findChildren(currVertex, graph, toVisit, world, costFunction, heuristic, end);
	}
		
	saveWay(currVertex, result);
	clearGraph(graph);
	
    return result;
}

Set<Edge> createMaze(int numRows, int numCols) {
	Set<Edge> result;
	Map<Loc, Set<Loc> > clusters;         // loc as key & its cluster as value
	TrailblazerPQueue<Edge> toVisit;     // edges to visit

	createGraph(numRows, numCols, toVisit, clusters);
	int numOfClusters = clusters.size();
	
	while (numOfClusters > 1) {
		Edge edge = toVisit.dequeueMin();
		if (clusters[edge.start].contains(edge.end)) continue;  // if start & end already connected
		mergeClusters(edge, clusters, numOfClusters);          // else merge them
		result.add(edge);                                     // and save in result
	}

    return result;
}


directVertex* getVertex(Loc loc, directVertex* parent, double cost, int color) {
	directVertex* currVertex = new directVertex;
	currVertex -> loc = loc;
	currVertex -> parent = parent;
	currVertex -> cost = cost;
	currVertex -> color = color;

	return currVertex;
}

/* Method fills graph with default values */
void fillWith(directVertex* defaultVal, Grid<directVertex* >& graph) {
	for (int i = 0; i < graph.nRows; i++) {
		for (int j = 0; j < graph.nCols; j++) {
			graph[i][j] = defaultVal;
		}
	}
}

/* dequeue min , save at currVertex and make it GREEN */
void dequeueMinTo(directVertex* & currVertex, TrailblazerPQueue<directVertex* > & toVisit, Grid<double>& world) {
	currVertex = toVisit.dequeueMin();
	currVertex -> color = GREEN;
	colorCell(world, currVertex -> loc, GREEN);
}

/* Find eight neighbour/child vertex of currVertex */
void findChildren(directVertex* currVertex, Grid<directVertex* >& graph, TrailblazerPQueue<directVertex* > &toVisit,
					Grid<double>& world, double costFn(Loc from, Loc to, Grid<double>& world),
					double heuristic(Loc start, Loc end, Grid<double>& world), Loc end) {
	int row = currVertex -> loc.row - 1;
	int col = currVertex -> loc.col - 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			/* if out of bound or current Vertex just IGNORE */
			if ((i == 1 && j == 1) || (!graph.inBounds(row + i, col + j))) continue;
			Loc child = makeLoc(row + i, col + j);
			double lastStepCost = costFn(currVertex -> loc, child, world);
			double distance = heuristic(child, end, world);

			checkLocation(child, currVertex, lastStepCost, distance, graph, toVisit, world); 
		}
	}
}

/* Enqueue vertex at toVisit and make it Yellow */
void enqueueVertex(directVertex* vertex, double distance, TrailblazerPQueue<directVertex* > &toVisit, Grid<double>& world) {
	colorCell(world, vertex->loc, YELLOW);
	toVisit.enqueue(vertex, (vertex -> cost) + distance);
}

void checkLocation(Loc childLoc, directVertex* parent, double lastStepCost, double distance, Grid<directVertex* >& graph,
				  TrailblazerPQueue<directVertex* > &toVisit, Grid<double>& world) {
	directVertex* childVertex = graph[childLoc.row][childLoc.col];
					/* If Vertex is Gray */
	if (childVertex == NULL) { 
		childVertex = graph[childLoc.row][childLoc.col] = getVertex(childLoc, parent, parent -> cost + lastStepCost, YELLOW);
		enqueueVertex(childVertex, distance, toVisit, world);
					/* Else if Vertex is Yellow and found cheaper way */
	} else if ((childVertex -> color == YELLOW) && ((childVertex -> cost) > (parent -> cost + lastStepCost))) {
		childVertex -> cost = parent -> cost + lastStepCost;
		childVertex -> parent = parent;
		toVisit.decreaseKey(childVertex, childVertex -> cost + distance);
	} 
}

/* Save each vertex in result From currVertex To Starting Vertex */
void saveWay(directVertex* currVertex, 	Vector<Loc> & result) {
	if (currVertex == NULL) return;

	saveWay(currVertex -> parent, result);
	result.add(currVertex -> loc);
}

/* delete non-NULL Vertexes */
void clearGraph(Grid<directVertex* >& graph) {
	for (int i = 0; i < graph.nRows; i++) {
		for (int j = 0; j < graph.nCols; j++) {
			if (graph[i][j] != NULL) delete graph[i][j];
		}
	}
}

bool inBounds(int x, int y, int numRows, int numCols) {
	return (x >= 0 && x < numRows) && (y >= 0 && y < numCols);
}

/* Method creates graph for MAZE */
void createGraph(int numRows, int numCols, TrailblazerPQueue<Edge> &toVisit, Map<Loc, Set<Loc> > &clusters) {
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			Loc currLoc = makeLoc(i, j);
			/* Save Each Edge */
			if (inBounds(i, j + 1, numRows, numCols)) saveEdge(currLoc, makeLoc(i, j + 1), toVisit, clusters);
			if (inBounds(i + 1, j, numRows, numCols)) saveEdge(currLoc, makeLoc(i + 1, j), toVisit, clusters);
				
		}
	}
}

/* Create new cluster and add itself */
void addCluster(Loc loc, Map<Loc, Set<Loc> > &clusters) {
	Set<Loc> clust;
	clust.add(loc);
	clusters[loc] = clust;
}

/* Create Edge by start-end Locs, Enqueue and add new cluster */
void saveEdge(Loc start, Loc end, TrailblazerPQueue<Edge> &toVisit, Map<Loc, Set<Loc> > &clusters) {
	toVisit.enqueue(makeEdge(start, end), randomReal(0,1));
	addCluster(start, clusters);
	addCluster(end, clusters);
}

/* change cluster (by MERGED) of each loc's cluster member */
void changeClusters(Loc loc, Set<Loc> &merged, Map<Loc, Set<Loc> > &clusters) {
	/* for each member of loc's cluster */
	foreach (Loc currLoc in clusters[loc]) {
		clusters[currLoc] = merged;
	}
}

void mergeClusters(Edge edge, Map<Loc, Set<Loc> > &clusters, int &numOfClusters) {
	Set<Loc> merged = clusters[edge.start] + clusters[edge.end];
	changeClusters(edge.start, merged, clusters); // change cluster of start
	changeClusters(edge.end, merged, clusters);  // change cluster of end
	numOfClusters--;                            // decrease num of clusters
}