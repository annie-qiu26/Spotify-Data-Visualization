#include "spotify_request.h"

utility::string_t SpotifyRequests::request_authentication() {
	web::uri_builder get_uri(L"https://accounts.spotify.com/authorize/?");
	get_uri.append_query(L"client_id", client_id);
	get_uri.append_query(L"response_type", response_type);
	get_uri.append_query(L"redirect_uri", redirect_uri);
	get_uri.append_query(L"scope", scope);
	//get_uri.append_query(L"state", client_id);
	get_uri.append_query(L"show_dialog", show_dialog);
	utility::string_t uri_encoded = get_uri.to_string();
	return uri_encoded;
}