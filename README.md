## Async C++ HTTP Server with Boost.Beast
A lightweight, multi-threaded HTTP server implemented in C++ using Boost.Asio and Boost.Beast.

This project demonstrates how to build an asynchronous web server that serves static HTML content and handles REST API requests (GET/POST) with JSON persistence. It uses nlohmann/json for data serialization and a file-based database.

## 🚀 Features
- Asynchronous I/O: Uses Boost.Asio for non-blocking operations.

- Multi-threading: Supports concurrent request handling using a thread pool.

- HTTP/1.1 Support: Handles Keep-Alive connections and standard HTTP headers.

REST API:

- GET /get_users: Retrieves the list of users.

- POST /add_user: Adds a new user to the database.

- Data Persistence: Saves user data to a local db.json file.

- Thread Safety: Uses mutexes to safely access the JSON database across multiple threads.

## 🛠️ Dependencies
To build this project, you need the following libraries:

C++ Compiler: Supporting C++14 or higher (C++17 recommended).

Boost Libraries: Specifically Boost.System, Boost.Asio, and Boost.Beast.

nlohmann/json: A modern JSON library for C++.

📂 Project Structure
```text
├── include/
│   ├── listener.h      # TCP Acceptor logic
│   ├── session.h       # HTTP Session management
│   ├── router.h        # Request routing and business logic
├── src/
│   ├── main.cpp        # Server entry point and thread pool setup
│   ├── listener.cpp    # Implementation of connection accepting
│   ├── session.cpp     # Implementation of read/write cycles
│   ├── router.cpp      # Implementation of API endpoints and file I/O
├── index.html          # Frontend interface
├── db.json             # JSON database storage
└── CMakeLists.txt      # Build configuration
```
## 🔨 How to Build
It is recommended to use CMake to build the project. Below is an example of how to compile it.

1. Create a CMakeLists.txt

If you don't have one, create a file named CMakeLists.txt in the root directory with the following content:

```text
cmake_minimum_required(VERSION 3.10)
project(SimpleHttpServer)

set(CMAKE_CXX_STANDARD 17)
```

# Find Boost
```text
find_package(Boost REQUIRED COMPONENTS system)
```
# Find nlohmann_json (assuming it is installed on the system)
```text
find_package(nlohmann_json REQUIRED)

add_executable(server
    main.cpp
    listener.cpp
    session.cpp
    router.cpp
)

target_link_libraries(server PRIVATE Boost::system nlohmann_json::nlohmann_json)
```
2. Compile

```text
mkdir build
cd build
cmake ..
make
```
## ▶️ How to Run
After compiling, the executable must be run from a directory that contains index.html and db.json (or you must copy those files to your build folder).

Ensure index.html and db.json are in the same folder as the executable.

Run the server:

```bash
./server
```
Output:

```text
--- HTTP Server Initialization ---
Address: 0.0.0.0:8080
Operating threads: [Number of Cores]
Loaded DB: X entries.
Access the App: Open your web browser and go to: http://localhost:8080
```
