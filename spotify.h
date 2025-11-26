#ifndef SPOTIFY_H_INCLUDED
#define SPOTIFY_H_INCLUDED
#include <string>

struct time {
    int menit, detik;
}

// Tipe Struct Infotype untuk Elemen Linked List
struct song_info {
    int song_id, artist_id;
    string song_name, artist_name;
    time duration;
    int times_played;
};

struct playlist_info {
    int playlist_id;
    string playlist_name;
    int playlist_size;
};

struct artist_info {
    int artist_id;
    string artist_name;
    int song_count, listener;
};

struct user_info {
    int user_id;
    string user_name;
    int playlist_count;
    bool isAdmin;
    Library library_musik;
};

//Tipe Address dan Struct Elemen Linked List
typedef struct songElement *songAddress;
struct songElement {
    song_info info;
    songAddress next, prev;
};

typedef struct playlistElement *playlistAddress;
struct playlistElement {
    playlist_info info;
    playlistAddress next_playlist;
    songAddress next_song;
};

typedef struct artistElement *artistAddress;
struct artistElement {
    artist_info info;
    artistAddress next_artist;
    songAddress next_song;
};

typedef struct userElement *userAddress;
struct userElement {
    user_info info;
    userAddress next_user;
    playlistAddress next_playlist;
};

typedef struct songElementMLL *songAddressMLL;
struct songElementMLL {
    songAddress song_pointer;
    songAddressMLL next_song, prev_song;
};

//Tipe List
struct Library {
    songAddress first, last;
};

struct Users {
    userAddress first;
};

struct Artists {
    artistAddress first;
};

#endif // SPOTIFY_H_INCLUDED