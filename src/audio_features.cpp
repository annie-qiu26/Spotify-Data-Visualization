#include "audio_features.h"

using namespace std;

// Function that takes in JSON Value to assign each feature in
// JSON Value to appropriate variable in AudioFeature object
// Assumes JSON value is valid
void AudioFeatures::ParseToFeatures(Json::Value& audio) {
        danceability = audio["danceability"].asDouble();
        energy = audio["energy"].asDouble();
        loudness = audio["loudness"].asDouble();
        speechiness = audio["speechiness"].asDouble();
        acousticness = audio["acousticness"].asDouble();
        instrumentalness = audio["instrumentalness"].asDouble();
        liveness = audio["liveness"].asDouble();
        valence = audio["valence"].asDouble();
        tempo = audio["tempo"].asDouble();
        duration_ms = audio["duration_ms"].asInt();
}

// Only brings in numeric features to a list that will be used
// for analyzing data
vector<pair<string, double>> AudioFeatures::GetFeatures() {
        vector<pair<string, double>> features;
        features.push_back(make_pair("Danceability", danceability));
        features.push_back(make_pair("Energy", energy));
        features.push_back(make_pair("Loudness", loudness));
        features.push_back(make_pair("Speechiness", speechiness));
        features.push_back(make_pair("Acousticness", acousticness));
        features.push_back(make_pair("Instrumentalness", instrumentalness));
        features.push_back(make_pair("Liveness", liveness));
        features.push_back(make_pair("Valence", valence));
        features.push_back(make_pair("Tempo", tempo));
        features.push_back(make_pair("Duration in Milliseconds", duration_ms));
        return features;
}
