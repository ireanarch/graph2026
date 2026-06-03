/**
 * @file kuhn_test.cpp
 * @author Your Name
 *
 * Тесты для алгоритма graph::Kuhn.
 */

#include <httplib.h>
#include <unordered_set>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

static void EmptyGraphTest(httplib::Client* cli);
static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);
static void RandomTestHelper(httplib::Client* cli,
    const std::string& graphType);

void TestKuhn(httplib::Client* cli) {
    TestSuite suite("TestKuhn");

    RUN_TEST_REMOTE(suite, cli, EmptyGraphTest);
    RUN_TEST_REMOTE(suite, cli, SimpleTest);
    RUN_TEST_REMOTE(suite, cli, RandomTest);
}

/**
 * @brief Тест для пустого графа.
 *
 * @param cli Указатель на HTTP клиент.
 */
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

/**
 * @brief Простейший статический тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
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

/**
 * @brief Случайный тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void RandomTest(httplib::Client* cli) {
    RandomTestHelper(cli, "Kuhn");
}

/**
 * @brief Вспомогательная функция для случайного теста.
 *
 * @param cli Указатель на HTTP клиент.
 * @param graphType Тип графа (не используется).
 */
static void RandomTestHelper(httplib::Client* cli,
    const std::string& /* graphType */) {
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

        // Генерируем случайный двудольный граф
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

        // Проверяем, что размер не превышает min(n, k)
        REQUIRE(size <= std::min(n, k));

        // Проверяем, что все индексы в допустимом диапазоне
        for (int i = 0; i < k; ++i) {
            if (matching[i] != -1) {
                REQUIRE((matching[i] >= 0 && matching[i] < n));
            }
        }
    }
}
