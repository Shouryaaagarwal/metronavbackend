FROM gcc:latest

# Avoid tzdata prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install essential packages only
RUN apt-get update && apt-get install -y \
    cmake \
    libboost-system-dev \
    libboost-thread-dev \
    libboost-filesystem-dev \
    libboost-program-options-dev \
    libboost-regex-dev \
    libboost-chrono-dev \
    libboost-date-time-dev \
    libboost-atomic-dev \
    && apt-get clean

# Set working directory
WORKDIR /app

# Copy all project files
COPY . .

# Build the app
RUN cmake . && make

# Expose the API port
EXPOSE 8080

# Run the app
CMD ["./metro_api"]
