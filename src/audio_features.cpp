#include "audio_features.h"

using namespace std;

AudioFeatures::AudioFeatures()
{
}


AudioFeatures::~AudioFeatures()
{
}

void AudioFeatures::ParseToFeatures(Json::Reader reader) {
        danceability = audio["danceability"].asDouble();
        energy = audio["energy"].asDouble();
        key = audio["key"].asInt();
        loudness = audio["loudness"].asDouble();
        mode = audio["mode"].asInt();
        speechiness = audio["speechiness"].asDouble();
        acousticness = audio["acousticness"].asDouble();
        instrumentalness = audio["instrumentalness"].asDouble();
        liveness = audio["liveness"].asDouble();
        valence = audio["valence"].asDouble();
        tempo = audio["tempo"].asDouble();
        type = audio["type"].asString();
        id = audio["id"].asString();
        uri = audio["uri"].asString();
        track_href = audio["track_href"].asString();
        analysis_url = audio["analysis_url"].asString();
        duration_ms = audio["duration_ms"].asInt();
        time_signature = audio["time_signature"].asInt();
}

// Only brings in numeric features to a list that will be used
// for analyzing data
vector<pair<string, double>> AudioFeatures::GetFeatures() {
        vector<pair<string, double>> features;
        features.push_back(make_pair("danceability", danceability));
        features.push_back(make_pair("energy", energy));
        features.push_back(make_pair("key", key)); //*
        features.push_back(make_pair("loudness", loudness));
        features.push_back(make_pair("mode", mode)); //*
        features.push_back(make_pair("speechiness", speechiness));
        features.push_back(make_pair("acousticness", acousticness));
        features.push_back(make_pair("instrumentalness", instrumentalness));
        features.push_back(make_pair("liveness", liveness));
        features.push_back(make_pair("valence", valence));
        features.push_back(make_pair("tempo", tempo));
        features.push_back(make_pair("duration_ms", duration_ms)); //*
        features.push_back(make_pair("time_signature", time_signature)); //*

}

//* these features might not have an affect in predictions
