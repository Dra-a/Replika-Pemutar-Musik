#ifndef SPOTIFY_H_INCLUDED
#define SPOTIFY_H_INCLUDED
#include<string>

struct userInfo{
    idUser int;
    nameUser string;
}

struct songInfo{
    idSong int;
    nameSong string;
    durasi int;
    times_played int;
    nameArtist string;
    idArtist int;
}

struct artist{
    idArtist int;
    nameArtist string;
}

#endif // SPOTIFY_H_INCLUDED