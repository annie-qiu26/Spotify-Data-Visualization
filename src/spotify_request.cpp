#include "spotify_request.h"

utility::string_t SpotifyRequest::request_authentication() {
	//utility::string_t  starting_uri_s("https://accounts.spotify.com/authorize/?");
	//web::uri starting_uri(starting_uri_s);
	web::uri_builder get_uri(U("https://accounts.spotify.com/authorize/?"));
	get_uri.append_query(U("client_id"), U("8e8f59148afa40b08367c08ad922bf1f"));
	get_uri.append_query(U("response_type"), U("code"));
	get_uri.append_query(U("redirect_uri"), U("http://localhost:8888/callback"));
	get_uri.append_query(U("scope"), U("playlist-read-private playlist-read-collaborative playlist-modify-public playlist-modify-private"));
	//get_uri.append_query(L"state", client_id);
	get_uri.append_query(U("show_dialog"), U("false"));
	utility::string_t uri_encoded = get_uri.to_string();
	return uri_encoded;
}