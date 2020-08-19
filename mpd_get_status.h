#ifndef MPD_GET_STATUS_H
#define MPD_GET_STATUS_H

#ifdef MPD_SUPPORT

#include <cassert>

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <mpd/message.h>

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <mpd/message.h>


#include <song_metadata.h>

static int handle_error(struct mpd_connection *c) {
    assert(mpd_connection_get_error(c) != MPD_ERROR_SUCCESS);

    fprintf(stderr, "%s\n", mpd_connection_get_error_message(c));
    mpd_connection_free(c);
    return EXIT_FAILURE;
}

// mpd can return a NULL if the tag doesn't exists so we return an empty str
#define ine(x) (x ? x : "")

song_metadata get_mpd_status() {
    mpd_connection *conn = mpd_connection_new(NULL, 0, 0);

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
        handle_error(conn);
        return song_metadata{};
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

    song_metadata sdata;
    sdata.title = std::string(ine(mpd_song_get_tag(song, MPD_TAG_TITLE, 0)));
    sdata.artist = std::string(ine(mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)));
    sdata.album = std::string(ine(mpd_song_get_tag(song, MPD_TAG_ALBUM, 0)));
    sdata.date = std::string(ine(mpd_song_get_tag(song, MPD_TAG_DATE, 0)));
    sdata.genre = std::string(ine(mpd_song_get_tag(song, MPD_TAG_GENRE, 0)));
    sdata.comment = std::string(ine(mpd_song_get_tag(song, MPD_TAG_COMMENT, 0)));
    sdata.file_name = std::string(ine(mpd_song_get_uri(song)));
    sdata.track = ine(mpd_song_get_tag(song, MPD_TAG_TRACK, 0));
    sdata.duration = mpd_song_get_duration(song);
    sdata.elapsed_time = mpd_status_get_elapsed_time(status);


    mpd_song_free(song);
    mpd_status_free(status);
    mpd_connection_free(conn);
    return sdata;
}

#endif // MPD_SUPPORT
#endif // MPD_GET_STATUS_H
