#include "../../include/router.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

const std::string DB_FILE = "db.json";

static std::vector<json> users_db;
static std::mutex db_mutex;

// Persists the current in-memory user list to the JSON file on disk
void save_db() {
    std::ofstream file(DB_FILE);
    if (file.is_open()) {
        json j = users_db;
        file << j.dump(4);
    }
}

// Loads user data from the JSON file into memory at startup
void load_db() {
    std::ifstream file(DB_FILE);
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            users_db = j.get<std::vector<json>>();
            std::cout << "Loaded DB: " << users_db.size() << " entries.\n";
        } catch (std::exception& e) {
            std::cerr << "Error JSON: " << e.what() << "\n";
        }
    }
}
// Helper utility to read a file's content into a string
std::string read_html_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static bool db_loaded = false;

// Main entry point: Dispatches HTTP requests based on method and URI
http::response<http::string_body> Router::handle_request(http::request<http::string_body> req) {
    {
        std::lock_guard<std::mutex> lock(db_mutex);
        if (!db_loaded) {
            load_db();
            db_loaded = true;
        }
    }

    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::server, "C++ Web Server");
    res.keep_alive(req.keep_alive());

    if (req.method() == http::verb::get && (req.target() == "/" || req.target() == "/index.html")) {
        res.set(http::field::content_type, "text/html");

        std::string html_content = read_html_file("index.html");

        if(html_content.empty()) {
            res.result(http::status::not_found);
            res.body() = "Error. Could't find file index.html in working folder";
            res.set(http::field::content_type, "text/plain");
        } else {
            res.body() = html_content;
        }
    }

    else if (req.method() == http::verb::get && req.target() == "/get_users") {
        res.set(http::field::content_type, "application/json");
        std::lock_guard<std::mutex> lock(db_mutex);

        json response_body;
        response_body["users"] = users_db;
        res.body() = response_body.dump(4);
    }

    else if (req.method() == http::verb::post && req.target() == "/add_user") {
        res.set(http::field::content_type, "application/json");
        try {
            auto new_user = json::parse(req.body());

            if(!new_user.contains("name")) {
                res.result(http::status::bad_request);
                res.body() = R"({"error": "Missing 'name'"})";
            } else {
                {
                    std::lock_guard<std::mutex> lock(db_mutex);
                    users_db.push_back(new_user);
                    save_db();
                }
                res.result(http::status::created);
                res.body() = R"({"status": "OK"})";
            }
        }
        catch (std::exception& e) {
            res.result(http::status::bad_request);
            res.body() = "Invalid JSON";
        }
    }

    else {
        res.result(http::status::not_found);
        res.body() = "404 Not Found";
    }

    res.prepare_payload();
    return res;
}