#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include "../src/top_tracks.h"

using namespace std;

// Temporary solution from:
// https://www.gamedev.net/forums/topic/353162-reading-a-whole-file-into-a-string-with-ifstream/
ifstream infile("../data/top_tracks.json");
string file_contents((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
TopTracks test_obj;
vector<string> ids;

TEST_CASE("Parse to IDs Size Test") {
        ids = test_obj.ParseToIDs(file_contents);
        REQUIRE(ids.size() == 50);
}

TEST_CASE("Parse to IDs Content Test") {
        REQUIRE(ids[0] == "7iS5V0U7FEX2iSEWn0m9Xw");
        REQUIRE(ids[30] == "1g1TeDflB6atAy7HKwrzXu");
        REQUIRE(ids[49] == "15ahYSiHAIMxAnujlXVtta");
}

TEST_CASE("Parse to IDs Invalid Test") {
        REQUIRE_THROWS(test_obj.ParseToIDs(NULL));
}
