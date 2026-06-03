/**
 * @file methods/methods.hpp
 * @author Mikhail Lozhnikov
 *
 * Объявления функций для серверной части алгоритмов.
 */

#ifndef METHODS_METHODS_HPP_
#define METHODS_METHODS_HPP_

#include <nlohmann/json.hpp>

namespace graph {

int KuhnMethod(const nlohmann::json& input, nlohmann::json* output);

}  // namespace graph

#endif  // METHODS_METHODS_HPP_
