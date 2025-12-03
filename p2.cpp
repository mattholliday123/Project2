#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>


struct State;

/*Defines a transition
 * next defines the state it's going to
 * paired_next is for our dfa_c where we have paired states
 * c is the character that is used for transition
 */
struct Transition{
  State* next;
  //only use for dfa_c
  std::pair<int16_t, int16_t> paired_next;
  char c;
};

/*Defines a State
 * id is the state number such '0' in q0 
 * transitions is the list of transitions as type Transition
 * accept: whether this state is a final state defined as f in input file
 * paired_id is for dfa_c where we need pairs of states
 */
struct State{
  int id;
  std::vector<Transition> transitions;
  bool accept;
  //only use for dfa3
  std::pair<State*, State*> paired_id; 
};

/*Defines C_State for our dfa_c
 * We have States and a and b for our paired states such as (q,r) where a is q and b is r 
 * transtitions is a list of transitions for our paired state. Should only use paired_next.
 */
struct C_State{
  State* a;
  State* b;
  std::vector<Transition> transitions;
};


//function dec for simulation
std::string run_dfa(std::map<int16_t, State*> &dfa);
//function dec for read input to create dfa
void read_machine(std::ifstream &fd, std::map<int16_t, State*>& dfa, std::string line);
//function dec to check equality
std::string check_equality(std::map<std::pair<int16_t, int16_t>,C_State*>& dfa3);
//function dec to create dfa_c
void create_c(const std::map<int16_t, State*>& dfa, const std::map<int16_t, State*>& dfa2, std::map<std::pair<int16_t, int16_t>,C_State*>& dfa3);

int main(int argc, char** argv){
  std::map<int16_t, State*> dfa;
  std::map<int16_t, State*> dfa2;
  std::map<std::pair<int16_t, int16_t>,C_State*> dfa3;

  std::ifstream fd(argv[1]);
  if(fd.is_open()){
    std::string line1; 
    std::getline(fd, line1);
    std::string line2; 
    std::getline(fd, line2);
    read_machine(fd, dfa, line1);
    read_machine(fd, dfa2, line2);
  }
  else
    std::cout << "Error opening file\n";
  fd.close();

  //create the paired dfa
  create_c(dfa, dfa2, dfa3);
    //if states differ on acceptance within pair (q,r), break and simulate dfa, otherwise they are equivalent
    for (auto& [key, pair] : dfa3) {
      if((pair->a->accept && !pair->b->accept) || !pair->a->accept && pair->a->accept){
        break;
      }else{
        std::cout <<"yes\n";
        exit(1);
    }
  }

  //result is empty if they are same langauge, otherwise result is string that differs
  //check for equality
  std::string result = check_equality(dfa3);

  if(result.empty())
    std::cout <<"yes\n";
  else{
    std::cout <<"no\n";
    std::cout << "Difference: " << result << std::endl;
  }
}

/* Function to read input file to create a dfa
 * @Param fd - Our file descriptor
 * @Param dfa - the dfa we are creating and modifying
 * @Param line - The line we are parsing
 */
void read_machine(std::ifstream &fd, std::map<int16_t, State*>& dfa, std::string line){
  std::vector<State> states;
/*read in file
  *in format q0fq1q2fq3,q0aq1,q0bq0,q1aq2,q1bq3,q2aq2,q2bq3,q3aq0,q3bq2
  *where q0 is start, q0 and q2 are accept states
  */
  if(fd.is_open()){
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

/* function to create our dfa_c
 * @Param dfa1 - our first machine
 * @Param dfa2 - our second machine
 * @Param dfa3 - the dfa_c we are creating and modifying
 */
void create_c(const std::map<int16_t, State*>& dfa1, const std::map<int16_t, State*>& dfa2,std::map<std::pair<int16_t,int16_t>, C_State*>& dfa3){
    // Create all states (q, r) where q is state1 and r is state2
    for (auto& [id1, state1] : dfa1) {
        for (auto& [id2, state2] : dfa2) {
            auto key = std::make_pair(id1, id2);
            dfa3[key] = new C_State;
            dfa3[key]->a = state1;
            dfa3[key]->b = state2;
        }
    }
    // create the transitions
    for (auto& [key, pair] : dfa3) {
        State* s1 = pair->a;
        State* s2 = pair->b;
        for (auto& t1 : s1->transitions) {
            for (auto& t2 : s2->transitions) {
                if (t1.c == t2.c) {
                    auto nextKey = std::make_pair(t1.next->id, t2.next->id);
                    // Create transition in product DFA
                    Transition t;
                    t.c = t1.c;
                    t.paired_next = nextKey;
                    pair->transitions.push_back(t);
                }
            }
        }
    }
}

/*We run this function to run our dfa_c to find a string that exists in one and not the otherwise
 * @Param dfa - this is dfa_c we are simulating
 */
std::string check_equality(std::map<std::pair<int16_t, int16_t>,C_State*>& dfa3){
  //no accept state reached, return empty
  return ""; 
}
