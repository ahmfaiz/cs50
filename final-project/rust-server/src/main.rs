use std::{
    fs,
    io::{prelude::*, BufReader},
    net::{TcpListener, TcpStream}
};
use rust_server::ThreadPool;

fn main() {
    let socket = TcpListener::bind("127.0.0.1:9001").unwrap();
    let pool = ThreadPool::new(4);

    for stream in socket.incoming() {
        let stream = stream.unwrap();
        println!("Connection recieved");
        
        pool.execute(|| {
            handle_connection(stream);
        });
    }

    println!("Shutting down!");
}

fn handle_connection(mut stream: TcpStream) {
    let buffer = BufReader::new(&stream);
    let request_line = buffer.lines().next().unwrap().unwrap();

    let (status_line, file_name) = if request_line == "GET / HTTP/1.1" {
        ("HTTP/1.1 200 OK", "first.html")
    } else {
        ("HTTP/1.1 404 NOT FOUND", "404.html")
    };

    let content = fs::read_to_string(file_name).unwrap();
    let length = content.len();

    let response = format!("{status_line}\r\nContent-Length: {length}\r\n\r\n{content}");
    stream.write_all(response.as_bytes()).unwrap();
}
