#include <cstdint>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>


struct State;

struct Transition{
  State* prev;
  State* next;
  char c;
};

struct State{
  int id;
  std::string state;
  std::vector<Transition> transitions;
  bool accept;
  //only use for dfa3
  std::pair<int, int> paired_id; 
};

//function dec for simulation
std::string run_dfa(std::map<int16_t, State*> &dfa);
//function dec for read input to create dfa
void read_machine(std::ifstream &fd, std::map<int16_t, State*>& dfa);
//function dec to check equality
std::string check_equality(std::map<int16_t, State*>& dfa3);

std::map<int16_t, State*> create_c(std::map<int16_t, State*>& dfa,std::map<int16_t, State*>& dfa2);

int main(int argc, char** argv){
  std::map<int16_t, State*> dfa;
  std::map<int16_t, State*> dfa2;
  std::map<int16_t, State*> dfa3;

  std::ifstream fd(argv[1]);
  if(fd.is_open()){
    read_machine(fd, dfa);
    read_machine(fd, dfa2);
  }
  else
    std::cout << "Error opening file\n";
  fd.close();

  //create the paired dfa
  dfa3 = create_c(dfa, dfa2);
  //check for equality
  //result is empty if they are same langauge, otherwise result is string that differs
  std::string result = check_equality(dfa3);

  if(result.empty())
    std::cout <<"yes\n";
  else{
    std::cout <<"no\n";
    std::cout << "Difference: " << result << std::endl;
  }

}

void read_machine(std::ifstream &fd, std::map<int16_t, State*>& dfa){

  std::vector<State> states;
/*read in file
  *in format q0fq1q2fq3,q0aq1,q0bq0,q1aq2,q1bq3,q2aq2,q2bq3,q3aq0,q3bq2
  *where q0 is start, q0 and q2 are accept states
  */
  if(fd.is_open()){
    std::string line;
    //read first part, setting up the dfa
    getline(fd, line, ',');
    for(size_t i = 0; i < line.length() - 1; ++i){
      if(line[i] == 'q'){
        State s;
        s.id = line[i+1] - '0';
        if(line[i+2] == 'f')
          s.accept = true;
        else
          s.accept = false;
        states.push_back(s);
      }
    }

    //read rest of file
    while(std::getline(fd, line, ',')){
      //convert state num in string into num
      //Note that states will be in order given by input so we can hardcode
      State& temp = states[line[1] - '0'];
      State& temp2 = states[line[4] - '0'];
      Transition t;
      t.next= &temp2;
      t.c = line[2];
      if(!dfa.count(temp.id)){
        dfa[temp.id] = &temp;
      }
      temp.transitions.push_back(t);
    }
  }
}

std::string check_equality(std::map<int16_t, State*>& dfa,std::map<int16_t, State*>& dfa2){
  std::string result = "";


  //return empty string if dfas are equivalent
  return result; 
}
