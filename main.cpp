//
//  Project 1 - DNA Profiling
//  CS251 - Zach Savignano
//
//

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct database {
  vector<vector<char>> STRs;
  vector<string> names;
  vector<int> STRcounts;
};

vector<char> stringParse(string &str) {
  vector<char> vec;
  for (size_t i = 0; i < str.size(); i++) {
    vec.push_back(str.at(i));
  }
  return vec;
}

/*
string parseComma(string str) {
  stringstream ss(str);
  string temp;
  while (getline(ss, temp, ',')) {
    .push_back(temp);
  }
}*/

// load_db() - loads text file and stores
//
bool load_db(database &db) {
  string filename;
  string line;
  string temp;
  ifstream inFS;
  int count = 0;
  db.names.clear();
  db.STRs.clear();
  db.STRcounts.clear();

  cin >> filename;
  inFS.open(filename);
  if (!inFS.is_open()) {
    cout << "Error: unable to open \'" << filename << "\'" << endl;
    return false;
  } else {
    cout << "Loading database..." << endl;
    getline(inFS, line);
    stringstream ss(line);

    // get STRs from file (1st line - "name")
    while (getline(ss, temp, ',')) {
      if (count != 0) {
        db.STRs.push_back(stringParse(temp));
      }
      count++;
    }
    while (getline(inFS, line)) {
      stringstream ss(line);
      count = 0;
      while (getline(ss, temp, ',')) {
        if (count == 0) {
          db.names.push_back(temp);
        } else {
          db.STRcounts.push_back(stoi(temp));
        }
        count++;
      }
    }
    inFS.close();
  }
  return true;
}

// print char vec
//
void printCharVec(vector<char> &vec) {
  for (int i = 0; i < vec.size(); i++) {
    cout << vec.at(i);
  }
}
// display() - displays current info
//
void display(database &db, vector<char> &dna, vector<int> &fileSTRcounts,
             bool dbLoad, bool dnaLoad, bool processed) {
  cout << "Database loaded:" << endl;
  for (int i = 0; i < db.names.size(); i++) {
    cout << db.names.at(i) << " ";
    for (int j = 0; j < db.STRs.size(); j++) {
      cout << db.STRcounts.at(i * db.STRs.size() + j);
      if (j < db.STRcounts.size() - 1)
        cout << " ";
    }
    cout << endl;
  }
  cout << endl;
  if (!dnaLoad) {
    cout << "No DNA loaded." << endl << endl;
  } else {
    cout << "DNA loaded:" << endl;
    for (int i = 0; i < dna.size(); i++) {
      cout << dna.at(i);
    }
    cout << endl << endl;
  }
  if (!processed) {
    cout << "No DNA has been processed." << endl << endl;
  } else {
    cout << "DNA processed, STR counts:" << endl;
    for (int i = 0; i < db.STRs.size(); i++) {
      printCharVec(db.STRs[i]);
      cout << ": " << fileSTRcounts.at(i) << endl;
    }
  }
}

// load_dna() - loads dna
//
bool load_dna(vector<char> &dna) {
  ifstream inFS;
  string filename;
  string temp;
  dna.clear();

  cin >> filename;
  inFS.open(filename);
  if (!inFS.is_open()) {
    cout << "Error: unable to open \'" << filename << "\'" << endl;
    return false;
  } else {
    cout << "Loading DNA..." << endl;
    getline(inFS, temp);
    dna = stringParse(temp);
  }
  inFS.close();
  return true;
}

// process() - process data
//
void process(vector<char> &dna, vector<int> &counts, database &db) {
  int size = db.STRs.size();
  int length = 0;
  int count = 0;
  bool compSTR = true;
  counts.clear();
  cout << "Processing DNA..." << endl;
  // loop for each STR from database
  for (int i = 0; i < size; i++) {
    length = db.STRs[i].size();
    // loop for length of dna string
    for (int j = 0; j < dna.size() - (length - 1); j++) {
      int k = 0;
      // loop for length of database STR
      while (k < length && compSTR) {
        if (dna.at(j + k) == db.STRs[i].at(k)) {
          compSTR = true;
        } else {
          compSTR = false;
        }
        k++;
      }
      if (compSTR) {
        count++;
      }
      compSTR = true;
    }
    counts.push_back(count);
    count = 0;
  }
}

// search() - search data
//
void search(database &db, vector<int> &counts) {
  int found = 0;
  cout << "Searching database..." << endl;
  for (int j = 0; j < db.names.size(); j++) {
    for (int i = 0; i < counts.size(); i++) {
      int num = db.STRcounts.at(j * db.STRs.size() + i);
      if (num == counts.at(i)) {
        found++;
      }
    }
    if (found == counts.size()) {
      cout << "Found in database! DNA matches: " << db.names.at(j) << endl;
    }
    found = 0;
  }
}

int main() {

  string com = "";
  vector<char> dna;
  vector<int> fileSTRcounts;
  database db;
  bool dbLoad = false;
  bool dnaLoad = false;
  bool processed = false;

  cout << "Welcome to the DNA Profiling Application." << endl;
  cout << "Enter command or # to exit:" << endl;
  cin >> com;
  while (com != "#") {
    if (com == "load_db") {
      if (!load_db(db))
        return 0;
      dbLoad = true;
    }
    if (com == "display") {
      display(db, dna, fileSTRcounts, dbLoad, dnaLoad, processed);
    }
    if (com == "load_dna") {
      if (!load_dna(dna))
        return 0;
      dnaLoad = true;
    }
    if (com == "process") {
      if (!dbLoad) {
        cout << "No database loaded." << endl;
      } else if (dbLoad && !dnaLoad) {
        cout << "No DNA loaded." << endl;
      } else {
        process(dna, fileSTRcounts, db);
        processed = true;
      }
    }
    if (com == "search") {
      if (!dbLoad) {
        cout << "No database loaded." << endl;
      } else if (dbLoad && !dnaLoad) {
        cout << "No DNA loaded." << endl;
      } else if (!processed) {
        cout << "No DNA processed." << endl;
      } else {
        search(db, fileSTRcounts);
      }
    }
    cout << "Enter command or # to exit:" << endl;
    cin >> com;
  }

  return 0;
}
