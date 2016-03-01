mobility: ./XML/tinyxml.o ./XML/tinyxmlerror.o ./XML/tinyxmlparser.o ./XML/tinystr.o mobile.o
	g++ -o mobility ./XML/tinyxml.o ./XML/tinyxmlerror.o ./XML/tinyxmlparser.o ./XML/tinystr.o mobile.o
./XML/tinyxml.o: ./XML/tinyxml.cpp ./XML/tinyxml.h
	g++ -c ./XML/tinyxml.cpp
./XML/tinyxmlerror.o: ./XML/tinyxmlerror.cpp
	g++ -c ./XML/tinyxmlerror.cpp
./XML/tinyxmlparser.o: ./XML/tinyxmlparser.cpp
	g++ -c ./XML/tinyxmlparser.cpp
./XML/tinystr.o: ./XML/tinystr.cpp ./XML/tinystr.h
	g++ -c ./XML/tinystr.cpp
mobility.o: mobile.cpp group.hpp node.cpp utilities.hpp point-area.hpp
	g++ -c mobile.cpp