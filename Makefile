CXX = g++ -lstdc++
DEBUG = yes
CXXFLAGSD = -g -O1 -std=c++0x
CXXFLAGSR = -O2 -Wall -W -std=c++0x


INCPATH = -lsfml-network -lsfml-system -lboost_system -lboost_filesystem -lcurl -lboost_iostreams -lboost_locale


main : main.o outilParsage.o tout.o jdm.o outilNet.o json.o BakuSemantic.o wikipedia.o bakuSemanticLearn.o
	$(CXX) $(CXXFLAGSD) main.o outilParsage.o tout.o jdm.o outilNet.o json.o BakuSemantic.o wikipedia.o bakuSemanticLearn.o $(INCPATH) -o Bakounibot

all : main

main.o : main.cpp
	$(CXX) $(CXXFLAGSD) -c main.cpp -o main.o

Debug : metamain main

outilParsage.o : outilParsage.cpp
	$(CXX) $(CXXFLAGSD) -c outilParsage.cpp -o outilParsage.o

tout.o : tout.cpp
	$(CXX) $(CXXFLAGSD) -c tout.cpp -o tout.o

jdm.o : jdm.cpp
	$(CXX) $(CXXFLAGSD) -c jdm.cpp -o jdm.o

outilNet.o : outilNet.cpp
	$(CXX) $(CXXFLAGSD) -c outilNet.cpp -o outilNet.o

json.o : json.cpp
	$(CXX) $(CXXFLAGSD)  -c json.cpp -o json.o

BakuSemantic.o : BakuSemantic.cpp
	$(CXX) $(CXXFLAGSD)  -c BakuSemantic.cpp -o BakuSemantic.o

wikipedia.o : wikipedia.cpp
	$(CXX) $(CXXFLAGSD)  -c wikipedia.cpp -o wikipedia.o

bakuSemanticLearn.o : bakuSemanticLearn.cpp
	$(CXX) $(CXXFLAGSD)  -c bakuSemanticLearn.cpp -o bakuSemanticLearn.o

clean :
	rm -rf *.o

tar :
	tar -cvf bakunibot.tar\
	 	Makefile\
		*.cpp\
		*.h\
		*.html\
		*.txt



