#include "gui/gui.h"
#include "json/update.h"
#include "utils.h"

std::string readFileAsString(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        std::cerr << "Could not open the file - '" << filePath << "'" << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

int main(int argc, char **argv)
{
  if (!GUI::initSDL())
  {
    std::cerr << "Failed to initialize SDL!" << std::endl;
    return -1;
  }

  if (argc < 2) {
    std::cerr << "include file which contains updates" << std::endl;
    return -1;
  }

  // update vector to parse
  std::string str = readFileAsString(argv[1]);
  // std::cout << str <<  std::endl;
  // update vector to parse
  
  std::vector<miningbots::json::Update> updates = ReadJson<std::vector<miningbots::json::Update>>(str);
  
  std::cout << updates.size() << std::endl;

  GUI::parsing_the_updates(updates);

  // Closing the game
  GUI::closeSDL();

  return 0;
}
