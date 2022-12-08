#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
typedef struct LINKED_LIST_NODE_s *LINKED_LIST_NODE;
typedef struct LINKED_LIST_NODE_s
{
	LINKED_LIST_NODE next; 
	void *data;
} LINKED_LIST_NODE_t[1];

typedef struct LINKED_LIST_s *LINKED_LIST;
typedef struct LINKED_LIST_s
{
	LINKED_LIST_NODE head; 
} LINKED_LIST_t[1], *LINKED_LIST;

typedef struct // Edge struct
{
	int source;
	int destination;

} EDGE;

typedef struct // Graph struct with edges array
{
	int num_vertices;
	int num_edges;

	EDGE *edges;
} GRAPH;

GRAPH *createGraph(int vertices, int edges);
void fillGraph(GRAPH *graph);


GRAPH *createGraph(int vertices, int edges) // Allocate memory with given vertices and edges sizes
{
	GRAPH *graph = calloc(1, sizeof(*graph));
	if (graph == NULL)
	{
		printf("Error:");
		exit(EXIT_FAILURE);
	}

	graph->num_vertices = vertices;
	graph->num_edges = edges;
	graph->edges = calloc(edges, sizeof(EDGE));

	if (graph->edges == NULL)
	{
		printf("Error:");
		exit(EXIT_FAILURE);
	}
	return graph;
}

void fillGraph(GRAPH *graph) // Define edges manually
{
	graph->edges[0].source = 1;
	graph->edges[0].destination = 2;

	graph->edges[1].source = 1;
	graph->edges[1].destination = 3;

	graph->edges[2].source = 1;
	graph->edges[2].destination = 4;

	graph->edges[3].source = 2;
	graph->edges[3].destination = 4;

	graph->edges[4].source = 2;
	graph->edges[4].destination = 5;

	graph->edges[5].source = 4;
	graph->edges[5].destination = 3;

	graph->edges[6].source = 4;
	graph->edges[6].destination = 5;
}


int **createMatrix(GRAPH *graph) // Create a matrix with given graph
{

	int **matrix = (int **)malloc(sizeof(int *) * graph->num_vertices);
	for (int i = 0; i < graph->num_vertices; i++)
		matrix[i] = (int *)malloc(sizeof(int) * graph->num_vertices);
	

	for (int i = 0; i < graph->num_vertices; i++)
	{
		for (int j = 0; j < graph->num_vertices; j++)
		{
			matrix[i][j] = 0;
		}
	}

	for (int i = 0; i < graph->num_edges; i++)
		matrix[graph->edges[i].source - 1][graph->edges[i].destination - 1] = 1;

	return matrix;
}

void printMatrix(int **matrix, int size) // Print the given matrix
{
	
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}




LINKED_LIST newList() // Create a new list with first node null
{
	LINKED_LIST list = (LINKED_LIST)malloc(sizeof(LINKED_LIST_t));
	list->head = NULL;
	return list;
}

LINKED_LIST_NODE newNode(void *data) // Create a node
{
	LINKED_LIST_NODE node = (LINKED_LIST_NODE)malloc(sizeof(LINKED_LIST_NODE_t));
	node->next = NULL;
	node->data = data;
	return node;
}


void addLast(LINKED_LIST list, LINKED_LIST_NODE node) // Insert the given node to the end of the given list
{
	LINKED_LIST_NODE root = list->head;
	if (root == NULL)
	{
		list->head = node;
		list->head->next = NULL;
	}
	else
	{
		while (root->next != NULL)
		{
			root = root->next;
		}
		root->next = node;
		node->next = NULL;
	}
}

void addFirst(LINKED_LIST list, LINKED_LIST_NODE node)
{
	LINKED_LIST_NODE root = list->head;
	if (root == NULL)
	{
		list->head = node;
	}
	else
	{
		node->next = root;
		list->head = node;
	}
}


void printList(LINKED_LIST list) // Print the given list
{
	LINKED_LIST_NODE root = list->head;

	while (root != NULL)
	{
		if (root->next == NULL)
		{
			printf("Vertex%d", root->data);
		}
		else
			printf("Vertex%d--> ", root->data);

		root = root->next;
	}
}



int *createIndegrees(int **matrix, int size) // Create an indegree array with the given matrix
{

	int *indegreeArray = (int *)malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++)
		indegreeArray[i] = 0;


	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (matrix[j][i] == 1)
				indegreeArray[i] += 1;
		}
	}

	return indegreeArray;
}

int findOutdegrees(int **matrix, int size, int vertex) // Find outdegree of the given vertex
{

	int counter = 0;

	for (int i = 0; i < size; i++)
	{
		if (matrix[vertex][i] == 1)
		{
			counter++;
		}
	}
	return counter;
}

