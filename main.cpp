#define CROW_USE_BOOST_ASIO
#include "crow_all.h"
#include "metro_graph.h"
MetroGraph g; 

struct CORS {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        if (req.method == "OPTIONS"_method) {
            res.end();
        }
    }

    void after_handle(crow::request&, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "*");
    }
};  

int main() {
crow::App<CORS> app;

    buildMetroMap(g);  // or g.readFromFile("metro.txt");

    CROW_ROUTE(app, "/stations").methods("GET"_method)([]() {
        std::vector<std::string> stationList;
        g.getStationNames(stationList);
        crow::json::wvalue res;
        res["stations"] = stationList;
        return res;
    });

    CROW_ROUTE(app, "/shortest-path").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string src = body["source"].s();
        std::string dest = body["destination"].s();
        bool useTime = body["useTime"].b();

        if (!g.isValidStation(src) || !g.isValidStation(dest)) {
            return crow::response(400, "Invalid stations");
        }

        auto [val, path] = g.dijkstra(src, dest, useTime);
        if (val == -1) return crow::response(400, path);

        auto interchanges = g.getInterchanges(path);
        int interchangeCount = std::stoi(interchanges.back());
        std::string pathDisplay = interchanges[0];

        auto breakdownVec = g.getPathBreakdown(path, useTime);
        std::vector<std::string> breakdown;
        for (const auto& [seg, cost] : breakdownVec) {
            breakdown.push_back(seg + ": " + std::to_string(cost) + (useTime ? " minutes" : " km"));
        }

        crow::json::wvalue res;
        res["pathDisplay"] = pathDisplay;
        res["interchangeCount"] = interchangeCount;
        res["total"] = val + (useTime ? interchangeCount * 5 : 0);
        res["fare"] = g.getFare(path);
        res["breakdown"] = breakdown;

        return crow::response(res);
    });

    app.port(8080).multithreaded().run();
}
