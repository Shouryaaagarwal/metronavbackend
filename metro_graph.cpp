// #include <iostream>
// #include <unordered_map>
// #include <vector>
// #include <queue>
// #include <string>
// #include <sstream>
// #include <fstream>
// #include <climits>
// #include <cmath>
// #include <algorithm>
// #include <cctype>

// #ifdef _WIN32
// #include <cstdlib>
// #define CLEAR_SCREEN "cls"
// #else
// #define CLEAR_SCREEN "clear"
// #endif

// using namespace std;

// class MetroGraph {
// private:
//     unordered_map<string, unordered_map<string, int> > vertices;

//     // Helper to extract line code from station name (e.g., "Noida Sector 62~BL" -> "BL")
//     string getLineCode(const string& station) const {
//         size_t pos = station.find('~');
//         if (pos != string::npos && pos + 1 < station.length()) {
//             return station.substr(pos + 1);
//         }
//         return "";
//     }

//     // Calculate fare based on distance (in km)
//     int calculateFare(int distance) const {
//         if (distance <= 2) return 10;
//         else if (distance <= 5) return 20;
//         else if (distance <= 12) return 30;
//         else if (distance <= 21) return 40;
//         else if (distance <= 32) return 50;
//         else return 60; // Fixed Rs60 for distances above 32 km
//     }

// public:
//     void addVertex(const string& name) {
//         vertices[name];
//     }

//     void addEdge(const string& from, const string& to, int weight) {
//         if (vertices.find(from) == vertices.end() || vertices.find(to) == vertices.end()) return;
//         vertices[from][to] = weight;
//         vertices[to][from] = weight; // Undirected graph
//     }

//     bool readFromFile(const string& filename) {
//         ifstream file(filename);
//         if (!file.is_open()) {
//             cout << "Error: Unable to open " << filename << endl;
//             return false;
//         }

//         int numStations;
//         file >> numStations;
//         if (numStations <= 0) {
//             cout << "Error: Invalid number of stations" << endl;
//             file.close();
//             return false;
//         }

//         string line;
//         getline(file, line); // Consume newline
//         while (getline(file, line)) {
//             istringstream iss(line);
//             string from, to;
//             int weight;
//             if (!(iss >> from >> to >> weight) || weight <= 0) {
//                 cout << "Error: Invalid line format or weight in " << filename << endl;
//                 file.close();
//                 return false;
//             }
//             addVertex(from);
//             addVertex(to);
//             addEdge(from, to, weight);
//         }
//         file.close();
//         return true;
//     }

//     void displayStations() const {
//         cout << "\nAvailable Delhi Metro Stations:\n";
//         cout << "----------------------------------------\n";
//         vector<string> stations;
//         for (const auto& v : vertices) {
//             stations.push_back(v.first);
//         }
//         sort(stations.begin(), stations.end());
//         for (size_t i = 0; i < stations.size(); ++i) {
//             cout << (i + 1) << ". " << stations[i] << endl;
//         }
//         cout << "----------------------------------------\n";
//     }

//     void displayMap() const {
//         cout << "\nDelhi Metro Network Map:\n";
//         cout << "----------------------------------------\n";
//         for (const auto& v : vertices) {
//             cout << v.first << " =>\n";
//             for (const auto& n : v.second) {
//                 cout << "\t" << n.first << " (" << n.second << " km)\n";
//             }
//         }
//         cout << "----------------------------------------\n";
//     }

//     pair<int, string> dijkstra(const string& src, const string& dest, bool useTime = false) const {
//         if (vertices.find(src) == vertices.end() || vertices.find(dest) == vertices.end()) {
//             return make_pair(-1, "Invalid station name(s). Please check the station list.");
//         }

//         unordered_map<string, int> dist;
//         unordered_map<string, string> parent;
//         priority_queue<pair<int, string>, vector<pair<int, string> >, greater<pair<int, string> > > pq;

//         for (const auto& v : vertices) {
//             dist[v.first] = INT_MAX;
//         }
//         dist[src] = 0;
//         pq.push(make_pair(0, src));

//         while (!pq.empty()) {
//             int d = pq.top().first;
//             string u = pq.top().second;
//             pq.pop();
//             if (d > dist[u]) continue;

//             for (const auto& neighbor : vertices.at(u)) {
//                 string v = neighbor.first;
//                 int weight = neighbor.second;
//                 int cost;
//                 if (useTime) {
//                     // Time in minutes: (distance / speed) * 60 + 2 min stop time
//                     cost = d + static_cast<int>((static_cast<double>(weight) / 35.0) * 60) + 2;
//                 } else {
//                     cost = d + weight;
//                 }
//                 if (cost < dist[v]) {
//                     dist[v] = cost;
//                     parent[v] = u;
//                     pq.push(make_pair(cost, v));
//                 }
//             }
//         }

//         if (dist[dest] == INT_MAX) {
//             return make_pair(-1, "No path exists between " + src + " and " + dest);
//         }

//         // Reconstruct path
//         string path = dest;
//         vector<string> pathList;
//         pathList.push_back(dest);
//         for (string at = dest; at != src; at = parent.at(at)) {
//             pathList.push_back(parent.at(at));
//         }
//         reverse(pathList.begin(), pathList.end());
//         string pathStr;
//         for (size_t i = 0; i < pathList.size(); ++i) {
//             pathStr += pathList[i];
//             if (i < pathList.size() - 1) pathStr += " -> ";
//         }
//         return make_pair(dist[dest], pathStr);
//     }

//     vector<string> getInterchanges(const string& path) const {
//         vector<string> result;
//         vector<string> stations;
//         stringstream ss(path);
//         string station;
//         const string delimiter = " -> ";
//         size_t pos = 0;
//         string pathCopy = path;
//         while ((pos = pathCopy.find(delimiter)) != string::npos) {
//             station = pathCopy.substr(0, pos);
//             if (!station.empty()) stations.push_back(station);
//             pathCopy.erase(0, pos + delimiter.length());
//         }
//         if (!pathCopy.empty()) stations.push_back(pathCopy);

//         if (stations.size() < 2) return result;

//         int interchanges = 0;
//         string pathDisplay = stations[0];
//         for (size_t i = 1; i < stations.size(); ++i) {
//             string curr = stations[i];
//             string prev = stations[i - 1];
//             string currLine = getLineCode(curr);
//             string prevLine = getLineCode(prev);
//             string interchangeNote = "";
//             if (!currLine.empty() && !prevLine.empty() && currLine != prevLine) {
//                 interchangeNote = " (Interchange: " + prevLine + " to " + currLine + ")";
//                 interchanges++;
//             }
//             pathDisplay += " -> " + curr + interchangeNote;
//         }
//         result.push_back(pathDisplay);
//         result.push_back(to_string(interchanges));
//         return result;
//     }

