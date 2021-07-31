#include<string>
#include<iostream>
#include <fstream>
#include <sstream>
#include<math.h>
#include "HashMap.h"
#include "Playlist.h"
using namespace std;

vector<vector<string>> read_data() {
    fstream file;
    file.open("spotify_dataset.csv", ios::in);

    string user_id;
    string artistname;
    string trackname;
    string playlistname;

    string line;
    string word = "";

    //customize read in starting row
    //for (int i = 0; i < 100000; i++) {
        //getline(file, line);
   // }

    //start reading each row in the for loop
    vector<vector<string>> allRows;
    for (int i = 0; i < 15; i++) {
        vector <string> row;

        getline(file, line);
        stringstream s(line);

        //while loop store the stringstream variable s into a string variable word
        //if the string is empty so will be the string stored into vector row
        while (getline(s, word, ',')) {
            int size = word.length();
            if (word.empty() == true) {
                word = "";
            }
            else {
                word = word.substr(1, size - 2);
            }
            row.push_back(word);
        }
        allRows.push_back(row);
    }
    return allRows;
}

//createTrackMap generates a map storing track names as keys and Track objects as values.
//Note that this function does not generate Playlist objects for each playlist. Use the createPlaylistMap function for that.

HashMap<Track> createTrackMap(string filename, int numEntries) {
    fstream file;
    HashMap<Track> trackMap(1000);
    file.open(filename, ios::in);

    string user_id;
    string artistname;
    string trackname;
    string playlistname;

    string line;
    string word = "";

    //customize read in starting row
    for (int i = 0; i < 1; i++) {
        getline(file, line);
    }

    //start reading each row in the for loop
    for (int i = 0; i < numEntries; i++) {
        vector <string> row = {};

        getline(file, line);
        stringstream s(line);

        //while loop store the stringstream variable s into a string variable word
        //if the string is empty so will be the string stored into vector row
        while (getline(s, word, ',')) {

            if (word.empty() == true) {
                word = "";
            }

            else {
                int size = word.length();

                //when data begins with "\"" it means it has more than one ','
                //the while loop keeps adding strings before each ',' until one string ends with "\""
                if (word.at(0) == '\"') {
                    string temphalf = "";
                    word = word.substr(1, size);
                    getline(s, temphalf, ',');

                    while (temphalf.at(temphalf.length() - 1) != '\"') {
                        word = word + "," + temphalf;
                        getline(s, temphalf, ',');
                        size = temphalf.length();
                    }

                    temphalf = temphalf.substr(0, (temphalf.length() - 1));
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


        bool alreadyInMap = false;
        //If the track is already in the map, add the current playlist to its vector of associated playlists.
        try {
            list<Track>& tracksAtValue = trackMap.retrieve(trackname); //Retrieve the list of all tracks with the entered name
            for (Track t : tracksAtValue) { //Check for the track in the list of values associated with the name. It is not necessarily there, as different tracks with the same name will be stored under the same entry.
                if (t.artist == artistname) {
                    t.playlistNames.push_back(playlistname);
                    alreadyInMap = true;
                }
            }
        }
        catch (out_of_range e) {
            //Do nothing in this case
        }
        //If the track was not found in the map, add it.
        if (alreadyInMap == false) {
            Track newTrack(trackname, artistname);
            newTrack.addPlaylist(playlistname);
            trackMap.insert(trackname, newTrack);
        }

    }

    file.close();
    return trackMap;
}


int main() {

    //Test code for map

    HashMap<int> theMap(2);
    theMap.insert("a", 7);
    theMap.insert("a", 3);
    theMap.insert("a", 5);
    theMap.print("a");
    theMap.print();
    cout << endl;
    theMap.insert("abcddcba", 2);
    theMap.insert("dcbaabcd", 3);
    theMap.print();
    cout << endl;
    theMap.insert("b", 2);
    theMap.insert("bb", 3);
    theMap.insert("ccc", 3);
    theMap.print();
    cout << endl;
    theMap.insert("a longer string to test whether the hashing function still works on long strings", 4);
    theMap.insert("test", 5);
    theMap.print();

    //Test code for reading in data

    vector<vector<string>> theRows = read_data();
    vector<Track> tracks;
    for (vector<string> row : theRows) {
        string user_id = row.at(0);
        string artistname = row.at(1);
        string trackname = row.at(2);
        string playlistname = row.at(3);

        cout << "user_id: " << user_id << endl;
        cout << "artistname: " << artistname << endl;
        cout << "trackname: " << trackname << endl;
        cout << "playlistname: " << playlistname << endl;

        Track aTrack(trackname, artistname);
        tracks.push_back(aTrack);

        cout << endl;

    }

    HashMap<Track> trackMap = createTrackMap("spotify_dataset.csv", 30);
    trackMap.print();

    return 0;
}
