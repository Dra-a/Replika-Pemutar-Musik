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