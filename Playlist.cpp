#include "Playlist.h"

/*******************Functions for Track struct***********************/

Track::Track() {
	name = "undefined name";
	artist = "undefined artist";
}

Track::Track(string _name, string _artist) {
	name = _name;
	artist = _artist;
}

bool Track::operator==(const Track& rhs) {
	return(this->name == rhs.name && this->artist == rhs.artist);
}

void Track::addPlaylist(string playlistName) {
	playlistNames.push_back(playlistName);
}

void Track::printInfo() {
	cout << "Name of track: " << name << endl;
	cout << "Artist: " << artist << endl;
	cout << "Names of playlists containing this track:" << endl;
	for (string s : playlistNames) cout << s << endl;
	cout << endl;
}

ostream& operator<<(ostream& os, const Track& theTrack) {
	os << theTrack.name;
	return os;
}

/*******************Functions for Playlist class***********************/

Playlist::Playlist() {
	name = "undefined";
}

Playlist::Playlist(string _name) {
	name = _name;
}

Playlist::Playlist(string _name, vector<Track>& _tracks) {
	name = _name;
	for (Track t : _tracks) tracks.push_back(t);
}

string Playlist::getName() {
	return name;
}

vector<Track>& Playlist::getTracks() {
	return tracks;
}

void Playlist::addTrack(const Track& track) {
	tracks.push_back(track);
}

void Playlist::addTrack(string trackName, string artistName) {
	Track theTrack(trackName, artistName);
	tracks.push_back(theTrack);
}

void Playlist::printInfo() {
	cout << "Name of playlist: " << name << endl;
	cout << "Tracks in this playlist: " << endl;
	for (Track t : tracks) {
		cout << t.name << endl;
	}
}

ostream& operator<<(ostream& os, const Playlist& thePlaylist) {
	os << thePlaylist.name;
	return os;
}