//     vector<pair<string, int> > getPathBreakdown(const string& path, bool useTime = false) const {
//         vector<pair<string, int> > breakdown;
//         vector<string> stations;
//         stringstream ss(path);
//         string station;
//         const string delimiter = " -> ";
//         size_t pos = 0;
//         string pathCopy = path;
//         while ((pos = pathCopy.find(delimiter)) != string::npos) {
//             station = pathCopy.substr(0, pos);
//             if (!station.empty()) stations.push_back(station);
//             pathCopy.erase(0, pos + delimiter.length());
//         }
//         if (!pathCopy.empty()) stations.push_back(pathCopy);

//         for (size_t i = 0; i < stations.size() - 1; ++i) {
//             string from = stations[i];
//             string to = stations[i + 1];
//             if (vertices.find(from) == vertices.end() || vertices.find(to) == vertices.end() || 
//                 vertices.at(from).find(to) == vertices.at(from).end()) {
//                 continue; // Skip invalid edges
//             }
//             int weight = vertices.at(from).at(to);
//             int cost = useTime ? static_cast<int>((static_cast<double>(weight) / 35.0) * 60) + 2 : weight;
//             breakdown.push_back(make_pair(from + " -> " + to, cost));
//         }
//         return breakdown;
//     }

//     int getFare(const string& path) const {
//         vector<string> stations;
//         const string delimiter = " -> ";
//         size_t pos = 0;
//         string pathCopy = path;
//         string station;
//         while ((pos = pathCopy.find(delimiter)) != string::npos) {
//             station = pathCopy.substr(0, pos);
//             if (!station.empty()) stations.push_back(station);
//             pathCopy.erase(0, pos + delimiter.length());
//         }
//         if (!pathCopy.empty()) stations.push_back(pathCopy);

//         if (stations.size() < 2) return 0; // No valid path

//         int totalDistance = 0;
//         for (size_t i = 0; i < stations.size() - 1; ++i) {
//             string from = stations[i];
//             string to = stations[i + 1];
//             if (vertices.find(from) == vertices.end() || vertices.find(to) == vertices.end() || 
//                 vertices.at(from).find(to) == vertices.at(from).end()) {
//                 continue; // Skip invalid edges
//             }
//             int weight = vertices.at(from).at(to);
//             totalDistance += weight;
//         }
//         return calculateFare(totalDistance);
//     }

//     bool isValidStation(const string& station) const {
//         return vertices.find(station) != vertices.end();
//     }
// };

// void buildMetroMap(MetroGraph& g) {
//     // Blue Line (Line 3/4)
//     g.addVertex("Noida Sector 62~BL");
//     g.addVertex("Botanical Garden~BL");
//     g.addVertex("Yamuna Bank~BL");
//     g.addVertex("Vaishali~BL");
//     g.addVertex("Rajiv Chowk~BL");
//     g.addVertex("Moti Nagar~BL");
//     g.addVertex("Janakpuri West~BL");
//     g.addVertex("Dwarka Sector 21~BL");
//     g.addVertex("Laxmi Nagar~BL");
//     g.addVertex("Anand Vihar~BL");

//     // Yellow Line
//     g.addVertex("Samaypur Badli~YL");
//     g.addVertex("Vishwavidyalaya~YL");
//     g.addVertex("Kashmere Gate~YL");
//     g.addVertex("Chandni Chowk~YL");
//     g.addVertex("New Delhi~YL");
//     g.addVertex("Rajiv Chowk~YL");
//     g.addVertex("AIIMS~YL");
//     g.addVertex("Saket~YL");
//     g.addVertex("Hauz Khas~YL");
//     g.addVertex("Huda City Centre~YL");

//     // Red Line
//     g.addVertex("Shaheed Sthal~RL");
//     g.addVertex("Dilshad Garden~RL");
//     g.addVertex("Kashmere Gate~RL");
//     g.addVertex("Netaji Subhash Place~RL");

//     // Pink Line
//     g.addVertex("Majlis Park~PL");
//     g.addVertex("Azadpur~PL");
//     g.addVertex("Punjabi Bagh West~PL");
//     g.addVertex("Shiv Vihar~PL");

//     // Magenta Line
//     g.addVertex("Botanical Garden~ML");
//     g.addVertex("Janakpuri West~ML");
//     g.addVertex("Hauz Khas~ML");

//     // Orange Line (Airport Express)
//     g.addVertex("New Delhi~OL");
//     g.addVertex("Shivaji Stadium~OL");
//     g.addVertex("Dhaula Kuan~OL");
//     g.addVertex("IGI Airport~OL");

//     // Blue Line Edges
//     g.addEdge("Noida Sector 62~BL", "Botanical Garden~BL", 8);
//     g.addEdge("Botanical Garden~BL", "Yamuna Bank~BL", 10);
//     g.addEdge("Yamuna Bank~BL", "Laxmi Nagar~BL", 3);
//     g.addEdge("Laxmi Nagar~BL", "Anand Vihar~BL", 5);
//     g.addEdge("Anand Vihar~BL", "Vaishali~BL", 3);
//     g.addEdge("Yamuna Bank~BL", "Rajiv Chowk~BL", 6);
//     g.addEdge("Rajiv Chowk~BL", "Moti Nagar~BL", 9);
//     g.addEdge("Moti Nagar~BL", "Janakpuri West~BL", 7);
//     g.addEdge("Janakpuri West~BL", "Dwarka Sector 21~BL", 6);

//     // Yellow Line Edges
//     g.addEdge("Samaypur Badli~YL", "Vishwavidyalaya~YL", 7);
//     g.addEdge("Vishwavidyalaya~YL", "Kashmere Gate~YL", 4);
//     g.addEdge("Kashmere Gate~YL", "Chandni Chowk~YL", 2);
//     g.addEdge("Chandni Chowk~YL", "New Delhi~YL", 2);
//     g.addEdge("New Delhi~YL", "Rajiv Chowk~YL", 1);
//     g.addEdge("Rajiv Chowk~YL", "AIIMS~YL", 7);
//     g.addEdge("AIIMS~YL", "Hauz Khas~YL", 5);
//     g.addEdge("Hauz Khas~YL", "Saket~YL", 4);
//     g.addEdge("Saket~YL", "Huda City Centre~YL", 15);

//     // Red Line Edges
//     g.addEdge("Shaheed Sthal~RL", "Dilshad Garden~RL", 6);
//     g.addEdge("Dilshad Garden~RL", "Kashmere Gate~RL", 10);
//     g.addEdge("Kashmere Gate~RL", "Netaji Subhash Place~RL", 8);

//     // Pink Line Edges
//     g.addEdge("Majlis Park~PL", "Azadpur~PL", 3);
//     g.addEdge("Azadpur~PL", "Punjabi Bagh West~PL", 5);
//     g.addEdge("Punjabi Bagh West~PL", "Shiv Vihar~PL", 12);

//     // Magenta Line Edges
//     g.addEdge("Botanical Garden~ML", "Hauz Khas~ML", 14);
//     g.addEdge("Hauz Khas~ML", "Janakpuri West~ML", 10);

