/******************************************************************************
 * File: Trailblazer.h
 *
 * Exports functions that use Dijkstra's algorithm, A* search, and Kruskal's
 * algorithm as specified in the assignment handout.
 */

#ifndef Trailblazer_Included
#define Trailblazer_Included

#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "set.h"
#include "grid.h"

struct directVertex {
	directVertex* parent;  // way from starting loc
	double cost;          // The cost from starting loc
	int color;           // color of Vertex
	Loc loc;            // Loc of Vertex
};

directVertex* getVertex(Loc loc, directVertex* parent, double cost, int color);

void fillWith(directVertex* defaultVal, Grid<directVertex* >& graph);

void dequeueMinTo(directVertex* & currVertex, TrailblazerPQueue<directVertex* > & toVisit, Grid<double>& world);

void findChildren(directVertex* currVertex, Grid<directVertex* >& graph, TrailblazerPQueue<directVertex* > &toVisit,
					Grid<double>& world, double costFn(Loc from, Loc to, Grid<double>& world),
					double heuristic(Loc start, Loc end, Grid<double>& world), Loc end);

void checkLocation(Loc currLoc, directVertex* parent, double lastStepCost, double distance, Grid<directVertex* >& graph,
				  TrailblazerPQueue<directVertex* > &toVisit, Grid<double>& world);

void saveWay(directVertex* currVertex, 	Vector<Loc> & result);

void clearGraph(Grid<directVertex* >& graph);

void createGraph(int numRows, int numCols, TrailblazerPQueue<Edge> &toVisit, Map<Loc, Set<Loc> > &clusters);

void saveEdge(Loc start, Loc end, TrailblazerPQueue<Edge> &toVisit, Map<Loc, Set<Loc> > &clusters);

void mergeClusters(Edge edge, Map<Loc, Set<Loc> > &clusters, int &numOfClusters);



Vector<Loc>
shortestPath(Loc start, Loc end,Grid<double>& world,
double costFunction(Loc one, Loc two, Grid<double>& world),
double heuristic(Loc start, Loc end, Grid<double>& world));


Set<Edge> createMaze(int numRows, int numCols);

#endif
