osc-get: osc-get.o
	$(CC) -ljson-c -pedantic -Wall -Wextra osc-get.o -o osc-get
