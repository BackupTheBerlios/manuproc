
CXXFLAGS=-I../..
LDFLAGS=-L../.. 

sqlitetest: sqlitetest.o -lsqlite

query_test: query_test.o 
	$(CXX) $(CXXFLAGS) -o query_test query_test.cc $(LDFLAGS) \
	-lcommon++ -lsqlite

FetchIStream: FetchIStream.o
	$(CXX) $(CXXFLAGS) -o FetchIStream FetchIStream.cc \
	-lcommon++ -lsqlite $(LDFLAGS)
