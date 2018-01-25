README file
Po-Hsuan Lin

// Abstract // 
In this assignment, I created a client, a amazon web server and three backend servers to complete a reduction calculation task. Between client and the amazon web server, as known as aws, I constructed a TCP connection. Through the connection, data and instruction were sent to the server, where the majority of the code is concentrated at. For the second phase of the project, I created three UDP connections between aws and the three backend servers. These servers are in charge of the actual calculation, iterating through the data set and send answers back to aws. At the end of the project, aws combined the data collected from each individual backend servers, computed the reduction result and sent it all the way back to client through the previous TCP connection. 

// My codes //
Followings are the important function calls in each file:

client.cpp:
send length, data and instructions

aws.cpp:
receive length, data and instructions, send to backend servers, receive reduction

serverA.cpp, serverB.cpp, serverC.cpp: 
receive length, data and instruction, return calculation results

client.h, aws.h, serverA.h, serverB.h, serverC.h:
have all the function calls, so better structure and organization when I am coding the whole thing

// Execution instructions //
When running the program, my makefile has the same problems as a few others that a new line is at the end of the makefile. It will pop up a warning. Despite my best effort, I couldn't find a good solution to remove this warning, thus to compile my makefile. I think the problem is because I create my makefile on ubuntu and move it over to Windows. Not sure how to fix this problem, so please do the following:

gmake -f Makefile

// Format of messages exchanged //
My code is constructed from the ground up with functions. For some function calls, I have to return the value to main in order to pass on to other functions in the same server or other servers. Thus, the following format of messages might not be entirely accurate. Sometimes if I have a integer, I might just pass in int* by doing &integer. However the overall format types across the system are the same. For example, instructions are always in char*. 

From client to aws:
data: int*
length of the array: int
instruction: char*

From aws to backend:
data: int*
length of the array: int
instruction: char*

// Idiosyncrasy // 
Not exactly sure when my project will fail. I didn't think I cover everything with my cases but for simple cases, my code works very well. My code is also very scalable due to all the function calls. I can easily add a bunch of new servers, aws and clients to my system.

In the process of coding this though, I face many problems that other students might not have encountered. I have to constantly return my values or dynamically allocate new spaces to pass on the pointers. The process is tedious; I have to code multiple sets of sendto/recvfrom for the servers. It was a challenging task but it is all worth it at the end. 

// Reused codes // 
None. I didn't use anyone's code. Only look at this person's code for reference. https://github.com/latera1n/EE450-Socket-Project/blob/master/client.c. This person's project is very different from mine, so there is hardly anything I can use, so I only used it as a starting point. 

A couple of things also help. The video on Hello_world TCP server programming example really helped me understand what I should do. But because of that, I started coding the project in C at the beginning but got frustrated, so went back to C++ which I am better at and more comfortable with. Hence, in my code, you will see some C and some C++. 
