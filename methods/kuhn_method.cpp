#include "methods/methods.hpp"

#include <vector>

#include "nlohmann/json.hpp"
#include "kuhn.hpp"

int KuhnMethod(const nlohmann::json& input, nlohmann::json* output) {
    int n = input["n"].get<int>();
    int k = input["k"].get<int>();

    std::vector<std::vector<int>> g(n);
    for (const auto& edge : input["edges"]) {
        g[edge[0].get<int>()].push_back(edge[1].get<int>());
    }

    std::vector<int> mt = graph::Kuhn(n, k, g);
    (*output)["matching"] = mt;

    return 0;
}