# Use the official Golang image as a parent image
FROM golang:1.18-alpine

# Set the working directory inside the container
WORKDIR /app

# Copy the local code to the container
COPY . .

# Build the Go app
RUN go build -o client .

# Run the client program on container startup
CMD ["./client"]
