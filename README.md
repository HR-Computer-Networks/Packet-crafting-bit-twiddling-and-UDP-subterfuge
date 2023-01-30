# Speark easy to the port, and perhaps it will let you in


Somewhere on 130.208.242.120 (inside the RU network), a server is listening to some ports in the range 4000-4100.
Find the ports, send them the right packets, and use the secret knock to gain access to the
secret information!

1 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .   
Write a UDP port scanner, that takes in as arguments the IP address of the machine,
and a range of ports to scan between. The scanner should be run with the command:   
./scanner <IP address> <low port> <high port>   
Use it to scan between ports 4000-4100 on 130.208.242.120 and print out the open ports
that you find in this range.   
Do not rely on the ports always being the same. Also, note that UDP is an unreliable
protocol. Some packets may be dropped randomly.
    
2 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .   
The ports you discovered in part 1 are puzzle ports, safeguarding information about
two additional ports which are not showing up on your scan. Your task is to write a
separate program to solve the puzzle ports, in order to reveal the two hidden ports
and the secret phrase. Each port will send you instructions on how to reveal its secret
port if you send it a UDP message.   
The program should be run with the command:   
./puzzlesolver <IP address>   
or   
./puzzlesolver <IP address> <port1> <port2> <port3> <port4>   
The program should interact with the ports discovered in part 1 by sending them a UDP
message following the instructions provided by the puzzle port.
Remember that the ports might change over time. So, do not hard-code the ports in
your program, but allow for the ports to be discovered by your program or be provided
as command line arguments.   
    
3 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .   
After identifying one of the ports as oracle port and finding the two hidden ports, you
can send a comma-separated message containing the hidden ports to the oracle, and the
oracle will reply with a message telling you the order and number of knocks to use on
the hidden ports. For the final part of this assignment, you should extend your program
from part 2 with functionality to knock on the hidden ports in the correct order, and
finally print out the message from the final hidden port.   
You may either fully automate this step after finishing the previous step, or you can
execute the port-knocking functionality with the following command line (last parameter
being NOT a number)   
./puzzlesolver <IP address> <oracleport> <hiddenport1> <hiddenport2> <secretphrase>   
Each knock must contain the secret phrase from part 2 as a message.   
    
4 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .    
After completing the port-knocking, you were sent a secret message,
follow the instructions in the secret message.

