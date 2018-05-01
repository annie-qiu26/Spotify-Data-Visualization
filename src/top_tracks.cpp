#include "top_tracks.h"
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

// Function used to standardize data to help with plotting
vector <vector<pair<string, double>>> TopTracks::StandardizeFeatures(vector<vector<pair<string, double>>> dataset) {
        vector <double> means = CalculateMeans(dataset);
        vector <double> stds = CalculateStds(dataset);
        vector <vector <pair<string, double>>> standardized_dataset(dataset.size());
        for (unsigned int i = 0; i < dataset.size(); i++) {
                for (unsigned int j = 0; j < dataset[i].size(); j++) {
                        standardized_dataset[i].push_back(make_pair(dataset[i][j].first,
                                (dataset[i][j].second - means[j]) / stds[j]));
                }
        }

        return standardized_dataset;

}

// Helper function to standardize features
vector <double> TopTracks::CalculateMeans(vector<vector<pair<string, double>>> dataset) {
        vector<double> means;
        // if dataset is empty
        if (dataset.size() == 0) {
                return means;
        }

        // To intialize vector with names and values from the first number in each set
        for (unsigned int i = 0; i < dataset[0].size(); i++) {
                means.push_back(dataset[0][i].second);
        }


        // Add up all the values in the dataset with respect to the feature
        for (unsigned int i = 1; i < dataset.size(); i++) {
                for (unsigned int j = 0; j < dataset[i].size(); j++) {
                        means[j] += dataset[i][j].second;
                }
        }

        // Divide all the sums by the number of data items
        for (unsigned int i = 0; i < dataset[0].size(); i++) {
                means[i] /= dataset.size();
        }

        return means;
}

// Helper function to standardize features
vector <double> TopTracks::CalculateStds(vector<vector<pair<string, double>>> dataset) {
	vector<double> stds;
	// if dataset is empty
	if (dataset.size() == 0) {
		return stds;
	}

	vector <double> means = CalculateMeans(dataset);

	// To intialize vector with names and values from the first number in each set
	for (unsigned int i = 0; i < dataset[0].size(); i++) {
		stds.push_back((dataset[0][i].second - means[i]) * (dataset[0][i].second - means[i]));
	}

	// Add up all the differences in the dataset with respect to the feature
	for (unsigned int i = 1; i < dataset.size(); i++) {
		for (unsigned int j = 0; j < dataset[i].size(); j++) {
			stds[j] += (dataset[i][j].second - means[j]) * (dataset[i][j].second - means[j]);
		}
	}

	// Divide all the sums by the number of data items minus 1, if size 1, data has no variance
	if (dataset.size() > 1) {
		for (unsigned int i = 0; i < dataset[0].size(); i++) {
			stds[i] = sqrt(stds[i] / (dataset.size() - 1));
		}
	}
	return stds;
}
