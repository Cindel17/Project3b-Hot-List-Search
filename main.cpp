#include<string>
#include<iostream>
#include <fstream>
#include <sstream>
#include<math.h>
#include<chrono>
#include "HashMap.h"
#include "Playlist.h"
#include "Trie.h"
using namespace std;
using namespace std::chrono;

//**************Function to read in the data*********************//

vector<string> parseLine(string theLine) {
    vector<string> returnVec(4, "");

    int vecPosition = 0;
    bool keepReading = false; //Keep track of whether we are currently reading in a string enclosed in quotes.

    for (int i = 0; i < theLine.size(); i++) {
       
        if (keepReading == true) {
            /* If the next character is an escape character, do the following:
                1. Increment i without adding the escape character to the string.
                2. Add the escaped character to the string.
            */

            if (theLine.at(i) == '\\') {
                i++;
                returnVec.at(vecPosition).push_back(theLine.at(i));
            }

            //If the next character is a closing quote, set keepReading to false until we find the next open quote.
            else if (theLine.at(i) == '\"' && i == theLine.size() - 1) keepReading = false;
            else if(theLine.at(i) == '\"' && theLine.at(i+1) == ',') keepReading = false;

            else returnVec.at(vecPosition).push_back(theLine.at(i));
        }
        else {
            if (theLine.at(i) == ',') vecPosition++;
            else if (theLine.at(i) == '\"') keepReading = true;
        }
    }


    return returnVec;
}

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

        getline(file, line);

        vector<string> row = parseLine(line);

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

        getline(file, line);

        vector<string> row = parseLine(line);

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

//**************Functions to initialize a trie*********************//

//createTrackTrie generates a trie of Track objects searchable by their name.
TrieNode<Track*> createTrackTrie(string filename, int numEntries) {
    fstream file;
    TrieNode<Track*> root;
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

        getline(file, line);

        vector<string> row = parseLine(line);

        //here are the vector indexes of each data category 
        user_id = row.at(0);
        artistname = row.at(1);
        trackname = row.at(2);
        playlistname = row.at(3);

        TrieNode<Track*>* currNode = root.search(trackname);
        if (currNode != nullptr) {
            Track* trackPtr = currNode->getData();
            trackPtr->addPlaylist(playlistname);
        }
        else {
            Track* newTrack = new Track(trackname, artistname);
            newTrack->addPlaylist(playlistname);
            root.insert(trackname, newTrack);
        }

    }

    file.close();
    return root;
}

//createPlaylistTrie creates a trie of Playlist objects searchable by their name.
TrieNode<Playlist*> createPlaylistTrie(string filename, int numEntries) {
    fstream file;
    TrieNode<Playlist*> root;
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

        getline(file, line);

        vector<string> row = parseLine(line);

        //here are the vector indexes of each data category 
        user_id = row.at(0);
        artistname = row.at(1);
        trackname = row.at(2);
        playlistname = row.at(3);

        TrieNode<Playlist*>* currNode = root.search(playlistname);
        if (currNode != nullptr) {
            Playlist* playlist = currNode->getData();
            playlist->addTrack(trackname, artistname);
        }
        else {
            Playlist* playlistPtr = new Playlist(playlistname);
            playlistPtr->addTrack(trackname, artistname);
            root.insert(playlistname, playlistPtr);
        }

    }

    file.close();
    return root;
}