//     // Orange Line Edges
//     g.addEdge("New Delhi~OL", "Shivaji Stadium~OL", 2);
//     g.addEdge("Shivaji Stadium~OL", "Dhaula Kuan~OL", 7);
//     g.addEdge("Dhaula Kuan~OL", "IGI Airport~OL", 8);

//     // Interchange Edges (zero distance for same station, different lines)
//     g.addEdge("Rajiv Chowk~BL", "Rajiv Chowk~YL", 0);
//     g.addEdge("New Delhi~YL", "New Delhi~OL", 0);
//     g.addEdge("Kashmere Gate~YL", "Kashmere Gate~RL", 0);
//     g.addEdge("Botanical Garden~BL", "Botanical Garden~ML", 0);
//     g.addEdge("Hauz Khas~YL", "Hauz Khas~ML", 0);
//     g.addEdge("Janakpuri West~BL", "Janakpuri West~ML", 0);
// }

// int main() {
//     system(CLEAR_SCREEN);
//     MetroGraph g;
//     if (!g.readFromFile("metro.txt")) {
//         cout << "Failed to load metro map from file. Using default Delhi Metro map.\n";
//         buildMetroMap(g);
//     }

//     cout << "\n=== DELHI METRO TRAVEL PLANNER ===\n";
//     cout << "Welcome to the Delhi Metro Travel Planner. Find the shortest routes, travel times, and fares between stations.\n";

//     while (true) {
//         cout << "\nMenu Options:\n";
//         cout << "1. List All Stations\n";
//         cout << "2. Display Metro Network Map\n";
//         cout << "3. Find Shortest Distance Path\n";
//         cout << "4. Find Shortest Time Path\n";
//         cout << "5. Calculate Fare\n";
//         cout << "6. Exit\n";
//         cout << "\nEnter your choice (1-6): ";
//         int choice;
//         cin >> choice;
//         cin.ignore(); // Clear newline

//         if (choice == 6) {
//             cout << "\nThank you for using the Delhi Metro Travel Planner. Goodbye!\n";
//             break;
//         }

//         switch (choice) {
//         case 1:
//             g.displayStations();
//             break;

//         case 2:
//             g.displayMap();
//             break;

//         case 3: {
//             cout << "\nEnter source station (exact name, e.g., Noida Sector 62~BL): ";
//             string src;
//             getline(cin, src);
//             if (src.empty() || src.find_first_not_of(" \t") == string::npos) {
//                 cout << "\nError: Source station cannot be empty.\n";
//                 break;
//             }
//             cout << "Enter destination station: ";
//             string dest;
//             getline(cin, dest);
//             if (dest.empty() || dest.find_first_not_of(" \t") == string::npos) {
//                 cout << "\nError: Destination station cannot be empty.\n";
//                 break;
//             }

//             if (!g.isValidStation(src) || !g.isValidStation(dest)) {
//                 cout << "\nError: One or both station names are invalid. Please check the station list.\n";
//                 break;
//             }

//             pair<int, string> result = g.dijkstra(src, dest);
//             int dist = result.first;
//             string path = result.second;
//             if (dist == -1) {
//                 cout << "\nError: " << path << endl;
//             } else {
//                 cout << "\nShortest Distance Path from " << src << " to " << dest << ":\n";
//                 cout << "----------------------------------------\n";
//                 vector<string> interchanges = g.getInterchanges(path);
//                 int interchangeCount = stoi(interchanges.back());
//                 string pathDisplay = interchanges[0];
//                 interchanges.pop_back();
//                 cout << "Path: " << pathDisplay << endl;
//                 cout << "----------------------------------------\n";
//                 cout << "Number of Interchanges: " << interchangeCount << endl;
//                 cout << "Total Distance: " << dist << " km\n";
//                 cout << "Estimated Fare: Rs" << g.getFare(path) << endl;
//                 cout << "\nDistance Breakdown (km per segment):\n";
//                 cout << "----------------------------------------\n";
//                 vector<pair<string, int> > breakdown = g.getPathBreakdown(path);
//                 for (const auto& segment : breakdown) {
//                     cout << segment.first << ": " << segment.second << " km\n";
//                 }
//                 cout << "----------------------------------------\n";
//             }
//             break;
//         }

//         case 4: {
//             cout << "\nEnter source station (exact name, e.g., Noida Sector 62~BL): ";
//             string src;
//             getline(cin, src);
//             if (src.empty() || src.find_first_not_of(" \t") == string::npos) {
//                 cout << "\nError: Source station cannot be empty.\n";
//                 break;
//             }
//             cout << "Enter destination station: ";
//             string dest;
//             getline(cin, dest);
//             if (dest.empty() || dest.find_first_not_of(" \t") == string::npos) {
//                 cout << "\nError: Destination station cannot be empty.\n";
//                 break;
//             }

//             if (!g.isValidStation(src) || !g.isValidStation(dest)) {
//                 cout << "\nError: One or both station names are invalid. Please check the station list.\n";
//                 break;
//             }

//             pair<int, string> result = g.dijkstra(src, dest, true);
//             int time = result.first;
//             string path = result.second;
//             if (time == -1) {
//                 cout << "\nError: " << path << endl;
//             } else {
//                 cout << "\nShortest Time Path from " << src << " to " << dest << ":\n";
//                 cout << "----------------------------------------\n";
//                 vector<string> interchanges = g.getInterchanges(path);
//                 int interchangeCount = stoi(interchanges.back());
//                 string pathDisplay = interchanges[0];
//                 interchanges.pop_back();
//                 cout << "Path: " << pathDisplay << endl;
//                 cout << "----------------------------------------\n";
//                 cout << "Number of Interchanges: " << interchangeCount << endl;
//                 int totalTime = time + (interchangeCount * 5); // 5 min per interchange
//                 cout << "Total Time: " << totalTime << " minutes\n";
//                 cout << "Estimated Fare: Rs" << g.getFare(path) << endl;
//                 cout << "\nTime Breakdown (minutes per segment, including 2 min stop time):\n";
//                 cout << "----------------------------------------\n";
//                 vector<pair<string, int> > breakdown = g.getPathBreakdown(path, true);
//                 for (const auto& segment : breakdown) {
//                     cout << segment.first << ": " << segment.second << " minutes\n";
//                 }
//                 cout << "----------------------------------------\n";
//             }
//             break;
//         }

//         case 5: {
//             cout << "\nEnter source station (exact name, e.g., Noida Sector 62~BL): ";
//             string src;
//             getline(cin, src);
//             if (src.empty() || src.find_first_not_of(" \t") == string::npos) {
//                 cout << "\nError: Source station cannot be empty.\n";
//                 break;
//             }
//             cout << "Enter destination station: ";
//             string dest;
//             getline(cin, dest);
//             if (dest.empty() || dest.find_first_not_of(" \t") == string::npos) {
//                 cout << "\nError: Destination station cannot be empty.\n";
//                 break;
//             }

//             if (!g.isValidStation(src) || !g.isValidStation(dest)) {
//                 cout << "\nError: One or both station names are invalid. Please check the station list.\n";
//                 break;
//             }

