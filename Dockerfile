FROM gcc:latest

# Install dependencies
RUN apt-get update && apt-get install -y cmake libboost-all-dev

# Copy project files
WORKDIR /app
COPY . .

# Build the app
RUN cmake . && make

# Expose the port Crow listens on
EXPOSE 8080

# Run the compiled binary
CMD ["./metro_api"]
