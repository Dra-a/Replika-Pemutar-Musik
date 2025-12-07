#include "spotify.h"
#include <iostream>

void createLibrary(Library &L){
    L.first = nullptr;
    L.last = nullptr;
}

songAddress allocateSong(song_info info){
    songAddress P = new songElement;
    P->next = nullptr;
    P->prev = nullptr;
    P->info = info;
    return P;
}

void addSong(Library &L, songAddress P){
    if (L.first == nullptr && L.last == nullptr){
        L.first = P;
        L.last = P;
    } else{
        L.last->next = P;
        P->prev = L.last;
        L.last = P;
    }
}

void addUser(Users &U, userAddress P){
    if (U.first == nullptr){
        U.first = P;
    } else{
        P->prev = U.last;
        U.last->next = P;
        U.last = P;
    }
}

void addPlaylist(userAddress &P, playlistAddress Q){
    if (P->first_Playlist == nullptr){
        P->first_Playlist = Q;
    } else{
        playlistAddress temp = P->first_Playlist;
        while (temp->next != nullptr){
                temp = temp->next;
        }
        temp->next = Q;
        Q->prev = temp;
    }
}

void addSongToPlaylist(playlistAddress &P, songAddress song){
    if (P->first_song == nullptr){
        P->first_song = song;
    } else{
        songAddress temp = P->first_song;
        while (temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = song;
        song->prev = temp;
    }
}
