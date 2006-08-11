
CXXFLAGS=-I../..
LDFLAGS=-L../.. 

sqlitetest: sqlitetest.o
	$(CC) -o $@ $< -lsqlite3

query_test: query_test.o 
	$(CXX) $(CXXFLAGS) -o query_test query_test.cc $(LDFLAGS) \
	-lcommon++ -lsqlite3

FetchIStream: FetchIStream.o
	$(CXX) $(CXXFLAGS) -o FetchIStream FetchIStream.cc \
	-lcommon++ -lsqlite3 $(LDFLAGS)
