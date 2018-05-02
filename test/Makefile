audio_test: audio_features_test.cpp ../src/audio_features.cpp ../src/audio_features.h ../data/audio_features.json
	g++ -Wall -std=c++11 audio_features_test.cpp ../src/audio_features.cpp -o audio_test -ljsoncpp
top_tracks_test: top_tracks_test.cpp ../src/top_tracks.cpp ../src/top_tracks.h ../src/audio_features.cpp ../src/audio_features.h ../data/disliked_songs_features.json ../data/liked_songs_features.json
	g++ -Wall -std=c++11 top_tracks_test.cpp ../src/top_tracks.cpp ../src/audio_features.cpp -o top_tracks_test -g -ljsoncpp
predictor_test: track_predictor_test.cpp ../src/track_predictor.h ../src/track_predictor.cpp ../src/top_tracks.cpp ../src/top_tracks.h ../src/audio_features.cpp ../src/audio_features.h ../data/disliked_songs_features.json ../data/liked_songs_features.json
	g++ -Wall -std=c++11 track_predictor_test.cpp ../src/track_predictor.cpp ../src/top_tracks.cpp ../src/audio_features.cpp -o predictor_test -g -ljsoncpp
clean:
	rm top_tracks_test predictor_test audio_test
