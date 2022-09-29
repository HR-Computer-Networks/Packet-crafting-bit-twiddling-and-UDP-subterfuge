PART 1: port scan

To run part one, use the following command:

make port_scanner && ./port_scanner 130.208.242.120 4000 4100 

The first part of the command is used to compile the code, while 
the second part runs it with the IP address and the range of the 
ports we want to scan.

----------------------------------------------------------------
WARNING: This next part only works on MacOS
PART 2: Puzzle port
    
To run part two, use the following command:

make puzzlesolver && ./puzzlesolver 130.208.242.120 port1 port2 
port3 port4.

This time, you need to replace port from 1 to 4 with the ports 
that have been scanned by the port_scanner.

Since the script is using UDP protocol, running it once and
having all the ports give a response in one try usually will
not work. This can be solved by implementing a loop that keeps 
contacting a port when it gets a timeout, and then switches 
to the next port after the answers are received.
We did not implement such a system, is it therefore necessary to
run it multiple time if a timeout appears.

Also, we did not implement anything to recognize the type of 
port we are looking at, so everytime we are assuming that the 
types are in a premade order, which is:
1) Boss port 
2) Evil bit port
3) Checksum port 
4) Oracle port

Thus, run as:
make puzzlesolver && ./puzzlesolver 130.208.242.120 boss_port evil_port checksum_port oracle_port 

----------------------------------------------------------------
PART 3: Knocking

make puzzlesolver && ./puzzlesolver 130.208.242.120 oracleport hidden1 hidden2 secret_message

By running this, the code sends UDP request to the oracle port, 
with a message containing "hidden1, hidden2". In return we receive 
the order in which to knock the ports. 

We send UDP packets containing the secret phrase in the order 
given before, and at the end we receive a message 
"you may enter"

Point 3 is implemented and works as expected. Passing the 
correct ports from the terminal gives the expected results.
Might be necessary to run it more than once in case of UDP
losses or network errors.