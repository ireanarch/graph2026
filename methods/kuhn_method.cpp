#include <vector>
#include <nlohmann/json.hpp>
#include "kuhn.hpp"

namespace graph {

    namespace {

        bool Dfs(int v, const std::vector<std::vector<int>>& g,
            std::vector<int>& mt, std::vector<bool>& used) {
            if (used[v]) return false;
            used[v] = true;

            for (size_t i = 0; i < g[v].size(); ++i) {
                int to = g[v][i];
                if (mt[to] == -1 || Dfs(mt[to], g, mt, used)) {
                    mt[to] = v;
                    return true;
                }
            }
            return false;
        }

    }  // namespace

    std::vector<int> Kuhn(int n, int k,
        const std::vector<std::vector<int>>& g) {
        std::vector<int> mt(k, -1);
        std::vector<bool> used(n);

        for (int v = 0; v < n; ++v) {
            used.assign(n, false);
            Dfs(v, g, mt, used);
        }

        return mt;
    }

    int KuhnMethod(const nlohmann::json& input, nlohmann::json* output) {
        try {
            int n = input.at("n").get<int>();
            int k = input.at("k").get<int>();
            auto g = input.at("g").get<std::vector<std::vector<int>>>();

            std::vector<int> mt = Kuhn(n, k, g);

            (*output)["matching"] = mt;
            int size = 0;
            for (int i = 0; i < k; ++i) {
                if (mt[i] != -1) ++size;
            }
            (*output)["size"] = size;

            return 0;
        }
        catch (const std::exception& e) {
            (*output)["error"] = e.what();
            return -1;
        }
    }

}  // namespace graph
