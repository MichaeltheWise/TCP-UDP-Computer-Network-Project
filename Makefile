CC = g++
CFLAGS = -lnsl -lresolv
TARGETS = client_compile aws_compile serverA_compile serverB_compile serverC_compile

all: $(TARGETS)

client_compile: client.cpp 
	$(CC) -o client client.cpp $(CFLAGS)

aws_compile: aws.cpp
	$(CC) -o aws.out aws.cpp $(CFLAGS)

serverA_compile: serverA.cpp
	$(CC) -o serverA.out serverA.cpp $(CFLAGS)

serverB_compile: serverB.cpp
	$(CC) -o serverB.out serverB.cpp $(CFLAGS)

serverC_compile: serverC.cpp
	$(CC) -o serverC.out serverC.cpp $(CFLAGS)

aws:
	./aws.out

serverA:
	./serverA.out

serverB:
	./serverB.out

serverC:
	./serverC.out

clean:
	$(RM) client.out aws.out serverA.out serverB.out serverC.out

.PHONY: all