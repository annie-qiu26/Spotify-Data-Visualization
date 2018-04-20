#include "top_tracks.h"
#include "audio_features.h"
//For debugging
#include <iostream>

using namespace std;

TopTracks::TopTracks() {

}

TopTracks::~TopTracks() {

}

// Takes in json data of top tracks and returns all the
// track ids in a vector of strings
vector<string> TopTracks::ParseToIDs(string json_data) {
        vector<string> ids;
        Json::Reader reader;
        Json::Value top_tracks;
        // check validity of json_string
        bool success = reader.parse(json_data, top_tracks);
        if (!success) {
                throw "Failed to parse data";
        }
        // finds ids of each track in items
        Json::Value& items = top_tracks["items"];
        for (unsigned int i = 0; i < items.size(); i++) {
                ids.push_back(items[i]["id"].asString());
        }
        return ids;
}

// Takes in json data of top tracks of a playlist
// and returns all the track ids in a vector of strings
vector<string> TopTracks::ParseToIDsPlaylist(string json_data) {
        vector<string> ids;
        Json::Reader reader;
        Json::Value top_tracks;
        // check validity of json_string
        bool success = reader.parse(json_data, top_tracks);
        if (!success) {
                throw "Failed to parse data";
        }
        // finds ids of each track in items
        Json::Value& items = top_tracks["items"];
        for (unsigned int i = 0; i < items.size(); i++) {
                ids.push_back(items[i]["track"]["id"].asString());
        }
        return ids;
}

// Function that takes in JSON data and returns a vector of
// vector filled with audio features of all the tracks
vector<vector<pair<string, double>>> TopTracks::GetDataset(Json::Value& audio, int size) {
        vector<vector<pair<string, double>>> dataset;
        for (int i = 0; i < size; i++) {
                AudioFeatures track;
                track.ParseToFeatures(audio[i]);
                dataset.push_back(track.GetFeatures());
        }
        return dataset;
}
