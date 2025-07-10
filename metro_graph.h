
#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

class MetroGraph {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, int>> vertices;

    std::string getLineCode(const std::string& station) const;
    int calculateFare(int distance) const;

public:
    void addVertex(const std::string& name);
    void addEdge(const std::string& from, const std::string& to, int weight);
    bool readFromFile(const std::string& filename);
    bool isValidStation(const std::string& station) const;
    void getStationNames(std::vector<std::string>& out) const;

    std::pair<int, std::string> dijkstra(const std::string& src, const std::string& dest, bool useTime = false) const;
    std::vector<std::string> getInterchanges(const std::string& path) const;
    std::vector<std::pair<std::string, int>> getPathBreakdown(const std::string& path, bool useTime = false) const;
    int getFare(const std::string& path) const;
};

void buildMetroMap(MetroGraph& g);
