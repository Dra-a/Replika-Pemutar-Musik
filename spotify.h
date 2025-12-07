#ifndef SPOTIFY_H_INCLUDED
#define SPOTIFY_H_INCLUDED
#include <string>

struct time {
    int menit, detik;
}

// Tipe Struct Infotype untuk Elemen Linked List
struct song_info {
    string song_name, artist_name;
    time duration;
    int times_played;
};

struct playlist_info {
    string playlist_name;
    int playlist_size;
};

struct user_info {
    string user_name;
    int playlist_count;
    bool isAdmin;
};

// Tipe Address dan Struct Elemen Linked List
typedef struct songElement *songAddress;
struct songElement {
    song_info info;
    songAddress next, prev;
};

typedef struct playlistElement *playlistAddress;
struct playlistElement {
    playlist_info info;
    playlistAddress next_playlist;
    songAddress first_song;
};

typedef struct songElementMLL *songAddressMLL;
struct songElementMLL {
    songAddress song_pointer;
    songAddressMLL next_song, prev_song;
};

// Tipe List
struct Library {
    songAddress first, last;
};

struct Users {
    userAddress first;
    userAddress last;
};

struct Artists {
    playlistAddress first;
};

// Elemen User dapat mengakses library dan artists

typedef struct userElement *userAddress;
struct userElement {
    user_info info;
    userAddress next_user;
    playlistAddress first_playlist;
    Library library;
    Artists artists;
};

void createLibrary(Library &L);
void createArtists(Artists &A);
void createUsers(Users &U);

// Function dan Procedure Lagu
songAddress allocateSong(song_info info);
void addSong(Library &L, songAddress P);
void deleteSong(Library &L, string song_name, songAddress &P);
void editSong(Library &L, string song_name);
void displayLibrary(Library L);

songAddress findSong(Library L, string song_name);
void sortSong(Library &L);

// Function dan Procedure Playlist
playlistAddress allocatePlaylist(playlist_info info);
void addPlaylist(userAddress &P, playlistAddress Q);
void deletePlaylist(userAddress &P, string playlist_name, playlistAddress &Q);
void editPlaylist(userAddress &P, string playlist_name);
void displayPlaylists(userAddress P)

void displaySongsInPlaylist(playlistAddress P);
void addSongToPlaylist(playlistAddress &P, songAddress song);
void removeSongFromPlaylist(playlistAddress &P, string song_name);

// Function dan Procedure User
userAddress allocateUser(user_info info);
void addUser(Users &U, userAddress P);
userAddress findUser(Users U, int user_name);

// Function dan Procedure Fitur Tambahan
songAddress findSimilarSong(Library L, songAddress current_song);


#endif // SPOTIFY_H_INCLUDED
