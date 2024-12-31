# 🦀 Rust Multi-threaded HTTP Server

This repository contains a Rust implementation of a multi-threaded HTTP server. The server leverages a custom thread pool to handle multiple connections concurrently, ensuring efficient resource usage.

## Features

- **Custom Thread Pool**: Implements a thread pool to manage worker threads and efficiently handle multiple requests.
- **Concurrency**: Utilizes Rust's standard library constructs like channels (`mpsc`) and mutexes for thread-safe communication.
- **TCP Listener**: Listens for incoming connections on a specified address and port.

## Files Overview

### `main.rs`

The entry point of the application:
- Binds to the address `127.0.0.1:9001`.
- Accepts incoming TCP connections.
- Delegates request handling to the thread pool.
- Includes the `handle_connection` function to process client requests.

### `lib.rs`

Defines the `ThreadPool` struct and its associated functionality:
- Manages a fixed number of worker threads.
- Uses a message-passing system for job delegation.
- Ensures proper cleanup of worker threads upon shutdown.

## How to Run

### Prerequisites
- Rust programming language installed. You can install it using [rustup](https://rustup.rs/).

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/ahmfaiz/rust-server.git
   cd rust-server
   ```
2. Build the project:
   ```bash
   cargo build
   ```
3. Run the server:
   ```bash
   cargo run
   ```

The server will start listening on `127.0.0.1:9001`. You can connect to it using a web browser or tools like `curl`.

## Example Usage
To test the server:
- Open a terminal and run the server.
- Use a browser or `curl` to make a request:
  ```bash
  curl http://127.0.0.1:9001
  ```

## Customization
You can modify the following:
- **Thread Pool Size**: Adjust the number of threads in the pool by changing the parameter in `ThreadPool::new` in `main.rs`.
- **Listening Address**: Change the address and port in `TcpListener::bind`.

## Acknowledgements
- Inspired by the Rust book's section on multi-threaded servers.
- Special thanks to the Rust community for their excellent documentation and resources.
