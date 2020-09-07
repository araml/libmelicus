#ifndef ERROR_ENUM
#define ERROR_ENUM

const char err_strs[][64] = {
   "OK" ,
   "No song playing",
   "Service down",
   "Can't load mpd shared object",
   "Mpd isn't loaded"
};

enum error_status {
    OK = 0,
    NO_SONG_PLAYING,
    SERVICE_DOWN,
    NULL_STATUS,
    CANT_LOAD_MPD_DLL,
    MPD_IS_NOT_LOADED,
    CONN_ERROR,
};

#endif // ERROR_ENUM
