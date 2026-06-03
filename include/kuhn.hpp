/**
 * @file include/kuhn.hpp
 * @author Your Name
 *
 * Алгоритм Куна для поиска максимального паросочетания в двудольном графе.
 */

#ifndef INCLUDE_KUHN_HPP_
#define INCLUDE_KUHN_HPP_

#include <vector>

namespace graph {

    /**
     * @brief Вспомогательная функция для поиска увеличивающей цепи (DFS).
     *
     * @param v Текущая вершина левой доли.
     * @param g Список смежности (граф).
     * @param mt Массив паросочетания.
     * @param used Массив посещённых вершин.
     * @return true если удалось найти увеличивающую цепь, false иначе.
     */
    bool Dfs(int v, const std::vector<std::vector<int>>& g,
        std::vector<int>& mt, std::vector<bool>& used);

    /**
     * @brief Алгоритм Куна для поиска максимального паросочетания.
     *
     * @param n Количество вершин в левой доле.
     * @param k Количество вершин в правой доле.
     * @param g Список смежности (g[i] — список правых вершин, смежных с левой i).
     * @return Вектор размера k, где result[j] = i, если правая вершина j
     *         соединена с левой i, или -1 если свободна.
     */
    std::vector<int> Kuhn(int n, int k,
        const std::vector<std::vector<int>>& g);

}  // namespace graph

#endif  // INCLUDE_KUHN_HPP_