//             pair<int, string> result = g.dijkstra(src, dest);
//             int dist = result.first;
//             string path = result.second;
//             if (dist == -1) {
//                 cout << "\nError: " << path << endl;
//             } else {
//                 cout << "\nFare Calculation from " << src << " to " << dest << ":\n";
//                 cout << "----------------------------------------\n";
//                 vector<string> interchanges = g.getInterchanges(path);
//                 int interchangeCount = stoi(interchanges.back());
//                 string pathDisplay = interchanges[0];
//                 interchanges.pop_back();
//                 cout << "Path: " << pathDisplay << endl;
//                 cout << "----------------------------------------\n";
//                 cout << "Total Distance: " << dist << " km\n";
//                 cout << "Estimated Fare: Rs" << g.getFare(path) << endl;
//             }
//             break;
//         }

//         default:
//             cout << "\nInvalid choice. Please select an option between 1 and 6.\n";
//         }
//         cout << "\nPress Enter to continue...";
//         cin.get();
//         system(CLEAR_SCREEN);
//     }

//     return 0;
// }


// #include <iostream>
// #include <unordered_map>
// #include <vector>
// #include <queue>
// #include <string>
// #include <sstream>
// #include <fstream>
// #include <climits>
// #include <cmath>
// #include <algorithm>
// #include <cctype>

// #ifdef _WIN32
// #include <cstdlib>
// #define CLEAR_SCREEN "cls"
// #else
// #define CLEAR_SCREEN "clear"
// #endif
// #include "metro_graph.h"

// using namespace std;

// class MetroGraph {
// private:
//     unordered_map<string, unordered_map<string, int> > vertices;

//     // Helper to extract line code from station name (e.g., "Noida Sector 62~BL" -> "BL")
//     string getLineCode(const string& station) const {
//         size_t pos = station.find('~');
//         if (pos != string::npos && pos + 1 < station.length()) {
//             return station.substr(pos + 1);
//         }
//         return "";
//     }

//     // Calculate fare based on distance (in km)
//     int calculateFare(int distance) const {
//         if (distance <= 2) return 10;
//         else if (distance <= 5) return 20;
//         else if (distance <= 12) return 30;
//         else if (distance <= 21) return 40;
//         else if (distance <= 32) return 50;
//         else return 60; // Fixed Rs60 for distances above 32 km
//     }

// public:
//     void addVertex(const string& name) {
//         vertices[name];
//     }

//     void addEdge(const string& from, const string& to, int weight) {
//         if (vertices.find(from) == vertices.end() || vertices.find(to) == vertices.end()) return;
//         vertices[from][to] = weight;
//         vertices[to][from] = weight; // Undirected graph
//     }

//     bool readFromFile(const string& filename) {
//         ifstream file(filename);
//         if (!file.is_open()) {
//             cout << "Error: Unable to open " << filename << endl;
//             return false;
//         }

//         int numStations;
//         file >> numStations;
//         if (numStations <= 0) {
//             cout << "Error: Invalid number of stations" << endl;
//             file.close();
//             return false;
//         }

//         string line;
//         getline(file, line); // Consume newline
//         while (getline(file, line)) {
//             istringstream iss(line);
//             string from, to;
//             int weight;
//             if (!(iss >> from >> to >> weight) || weight <= 0) {
//                 cout << "Error: Invalid line format or weight in " << filename << endl;
//                 file.close();
//                 return false;
//             }
//             addVertex(from);
//             addVertex(to);
//             addEdge(from, to, weight);
//         }
//         file.close();
//         return true;
//     }

//     void displayStations() const {
//         cout << "\nAvailable Delhi Metro Stations:\n";
//         cout << "----------------------------------------\n";
//         vector<string> stations;
//         for (const auto& v : vertices) {
//             stations.push_back(v.first);
//         }
//         sort(stations.begin(), stations.end());
//         for (size_t i = 0; i < stations.size(); ++i) {
//             cout << (i + 1) << ". " << stations[i] << endl;
//         }
//         cout << "----------------------------------------\n";
//     }

//     void displayMap() const {
//         cout << "\nDelhi Metro Network Map:\n";
//         cout << "----------------------------------------\n";
//         for (const auto& v : vertices) {
//             cout << v.first << " =>\n";
//             for (const auto& n : v.second) {
//                 cout << "\t" << n.first << " (" << n.second << " km)\n";
//             }
//         }
//         cout << "----------------------------------------\n";
//     }

//     pair<int, string> dijkstra(const string& src, const string& dest, bool useTime = false) const {
//         if (vertices.find(src) == vertices.end() || vertices.find(dest) == vertices.end()) {
//             return make_pair(-1, "Invalid station name(s). Please check the station list.");
//         }

//         unordered_map<string, int> dist;
//         unordered_map<string, string> parent;
//         priority_queue<pair<int, string>, vector<pair<int, string> >, greater<pair<int, string> > > pq;

//         for (const auto& v : vertices) {
//             dist[v.first] = INT_MAX;
//         }
//         dist[src] = 0;
//         pq.push(make_pair(0, src));

//         while (!pq.empty()) {
//             int d = pq.top().first;
//             string u = pq.top().second;
//             pq.pop();
//             if (d > dist[u]) continue;

//             for (const auto& neighbor : vertices.at(u)) {
//                 string v = neighbor.first;
//                 int weight = neighbor.second;
//                 int cost;
//                 if (useTime) {
//                     // Time in minutes: (distance / speed) * 60 + 2 min stop time
//                     cost = d + static_cast<int>((static_cast<double>(weight) / 35.0) * 60) + 2;
//                 } else {
//                     cost = d + weight;
//                 }
//                 if (cost < dist[v]) {
//                     dist[v] = cost;
//                     parent[v] = u;
//                     pq.push(make_pair(cost, v));
//                 }
//             }
//         }

//         if (dist[dest] == INT_MAX) {
//             return make_pair(-1, "No path exists between " + src + " and " + dest);
//         }

//         // Reconstruct path
//         string path = dest;
//         vector<string> pathList;
//         pathList.push_back(dest);
//         for (string at = dest; at != src; at = parent.at(at)) {
//             pathList.push_back(parent.at(at));
//         }
//         reverse(pathList.begin(), pathList.end());
//         string pathStr;
//         for (size_t i = 0; i < pathList.size(); ++i) {
//             pathStr += pathList[i];
//             if (i < pathList.size() - 1) pathStr += " -> ";
//         }
//         return make_pair(dist[dest], pathStr);
//     }

//     vector<string> getInterchanges(const string& path) const {
//         vector<string> result;
//         vector<string> stations;
//         stringstream ss(path);
//         string station;
//         const string delimiter = " -> ";
//         size_t pos = 0;
//         string pathCopy = path;
//         while ((pos = pathCopy.find(delimiter)) != string::npos) {
//             station = pathCopy.substr(0, pos);
//             if (!station.empty()) stations.push_back(station);
//             pathCopy.erase(0, pos + delimiter.length());
//         }
//         if (!pathCopy.empty()) stations.push_back(pathCopy);

