/**
 * @file methods/main.cpp
 * @author Mikhail Lozhnikov
 */

#include <httplib.h>
#include <iostream>
#include <cstdio>
#include <nlohmann/json.hpp>
#include "methods.hpp"

int main(int argc, char* argv[]) {
    int port = 8080;

    if (argc >= 2) {
        if (std::sscanf(argv[1], "%d", &port) != 1)
            return -1;
    }

    std::cerr << "Listening on port " << port << "..." << std::endl;

    httplib::Server svr;

    svr.Get("/stop", [&](const httplib::Request&, httplib::Response&) {
        svr.stop();
        });

    svr.Post("/Kuhn", [&](const httplib::Request& req, httplib::Response& res) {
        nlohmann::json input = nlohmann::json::parse(req.body);
        nlohmann::json output;
        int code = graph::KuhnMethod(input, &output);
        res.status = code == 0 ? 200 : 400;
        res.set_content(output.dump(), "application/json");
        });

    svr.listen("0.0.0.0", port);

    return 0;
}