int main() {

    HashMap<Track*> trackMap;
    HashMap<Playlist*> playlistMap;
    TrieNode<Track*> trackTrie;
    TrieNode<Playlist*> playlistTrie;

    try {

        int numEntries = 0;
        cout << "Welcome to Hot List Search!" << endl;
        cout << "How many entries of the database would you like to load?" << endl;
        cin >> numEntries;

        cout << "Creating track map..." << endl;

        auto beforeMap = high_resolution_clock::now();
        trackMap = createTrackMap("spotify_dataset.csv", numEntries);
        auto afterMap = high_resolution_clock::now();
        cout << "Track map information: " << endl;
        cout << "Bucket count: " << trackMap.bucketCount() << endl;
        cout << "Number of entries: " << trackMap.getAllKeys().size() << endl;
        cout << "Load factor: " << (float)trackMap.getAllKeys().size() / (float)trackMap.bucketCount() << endl;

        auto timeTaken = duration_cast<milliseconds>(afterMap - beforeMap);
        cout << "Time taken to create track map: " << timeTaken.count() << " milliseconds " << endl;

        cout << endl;

        cout << "Creating playlist map..." << endl;
        cout << "Playlist map information:" << endl;
        auto beforeMap2 = high_resolution_clock::now();
        playlistMap = createPlaylistMap("spotify_dataset.csv", numEntries);
        auto afterMap2 = high_resolution_clock::now();
        cout << "Bucket count: " << playlistMap.bucketCount() << endl;
        cout << "Number of entries: " << playlistMap.getAllKeys().size() << endl;
        cout << "Load factor: " << (float)playlistMap.getAllKeys().size() / (float)playlistMap.bucketCount() << endl;

        auto timeTaken2 = duration_cast<milliseconds>(afterMap2 - beforeMap2);
        cout << "Time taken to create playlist map: " << timeTaken2.count() << " milliseconds" << endl;

        cout << endl;


        //Trie test code

        cout << "Creating track trie..." << endl;
        auto beforeTrackTrie = high_resolution_clock::now();
        trackTrie = createTrackTrie("spotify_dataset.csv", numEntries);
        auto afterTrackTrie = high_resolution_clock::now();
        auto timeTakenTrackTrie = duration_cast<milliseconds>(afterTrackTrie - beforeTrackTrie);
        cout << "Time taken to create track trie: " << timeTakenTrackTrie.count() << " milliseconds" << endl;

        cout << "Creating playlist trie..." << endl;
        auto beforePlaylistTrie = high_resolution_clock::now();
        playlistTrie = createPlaylistTrie("spotify_dataset.csv", numEntries);
        auto afterPlaylistTrie = high_resolution_clock::now();
        auto timeTakenPlaylistTrie = duration_cast<milliseconds>(afterPlaylistTrie - beforePlaylistTrie);
        cout << "Time taken to create playlist trie: " << timeTakenPlaylistTrie.count() << " milliseconds" << endl;

        cout << endl;
    }
    catch (out_of_range e) {
        cout << "The program failed to read the data file. Check that the file is configured correctly." << endl;
        cout << "The file should be called spotify_dataset.csv and stored in the same directory as main." << endl;
    }
    

    int choice = 0;

    while (choice != 3) {
        cout << "What would you like to do? Enter a number:" << endl;
        cout << "1. Search for a track in the database" << endl;
        cout << "2. Search for a playlist in the database" << endl;
        cout << "3. Exit" << endl;

        cin >> choice;

        int whichStructure = 0;
        string searchTerm;

        if (choice != 3) {

            cout << "Would you like to search the map or the trie? Enter a number:" << endl;
            cout << "1. Trie" << endl;
            cout << "2. Map" << endl;
            cin >> whichStructure;

            string newline;
            getline(cin, newline);

            cout << "Enter a search term: " << endl;
            getline(cin, searchTerm);

        }

        if (whichStructure == 1) {
            if (choice == 1) {
                auto beforeSearch = high_resolution_clock::now();
                TrieNode<Track*>* trackNode = trackTrie.search(searchTerm);
                auto afterSearch = high_resolution_clock::now();
                auto searchTime = duration_cast<microseconds>(afterSearch - beforeSearch);
                cout << "Your search took " << searchTime.count() << " microseconds." << endl;
                if(trackNode == nullptr) {
                    cout << "Your search did not return any results." << endl;
                }
                else {
                    cout << "We found the following track: " << endl;
                    Track* theTrack = trackTrie.search(searchTerm)->getData();
                    theTrack->printInfo();
                }
            }
            else if (choice == 2) {
                auto beforeSearch = high_resolution_clock::now();
                TrieNode<Playlist*>* playlistNode = playlistTrie.search(searchTerm);
                auto afterSearch = high_resolution_clock::now();
                auto searchTime = duration_cast<microseconds>(afterSearch - beforeSearch);
                cout << "Your search took " << searchTime.count() << " microseconds." << endl;
                if(playlistNode != nullptr) {
                    Playlist* thePlaylist = playlistTrie.search(searchTerm)->getData();
                    cout << "We found the following playlist: " << endl;
                    thePlaylist->printInfo();
                }
                else {
                    cout << "Your search did not return any results." << endl;
                }
            }
        }
        else if (whichStructure == 2) {
            if (choice == 2) {
                try {
                    auto beforeSearch = high_resolution_clock::now();
                    list<Playlist*> playlists = playlistMap.retrieve(searchTerm);
                    auto afterSearch = high_resolution_clock::now();
                    auto searchTime = duration_cast<microseconds>(afterSearch - beforeSearch);
                    cout << "Your search took " << searchTime.count() << " microseconds." << endl;
                    cout << "We found the following playlist: " << endl;
                    for (Playlist* p : playlists) p->printInfo();
                }
                catch (out_of_range e) {
                    cout << "Your search did not return any results." << endl;
                }
            }
            else if (choice == 1) {
                try {
                    auto beforeSearch = high_resolution_clock::now();
                    list<Track*> tracks = trackMap.retrieve(searchTerm);
                    auto afterSearch = high_resolution_clock::now();
                    auto searchTime = duration_cast<microseconds>(afterSearch - beforeSearch);
                    cout << "Your search took " << searchTime.count() << " microseconds." << endl;
                    cout << "We found the following track: " << endl;
                    for (Track* t: tracks) t->printInfo();
                }
                catch (out_of_range e) {
                    cout << "Your search did not return any results." << endl;
                }

            }

        }

    }

    
    return 0;
}
