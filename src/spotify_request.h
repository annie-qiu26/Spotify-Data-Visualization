#ifndef SPOTIFY_REQUESTS_H
#define SPOTIFY_REQUESTS_H

#include <string>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>

class SpotifyRequest {
public:
	utility::string_t request_authentication();

private: 
	std::string client_id = "8e8f59148afa40b08367c08ad922bf1f";
	std::string response_type = "code";
	std::string redirect_uri = "http://localhost:8888/callback";
	std::string scope = "playlist-read-private playlist-read-collaborative playlist-modify-public playlist-modify-private user-follow-modify user-follow-read user-library-read user-library-modify user-read-private user-read-birthdate user-read-email user-top-read ugc-image-upload user-read-playback-state user-modify-playback-state user-read-currently-playing user-read-recently-played";
	std::string state = "";
	std::string show_dialog = "false";
};

#endif