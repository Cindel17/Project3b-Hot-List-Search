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

ostream& operator<<(ostream& os, const Track& theTrack) {
	os << theTrack.name;
	return os;
}

/*******************Functions for Playlist class***********************/

Playlist::Playlist() {
	name = "undefined";
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

ostream& operator<<(ostream& os, const Playlist& thePlaylist) {
	os << thePlaylist.name;
	return os;
}
