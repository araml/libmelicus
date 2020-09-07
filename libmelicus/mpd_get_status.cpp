#include <cassert>
#include <dlfcn.h>

#include <mpd/tag.h>
#include <mpd/error.h>

#include <audio_status.hpp>

typedef struct mpd_connection* (*mpd_cn)(const char *, unsigned int, unsigned int);
mpd_cn mpd_connection_new;

typedef mpd_error (*mpd_cge) (const struct mpd_connection *connection);
mpd_cge mpd_connection_get_error;

typedef bool (*mpd_clb) (struct mpd_connection *connection, bool discrete_ok);
mpd_clb mpd_command_list_begin;

typedef bool (*mpd_cle) (struct mpd_connection *connection);
mpd_cle mpd_command_list_end;

typedef bool (*mpd_ss) (struct mpd_connection *connection);
mpd_ss mpd_send_status;

typedef struct mpd_status* (*mpd_rst) (struct mpd_connection *connection);
mpd_rst mpd_recv_status;

typedef bool (*mpd_scs) (struct mpd_connection *connection);
mpd_scs mpd_send_current_song;

typedef bool (*mpd_rn) (struct mpd_connection *connection);
mpd_rn mpd_response_next;

typedef struct mpd_song* (*mpd_rso) (struct mpd_connection *connection);
mpd_rso mpd_recv_song;

typedef  void (*mpd_sf) (struct mpd_song *song);
mpd_sf mpd_song_free;

typedef void (*mpd_stf) (struct mpd_status *status);
mpd_stf mpd_status_free;

typedef void (*mpd_cf) (struct mpd_connection *connection);
mpd_cf mpd_connection_free;

typedef const char * (* mpd_cgem) (const struct mpd_connection *connection);
mpd_cgem mpd_connection_get_error_message;

typedef const char * (*mpd_sgt) (const struct mpd_song *song, enum mpd_tag_type type, unsigned idx);
mpd_sgt mpd_song_get_tag;

typedef const char * (*mpd_sgu) (const struct mpd_song *song);
mpd_sgu mpd_song_get_uri;

typedef unsigned (*mpd_sgd) (const struct mpd_song *song);
mpd_sgd mpd_song_get_duration;

typedef unsigned (*mpd_sget) (const struct mpd_status *status);
mpd_sget mpd_status_get_elapsed_time;

static int handle_error(struct mpd_connection *c) {
    assert(mpd_connection_get_error(c) != MPD_ERROR_SUCCESS);

    fprintf(stderr, "%s\n", mpd_connection_get_error_message(c));
    mpd_connection_free(c);
    return EXIT_FAILURE;
}

// mpd can return a NULL if the tag doesn't exists so we return an empty str
#define ine(x) (x ? x : "")

bool mpd_on = false;
void *libmpdclient;

namespace melicus {
    int init_mpd() {
        libmpdclient = dlopen("libmpdclient.so", RTLD_LAZY);

        // If we can't load the .so file we set a flag letting know the user it
        // is not available
        if (!libmpdclient) {
            mpd_on = false;
            return 1;
        } else {
            mpd_on = true;
        }

        mpd_connection_new = (mpd_cn) dlsym(libmpdclient, "mpd_connection_new");
        mpd_connection_get_error = (mpd_cge) dlsym(libmpdclient, "mpd_connection_get_error");
        mpd_command_list_begin = (mpd_clb) dlsym(libmpdclient, "mpd_command_list_begin");
        mpd_command_list_end = (mpd_cle) dlsym(libmpdclient, "mpd_command_list_end");
        mpd_send_status = (mpd_ss) dlsym(libmpdclient, "mpd_send_status");
        mpd_recv_status = (mpd_rst) dlsym(libmpdclient, "mpd_recv_status");
        mpd_send_current_song = (mpd_scs) dlsym(libmpdclient, "mpd_send_current_song");
        mpd_response_next = (mpd_rn) dlsym(libmpdclient, "mpd_response_next");
        mpd_recv_song = (mpd_rso) dlsym(libmpdclient, "mpd_recv_song");
        mpd_song_free = (mpd_sf) dlsym(libmpdclient, "mpd_song_free");
        mpd_connection_get_error_message = (mpd_cgem) dlsym(libmpdclient, "mpd_connection_get_error_message");
        mpd_status_free = (mpd_stf) dlsym(libmpdclient, "mpd_status_free");
        mpd_connection_free = (mpd_cf) dlsym(libmpdclient, "mpd_connection_free");
        mpd_song_get_tag = (mpd_sgt) dlsym(libmpdclient, "mpd_song_get_tag");
        mpd_song_get_uri = (mpd_sgu) dlsym(libmpdclient, "mpd_song_get_uri");
        mpd_song_get_duration = (mpd_sgd) dlsym(libmpdclient, "mpd_song_get_duration");
        mpd_status_get_elapsed_time = (mpd_sget) dlsym(libmpdclient, "mpd_status_get_elapsed_time");

        return 0;
    }

    void close_mpd() {
        if (libmpdclient)
            dlclose(libmpdclient);
    }

    std::tuple<error_status, audio_status> get_mpd_status() {
        if (!mpd_on) {
            return {MPD_IS_NOT_LOADED, {}};
        }

        mpd_connection *conn = mpd_connection_new(NULL, 0, 0);

        if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
            handle_error(conn);
            return {OK, {}};
        }

        mpd_command_list_begin(conn, true);
        mpd_send_status(conn);
        mpd_send_current_song(conn);
        mpd_command_list_end(conn);

        mpd_status *status = mpd_recv_status(conn);
        if (status)
            mpd_response_next(conn);

        mpd_song *song = mpd_recv_song(conn);

        if (!status || !song) {
            std::cout << "Error getting status " << std::endl;
        }

        audio_status sdata;
        sdata.title = std::string(ine(mpd_song_get_tag(song, MPD_TAG_TITLE, 0)));
        sdata.artist = std::string(ine(mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)));
        sdata.album = std::string(ine(mpd_song_get_tag(song, MPD_TAG_ALBUM, 0)));
        sdata.date = std::string(ine(mpd_song_get_tag(song, MPD_TAG_DATE, 0)));
        sdata.genre = std::string(ine(mpd_song_get_tag(song, MPD_TAG_GENRE, 0)));
        sdata.comment = std::string(ine(mpd_song_get_tag(song, MPD_TAG_COMMENT, 0)));
        sdata.file_name = std::string(ine(mpd_song_get_uri(song)));
        sdata.track = ine(mpd_song_get_tag(song, MPD_TAG_TRACK, 0));
        sdata.duration = (int)mpd_song_get_duration(song);
        sdata.elapsed_time = (int)mpd_status_get_elapsed_time(status);


        mpd_song_free(song);
        mpd_status_free(status);
        mpd_connection_free(conn);
        return {OK, sdata};
    }
}