void updateMatrix(int** matrix, int size, int vertexZeroIndegree){
    for(int i= 0; i< size ;i++){
        if(matrix[vertexZeroIndegree][i] == 1){
            matrix[vertexZeroIndegree][i] = 0;
        }
    }
}



int DepthFirstSearch(int **matrix, int size, int vertex) // Depth first search algorithm, that work with given matrix and vertex
{

	if (findOutdegrees(matrix, size, vertex) == 0)
		return vertex;

	for (int i = 0; i < size; i++)
	{
		if (matrix[vertex][i] == 1)
		{
			DepthFirstSearch(matrix, size, i);
			break;
		}
	}
}

LINKED_LIST TopologicalDepthFirstSearch(int **matrix, int size) // Topological sort algorithm uses Depth First Search algorithm, Return outputlist
{

	LINKED_LIST topologicalList = newList();

	int *inDegreeArray = createIndegrees(matrix, size);
	int vertexZeroIndegree;
	for (int i = 0; i < size; i++)
	{
		if (inDegreeArray[i] == 0)
			vertexZeroIndegree = i; // Find the vertex with zero indegree
	}

	while (findOutdegrees(matrix, size, vertexZeroIndegree) != 0) // If our zero indegree vertex have outdegree
	{
		int number = DepthFirstSearch(matrix, size, vertexZeroIndegree); // Find deepest vertex
		addFirst(topologicalList, newNode(number + 1)); // Add the list as first element

		for (int i = 0; i < size; i++)
			matrix[i][number] = 0;  // Remove the edges which are going to the deepest  vertex
		

        printf("\n\n Visited Deepest Vertex : %d\n",number +1);
        printMatrix(matrix,size);

    }
    printf("\n\nVisited Deepest Vertex : %d\n",vertexZeroIndegree +1);
    printMatrix(matrix,size);
	addFirst(topologicalList, newNode(vertexZeroIndegree + 1)); // Add the last zero indegree vertex


	return topologicalList;
}


LINKED_LIST TopologicalSourceRemoval(int **matrix, int size){ // Topological sort algorithm uses Source Removal algorithm, Return output list
	
	LINKED_LIST topologicalList = newList(); // New list
	bool visited[size]; // Visited array

	int numVisitedVertices = 0; // Visited vertices	
	int* indegreeArray = createIndegrees(matrix, size); // Indegree array
	int vertexZeroIndegree = -1;

    for(int i = 0; i  < size; i++)
        visited[i] = false;

	while(numVisitedVertices < size){
		bool zeroDegree = false;

		for(int i= 0; i < size; i++)
		{
			if(indegreeArray[i] == 0)
				zeroDegree = true;
		}

		if(zeroDegree == false){
			break;
		}
		else
		{
			for(int i = 0; i <size; i++){
				if(indegreeArray[i] == 0 && !visited[i])
				{
					vertexZeroIndegree = i; // Find vertex with 0 indegree
				}
			}
            numVisitedVertices++;
			visited[vertexZeroIndegree] = true; // Visited vertex

            addLast(topologicalList, newNode(vertexZeroIndegree+1)); // Add the node to the end of the list
            updateMatrix(matrix,size,vertexZeroIndegree); // Remove the edges from vertex who have zero indegree

            indegreeArray = createIndegrees(matrix,size); // Update indegree array

			printf("\n\n");
            printf("Visited Vertex : %d\n",vertexZeroIndegree +1);
            printMatrix(matrix,size);
	
		}
		
	}
	return topologicalList;
	
}



int main()
{

    // THE DEPTH FIRST SEARCH ALGORITHM
    //----------------------------------------------------------------------------------------------------

	GRAPH *graph = createGraph(5, 7);  // Creating graph
	fillGraph(graph); // Fill the graph with given edges

	int **matrix = createMatrix(graph); // Create the matrix version of graph
	printf("The matrix for Depth First Search :\n");
	printMatrix(matrix, graph->num_vertices);

	LINKED_LIST topologicalList = TopologicalDepthFirstSearch(matrix, graph->num_vertices);
	printf("\nSorted list with Depth First Search: \n");
	printList(topologicalList);

    //----------------------------------------------------------------------------------------------------




    // THE SOURCE REMOVAL ALGORITHM
    //----------------------------------------------------------------------------------------------------
	
	GRAPH *graph1 = createGraph(5,7); // Creating graph
	fillGraph(graph1); // Fill the graph with given edges
	
	int **matrix1 = createMatrix(graph1); // Create the matrix version of graph
	printf("\n\n\n\n\n\n\nThe matrix for Source Removal algorithm : \n");
	printMatrix(matrix1, graph1->num_vertices);
	
	LINKED_LIST topologicalList1 = TopologicalSourceRemoval(matrix1, graph->num_vertices);
	printf("\nSorted list with Source Removal Algorithm : \n");
	printList(topologicalList1);
    printf("\n");

    //----------------------------------------------------------------------------------------------------

    return 0;
}

