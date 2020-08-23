# libmelicus

libmelicus is a support library for [https://github.com/araml/melicus](https://github.com/araml/melicus)

It provides a standarized C and C++ API to query the status of several music players.

## Supported music players

- [x] cmus
- [x] mpd
- [ ] Moc
- [ ] Mpv
- [ ] mp3blaster
- [ ] DeadBeef
- [ ] Spotify

## Compilation

You can run the `build.sh` script or config cmake by hand enabling or disabling specific music players.

It supports any music player that uses **unix sockets** for communication.

If a music player uses a custom lib/dbus the support is **disabled** by default and must be enabled.

For example to enable mpd_support

`cmake -G Ninja -DMPD_SUPPORT=ON ..`

## Examples

You can check the examples folder for complete examples/test that it works with your selected music player

It is very easy to use:

```
melicus::audio_status st = get_cmus_status();
// do something with st
```

Will return an audio_status object with all the pertaining information (title, artist, etc. You can check the details in audio_status.hpp)

Alternatively the C api is very similar just with explicit memory deallocation

The shared object is located in /build/libmelicus/, for C include `audio_status.h` and for C++ include `audio_status.hpp`

```
audio_status *st = get_cmus_status();
// do something with st..
audio_status_free(st);
```
