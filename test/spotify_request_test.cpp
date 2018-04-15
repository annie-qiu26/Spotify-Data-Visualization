#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "../src/spotify_request.h"
#include <iostream>

using namespace utility;

TEST_CASE("Build URI Test") {
	SpotifyRequest test;
	utility::string_t testURI = test.request_authentication();
	utility::string_t expectedURI = U("https://accounts.spotify.com/authorize/?client_id=8e8f59148afa40b08367c08ad922bf1f&response_type=code&redirect_uri=http://localhost:8888/callback&scope=playlist-read-private%20playlist-read-collaborative%20playlist-modify-public%20playlist-modify-private&show_dialog=false");
	REQUIRE(testURI == expectedURI);
}