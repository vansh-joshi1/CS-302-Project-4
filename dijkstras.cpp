// dijsktras.cpp
// Name: Vansh Joshi
// Date: 10/23/2024

// credit to chatGPT for helping me implement the findPath function
// credit to Mia Patrikios for helping start off with the functions
// credit dev.to helping me understand how to path find in C++ and make my function
// https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h

#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

using namespace std;
// Main Execution

// class representing a node graph
class graph_Node{
    public:
        int x; 
        int y;
        int distance_from_start;
        map <int, char> neighbor;
        graph_Node* prev;
};

// reads the tile types and their associated point values from input
void readTilePoints (int& num_tiles, map<char, int>& tile_points) {
    char tile;
    int point;
    int temp_count = 0;
    
    cin >> num_tiles;
    while(temp_count < num_tiles) {
        cin >> tile >> point;
        tile_points.insert(pair<char, int>(tile, point));
        temp_count++;
    }
}

// reads the map dimensions and tile layout from input
void readMap (int& map_row, int& map_col, vector<vector<int>>& my_map) {
    // read mapdimensions
    cin >> map_row >> map_col;
    my_map.resize(map_row);
    const int cols = map_col;
    
    // read map tiles
    for (int i = 0; i < map_row; i++) {
        my_map[i].resize(cols);
        for(int j = 0; j < cols; j++) {
            char map_tile;
            cin >> map_tile;
            my_map[i][j] = map_tile;
        }
    }
}

// reads start and finish coordinates from input
void readStartFinish (int& start_x, int& start_y, int& finish_x, int& finish_y) {
    cin >> start_x >> start_y;
    cin >> finish_x >> finish_y;
}

// creates a graph representation of the map and initializes nodes and their connections to adjacent tiles
void createGraph(vector<vector<int>>& my_map, vector<vector<graph_Node*>>& my_graph, int map_col) {
    // Initialize graph dimensions
    const int rows = my_map.size();
    my_graph.resize(rows);

    const int dirX[] = {1, -1, 0, 0}; // down, up, same, same
    const int dirY[] = {0, 0, -1, 1};

    // create noes and establish connections
    for (int i = 0; i < rows; i++) {
        my_graph[i].resize(my_map[i].size());
        for(int j = 0; j < int(my_map[i].size()); j++) {

            // initizalize current node
            graph_Node* temp_node = new graph_Node();
            temp_node->x = i;
            temp_node->y = j;
            temp_node->distance_from_start = 10000000;

            for (int dir = 0; dir < 4; dir++) {
                int new_i = i + dirX[dir];
                int new_j = j + dirY[dir];
            // add neighbor if within bounds
            if (new_i >= 0 && new_i < rows && new_j >= 0 && new_j < int(my_map[i].size())) {
                int neighbor_pos = new_i * map_col + new_j;
                temp_node->neighbor.insert(pair<int, char>(neighbor_pos, my_map[new_i][new_j]));
            }
        }
        my_graph[i][j] = temp_node;
    }
}
}

// implement algorithm to find shortest path by visits all noes to ensure shortest path is found
void findPath(vector<vector<graph_Node*>>& my_graph, vector<vector<int>>& my_map, map<char, int>& tile_points, int start_x, int start_y, int map_row, int map_col, set<int>& path) {
    map<int, char>::iterator neighbor_it;
    multimap<int, int> distance_map;
    multimap<int, int>::iterator distance_it;
    set<string> graph_route;
    int cur_xy = (start_x)*map_col + (start_y);
    graph_Node* cur_node;

    // initialize start node
    graph_route.insert(to_string(cur_xy));
    cur_node = my_graph[cur_xy / map_col][cur_xy % map_col];
    cur_node->distance_from_start = 0;
    distance_map.insert(pair<int, int>(0, cur_xy));
    cur_node->prev = NULL;

    // use AI to help me write this part of the code
    // checks all neighbors of current node and if you find a shorter path update the distance
    while(int(graph_route.size()) != map_row*map_col) {
        for(neighbor_it = cur_node->neighbor.begin(); neighbor_it != cur_node->neighbor.end(); neighbor_it++) {
            if(my_graph[neighbor_it->first / map_col][neighbor_it->first % map_col]->distance_from_start > (cur_node->distance_from_start + tile_points[my_map[cur_node->x][cur_node->y]])) {
                my_graph[neighbor_it->first / map_col][neighbor_it->first % map_col]->distance_from_start = (cur_node->distance_from_start + tile_points[my_map[cur_node->x][cur_node->y]]);
                my_graph[neighbor_it->first / map_col][neighbor_it->first % map_col]->prev = cur_node;
                distance_map.insert(pair<int, int>(my_graph[neighbor_it->first / map_col][neighbor_it->first % map_col]->distance_from_start, neighbor_it->first));
            }
        }

        // find the next unvisited node with minimum distance
        distance_it = distance_map.begin();
        while(graph_route.find(to_string(distance_it->second)) != graph_route.end()) {
            distance_map.erase(distance_it);
            distance_it = distance_map.begin();
        }

        // mark node as vistied and update current node
        graph_route.insert(to_string(distance_it->second));
        cur_xy = distance_it->second;
        distance_map.erase(distance_it);
        cur_node = my_graph[cur_xy / map_col][cur_xy % map_col];
    }
}

// builds the final path from start to finish
void constructPath(vector<vector<graph_Node*>>& my_graph, int finish_x, int finish_y, int map_col, set<int>& path) {
    // start at the finish node position
    graph_Node* cur_node = my_graph[finish_x][finish_y];
    
    // follow prev pointers back to start node (NULL)
    while(cur_node != NULL) {
        path.insert(cur_node->x * map_col + cur_node->y);
        cur_node = cur_node->prev;
    }
}

// cleans up all nodes in the graph
void cleanup(vector<vector<graph_Node*>>& my_graph) {
     const int rows = my_graph.size();

    // go through each node in the graph and delete all node in the row
    for (int i = 0; i < rows; i++) {
        for(int j = 0; j < int(my_graph[i].size()); j++) {
            delete my_graph[i][j];
        }
    }
    my_graph.clear();
}

int main(int argc, char *argv[]) {
    int num_tiles = 0, start_x = 0, start_y = 0, finish_x = 0, finish_y = 0;
    int map_row = 0, map_col = 0;
    map<char, int> tile_points;
    vector<vector<int>> my_map;
    vector<vector<graph_Node*>> my_graph;
    set<int> path;
    set<int>::iterator path_it;

    // read intput and process map
    readTilePoints(num_tiles, tile_points);
    readMap(map_row, map_col, my_map);
    readStartFinish(start_x, start_y, finish_x, finish_y);
    createGraph(my_map, my_graph, map_col);
    findPath(my_graph, my_map, tile_points, start_x, start_y, map_row, map_col, path);
    constructPath(my_graph, finish_x, finish_y, map_col, path);

    cout << my_graph[finish_x][finish_y]->distance_from_start << endl;

    for(path_it = path.begin(); path_it != path.end(); path_it++) {
        cout << (*path_it) / map_col << " " << (*path_it) % map_col << endl;
    }

    cleanup(my_graph);
    my_map.clear();

    return 0;
}
