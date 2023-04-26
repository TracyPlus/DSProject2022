CXXFLAGS=--std=c++11


bvh_parser.o: bvh_parser.cpp bvh_parser.h
	$(CXX) $(CXXFLAGS) -c bvh_parser.cpp

json.o:
	@[ -f ./json.o ]

base: json.o bvh_parser.o
	$(CXX) $(CXXFLAGS) -o base json.o bvh_parser.o

bonus: bvh_parser.o json.cpp bvh_parser.h
	@[ -f ./json.o ] && rm json.o || true
	$(CXX) $(CXXFLAGS) -c json.cpp
	$(CXX) $(CXXFLAGS) -o bonus json.o bvh_parser.o

.PHONY: clean
clean:
	@[ -f ./bvh_parser.o ] && rm bvh_parser.o || true
	@[ -f bonus ] && rm bonus || true
	@[ -f base ] && rm base || true