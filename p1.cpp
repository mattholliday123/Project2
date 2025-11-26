#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
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
  uint8_t accept;
};

//function dec for simulation
std::string run_dfa(std::map<int16_t, State> &dfa);


int main(int argc, char** argv){
  std::map<int16_t, State> dfa;
  std::vector<State> states;

  /*read in file
  *in format q0fq1q2fq3,q0aq1,q0bq0,q1aq2,q1bq3,q2aq2,q2bq3,q3aq0,q3bq2
  *where q0 is start, q0 and q2 are accept states
  */
  std::ifstream fd (argv[1]);
  if(fd.is_open()){
    std::string line;
    //read first part, setting up the dfa
    getline(fd, line, ',');
    for(int i = 0; i < line.length() - 2; ++i){
      char temp[2];
      if(line[i] == 'q'){
        State s;
        s.id = line[i+1];
        //s.state = line[i] + line[i+1];
        if(line[i+2] == 'f')
          s.accept = 1;
        else
          s.accept = 0;
        states.push_back(s);
      }
    }
    //read rest of file
    //example: q0aq1,q0bq2
    while(std::getline(fd, line, ',')){
      //convert state num in string into num
      //Note that states will be in order given by input so we can hardcode
      State temp = states[line[1] - '0'];
      State temp2 = states[line[4] - '0'];
      Transition t;
      //t.prev = &temp;
      t.next= &temp2;
      t.c = line[2];
      if(!dfa.count(temp.id)){
        dfa[temp.id] = temp;
        temp.transitions.push_back(t);
      }
      else
        dfa[temp.id].transitions.push_back(t);
    }
    fd.close();
  }


  std::string accepted_string = "";
  //run dfa, check empty
  for(auto state : states){
    if(state.accept){
      accepted_string = run_dfa(dfa);
      break;
    }
  }

  //if string is empty, this means we either have no accept states or accept state is unreachable
  if(accepted_string.empty()){
    std::cout <<"yes\n";
    return 0;
  }

  //dfa M is not empty
  std::cout<<"no\ndfa M accepts " << accepted_string;
  return 0;
}

/* function that runs dfa and looks for string that is accepted
 * if there is such string, return it.
 * if no such string, return empty string
 */
std::string run_dfa(std::map<int16_t, State> &dfa){


}