//         if (stations.size() < 2) return result;

//         int interchanges = 0;
//         string pathDisplay = stations[0];
//         for (size_t i = 1; i < stations.size(); ++i) {
//             string curr = stations[i];
//             string prev = stations[i - 1];
//             string currLine = getLineCode(curr);
//             string prevLine = getLineCode(prev);
//             string interchangeNote = "";
//             if (!currLine.empty() && !prevLine.empty() && currLine != prevLine) {
//                 interchangeNote = " (Interchange: " + prevLine + " to " + currLine + ")";
//                 interchanges++;
//             }
//             pathDisplay += " -> " + curr + interchangeNote;
//         }
//         result.push_back(pathDisplay);
//         result.push_back(to_string(interchanges));
//         return result;
//     }

//     vector<pair<string, int> > getPathBreakdown(const string& path, bool useTime = false) const {
//         vector<pair<string, int> > breakdown;
//         vector<string> stations;
//         stringstream ss(path);
//         string station;
//         const string delimiter = " -> ";
//         size_t pos = 0;
//         string pathCopy = path;
//         while ((pos = pathCopy.find(delimiter)) != string::npos) {
//             station = pathCopy.substr(0, pos);
//             if (!station.empty()) stations.push_back(station);
//             pathCopy.erase(0, pos + delimiter.length());
//         }
//         if (!pathCopy.empty()) stations.push_back(pathCopy);

//         for (size_t i = 0; i < stations.size() - 1; ++i) {
//             string from = stations[i];
//             string to = stations[i + 1];
//             if (vertices.find(from) == vertices.end() || vertices.find(to) == vertices.end() || 
//                 vertices.at(from).find(to) == vertices.at(from).end()) {
//                 continue; // Skip invalid edges
//             }
//             int weight = vertices.at(from).at(to);
//             int cost = useTime ? static_cast<int>((static_cast<double>(weight) / 35.0) * 60) + 2 : weight;
//             breakdown.push_back(make_pair(from + " -> " + to, cost));
//         }
//         return breakdown;
//     }

//     int getFare(const string& path) const {
//         vector<string> stations;
//         const string delimiter = " -> ";
//         size_t pos = 0;
//         string pathCopy = path;
//         string station;
//         while ((pos = pathCopy.find(delimiter)) != string::npos) {
//             station = pathCopy.substr(0, pos);
//             if (!station.empty()) stations.push_back(station);
//             pathCopy.erase(0, pos + delimiter.length());
//         }
//         if (!pathCopy.empty()) stations.push_back(pathCopy);

//         if (stations.size() < 2) return 0; // No valid path

//         int totalDistance = 0;
//         for (size_t i = 0; i < stations.size() - 1; ++i) {
//             string from = stations[i];
//             string to = stations[i + 1];
//             if (vertices.find(from) == vertices.end() || vertices.find(to) == vertices.end() || 
//                 vertices.at(from).find(to) == vertices.at(from).end()) {
//                 continue; // Skip invalid edges
//             }
//             int weight = vertices.at(from).at(to);
//             totalDistance += weight;
//         }
//         return calculateFare(totalDistance);
//     }

//     bool isValidStation(const string& station) const {
//         return vertices.find(station) != vertices.end();
//     }
// };

// void buildMetroMap(MetroGraph& g) {
//     // Blue Line (Line 3/4)
//     g.addVertex("Noida Sector 62~BL");
//     g.addVertex("Botanical Garden~BL");
//     g.addVertex("Yamuna Bank~BL");
//     g.addVertex("Vaishali~BL");
//     g.addVertex("Rajiv Chowk~BL");
//     g.addVertex("Moti Nagar~BL");
//     g.addVertex("Janakpuri West~BL");
//     g.addVertex("Dwarka Sector 21~BL");
//     g.addVertex("Laxmi Nagar~BL");
//     g.addVertex("Anand Vihar~BL");

//     // Yellow Line
//     g.addVertex("Samaypur Badli~YL");
//     g.addVertex("Vishwavidyalaya~YL");
//     g.addVertex("Kashmere Gate~YL");
//     g.addVertex("Chandni Chowk~YL");
//     g.addVertex("New Delhi~YL");
//     g.addVertex("Rajiv Chowk~YL");
//     g.addVertex("AIIMS~YL");
//     g.addVertex("Saket~YL");
//     g.addVertex("Hauz Khas~YL");
//     g.addVertex("Huda City Centre~YL");

//     // Red Line
//     g.addVertex("Shaheed Sthal~RL");
//     g.addVertex("Dilshad Garden~RL");
//     g.addVertex("Kashmere Gate~RL");
//     g.addVertex("Netaji Subhash Place~RL");

//     // Pink Line
//     g.addVertex("Majlis Park~PL");
//     g.addVertex("Azadpur~PL");
//     g.addVertex("Punjabi Bagh West~PL");
//     g.addVertex("Shiv Vihar~PL");

//     // Magenta Line
//     g.addVertex("Botanical Garden~ML");
//     g.addVertex("Janakpuri West~ML");
//     g.addVertex("Hauz Khas~ML");

//     // Orange Line (Airport Express)
//     g.addVertex("New Delhi~OL");
//     g.addVertex("Shivaji Stadium~OL");
//     g.addVertex("Dhaula Kuan~OL");
//     g.addVertex("IGI Airport~OL");

//     // Blue Line Edges
//     g.addEdge("Noida Sector 62~BL", "Botanical Garden~BL", 8);
//     g.addEdge("Botanical Garden~BL", "Yamuna Bank~BL", 10);
//     g.addEdge("Yamuna Bank~BL", "Laxmi Nagar~BL", 3);
//     g.addEdge("Laxmi Nagar~BL", "Anand Vihar~BL", 5);
//     g.addEdge("Anand Vihar~BL", "Vaishali~BL", 3);
//     g.addEdge("Yamuna Bank~BL", "Rajiv Chowk~BL", 6);
//     g.addEdge("Rajiv Chowk~BL", "Moti Nagar~BL", 9);
//     g.addEdge("Moti Nagar~BL", "Janakpuri West~BL", 7);
//     g.addEdge("Janakpuri West~BL", "Dwarka Sector 21~BL", 6);

//     // Yellow Line Edges
//     g.addEdge("Samaypur Badli~YL", "Vishwavidyalaya~YL", 7);
//     g.addEdge("Vishwavidyalaya~YL", "Kashmere Gate~YL", 4);
//     g.addEdge("Kashmere Gate~YL", "Chandni Chowk~YL", 2);
//     g.addEdge("Chandni Chowk~YL", "New Delhi~YL", 2);
//     g.addEdge("New Delhi~YL", "Rajiv Chowk~YL", 1);
//     g.addEdge("Rajiv Chowk~YL", "AIIMS~YL", 7);
//     g.addEdge("AIIMS~YL", "Hauz Khas~YL", 5);
//     g.addEdge("Hauz Khas~YL", "Saket~YL", 4);
//     g.addEdge("Saket~YL", "Huda City Centre~YL", 15);

