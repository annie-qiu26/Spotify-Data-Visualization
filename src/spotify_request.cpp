#include "spotify_request.h"

utility::string_t SpotifyRequest::request_authentication() {
	web::uri_builder get_uri(U("https://accounts.spotify.com/authorize/?"));
	get_uri.append_query(U("client_id"), client_id);
	get_uri.append_query(U("response_type"), response_type);
	get_uri.append_query(U("redirect_uri"), redirect_uri);
	get_uri.append_query(U("scope"), scope);
	get_uri.append_query(U("state"), state);
	get_uri.append_query(U("show_dialog"), show_dialog);
	utility::string_t uri_encoded = get_uri.to_string();
	return uri_encoded;
}