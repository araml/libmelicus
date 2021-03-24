# libmelicus

libmelicus is a support library for [https://github.com/araml/melicus](https://github.com/araml/melicus)

It provides a standarized C and C++ API to query the status of several music players.

## Supported music players

- [x] cmus
- [x] mpd
- [x] Moc (partial)
- [x] Mpv
- [ ] mp3blaster
- [x] DeadBeef (partial, missing song duration/elapsed times)
- [ ] ~~Spotify~~ (The terms of usage seem very bad)

## Compilation

You can run the `build.sh` script or if you wish run cmake by hand.

If a music player doesn't use unix sockets for IPC, like a custom lib or dbus then you need to initialize this **before** doing any function call (see examples/)

The shared object is located in /build/libmelicus/, for C include `audio_status.h` and for C++ include `audio_status.hpp`

## Examples

You can check the examples folder for complete examples/test that it works with your selected music player

Still the library is very easy to use:

```
melicus::audio_status st = get_cmus_status();
// do something with st
```

Will return an audio_status object with all the pertaining information (title, artist, etc. You can check the details in audio_status.hpp)

Alternatively the C api is very similar just with explicit memory allocation and deallocation

```
audio_status *st = melicus_create_status();
int err = cmus_status(st);
if (err) {
    // print error with melicus_perror or get error enum with
    // melicus_get_error_status()
} else {
    // do something with st..
}
audio_status_free(st);
```

### Extra libraries

As said before you need to initialize anything that doesn't use unix sockets by
default (you need to get the pertaining library in that case)

For example to use the mpd functions you need to first install libmpdclient and
then initialize it with:

`melicus_init(MPD_SUPPORT)`

