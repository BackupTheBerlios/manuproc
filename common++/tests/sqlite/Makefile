
CXXFLAGS=-I$(HOME)/cvs/c++ -I/usr/include/postgresql/
LDFLAGS=-L$(HOME)/cvs/c++

sqlitetest: sqlitetest.o -lsqlite

query_test: query_test.o 
	$(CXX) $(CXXFLAGS) -o query_test query_test.cc $(LDFLAGS) \
	-lcommon++ -lecpg -lpq -lsqlite


