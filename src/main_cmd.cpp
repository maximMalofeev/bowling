#include <iostream>
#include "game_backend.h"

int main(int argc, char** argv){
  if(argc < 2){
    std::cerr << "Unable to use without input file\n";
    std::cerr << "Usage: <bowling.exe> <input_file>" << std::endl;
    return 1;
  }

  bowling::GameBackend gb;
  if(!gb.start(argv[1])){
    std::cerr << "Unable to start game, bad input file" << std::endl;
    return 1;
  }

  while(gb.makeStep()){
    std::cout << "Frame #" << gb.frame() << ", Player #" << gb.player()
              << ", Ball #" << gb.ball() << ", Pins: " << gb.pins() << std::endl;
  }

  if(gb.state() == bowling::GameBackend::State::RulesBroken){
    std::cerr << "Rule were broken!" << std::endl;
    return 1;
  }

  std::cout << std::endl;
  std::vector<int> results;
  for(int p = 1; p <= gb.players(); p++){
    results.push_back(gb.score(p));
    std::cout << "Player #" << p << " score:" << gb.score(p) << std::endl;
  }

  auto maxScore = *(std::max_element(results.begin(), results.end()));
  if(std::count(results.begin(), results.end(), maxScore) == 1){
    for(int p = 1; p <= gb.players(); p++){
      if(results[p-1] == maxScore){
        std::cout << "Player #" << p << " win!" << std::endl;
      }
    }
  }else{
    std::cout << "There are no winner, this players has the same score:" << std::endl;
    for(int p = 1; p <= gb.players(); p++){
      if(results[p-1] == maxScore){
        std::cout << "Player #" << p << std::endl;
      }
    }
  }

  return 0;
}
