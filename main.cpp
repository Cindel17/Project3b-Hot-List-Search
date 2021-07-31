#include<string>
#include<iostream>
#include <fstream>
#include <sstream>
#include<math.h>
#include<chrono>
#include "HashMap.h"
#include "Playlist.h"
using namespace std;
using namespace std::chrono;


//**************Functions to initialize a map*********************//

//createTrackMap generates a map storing track names as keys and Track objects as values.

HashMap<Track*> createTrackMap(string filename, int numEntries) {
    fstream file;
    HashMap<Track*> trackMap(1000);
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
            int size = word.length();
            if (word.empty() == true) {
                word = "";
            }
            else {
                word = word.substr(1, size - 2);
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
            list<Track*>& tracksAtValue = trackMap.retrieve(trackname); //Retrieve the list of all tracks with the entered name
            for (Track* t : tracksAtValue) { //Check for the track in the list of values associated with the name. It is not necessarily there, as different tracks with the same name will be stored under the same entry.
                if (t->artist == artistname) {
                    t->playlistNames.push_back(playlistname);
                    alreadyInMap = true;
                }
            }
        }
        catch (out_of_range e) {
            //Do nothing in this case
        }
        //If the track was not found in the map, add it.
        if (alreadyInMap == false) {
            Track* newTrack = new Track(trackname, artistname);
            newTrack->addPlaylist(playlistname);
            trackMap.insert(trackname, newTrack);
        }

    }

    file.close();
    return trackMap;
}

//createPlaylistMap stores names of playlists as keys and Playlist objects as values.

HashMap<Playlist*> createPlaylistMap(string filename, int numEntries) {
    fstream file;
    HashMap<Playlist*> playlistMap(1000);
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
            int size = word.length();
            if (word.empty() == true) {
                word = "";
            }
            else {
                word = word.substr(1, size - 2);
            }
            row.push_back(word);
        }

        //here are the vector indexes of each data category 
        user_id = row.at(0);
        artistname = row.at(1);
        trackname = row.at(2);
        playlistname = row.at(3);

        //If the playlist is already in the map, add the current track to its vector of tracks
        try {
            //playlistMap.retrieve(playlistname).front()->printInfo();
            playlistMap.retrieve(playlistname).front()->addTrack(trackname, artistname);
        }
        catch (exception e) {
            Playlist* newPlaylist = new Playlist(playlistname);
            newPlaylist->addTrack(trackname, artistname);
            playlistMap.insert(playlistname, newPlaylist);
        }

    }

    file.close();
    return playlistMap;
}

//**************Functions to print the values of a map*********************//

/* For both printTrackMap and printPlaylistMap, numEntries indicates how many entries from the map will be printed.
   For example, to print the first 10 entries of a playlist map called theMap, run printPlaylistMap(theMap, 10).
 */

void printTrackMap(HashMap<Track*>& theMap, int numEntries) {
    vector<string> keys = theMap.getAllKeys();
    for (int i = 0; i < numEntries; i++) {
        string s = keys.at(i);
        list<Track*> tracks = theMap.retrieve(s);
        for (Track* t : tracks) {
            t->printInfo();
        }
    }
}

void printPlaylistMap(HashMap<Playlist*>& theMap, int numEntries) {
    vector<string> keys = theMap.getAllKeys();
    for (int i = 0; i < numEntries; i++) {
        string s = keys.at(i);
        list<Playlist*> playlists = theMap.retrieve(s);
        for (Playlist* p: playlists) {
            p->printInfo();
            cout << endl;
        }
    }
}


int main() {

    //Code to test creation of a map organized by track and a map organized by playlist.
    //Each test case constructs a map with the full 100,000 entries, but only prints the first 10.

    cout << "Creating track map..." << endl;

    auto beforeMap = high_resolution_clock::now();
    HashMap<Track*> trackMap = createTrackMap("spotify_dataset.csv", 100000);
    auto afterMap = high_resolution_clock::now();
    printTrackMap(trackMap, 10);
    cout << "Bucket count: " << trackMap.bucketCount() << endl;
    cout << "Number of entries: " << trackMap.getAllKeys().size() << endl;
    cout << "Load factor: " << (float)trackMap.getAllKeys().size() / (float)trackMap.bucketCount() << endl;

    auto timeTaken = duration_cast<milliseconds>(afterMap - beforeMap);
    cout << "Time taken to create track map: " << timeTaken.count() << " milliseconds " << endl;

    cout << endl;

    cout << "Creating playlist map..." << endl;

    auto beforeMap2 = high_resolution_clock::now();
    HashMap<Playlist*> playlistMap = createPlaylistMap("spotify_dataset.csv", 100000);
    auto afterMap2 = high_resolution_clock::now();
    printPlaylistMap(playlistMap, 10);
    cout << "Bucket count: " << playlistMap.bucketCount() << endl;
    cout << "Number of entries: " << playlistMap.getAllKeys().size() << endl;
    cout << "Load factor: " << (float)playlistMap.getAllKeys().size() / (float)playlistMap.bucketCount() << endl;

    auto timeTaken2 = duration_cast<milliseconds>(afterMap2 - beforeMap2);
    cout << "Time taken to create playlist map: " << timeTaken2.count() << " milliseconds" << endl;

    return 0;
}