//     // Red Line Edges
//     g.addEdge("Shaheed Sthal~RL", "Dilshad Garden~RL", 6);
//     g.addEdge("Dilshad Garden~RL", "Kashmere Gate~RL", 10);
//     g.addEdge("Kashmere Gate~RL", "Netaji Subhash Place~RL", 8);

//     // Pink Line Edges
//     g.addEdge("Majlis Park~PL", "Azadpur~PL", 3);
//     g.addEdge("Azadpur~PL", "Punjabi Bagh West~PL", 5);
//     g.addEdge("Punjabi Bagh West~PL", "Shiv Vihar~PL", 12);

//     // Magenta Line Edges
//     g.addEdge("Botanical Garden~ML", "Hauz Khas~ML", 14);
//     g.addEdge("Hauz Khas~ML", "Janakpuri West~ML", 10);

//     // Orange Line Edges
//     g.addEdge("New Delhi~OL", "Shivaji Stadium~OL", 2);
//     g.addEdge("Shivaji Stadium~OL", "Dhaula Kuan~OL", 7);
//     g.addEdge("Dhaula Kuan~OL", "IGI Airport~OL", 8);

//     // Interchange Edges (zero distance for same station, different lines)
//     g.addEdge("Rajiv Chowk~BL", "Rajiv Chowk~YL", 0);
//     g.addEdge("New Delhi~YL", "New Delhi~OL", 0);
//     g.addEdge("Kashmere Gate~YL", "Kashmere Gate~RL", 0);
//     g.addEdge("Botanical Garden~BL", "Botanical Garden~ML", 0);
//     g.addEdge("Hauz Khas~YL", "Hauz Khas~ML", 0);
//     g.addEdge("Janakpuri West~BL", "Janakpuri West~ML", 0);
// }
// void MetroGraph::getStationNames(std::vector<std::string>& out) const {
//     for (const auto& v : vertices) {
//         out.push_back(v.first);
//     }
// }

// int main() {
    // system(CLEAR_SCREEN);
    // MetroGraph g;
    // if (!g.readFromFile("metro.txt")) {
    //     cout << "Failed to load metro map from file. Using default Delhi Metro map.\n";
    //     buildMetroMap(g);
    // }

    // cout << "\n=== DELHI METRO TRAVEL PLANNER ===\n";
    // cout << "Welcome to the Delhi Metro Travel Planner. Find the shortest routes, travel times, and fares between stations.\n";

    // while (true) {
    //     cout << "\nMenu Options:\n";
    //     cout << "1. List All Stations\n";
    //     cout << "2. Display Metro Network Map\n";
    //     cout << "3. Find Shortest Distance Path\n";
    //     cout << "4. Find Shortest Time Path\n";
    //     cout << "5. Calculate Fare\n";
    //     cout << "6. Exit\n";
    //     cout << "\nEnter your choice (1-6): ";
    //     int choice;
    //     cin >> choice;
    //     cin.ignore(); // Clear newline

    //     if (choice == 6) {
    //         cout << "\nThank you for using the Delhi Metro Travel Planner. Goodbye!\n";
    //         break;
    //     }

    //     switch (choice) {
    //     case 1:
    //         g.displayStations();
    //         break;

    //     case 2:
    //         g.displayMap();
    //         break;

    //     case 3: {
    //         cout << "\nEnter source station (exact name, e.g., Noida Sector 62~BL): ";
    //         string src;
    //         getline(cin, src);
    //         if (src.empty() || src.find_first_not_of(" \t") == string::npos) {
    //             cout << "\nError: Source station cannot be empty.\n";
    //             break;
    //         }
    //         cout << "Enter destination station: ";
    //         string dest;
    //         getline(cin, dest);
    //         if (dest.empty() || dest.find_first_not_of(" \t") == string::npos) {
    //             cout << "\nError: Destination station cannot be empty.\n";
    //             break;
    //         }

    //         if (!g.isValidStation(src) || !g.isValidStation(dest)) {
    //             cout << "\nError: One or both station names are invalid. Please check the station list.\n";
    //             break;
    //         }

    //         pair<int, string> result = g.dijkstra(src, dest);
    //         int dist = result.first;
    //         string path = result.second;
    //         if (dist == -1) {
    //             cout << "\nError: " << path << endl;
    //         } else {
    //             cout << "\nShortest Distance Path from " << src << " to " << dest << ":\n";
    //             cout << "----------------------------------------\n";
    //             vector<string> interchanges = g.getInterchanges(path);
    //             int interchangeCount = stoi(interchanges.back());
    //             string pathDisplay = interchanges[0];
    //             interchanges.pop_back();
    //             cout << "Path: " << pathDisplay << endl;
    //             cout << "----------------------------------------\n";
    //             cout << "Number of Interchanges: " << interchangeCount << endl;
    //             cout << "Total Distance: " << dist << " km\n";
    //             cout << "Estimated Fare: Rs" << g.getFare(path) << endl;
    //             cout << "\nDistance Breakdown (km per segment):\n";
    //             cout << "----------------------------------------\n";
    //             vector<pair<string, int> > breakdown = g.getPathBreakdown(path);
    //             for (const auto& segment : breakdown) {
    //                 cout << segment.first << ": " << segment.second << " km\n";
    //             }
    //             cout << "----------------------------------------\n";
    //         }
    //         break;
    //     }

    //     case 4: {
    //         cout << "\nEnter source station (exact name, e.g., Noida Sector 62~BL): ";
    //         string src;
    //         getline(cin, src);
    //         if (src.empty() || src.find_first_not_of(" \t") == string::npos) {
    //             cout << "\nError: Source station cannot be empty.\n";
    //             break;
    //         }
    //         cout << "Enter destination station: ";
    //         string dest;
    //         getline(cin, dest);
    //         if (dest.empty() || dest.find_first_not_of(" \t") == string::npos) {
    //             cout << "\nError: Destination station cannot be empty.\n";
    //             break;
    //         }

    //         if (!g.isValidStation(src) || !g.isValidStation(dest)) {
    //             cout << "\nError: One or both station names are invalid. Please check the station list.\n";
    //             break;
    //         }

    //         pair<int, string> result = g.dijkstra(src, dest, true);
    //         int time = result.first;
    //         string path = result.second;
    //         if (time == -1) {
    //             cout << "\nError: " << path << endl;
    //         } else {
    //             cout << "\nShortest Time Path from " << src << " to " << dest << ":\n";
    //             cout << "----------------------------------------\n";
    //             vector<string> interchanges = g.getInterchanges(path);
    //             int interchangeCount = stoi(interchanges.back());
    //             string pathDisplay = interchanges[0];
    //             interchanges.pop_back();
    //             cout << "Path: " << pathDisplay << endl;
    //             cout << "----------------------------------------\n";
    //             cout << "Number of Interchanges: " << interchangeCount << endl;
    //             int totalTime = time + (interchangeCount * 5); // 5 min per interchange
    //             cout << "Total Time: " << totalTime << " minutes\n";
    //             cout << "Estimated Fare: Rs" << g.getFare(path) << endl;
    //             cout << "\nTime Breakdown (minutes per segment, including 2 min stop time):\n";
    //             cout << "----------------------------------------\n";
    //             vector<pair<string, int> > breakdown = g.getPathBreakdown(path, true);
    //             for (const auto& segment : breakdown) {
    //                 cout << segment.first << ": " << segment.second << " minutes\n";
    //             }
    //             cout << "----------------------------------------\n";
    //         }
    //         break;
    //     }

    //     case 5: {
    //         cout << "\nEnter source station (exact name, e.g., Noida Sector 62~BL): ";
    //         string src;
    //         getline(cin, src);
    //         if (src.empty() || src.find_first_not_of(" \t") == string::npos) {
    //             cout << "\nError: Source station cannot be empty.\n";
    //             break;
    //         }
    //         cout << "Enter destination station: ";
    //         string dest;
    //         getline(cin, dest);
    //         if (dest.empty() || dest.find_first_not_of(" \t") == string::npos) {
    //             cout << "\nError: Destination station cannot be empty.\n";
    //             break;
    //         }

    //         if (!g.isValidStation(src) || !g.isValidStation(dest)) {
    //             cout << "\nError: One or both station names are invalid. Please check the station list.\n";
    //             break;
    //         }

    //         pair<int, string> result = g.dijkstra(src, dest);
    //         int dist = result.first;
    //         string path = result.second;
    //         if (dist == -1) {
    //             cout << "\nError: " << path << endl;
    //         } else {
    //             cout << "\nFare Calculation from " << src << " to " << dest << ":\n";
    //             cout << "----------------------------------------\n";
    //             vector<string> interchanges = g.getInterchanges(path);
    //             int interchangeCount = stoi(interchanges.back());
    //             string pathDisplay = interchanges[0];
    //             interchanges.pop_back();
    //             cout << "Path: " << pathDisplay << endl;
    //             cout << "----------------------------------------\n";
    //             cout << "Total Distance: " << dist << " km\n";
    //             cout << "Estimated Fare: Rs" << g.getFare(path) << endl;
    //         }
    //         break;
    //     }

    //     default:
    //         cout << "\nInvalid choice. Please select an option between 1 and 6.\n";
    //     }
    //     cout << "\nPress Enter to continue...";
    //     cin.get();
    //     system(CLEAR_SCREEN);
    // }

