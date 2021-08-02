#pragma once
#include<string>
#include<vector>
#include<iostream>
using namespace std;

struct Track {
	string name;
	string artist;
	vector<string> playlistNames; 

	Track();
	Track(string name, string artist);
	bool operator==(const Track& rhs); //return whether the name and artist are equal
	void addPlaylist(string playlistName);
	void printInfo();
	friend ostream& operator<<(ostream& os, const Track& theTrack); //Overload the << operator so that cout prints the track name
};

class Playlist {
	vector<Track> tracks;
	string name;
public:
	Playlist();
	Playlist(string name);
	Playlist(string name, vector<Track>& tracks);
	string getName();
	vector<Track>& getTracks();
	void addTrack(const Track& track);
	void addTrack(string trackName, string artistName);
	void printInfo();
	friend ostream& operator<<(ostream& os, const Playlist& thePlaylist); //Overload the << operator so that cout prints the playlist name
};
