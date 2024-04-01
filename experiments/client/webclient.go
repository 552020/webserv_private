package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"os"
)

func main() {
	// Use host.docker.internal to connect to the host from the Docker container
	server := "host.docker.internal:8080"
	// Connect to the server
	conn, err := net.Dial("tcp", server)
	if err != nil {
		fmt.Println("Error connecting:", err)
		os.Exit(1)
	}
	defer conn.Close()

	// Send a simple HTTP GET request
	fmt.Fprint(conn, "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n")

	// Read and print the response
	// status, err := bufio.NewReader(conn).ReadString('\n')
	// if err != nil {
	// 	fmt.Println("Error reading response:", err)
	// 	os.Exit(1)
	// }
	// fmt.Print("Response from server: ", status)

	// Call the function to read and print the server response
	readServerResponse(conn)
}

func readServerResponse(conn net.Conn) {
	reader := bufio.NewReader(conn)

	fmt.Println("Server response:")
	for {
		line, err := reader.ReadString('\n')
		fmt.Print(line)
		if err != nil {
			if err != io.EOF {
				fmt.Println("Error reading response:", err)
			}
			break
		}
	}
}