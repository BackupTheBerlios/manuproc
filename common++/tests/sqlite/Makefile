
CXXFLAGS=-I../.. -I/usr/include/postgresql/
LDFLAGS=-L../..

sqlitetest: sqlitetest.o -lsqlite

query_test: query_test.o 
	$(CXX) $(CXXFLAGS) -o query_test query_test.cc $(LDFLAGS) \
	-lcommon++ -lecpg -lpq -lsqlite


