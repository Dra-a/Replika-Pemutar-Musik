#ifndef SPOTIFY_H_INCLUDED
#define SPOTIFY_H_INCLUDED

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct waktu {
    int menit, detik;
};

// Tipe Struct Infotype untuk Elemen Linked List
struct song_info {
    string song_name, artist_name;
    waktu duration;
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

typedef struct relasiMLL *relasiMLLAddress;
struct relasiMLL {
    songAddress song_pointer;
    relasiMLLAddress next, prev;
};

typedef struct playlistElement *playlistAddress;
struct playlistElement {
    playlist_info info;
    playlistAddress next;
    relasiMLLAddress first_song;
};

typedef struct userElement *userAddress;
struct userElement {
    user_info info;
    userAddress next;
    playlistAddress first_playlist;
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
void displayLibrary(Library L, int page, int n);
//void sortSong(Library &L);                                  //belum

// Function dan Procedure Playlist
playlistAddress allocatePlaylist(playlist_info info);
void addPlaylist(userAddress &P, playlistAddress Q);
void addArtist(Artists &A, playlistAddress P);
void deletePlaylist(userAddress &P, playlistAddress Q);
//void editPlaylist(userAddress &P, string playlist_name);                        //belum
void displayPlaylists(userAddress U, int page, int n);
void displayArtist(Artists P, int page, int n);

void displaySongsInPlaylist(playlistAddress P, int page, int n);
void addSongToPlaylist(playlistAddress &P, songAddress song);
void removeSongFromPlaylist(playlistAddress &P, songAddress song);

// Function dan Procedure User
userAddress allocateUser(user_info info);
void addUser(Users &U, userAddress P);
userAddress findUser(Users U, string user_name);

// Function dan Procedure Fitur Tambahan
relasiMLLAddress moveToSimilarSongs(Artists A, songAddress current_song);

// Menu dan keperluan tampilan
void centerText(const string text, int width);
void displaySongInfo(songAddress P, int number);
void homePage(userAddress U, int width);
int getLibrarySize(Library L);
int getPlaylistSize(playlistAddress P);
int getArtistsCount(Artists A);
int getPlaylistCount(userAddress U);
songAddress getSongFromLibrary(Library L, int page, int n, int song_number);
relasiMLLAddress getSongFromPlaylist(playlistAddress P, int page, int n, int song_number);
playlistAddress getPlaylistFromUser(userAddress U, int page, int n, int playlist_number);
playlistAddress getArtistPlaylistFromArtists(Artists A, int page, int n, int playlist_number);

void playFromPlaylist(playlistAddress P, int page, int n, int song_number, relasiMLLAddress &current, bool &isPlaying);
void playFromLibrary(Library L, int page, int n, Artists A, int song_number, relasiMLLAddress &current, bool &isPlaying);
void stopSong(bool &isPlaying);
void nextSong(relasiMLLAddress &current, bool &isPlaying);
void prevSong(relasiMLLAddress &current, bool &isPlaying);
void nowPlays(bool isPlaying, int width, relasiMLLAddress currentSong);
#endif // SPOTIFY_H_INCLUDED
