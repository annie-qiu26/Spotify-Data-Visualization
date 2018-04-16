# Development Logs
## Integrating Libraries, Parsing JSON data : 04/15/18
For the past two days, I've been trying to authenticate Spotify with HTTP request,
so far I haven't been able to accomplish this because the
[C++ Rest SDK's](https://github.com/Microsoft/cpprestsdk) documentation isn't
very specific on how to use their library. I was able to integrate this library
as well as [JsonCpp](https://github.com/open-source-parsers/jsoncpp) and
openFrameworks. I've been using [Spotify's API's](https://developer.spotify.com/web-api/)
web console to get JSON data that I can parse. While I'm still trying to figure
out how to make HTTP requests in C++, my next goal is to aggregate all the data
from the sample JSON files that I temporarily added to the repository.
