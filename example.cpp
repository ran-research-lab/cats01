#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "cxxopts.hpp" // from https://github.com/jarro2783/cxxopts
#include <chrono>

using namespace std;


// function linesFromFile:
//   Given the path of the file returns a vector of strings where
// each element in the vector is a line from the file.

vector<string> linesFromFile(string filePath) {
  ifstream inputFile;

  // Try to open the file whose path was received as argument
  inputFile.open(filePath); 
  if (!inputFile.is_open()) {
    cout << "File: " << filePath << " not found! Exiting....\n";
    exit(-1);
  }

  // Read each line of the file and push it back in the vector
  vector<string> result;
  string line;
  while(getline(inputFile, line)) {
    result.push_back(line);
  }
  return result;
} 


// function select:
//   Giiven a reference to a vector of strings, a reference to a vector of topics
//   and an integer idx, return the idx-th line in the vector that has any of the topics

string select(const vector<string> &lines,  const vector<string> &topics, int idx ) {
    int ctr = 0;
    for (const auto line: lines ) {
      bool found = false;
      unsigned int i = 0;

      while (!found && i < topics.size()) {
        if (line.find(topics[i]) != string::npos) { 
          found = true;
          if (ctr == idx) return line;
          ctr++;
        }
        i++;
      }
    }
    return "";
}

// function parse:
//   Uses the cxxopts parser library to parse the command line options.
// Returns the topics specified by the user. 

vector<string> parse(int argc, char* argv[], string &filePath) {
  vector<string> topics;
  try {
    cxxopts::Options options(argv[0], " - example command line options");
    options
      .positional_help("[optional args]")
      .show_positional_help();

    options
      .allow_unrecognised_options()
      .add_options()
      ("help", "Print help")
      ("t", "Topic", cxxopts::value<vector<string>>())
      ("f", "file", cxxopts::value<string>())
    ;


    auto result = options.parse(argc, argv);

    if (result.count("help")){
      cout << options.help({"", "Group"}) << endl;
      exit(0);
    }

    if(result.count("f")) {
      filePath = result["f"].as<string>();
    }
    else {
      cout << "Must specify a file!!" << endl;
      cout << options.help({"", "Group"}) << endl;
      exit(1);
    }


    if (result.count("t"))   {
      cout << "t = ";
      const auto values = result["t"].as<vector<string>>();
      for (const auto& v : values) {
        topics.push_back(v);
        cout << v << "-,-";
      }
      cout << endl;
    }
    else {
      cout << "Must specify a topic!!" << endl;
      cout << options.help({"", "Group"}) << endl;
      exit(1);
    }

    auto arguments = result.arguments();
    cout << "Saw " << arguments.size() << " arguments" << endl;
  }
  catch (const cxxopts::OptionException& e)
  {
    cout << "error parsing options: " << e.what() << endl;
    exit(1);
  }
  return topics;
}


// function main:
//   (1) parses the command line.
//   (2) reads the file that contains the lines to type
//   (3) reads the file onto a vector of strings
//   (4) then performs the typing tests on every line that contains the topic(s)

int main(int argc, char* argv[]) {
  int testNum = 0;
  string testText, typedText, filePath;
  
  vector<string> topics;

  double elapsed_secs;
  clock_t begin, end;
  topics = parse(argc, argv, filePath);

  vector<string> lines = linesFromFile(filePath); 

  while ( ( testText = select(lines, topics, testNum++)) != ""  ) {
    cout << testText << endl;
    auto begin = chrono::steady_clock::now();
    getline(cin, typedText);
    // getline(ctypedText,&size,stdin);
    auto end = chrono::steady_clock::now();
    // elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds> (end - begin).count()/1000.0 << " [s]" << endl;
    cout << "Time difference = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count()/1e9 << " [s]" << endl;
  } 
  cout << select(lines, topics, 4);





  // for (const auto e : lines) {
  //   cout << e << endl;
  // }

  return 0;
}

/*



  

  foo();           //función a la que queremos medir tiempo ejecución


*/