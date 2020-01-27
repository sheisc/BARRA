/**
    Calculate the number of usable ROP gadgets.

 */
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


std::set<std::string> instrGadgetAddrs;
std::set<std::string> retAddrs;
std::set<std::string> origGadgetAddrs;

std::set<std::string> usableGadgetAddrs;


void ReadGadgetAddrs(char * path, std::set<std::string> & gadgets){
  ifstream gInput(path);
  string line;

  while(getline(gInput,line)){
    gadgets.insert(line);
  }

}

int getUsableGadgets(std::set<std::string> & allG, std::set<std::string> &usableG, std::set<std::string> & permitG){
  std::set<std::string>::iterator it;
  for(const std::string &line: allG){
    if(permitG.find(line) != permitG.end()){
      usableG.insert(line);
    }
  }
  return usableG.size();
}

int main(int argc, char * argv[]){
  if(argc < 4){
    fprintf(stderr,"gadgets instr.GadgetAddrs.txt instr.retAddrs.txt orig.GadgetAddrs.txt\n");
    return 1;
  }
  ReadGadgetAddrs(argv[1],instrGadgetAddrs);
  ReadGadgetAddrs(argv[2],retAddrs);
  ReadGadgetAddrs(argv[3],origGadgetAddrs);
  //string appName(argv[3]);

  int cnt = getUsableGadgets(instrGadgetAddrs,usableGadgetAddrs,retAddrs);
  cout << " " << origGadgetAddrs.size() << " "
       << instrGadgetAddrs.size() << " " << retAddrs.size() << " "
       << cnt << " "  << endl ;

  for(const std::string &line: usableGadgetAddrs){
    cout << line << endl;
  }
  return 0;
}

