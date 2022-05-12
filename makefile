all: svr.out cli.out

svr.out: ctserver.o
	gcc -o svr.out ctserver.o
cli.out: client.o
	gcc -o cli.out client.o
ctserver.o:
	gcc -c ctserver.c
client.o: 
	gcc -c client.c
	
clean:
	rm svr.out cli.out ctserver.o client.o