# UDP Port Scanner & Puzzle Solver

A C++ networking application that implements UDP port scanning with multi-stage puzzle solving and port knocking techniques.

## Project Overview

This project demonstrates advanced network programming skills by implementing a UDP port scanner that:
- Scans ports 4000-4100 on a target server
- Identifies puzzle ports that contain cryptographic challenges
- Solves multi-stage puzzles to reveal hidden ports
- Performs port knocking sequences to access secret information

## Features

- **UDP Port Scanner**: Efficiently scans UDP ports in specified ranges
- **Puzzle Solving**: Interacts with puzzle ports to decode hidden information
- **Port Knocking**: Implements secure port knocking sequences
- **Command-line Interface**: Flexible CLI for different operational modes
- **Error Handling**: Robust handling of UDP's unreliable protocol behavior

## Build Instructions

```bash
# Build the port scanner
g++ -o port_scanner port_scanner.cpp

# Build test programs
g++ -o hello_world hello_world.cpp
```

## Usage

### Port Scanning
```bash
./port_scanner <IP_address> <low_port> <high_port>
```

### Puzzle Solving
```bash
./puzzlesolver <IP_address>
./puzzlesolver <IP_address> <port1> <port2> <port3> <port4>
```

### Port Knocking
```bash
./puzzlesolver <IP_address> <oracle_port> <hidden_port1> <hidden_port2> <secret_phrase>
```

## Technical Details

- **Language**: C++ with C-style networking
- **Protocol**: UDP (SOCK_DGRAM)
- **Libraries**: Standard C++ library, C networking headers
- **Platform**: Linux/Unix systems

## Skills Demonstrated

- C++ Network Programming
- UDP Protocol Implementation
- Socket Programming
- Cryptographic Puzzle Solving
- Port Knocking Techniques
- Multi-stage Problem Solving
- Command-line Interface Design

## Project Structure

- `port_scanner.cpp` - Main UDP port scanning implementation
- `hello_world.cpp` - Test program
- `AGENTS.md` - Build and coding guidelines

## License

This project is for educational and demonstration purposes.