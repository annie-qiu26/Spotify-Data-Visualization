#ifndef SPOTIFY_REQUESTS_H
#define SPOTIFY_REQUESTS_H

#include <string>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri_builder.h>

class SpotifyRequests {
	utility::string_t request_authentication();

private: 
	std::wstring client_id = L"8e8f59148afa40b08367c08ad922bf1f";
	std::wstring response_type = L"code";
	std::wstring redirect_uri = L"http://localhost:8888/callback";
	std::wstring scope = L"playlist-read-private playlist-read-collaborative playlist-modify-public playlist-modify-private user-follow-modify user-follow-read user-library-read user-library-modify user-read-private user-read-birthdate user-read-email user-top-read ugc-image-upload user-read-playback-state user-modify-playback-state user-read-currently-playing user-read-recently-played";
	std::wstring state = L"";
	std::wstring show_dialog = L"false";
};

#endif