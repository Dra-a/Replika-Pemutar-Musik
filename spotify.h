#ifndef SPOTIFY_H_INCLUDED
#define SPOTIFY_H_INCLUDED
#include <string>
using namespace std;

struct waktu {
    int menit, detik;
};

// Tipe Struct Infotype untuk Elemen Linked List
struct song_info {
    string song_name, artist_name;
    waktu duration;
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
    playlistAddress next;
    relasiMLLAddress first_song;
};

typedef struct relasiMLL *relasiMLLAddress;
struct relasiMLL {
    songAddress song_pointer;
    relasiMLLAddress next, prev;
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
    userAddress next;
    playlistAddress first_playlist;
    Library library;
    Artists artists;
};

void createLibrary(Library &L);
void createArtists(Artists &A);
void createUsers(Users &U);

// Function dan Procedure Lagu
songAddress allocateSong(song_info info);
void addSongToLibrary(Library &L, songAddress P);
void AddSongToArtists(Artists &A, songAddress P);
songAddress findSong(Library L, string song_name);

void deleteSongFromLibrary(Library &L, songAddress &P);
void editSongFromLibrary(Library &L, songAddress &P);
void displayLibrary(Library L);                             //belum
void sortSong(Library &L);                                  //belum

// Function dan Procedure Playlist
playlistAddress allocatePlaylist(playlist_info info);       //belum
void addPlaylist(userAddress &P, playlistAddress Q);
void deletePlaylist(userAddress &P, playlistAddress Q);     
void editPlaylist(userAddress &P, string playlist_name);                        //belum
void displayPlaylists(userAddress P);                                           //belum
void displayArtist(Artists P);                                                  //belum

void displaySongsInPlaylist(playlistAddress P);                                 //belum
void addSongToPlaylist(playlistAddress &P, songAddress song);
void removeSongFromPlaylist(playlistAddress &P, songAddress song);

// Function dan Procedure User
userAddress allocateUser(user_info info);
void addUser(Users &U, userAddress P);
userAddress findUser(Users U, string user_name);          

// Function dan Procedure Fitur Tambahan
songAddress findSimilarSong(Library L, songAddress current_song);

#endif // SPOTIFY_H_INCLUDED
