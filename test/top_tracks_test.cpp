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
ifstream infile2("../data/disliked_songs.json");
string file_contents((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
string file_contents2((istreambuf_iterator<char>(infile2)), istreambuf_iterator<char>());
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

TEST_CASE("Parse to IDs Playlist Size Test") {
        ids = test_obj.ParseToIDsPlaylist(file_contents2);
        REQUIRE(ids.size() == 100);
}

TEST_CASE("Parse to IDs Playlist Content Test") {
        REQUIRE(ids[0] == "5SGYanjZguKYg5PgVzFsvR");
        REQUIRE(ids[30] == "2gTYVoQCUh0QNUaFix01ld");
        REQUIRE(ids[99] == "1eLSF6HfrRA0AsNmTkUlKx");
}

TEST_CASE("Parse to IDs Playlist Invalid Test") {
        REQUIRE_THROWS(test_obj.ParseToIDsPlaylist(NULL));
}
