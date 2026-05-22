#include <stdio.h>
#include <stdlib.h>

const int INF = 99999;

typedef struct Edge {
    int weight;
    int from_id;
    int to_id;
} Edge;

typedef struct Graph {
    Edge** edges;
    int* nodes;
    int node_count;
    int edge_count;
} Graph;


Edge* init_Edge(int from_id, int to_id, int weight) {
    Edge* edge = malloc(sizeof(Edge));
    if (edge == NULL) {
        return NULL;
    }
    edge->from_id = from_id;
    edge->to_id = to_id;
    edge->weight = weight;
    return edge;
}

Graph* init_Graph() {
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->node_count = 0;
    graph->nodes = NULL;
    graph->edge_count = 0;
    graph->edges = NULL;
    return graph;
}

int add_node(Graph* graph, int value) {
    if (graph == NULL) {
        return 1;
    }
    int* new_nodes = (int*)realloc(graph->nodes, sizeof(int) * (graph->node_count + 1));
    if (new_nodes == NULL) {
        return 1;
    }
    graph->nodes = new_nodes;
    graph->nodes[graph->node_count] = value;
    graph->node_count++;

    return 0;
}


int add_edge(Graph* graph, int from_id, int to_id, int weight) {
    if (graph == NULL) {
        return 1;
    }
    if ((from_id >= graph->node_count) || (to_id >= graph->node_count)) {
        return 1;
    }
    if (weight < 0) {
        return 1;
    }
    Edge* edge = init_Edge(from_id, to_id, weight);
    if (edge == NULL) {
        return 1;
    }
    Edge** new_edges = (Edge**)realloc(graph->edges, sizeof(Edge*) * (graph->edge_count + 1));
    if (new_edges == NULL) {
        free(edge);
        return 1;
    }
    graph->edges = new_edges;
    graph->edges[graph->edge_count] = edge;
    graph->edge_count++;

    return 0;
}

int delete_edge(Graph* graph, int edge_id) {
    if (graph == NULL) {
        return 1;
    }
    if (edge_id >= graph->edge_count) {
        return 1;
    }
    free(graph->edges[edge_id]);
    for (int i = edge_id + 1; i < graph->edge_count; i++) {
        graph->edges[i - 1] = graph->edges[i];
    }
    graph->edges[graph->edge_count - 1] = NULL;
    graph->edge_count--;
    return 0;
}

int delete_node(Graph* graph, int node_id) {
    if (graph == NULL) {
        return 1;
    }
    if (node_id >= graph->node_count) {
        return 1;
    }
    for (int i = node_id + 1; i < graph->node_count; i++) {
        graph->nodes[i - 1] = graph->nodes[i];
    }
    graph->node_count--;
    int edge_id = 0;
    while (edge_id < graph->edge_count) {
        if ((graph->edges[edge_id]->from_id == node_id) || (graph->edges[edge_id]->to_id == node_id)) {
            delete_edge(graph, edge_id);
        } else {
            if (graph->edges[edge_id]->from_id > node_id) {
                graph->edges[edge_id]->from_id--;
            }
            if (graph->edges[edge_id]->to_id > node_id) {
                graph->edges[edge_id]->to_id--;
            }
            edge_id++;
        }
    }
    return 0;
}

void delete_graph(Graph* graph) {
    if (graph == NULL) {
        return;
    }
    while (graph->node_count > 0) {
        delete_node(graph, 0);
    }
    free(graph->nodes);
    free(graph->edges);
    free(graph);
}

void print_graph(Graph* graph, FILE* output) {
    if (graph == NULL) {
        fprintf(output, "Empty graph\n");
        return;
    }
    if (graph->edge_count == 0) {
        fprintf(output, "Empty graph\n");
        return;
    }
    for (int i = 0; i < graph->edge_count; i++) {
        fprintf(output, "Edge %d (weight %d): ", i, graph->edges[i]->weight);
        fprintf(output, "from id %d val %d ", graph->edges[i]->from_id, graph->nodes[graph->edges[i]->from_id]);
        fprintf(output, "to id %d val %d", graph->edges[i]->to_id, graph->nodes[graph->edges[i]->to_id]);
        fprintf(output, "\n");
    }
}

