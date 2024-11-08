#include "httpServer.h"
#include "request.h"
#include "response.h"
#include <cstdio>
#include <string>

int main() {

    httpLib::httpServer http(8000);

    // In-memory data storage for demonstration purposes
    std::map<int, std::string> users = {
        {1, "Alice"},
        {2, "Bob"},
        {3, "Charlie"}
    };

    int highestId = 3;

    http.GET("/users", [&users](httpLib::Request request) {
        std::string jsonString = "{\"users\": [";
        for (const auto& [id, name] : users) {
            jsonString += "{\"id\": " + std::to_string(id) + ", \"name\": \"" + name + "\"},";
        }
        if (!users.empty()) jsonString.pop_back(); // Remove trailing comma
        jsonString += "]}";
        
        return httpLib::Response(200, "application/json", jsonString);
    });

    http.GET("/users/:id", [&users](httpLib::Request request) {
        int userId = std::stoi(request.pathParams["id"]);
        if (users.find(userId) != users.end()) {
            std::string jsonString = "{\"id\": " + std::to_string(userId) + ", \"name\": \"" + users[userId] + "\"}";
            return httpLib::Response(200, "application/json", jsonString);
        } else {
            return httpLib::Response(404, "application/json", "{\"error\": \"User not found\"}");
        }
    });

    http.POST("/users", [&users, &highestId](httpLib::Request request) {
        int newId = highestId + 1;
        highestId = newId;
        users[newId] = request.content; // request.content is the new user's name

        std::string jsonString = "{\"message\": \"User created\", \"id\": " + std::to_string(newId) + "}";
        return httpLib::Response(201, "application/json", jsonString);
    });

    http.PUT("/users/:id", [&users](httpLib::Request request) {
        int userId = std::stoi(request.pathParams["id"]);
        if (users.find(userId) != users.end()) {
            users[userId] = request.content; // request.content is the new name
            std::string jsonString = "{\"message\": \"User updated\", \"id\": " + std::to_string(userId) + "}";
            return httpLib::Response(200, "application/json", jsonString);
        } else {
            return httpLib::Response(404, "application/json", "{\"error\": \"User not found\"}");
        }
    });

    http.DELETE("/users/:id", [&users](httpLib::Request request) {
        int userId = std::stoi(request.pathParams["id"]);
        if (users.erase(userId)) {
            return httpLib::Response(200, "application/json", "{\"message\": \"User deleted\"}");
        } else {
            return httpLib::Response(404, "application/json", "{\"error\": \"User not found\"}");
        }
    });

    http.OPTIONS("/users", [](httpLib::Request request) {
        std::string headers = "Allow: GET, POST, PUT, DELETE, OPTIONS";
        return httpLib::Response(204, "text/plain", headers);
    });

    http.run();

    return 0;
}