audio_test: audio_features_test.cpp ../src/audio_features.cpp ../src/audio_features.h
	g++ -Wall -std=c++11 audio_features_test.cpp ../src/audio_features.cpp -o audio_test -lboost_system -lcrypto -lssl -lcpprest -ljsoncpp
request_test: spotify_request_test.cpp ../src/spotify_request.cpp ../src/spotify_request.h
	g++ -Wall -std=c++11 ../jsoncpp/jsoncpp.cpp spotify_request_test.cpp ../src/spotify_request.cpp -o request_test -lboost_system -lcrypto -lssl -lcpprest
top_tracks_test: top_tracks_test.cpp ../src/top_tracks.cpp ../src/top_tracks.h ../src/audio_features.cpp ../src/audio_features.h
	g++ -Wall -std=c++11 top_tracks_test.cpp ../src/top_tracks.cpp ../src/audio_features.cpp -o top_tracks_test -lboost_system -lcrypto -lssl -lcpprest -ljsoncpp