//     return 0;
// }   





#include "metro_graph.h"
#include <fstream>
#include <sstream>
#include <climits>
#include <cmath>
#include <algorithm>     
#include <queue> // ✅ Add this line


// Implementation of MetroGraph methods

void MetroGraph::addVertex(const std::string& name) {
    vertices[name];
}

void MetroGraph::addEdge(const std::string& from, const std::string& to, int weight) {
    if (vertices.find(from) == vertices.end() || vertices.find(to) == vertices.end()) return;
    vertices[from][to] = weight;
    vertices[to][from] = weight;
}

bool MetroGraph::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    int numStations;
    file >> numStations;
    std::string line;
    std::getline(file, line); // consume newline

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string from, to;
        int weight;
        if (!(iss >> from >> to >> weight)) continue;
        addVertex(from);
        addVertex(to);
        addEdge(from, to, weight);
    }
    file.close();
    return true;
}

std::string MetroGraph::getLineCode(const std::string& station) const {
    size_t pos = station.find('~');
    return (pos != std::string::npos) ? station.substr(pos + 1) : "";
}

int MetroGraph::calculateFare(int distance) const {
    if (distance <= 2) return 10;
    if (distance <= 5) return 20;
    if (distance <= 12) return 30;
    if (distance <= 21) return 40;
    if (distance <= 32) return 50;
    return 60;
}

std::pair<int, std::string> MetroGraph::dijkstra(const std::string& src, const std::string& dest, bool useTime) const {
    if (vertices.find(src) == vertices.end() || vertices.find(dest) == vertices.end())
        return {-1, "Invalid station name(s)"};

    std::unordered_map<std::string, int> dist;
    std::unordered_map<std::string, std::string> parent;
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<>> pq;

    for (const auto& v : vertices) dist[v.first] = INT_MAX;
    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
    auto top = pq.top();
    int d = top.first;
    std::string u = top.second;
    pq.pop();

    if (d > dist[u]) continue;

    for (const auto& neighbor : vertices.at(u)) {
        int cost = useTime
            ? d + static_cast<int>((static_cast<double>(neighbor.second) / 35.0) * 60) + 2
            : d + neighbor.second;

        if (cost < dist[neighbor.first]) {
            dist[neighbor.first] = cost;
            parent[neighbor.first] = u;
            pq.push({cost, neighbor.first});
        }
    }
}


    if (dist[dest] == INT_MAX) return {-1, "No path"};

    std::string pathStr;
    std::vector<std::string> pathList = {dest};
    for (std::string at = dest; at != src; at = parent.at(at)) pathList.push_back(parent.at(at));
    std::reverse(pathList.begin(), pathList.end());

    for (size_t i = 0; i < pathList.size(); ++i) {
        pathStr += pathList[i];
        if (i < pathList.size() - 1) pathStr += " -> ";
    }

    return {dist[dest], pathStr};
}

std::vector<std::string> MetroGraph::getInterchanges(const std::string& path) const {
    std::vector<std::string> result, stations;
    std::string pathCopy = path, delimiter = " -> ";
    size_t pos;
    while ((pos = pathCopy.find(delimiter)) != std::string::npos) {
        stations.push_back(pathCopy.substr(0, pos));
        pathCopy.erase(0, pos + delimiter.length());
    }
    if (!pathCopy.empty()) stations.push_back(pathCopy);

    int interchanges = 0;
    std::string pathDisplay = stations[0];
    for (size_t i = 1; i < stations.size(); ++i) {
        std::string currLine = getLineCode(stations[i]);
        std::string prevLine = getLineCode(stations[i - 1]);
        if (!currLine.empty() && !prevLine.empty() && currLine != prevLine) {
            pathDisplay += " -> " + stations[i] + " (Interchange: " + prevLine + " to " + currLine + ")";
            interchanges++;
        } else {
            pathDisplay += " -> " + stations[i];
        }
    }

    result.push_back(pathDisplay);
    result.push_back(std::to_string(interchanges));
    return result;
}

