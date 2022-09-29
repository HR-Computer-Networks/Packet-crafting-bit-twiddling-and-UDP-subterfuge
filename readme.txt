PART 1: port scan

To run part one, use the following command:

make port_scanner && ./port_scanner 130.208.242.120 4000 4100 

The first part of the command is used to compile the code, while 
the second part runs it with the IP address and the range of the 
ports we want to scan.
----------------------------------------------------------------
WARNING: This next part seems to work only on MacOS

PART 2: Puzzle port

To run part two, use the following command:

make puzzlesolver && ./puzzlesolver 130.208.242.120 port1 port2 
port3 port4.

This time, you need to replace port from 1 to 4 with the ports 
that have been scanned by the port_scanner.

Since the script is using UDP protocol, running it once and
having all the ports give a response in one try usually will
not work. This can be solved by implementing a loop that keeps 
contacting a port when it gets a timeout, and than switches 
to the next port after the answers are received.
We did not implement such system, is it therefore necessary to
run it multiple time if a timeout appears.

Also, we did not implement anything to recognize the type of 
port we are looking at, so everytime we are assuming that the 
types are in a premade order, which is:
1)
2)
3)
4)
----------------------------------------------------------------
