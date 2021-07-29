#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

void read_data(){
  fstream file; 
  file.open("spotify_dataset.csv", ios::in);

  string user_id;
  string artistname;
  string trackname;
  string playlistname;

  string line;
  string word = "";

  //customize read in starting row
  for(int i = 0; i<1; i++){
    getline(file,line);
  }

  //start reading each row in the for loop
  for(int i = 0; i<100000; i++){
    vector <string> row = {};

    getline(file,line);
    stringstream s(line);

    //while loop store the stringstream variable s into a string variable word
    //if the string is empty so will be the string stored into vector row
    while(getline(s,word,',')){
      
      if(word.empty()==true){
        word = "";
      }

      else {
        int size = word.length();

        //when data begins with "\"" it means it has more than one ','
        //the while loop keeps adding strings before each ',' until one string ends with "\""
        if(word.at(0)=='\"'){
          string temphalf = "";
          word = word.substr(1,size);
          getline(s,temphalf,',');

          while(temphalf.at(temphalf.length()-1)!='\"'){
            word = word + "," + temphalf;
            getline(s,temphalf,',');
            size = temphalf.length();
          }  

          temphalf = temphalf.substr(0,(temphalf.length()-1));
          word = word + "," + temphalf;
        }

      }

      row.push_back(word);
    }
    
    //here are the vector indexes of each data category 
    user_id = row.at(0);
    artistname = row.at(1);
    trackname = row.at(2);
    playlistname = row.at(3);

    // you can print out the strings here
    /*
    cout << "user_id: " << user_id << endl; 
    cout << "artistname: " << artistname << endl;
    cout << "trackname: " << trackname << endl;
    cout << "playlistname: " << playlistname << endl;
    cout << endl;  */

    //***STORE THE DATA INTO UR CLASS OBJECT HERE: ***


  }

  file.close();
}

int main(){

 // read_data();
  return 0;
}