std::vector<std::pair<std::string, int>> MetroGraph::getPathBreakdown(const std::string& path, bool useTime) const {
    std::vector<std::pair<std::string, int>> breakdown;
    std::vector<std::string> stations;
    std::string pathCopy = path, delimiter = " -> ";
    size_t pos;

    while ((pos = pathCopy.find(delimiter)) != std::string::npos) {
        stations.push_back(pathCopy.substr(0, pos));
        pathCopy.erase(0, pos + delimiter.length());
    }
    if (!pathCopy.empty()) stations.push_back(pathCopy);

    for (size_t i = 0; i + 1 < stations.size(); ++i) {
        const auto& from = stations[i];
        const auto& to = stations[i + 1];
        if (vertices.count(from) && vertices.at(from).count(to)) {
            int weight = vertices.at(from).at(to);
            int cost = useTime ? static_cast<int>((static_cast<double>(weight) / 35.0) * 60) + 2 : weight;
            breakdown.emplace_back(from + " -> " + to, cost);
        }
    }
    return breakdown;
}

int MetroGraph::getFare(const std::string& path) const {
    std::vector<std::string> stations;
    std::string pathCopy = path, delimiter = " -> ";
    size_t pos;

    while ((pos = pathCopy.find(delimiter)) != std::string::npos) {
        stations.push_back(pathCopy.substr(0, pos));
        pathCopy.erase(0, pos + delimiter.length());
    }
    if (!pathCopy.empty()) stations.push_back(pathCopy);

    int totalDistance = 0;
    for (size_t i = 0; i + 1 < stations.size(); ++i) {
        const auto& from = stations[i];
        const auto& to = stations[i + 1];
        if (vertices.count(from) && vertices.at(from).count(to)) {
            totalDistance += vertices.at(from).at(to);
        }
    }

    return calculateFare(totalDistance);
}

bool MetroGraph::isValidStation(const std::string& station) const {
    return vertices.find(station) != vertices.end();
}

void MetroGraph::getStationNames(std::vector<std::string>& out) const {
    for (const auto& v : vertices) {
        out.push_back(v.first);
    }
}

// 🛠️ Build the hardcoded metro map
void buildMetroMap(MetroGraph& g) {
    // Blue Line (Line 3/4)
    g.addVertex("Noida Sector 62~BL");
    g.addVertex("Botanical Garden~BL");
    g.addVertex("Yamuna Bank~BL");
    g.addVertex("Vaishali~BL");
    g.addVertex("Rajiv Chowk~BL");
    g.addVertex("Moti Nagar~BL");
    g.addVertex("Janakpuri West~BL");
    g.addVertex("Dwarka Sector 21~BL");
    g.addVertex("Laxmi Nagar~BL");
    g.addVertex("Anand Vihar~BL");

    // Yellow Line
    g.addVertex("Samaypur Badli~YL");
    g.addVertex("Vishwavidyalaya~YL");
    g.addVertex("Kashmere Gate~YL");
    g.addVertex("Chandni Chowk~YL");
    g.addVertex("New Delhi~YL");
    g.addVertex("Rajiv Chowk~YL");
    g.addVertex("AIIMS~YL");
    g.addVertex("Saket~YL");
    g.addVertex("Hauz Khas~YL");
    g.addVertex("Huda City Centre~YL");

    // Red Line
    g.addVertex("Shaheed Sthal~RL");
    g.addVertex("Dilshad Garden~RL");
    g.addVertex("Kashmere Gate~RL");
    g.addVertex("Netaji Subhash Place~RL");

    // Pink Line
    g.addVertex("Majlis Park~PL");
    g.addVertex("Azadpur~PL");
    g.addVertex("Punjabi Bagh West~PL");
    g.addVertex("Shiv Vihar~PL");

    // Magenta Line
    g.addVertex("Botanical Garden~ML");
    g.addVertex("Janakpuri West~ML");
    g.addVertex("Hauz Khas~ML");

    // Orange Line (Airport Express)
    g.addVertex("New Delhi~OL");
    g.addVertex("Shivaji Stadium~OL");
    g.addVertex("Dhaula Kuan~OL");
    g.addVertex("IGI Airport~OL");

    // Blue Line Edges
    g.addEdge("Noida Sector 62~BL", "Botanical Garden~BL", 8);
    g.addEdge("Botanical Garden~BL", "Yamuna Bank~BL", 10);
    g.addEdge("Yamuna Bank~BL", "Laxmi Nagar~BL", 3);
    g.addEdge("Laxmi Nagar~BL", "Anand Vihar~BL", 5);
    g.addEdge("Anand Vihar~BL", "Vaishali~BL", 3);
    g.addEdge("Yamuna Bank~BL", "Rajiv Chowk~BL", 6);
    g.addEdge("Rajiv Chowk~BL", "Moti Nagar~BL", 9);
    g.addEdge("Moti Nagar~BL", "Janakpuri West~BL", 7);
    g.addEdge("Janakpuri West~BL", "Dwarka Sector 21~BL", 6);

    // Yellow Line Edges
    g.addEdge("Samaypur Badli~YL", "Vishwavidyalaya~YL", 7);
    g.addEdge("Vishwavidyalaya~YL", "Kashmere Gate~YL", 4);
    g.addEdge("Kashmere Gate~YL", "Chandni Chowk~YL", 2);
    g.addEdge("Chandni Chowk~YL", "New Delhi~YL", 2);
    g.addEdge("New Delhi~YL", "Rajiv Chowk~YL", 1);
    g.addEdge("Rajiv Chowk~YL", "AIIMS~YL", 7);
    g.addEdge("AIIMS~YL", "Hauz Khas~YL", 5);
    g.addEdge("Hauz Khas~YL", "Saket~YL", 4);
    g.addEdge("Saket~YL", "Huda City Centre~YL", 15);

    // Red Line Edges
    g.addEdge("Shaheed Sthal~RL", "Dilshad Garden~RL", 6);
    g.addEdge("Dilshad Garden~RL", "Kashmere Gate~RL", 10);
    g.addEdge("Kashmere Gate~RL", "Netaji Subhash Place~RL", 8);

    // Pink Line Edges
    g.addEdge("Majlis Park~PL", "Azadpur~PL", 3);
    g.addEdge("Azadpur~PL", "Punjabi Bagh West~PL", 5);
    g.addEdge("Punjabi Bagh West~PL", "Shiv Vihar~PL", 12);

    // Magenta Line Edges
    g.addEdge("Botanical Garden~ML", "Hauz Khas~ML", 14);
    g.addEdge("Hauz Khas~ML", "Janakpuri West~ML", 10);

    // Orange Line Edges
    g.addEdge("New Delhi~OL", "Shivaji Stadium~OL", 2);
    g.addEdge("Shivaji Stadium~OL", "Dhaula Kuan~OL", 7);
    g.addEdge("Dhaula Kuan~OL", "IGI Airport~OL", 8);

    // Interchange Edges (zero distance for same station, different lines)
    g.addEdge("Rajiv Chowk~BL", "Rajiv Chowk~YL", 0);
    g.addEdge("New Delhi~YL", "New Delhi~OL", 0);
    g.addEdge("Kashmere Gate~YL", "Kashmere Gate~RL", 0);
    g.addEdge("Botanical Garden~BL", "Botanical Garden~ML", 0);
    g.addEdge("Hauz Khas~YL", "Hauz Khas~ML", 0);
    g.addEdge("Janakpuri West~BL", "Janakpuri West~ML", 0);
}

