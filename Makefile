CC_FLAG = -std=c++11 -O2

server: web.o mysql.o
	clang++ web.o mysql.o -lmysqlcppconn-static -lmysqlclient -lpthread -lboost_system -o server $(CC_FLAG)

web.o: web.cpp
	clang++ -c web.cpp $(CC_FLAG)

test.o: test.cpp
	clang++ -c test.cpp $(CC_FLAG)

mysql.o: mysql.cpp mysql.h
	clang++ -c mysql.cpp $(CC_FLAG)


clean:
	rm *.o server

run:
	./server
		