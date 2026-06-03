/**
 * @file kuhn_test.cpp
 * @author Your Name
 *
 * Тесты для алгоритма graph::Kuhn.
 */

#include <httplib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

static void EmptyGraphTest(httplib::Client* cli);
static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);
static void RandomTestHelper(httplib::Client* cli);

void TestKuhn(httplib::Client* cli) {
    TestSuite suite("TestKuhn");

    RUN_TEST_REMOTE(suite, cli, EmptyGraphTest);
    RUN_TEST_REMOTE(suite, cli, SimpleTest);
    RUN_TEST_REMOTE(suite, cli, RandomTest);
}

static void EmptyGraphTest(httplib::Client* cli) {
    nlohmann::json input = R"(
{
  "n": 0,
  "k": 0,
  "g": []
}
)"_json;

    auto res = cli->Post("/Kuhn", input.dump(), "application/json");

    if (!res) {
        REQUIRE(false);
    }

    nlohmann::json output = nlohmann::json::parse(res->body);

    int size = output.at("size").get<int>();
    std::vector<int> matching = output.at("matching")
        .get<std::vector<int>>();

    REQUIRE_EQUAL(size, 0);
    REQUIRE_EQUAL(matching.size(), 0);
}

static void SimpleTest(httplib::Client* cli) {
    nlohmann::json input;

    input["n"] = 3;
    input["k"] = 3;
    input["g"] = std::vector<std::vector<int>>{
      {0, 1},
      {0, 2},
      {1}
    };

    auto res = cli->Post("/Kuhn", input.dump(), "application/json");

    if (!res) {
        REQUIRE(false);
    }

    nlohmann::json output = nlohmann::json::parse(res->body);

    int size = output.at("size").get<int>();
    std::vector<int> matching = output.at("matching")
        .get<std::vector<int>>();

    REQUIRE_EQUAL(size, 3);
    REQUIRE_EQUAL(matching.size(), 3);
}

static void RandomTest(httplib::Client* cli) {
    RandomTestHelper(cli);
}

static void RandomTestHelper(httplib::Client* cli) {
    const int numTries = 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> nDist(1, 20);
    std::uniform_int_distribution<int> kDist(1, 20);
    std::uniform_int_distribution<int> edgeDist(0, 1);

    for (int it = 0; it < numTries; ++it) {
        int n = nDist(gen);
        int k = kDist(gen);

        nlohmann::json input;
        input["n"] = n;
        input["k"] = k;
        input["g"] = std::vector<std::vector<int>>(n);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < k; ++j) {
                if (edgeDist(gen) == 1) {
                    input["g"][i].push_back(j);
                }
            }
        }

        auto res = cli->Post("/Kuhn", input.dump(), "application/json");

        if (!res) {
            REQUIRE(false);
        }

        nlohmann::json output = nlohmann::json::parse(res->body);
        int size = output.at("size").get<int>();
        std::vector<int> matching = output.at("matching")
            .get<std::vector<int>>();

        REQUIRE(size <= std::min(n, k));

        for (int i = 0; i < k; ++i) {
            if (matching[i] != -1) {
                REQUIRE((matching[i] >= 0 && matching[i] < n));
            }
        }
    }
}
