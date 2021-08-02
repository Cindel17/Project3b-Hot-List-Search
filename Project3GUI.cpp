#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include<string>
#include<iostream>
#include <sstream>
#include<math.h>
#include<chrono>
#include <vector>
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

    for (unsigned int i = 0; i < theLine.size(); i++) {

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
            else if (theLine.at(i) == '\"' && theLine.at(i + 1) == ',') keepReading = false;

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
            list<Track*> tracksAtValue = trackMap.retrieve(trackname); //Retrieve the list of all tracks with the entered name
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

int main()
{
    //creating maps, recording creation time
    auto beforeMap = high_resolution_clock::now();
    HashMap<Track*> trackMap = createTrackMap("spotify_dataset.csv", 100000);
    auto afterMap = high_resolution_clock::now();
    auto dif = duration_cast<seconds>(afterMap - beforeMap);
    string trackMapTime = "Time to create trackMap: " + to_string(dif.count()) + " seconds";
    auto beforeMap2 = high_resolution_clock::now();
    HashMap<Playlist*> playlistMap = createPlaylistMap("spotify_dataset.csv", 100000);
    auto afterMap2 = high_resolution_clock::now();
    auto dif2 = duration_cast<seconds>(afterMap2 - beforeMap2);
    string playlistMapTime = "Time to create playlistMap: " + to_string(dif2.count()) + " seconds";

    //creating tries, recording creation time
    auto beforeTrie = high_resolution_clock::now();
    TrieNode<Track*> trackTrie = createTrackTrie("spotify_dataset.csv", 100000);
    auto afterTrie = high_resolution_clock::now();
    auto dif3 = duration_cast<seconds>(afterTrie - beforeTrie);
    string trackTrieTime = "Time to create trackTrie: " + to_string(dif3.count()) + " seconds";
    auto beforeTrie2 = high_resolution_clock::now();
    TrieNode<Playlist*> playlistTrie = createPlaylistTrie("spotify_dataset.csv", 100000);
    auto afterTrie2 = high_resolution_clock::now();
    auto dif4 = duration_cast<seconds>(afterTrie2 - beforeTrie2);
    string playlistTrieTime = "Time to create playlistTrie: " + to_string(dif4.count()) + " seconds";

    bool songSelected = false;
    bool playlistWordSelected = false;
    bool structureSelected = false;

    //setting the sf objects before entering loop for updating window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Playlist finder!");
    sf::Event event;
    sf::Font font;
    if (!font.loadFromFile("TimesNewerRoman-Italic.otf"))//this font was taken from https://timesnewerroman.com/ and is open-source
        return EXIT_FAILURE;

    string songInput;
    string playlistWordInput;
    string structureInput;
    string searchResult;
    string searchTime;
    sf::Text title("Welcome! Enter a song OR playlist, then a structure and press search!", font, 18);
    sf::Text title1("Song to search for:", font, 13);
    sf::Text inputsong("", font, 13);
    sf::Text title2("Playlist to search for:", font, 13);
    sf::Text inputPlaylistWord("", font, 13);
    sf::Text title3("Data structure: Trie or Map?", font, 13);
    sf::Text inputStructure("", font, 13);
    sf::Text search("Search", font, 15);
    sf::Text tmapTime(trackMapTime, font, 13);
    sf::Text pmapTime(playlistMapTime, font, 13);
    sf::Text ttrieTime(trackTrieTime, font, 13);
    sf::Text ptrieTime(playlistTrieTime, font, 13);
    sf::Text sTime(searchTime, font, 15);
    sf::Text sResults(searchResult, font, 15);
    sf::RectangleShape inputBox(sf::Vector2f(200,20));
    sf::RectangleShape inputBox1(sf::Vector2f(200, 20));
    sf::RectangleShape inputBox2(sf::Vector2f(200, 20));
    sf::RectangleShape searchButton(sf::Vector2f(50, 20));
    title.setFillColor(sf::Color::Black);
    title1.setFillColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setFillColor(sf::Color::White);
    inputsong.setFillColor(sf::Color::Black);
    title2.setFillColor(sf::Color::Black);
    inputBox1.setOutlineThickness(2);
    inputBox1.setOutlineColor(sf::Color::Black);
    inputBox1.setFillColor(sf::Color::White);
    inputPlaylistWord.setFillColor(sf::Color::Black);
    title3.setFillColor(sf::Color::Black);
    inputBox2.setOutlineThickness(2);
    inputBox2.setOutlineColor(sf::Color::Black);
    inputBox2.setFillColor(sf::Color::White);
    inputStructure.setFillColor(sf::Color::Black);
    searchButton.setOutlineThickness(1);
    searchButton.setOutlineColor(sf::Color::Black);
    searchButton.setFillColor(sf::Color::Transparent);
    search.setFillColor(sf::Color::Black);
    tmapTime.setFillColor(sf::Color::Black);
    pmapTime.setFillColor(sf::Color::Black);
    ttrieTime.setFillColor(sf::Color::Black);
    ptrieTime.setFillColor(sf::Color::Black);
    sTime.setFillColor(sf::Color::Black);
    sResults.setFillColor(sf::Color::Black);

    while (window.isOpen())
    {
        title.setPosition(0, 0);
        title1.setPosition(0, 25);
        inputBox.setPosition(2, 45);
        inputsong.setPosition(3, 45);
        title2.setPosition(0, 70);
        inputBox1.setPosition(2, 90);
        inputPlaylistWord.setPosition(3, 90);
        title3.setPosition(0, 115);
        inputBox2.setPosition(2, 135);
        inputStructure.setPosition(3, 135);
        searchButton.setPosition(150, 175);
        search.setPosition(153,175);
        tmapTime.setPosition(1, 530);
        pmapTime.setPosition(1, 545);
        ttrieTime.setPosition(1, 560);
        ptrieTime.setPosition(1, 575);
        sTime.setPosition(250, 45);
        sResults.setPosition(250, 75);

        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                //if window is being closed
                if (event.type == sf::Event::Closed)
                    window.close();

                //to check when a text box/rectangle is being pressed
                if (event.type == sf::Event::MouseButtonPressed) {
                    auto position = sf::Mouse::getPosition(window);
                    if (inputBox.getGlobalBounds().contains(sf::Vector2f(position))) {
                        inputBox.setOutlineColor(sf::Color::Blue);
                        inputBox1.setOutlineColor(sf::Color::Black);
                        inputBox2.setOutlineColor(sf::Color::Black);
                        searchButton.setFillColor(sf::Color::White);
                        songSelected = true;
                        playlistWordSelected = false;
                        structureSelected = false;
                    }
                    else if (inputBox1.getGlobalBounds().contains(sf::Vector2f(position))) {
                        inputBox.setOutlineColor(sf::Color::Black);
                        inputBox1.setOutlineColor(sf::Color::Blue);
                        inputBox2.setOutlineColor(sf::Color::Black);
                        searchButton.setFillColor(sf::Color::White);
                        playlistWordSelected = true;
                        songSelected = false;
                        structureSelected = false;
                    }
                    else if (inputBox2.getGlobalBounds().contains(sf::Vector2f(position))) {
                        inputBox.setOutlineColor(sf::Color::Black);
                        inputBox1.setOutlineColor(sf::Color::Black);
                        inputBox2.setOutlineColor(sf::Color::Blue);
                        searchButton.setFillColor(sf::Color::White);
                        structureSelected = true;
                        songSelected = false;
                        playlistWordSelected = false;
                    }
                    else if(searchButton.getGlobalBounds().contains(sf::Vector2f(position))) {
                        searchButton.setFillColor(sf::Color::Blue);
                        inputBox.setOutlineColor(sf::Color::Black);
                        inputBox1.setOutlineColor(sf::Color::Black);
                        inputBox2.setOutlineColor(sf::Color::Black);
                        playlistWordSelected = false;
                        songSelected = false;

                        if (structureInput.compare("Trie") != 0 && structureInput.compare("trie") != 0 &&
                            structureInput.compare("Map") != 0 && structureInput.compare("map") != 0) {//no valid structure
                            string timeOfSearch = "You didn't make a search!";
                            string result = "Invalid data structure!";
                            sResults.setString(result);
                        }
                        else {
                            if (songInput.compare("") == 0 && playlistWordInput.compare("") != 0) {//only playlist field has input
                                if (structureInput.compare("Trie") == 0 || structureInput.compare("trie") == 0) {//not case sensitive
                                    auto beforeSearch = high_resolution_clock::now();
                                    TrieNode<Playlist*>* playlistNode = playlistTrie.search(playlistWordInput);
                                    auto afterSearch = high_resolution_clock::now();
                                    auto searchTime = duration_cast<microseconds>(afterSearch - beforeSearch);
                                    string timeOfSearch = "Your search took " + to_string(searchTime.count()) + " microseconds!";
                                    sTime.setString(timeOfSearch);

                                    if (playlistNode != nullptr) {
                                        string result = "We found the playlist " + playlistNode->getData()->getName()
                                            + " with songs:\n   ";

                                        //displaying tracks in playlist
                                        vector<Track> tracks = playlistNode->getData()->getTracks();
                                        if (tracks.size() > 25) {
                                            for (int i = 0; i < 25; i++) {
                                                result += tracks[i].name + " by " + tracks[i].artist + "\n   ";
                                            }
                                        }
                                        else {
                                            for (int i = 0; i < tracks.size(); i++) {
                                                result += tracks[i].name + " by " + tracks[i].artist + "\n   ";
                                            }
                                        }

                                        sResults.setString(result);
                                    }
                                    else {
                                        string result = "We found no playlists with that name!";
                                        sResults.setString(result);
                                    }
                                }
                                else {//if map is chosen
                                    try {
                                        auto beforeSearch = high_resolution_clock::now();
                                        list<Playlist*> playlist = playlistMap.retrieve(playlistWordInput);
                                        auto afterSearch = high_resolution_clock::now();
                                        auto searchTime = duration_cast<microseconds>(afterSearch - beforeSearch);
                                        string timeOfSearch = "Your search took " + to_string(searchTime.count()) + " microseconds!";
                                        sTime.setString(timeOfSearch);

                                        string result = "We found the playlist ";
                                        for (Playlist* p : playlist) result += p->getName();
                                        result += " with songs:\n   ";

                                        //displaying tracks in playlist
                                        vector<Track> tracks;
                                        for (Playlist* p : playlist) tracks.insert(tracks.end(), p->getTracks().begin(), p->getTracks().end());

                                        if (tracks.size() > 25) {
                                            for (int i = 0; i < 25; i++) {
                                                result += tracks[i].name + " by " + tracks[i].artist + "\n   ";
                                            }
                                        }
                                        else {
                                            for (int i = 0; i < tracks.size(); i++) {
                                                result += tracks[i].name + " by " + tracks[i].artist + "\n   ";
                                            }
                                        }

                                        sResults.setString(result);
                                    }
                                    catch (out_of_range e) {
                                        string result = "We found no playlists with that name!";
                                        sResults.setString(result);
                                    }
                                }
                            }
                            else if (playlistWordInput.compare("") == 0 && songInput.compare("") != 0) {//only song field has input
                                if (structureInput.compare("Trie") == 0 || structureInput.compare("trie") == 0) {
                                    auto beforeSearch = high_resolution_clock::now();
                                    TrieNode<Track*>* trackNode = trackTrie.search(songInput);
                                    auto afterSearch = high_resolution_clock::now();
                                    auto searchTime = duration_cast<microseconds>(afterSearch - beforeSearch);
                                    string timeOfSearch = "Your search took " + to_string(searchTime.count()) + " microseconds!";

                                    if (trackNode != nullptr) {
                                        vector<string> playlists = trackNode->getData()->playlistNames;
                                        
                                        string result = "We found the following playlists with that track: \n   ";
                                        for (int i = 0; i < playlists.size(); i++) {
                                            result += playlists[i] + "\n";
                                        }
                                        sResults.setString(result);
                                    }
                                    else {
                                        string result = "We found no playlists with a song of that name!";
                                        sResults.setString(result);
                                    }
                                }
                                else {//if map is chosen
                                    try {
                                        auto beforeSearch = high_resolution_clock::now();
                                        list<Track*> tracks = trackMap.retrieve(songInput);
                                        auto afterSearch = high_resolution_clock::now();
                                        auto searchTime = duration_cast<microseconds>(afterSearch - beforeSearch);
                                        string timeOfSearch = "Your search took " + to_string(searchTime.count()) + " microseconds!";

                                        vector<string> playlists;
                                        for (Track* t : tracks) playlists.insert(playlists.end(), t->playlistNames.begin(), t->playlistNames.end());                                     
                                    
                                        string result = "We found the following playlists with that track: \n   ";
                                        for (int i = 0; i < playlists.size(); i++) {
                                            result += playlists[i] + "\n";
                                        }
                                        sResults.setString(result);
                                    }
                                    catch (out_of_range e) {
                                        string result = "We found no playlists with a song of that name!";
                                        sResults.setString(result);
                                    }
                                }
                            }
                            else if (playlistWordInput.compare("") != 0 && songInput.compare("") != 0) {//both are filled out
                                string timeOfSearch = "You didn't make a search!";
                                sTime.setString(timeOfSearch);
                                string result = "Sorry! You must choose a song OR playlist!";
                                sResults.setString(result);
                            }
                            else {//neither filled out
                                string timeOfSearch = "You didn't make a search!";
                                sTime.setString(timeOfSearch);
                                string result = "Input a song OR playlist, then a structure to get started!";
                                sResults.setString(result);
                            }
                        }
                    }
                    else {//click on empty space in window
                        inputBox.setOutlineColor(sf::Color::Black);
                        inputBox1.setOutlineColor(sf::Color::Black);
                        inputBox2.setOutlineColor(sf::Color::Black);
                        searchButton.setFillColor(sf::Color::White);
                        songSelected = false;
                        playlistWordSelected = false;
                        structureSelected = false;
                    }
                }

                //if text is being entered
                if (event.type == sf::Event::TextEntered) {
                    //if song box is selected
                    if (event.text.unicode < 128 && songSelected) {
                        if (event.text.unicode == 8) //backspace
                            songInput = songInput.substr(0, songInput.size() - 1);
                        else
                            songInput += event.text.unicode;

                        inputsong.setString(songInput);
                    }
                    //if playlistWord box is selected
                    else if (event.text.unicode < 128 && playlistWordSelected) {
                        if (event.text.unicode == 8) //backspace
                            playlistWordInput = playlistWordInput.substr(0, playlistWordInput.size() - 1);
                        else
                            playlistWordInput += event.text.unicode;

                        inputPlaylistWord.setString(playlistWordInput);
                    }
                    //if structure box is selected
                    else if (event.text.unicode < 128 && structureSelected) {
                        if (event.text.unicode == 8) //backspace
                            structureInput = structureInput.substr(0, structureInput.size() - 1);
                        else
                            structureInput += event.text.unicode;

                        inputStructure.setString(structureInput);
                    }
                }
            }

            window.clear(sf::Color::White);
            window.draw(title);
            window.draw(title1);
            window.draw(inputBox);
            window.draw(inputsong);
            window.draw(title2);
            window.draw(inputBox1);
            window.draw(inputPlaylistWord);
            window.draw(title3);
            window.draw(inputBox2);
            window.draw(inputStructure);
            window.draw(searchButton);
            window.draw(search);
            window.draw(tmapTime);
            window.draw(pmapTime);
            window.draw(ttrieTime);
            window.draw(ptrieTime);
            window.draw(sTime);
            window.draw(sResults);
            window.display();
        }
    }

    return 0;
}