int* dijkstra(Graph* graph, int start_ind) {
    if (graph == NULL) {
        return NULL;
    }
    if (start_ind < 0 || start_ind >= graph->node_count) {
        return NULL;
    }
    int* distances = (int*)malloc(sizeof(int) * graph->node_count);
    if (distances == NULL) {
        return NULL;
    }
    int* nodes_queue = (int*)malloc(sizeof(int) * graph->node_count * graph->node_count);
    if (nodes_queue == NULL) {
        free(distances);
        return NULL;
    }
    int* dists_queue = (int*)malloc(sizeof(int) * graph->node_count * graph->node_count);
    if (dists_queue == NULL) {
        free(nodes_queue);
        free(distances);
        return NULL;
    }
    distances[start_ind] = 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (i != start_ind) {
            distances[i] = INF;
        }
    }
    int weight = 0, cur_dist = 0, neighbor = 0, cur_ind = 0;
    nodes_queue[0] = start_ind;
    dists_queue[0] = 0;
    int queue_len = 1;
    int min_dist_ind = 0;
    while (queue_len > 0) {
        min_dist_ind = 0;
        for (int i = 0; i < queue_len; i++) {
            if (dists_queue[i] < dists_queue[min_dist_ind]) {
                min_dist_ind = i;
            }
        }
        cur_ind = nodes_queue[min_dist_ind];
        cur_dist = dists_queue[min_dist_ind];
        for (int i = min_dist_ind + 1; i < queue_len; i++) {
            nodes_queue[i - 1] = nodes_queue[i];
            dists_queue[i - 1] = dists_queue[i];
        }
        queue_len--;
        if (cur_dist > distances[cur_ind]) {
            continue;
        }
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i]->from_id == cur_ind) {
                neighbor = graph->edges[i]->to_id;
                weight = graph->edges[i]->weight;

                if (cur_dist + weight < distances[neighbor]) {
                    distances[neighbor] = cur_dist + weight;
                    nodes_queue[queue_len] = neighbor;
                    dists_queue[queue_len] = cur_dist + weight;
                    queue_len++;
                }
            }
        }
    }

    free(nodes_queue);
    free(dists_queue);

    return distances;
}

int main(void) {
    FILE* input = fopen("input.txt", "r");
    if (input == NULL) {
        return 1;
    }
    FILE* output = fopen("output.txt", "w");
    if (output == NULL) {
        fclose(input);
        return 1;
    }
    Graph* graph = init_Graph();
    if (graph == NULL) {
        fprintf(output, "Error\n");
        fclose(input);
        fclose(output);
        return 1;
    }
    int command = 0;
    int val = 0;
    int from_id = 0, to_id = 0, weight = 0;
    int node_id = 0, edge_id = 0;
    int* distances;
    do {
        fscanf(input, "%d", &command);
        switch (command)
        {
        case 0:
            fprintf(output, "Exit\n");
            break;
        case 1:
            fscanf(input, "%d", &val);
            if (add_node(graph, val)) {
                fprintf(output, "Error");
                fclose(input);
                fclose(output);
                delete_graph(graph);
                return 1;
            }
            break;
        case 2:
            fscanf(input, "%d", &from_id);
            fscanf(input, "%d", &to_id);
            fscanf(input, "%d", &weight);
            if (add_edge(graph, from_id, to_id, weight)) {
                fprintf(output, "Error");
                fclose(input);
                fclose(output);
                delete_graph(graph);
                return 1;
            }
            break;
        case 3:
            fscanf(input, "%d", &node_id);
            if (delete_node(graph, node_id)) {
                fprintf(output, "Error");
                fclose(input);
                fclose(output);
                delete_graph(graph);
                return 1;
            }
            break;
        case 4:
            fscanf(input, "%d", &edge_id);
            if (delete_edge(graph, edge_id)) {
                fprintf(output, "Error");
                fclose(input);
                fclose(output);
                delete_graph(graph);
                return 1;
            }
            break;
        case 5:
            if (graph->node_count == 0) {
                fprintf(output, "No nodes\n");
                break;
            }
            fscanf(input, "%d", &node_id);
            distances = dijkstra(graph, node_id);
            if (distances == NULL) {
                fprintf(output, "Error");
                fclose(input);
                fclose(output);
                delete_graph(graph);
                return 1;
            }
            for (int i = 0; i < graph->node_count; i++) {
                if (distances[i] == INF) {
                    fprintf(output, "-1 ");
                }
                else {
                    fprintf(output, "%d ", distances[i]);
                }
            }
            fprintf(output, "\n");
            free(distances);
            break;
        case 6:
            print_graph(graph, output);
            break;
        default:
            fprintf(output, "Error: Unknown command\n");
            break;
        }
    } while (command != 0);
    
    fclose(input);
    fclose(output);
    delete_graph(graph);

    return 0;
}