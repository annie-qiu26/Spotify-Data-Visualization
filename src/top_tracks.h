#pragma once
#include <jsoncpp/json/json.h>
#include <string>
#include <vector>

using namespace std;

class TopTracks
{
public:
        TopTracks();
        ~TopTracks();
        vector<string> ParseToIDs(string json_data);
};
