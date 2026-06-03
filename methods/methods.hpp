/**
 * @file methods/methods.hpp
 * @author Mikhail Lozhnikov
 *
 * Объявления функций для серверной части алгоритмов. Эти функции должны
 * считать JSON, который прислал клиент, выполнить алгоритм и отправить клиенту
 * JSON с результатом работы алгоритма.
 */

#ifndef METHODS_METHODS_HPP_
#define METHODS_METHODS_HPP_
#include <nlohmann/json.hpp>

namespace graph {

	/* Сюда нужно вставить объявление серверной части алгоритма. */

	// Алгоритм Куна для поиска максимального паросочетания в двудольном графе
	int KuhnMethod(const nlohmann::json& input, nlohmann::json* output);

	/* Конец вставки. */

}  // namespace graph

#endif  // METHODS_METHODS_HPP_
