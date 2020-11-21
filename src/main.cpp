#include <iostream>
#include "Session.h"
#include "Graph.h"
using namespace std;


int main(int argc, char** argv){

    Session s("/home/spl211/spl_1st_assignment/json_files/config4.json");

    s.simulate();

    return 0;
}

//valgrind --leak-check=full --show-reachable=yes ./bin/cTrace ./config1.json