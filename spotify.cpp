#include "spotify.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h>
using namespace std;

#ifdef _WIN32
SetConsoleOutputCP(CP_UTF8);
#endif

void createLibrary(Library &L){
    //Membuat List Library yang memiliki pointer first dan last
    //untuk menyimpan seluruh lagu kedalam satu tempat yang mudah di akses
    L.first = nullptr;
    L.last = nullptr;
}

void createArtists(Artists &A) {
    //Membuat List Artists yang memiliki pointer first
    //untuk meyinmpan playlist yang mengelompokkan lagu berdasarkan nama artist nya
    A.first = nullptr;
}

void createUsers(Users &U) {
    //Membuat List Users yang memiliki pointer first dan last
    //Untuk menyimpan informasi dari pengguna yang terdaftar
    U.first = nullptr;
    U.last = nullptr;
}

songAddress allocateSong(song_info info){
    //Menerima song_info yang berisi berbagai informasi dari suatu lagu
    //Mengembalikan songAddress sebagai pointer dari songElement untuk lagu tersebut
    songAddress P = new songElement;
    P->next = nullptr;
    P->prev = nullptr;
    P->info.song_name = info.song_name;
    P->info.artist_name = info.artist_name;
    P->info.duration = info.duration;
    P->info.times_played = info.times_played;

    return P;
}

userAddress allocateUser(user_info info){
    //Menerima user_info yang berisi berbagai informasi dari suatu user
    //Mengembalikan userAddress sebagai pointer dari userElement untuk user tersebut
    userAddress P = new userElement;
    P->next = nullptr;
    P->info.user_name = info.user_name;
    P->info.playlist_count = info.playlist_count;
    P->info.isAdmin = info.isAdmin;
    P->first_playlist = nullptr;

    return P;
}

relasiMLLAddress allocateRelasi(songAddress song) {
    relasiMLLAddress P = new relasiMLL;
    P->song_pointer = song;
    P->next = nullptr;
    P->prev = nullptr;

    return P;
}

void addSongToLibrary(Library &L, songAddress P){
    //Menambahkan songElement yang ditunjuk oleh P ke dalam List Library
    if (L.first == nullptr && L.last == nullptr){
        L.first = P;
        L.last = P;
    } else{
        L.last->next = P;
        P->prev = L.last;
        L.last = P;
    }
}

void AddSongToArtists(Artists &A, songAddress P) {
    //Menambahkan songElement yang ditunjuk oleh P ke dalam playlist yang ada di List Artists
    //berdasarkan nama artisnya
    string nama = P->info.artist_name;
    playlistAddress Q = A.first;
    while (Q != nullptr && Q->info.playlist_name != nama) {
        Q = Q->next;
    }

    //Sudah ada playlist dengan artist yang sama
    if (Q != nullptr) {
        addSongToPlaylist(Q, P);
    } else {
    //Belum ada playlist, buat baru menggunakan nama artist
        playlist_info info;
        info.playlist_name = nama;
        info.playlist_size = 0;
        playlistAddress PL = allocatePlaylist(info);

        addSongToPlaylist(PL, P);
    }
}

songAddress findSong(Library L, string song_name) {
    //Menelusuri Library untuk mencari songElement dengan song_name yang sama
    //Mengembalikan songAddress dari songElement tersebut, mengembalikan nullptr jika tidak ditemukan
    songAddress P = L.first;
    while (P != nullptr) {
        if (P->info.song_name == song_name) {
            return P;
        }
        P = P->next;
    }
    return nullptr;
}

void deleteSongFromLibrary(Library &L, songAddress &P) {
    //Setelah memilih suatu lagu di library, admin memiliki opsi untuk menghapusnya

    //Jika merupakan elemen pertama
    if (L.first == P) {
        L.first = P->next;
        P->next = nullptr;
        L.first->prev = nullptr;
    } else {
        songAddress Q = P->prev;
        Q->next = P->next;
        P->next->prev = Q;
        P->next = nullptr;
        P->prev = nullptr;

        //Jika merupakan elemen terakhir
        if (L.last == P) {
            L.last = Q;
        }
    }
}

void editSongFromLibrary(Library &L, songAddress &P) {
    //Setelah memilih suatu lagu di library, admin memiliki opsi untuk mengeditnya
    song_info newInfo;
    newInfo.song_name = P->info.song_name;
    newInfo.artist_name = P->info.artist_name;
    newInfo.duration = P->info.duration;
    newInfo.times_played = P->info.times_played;

    cout << "Informasi mana yang ingin diubah?" << endl;
    cout << "1. Nama Lagu" << endl;
    cout << "2. Nama Artist" << endl;
    cout << "3. Lama Durasi" << endl;
    cout << "4. Jumlah Play" << endl;
    cout << "Esc" << endl;
    int pilihan;
    cin >> pilihan;
    
    if (pilihan == 1) {
        cout << "Masukkan nama lagu yang baru" << endl;
        string nama;
        cin >> nama;
        newInfo.song_name = nama;

    } else if (pilihan == 2) {
        cout << "Masukkan nama artist yang baru" << endl;
        string nama;
        cin >> nama;
        newInfo.artist_name = nama;
    } else if (pilihan == 3) {
        cout << "Masukkan lama durasi yang baru" << endl;
        waktu waktu;
        cin >> waktu.menit >> waktu.detik;
        newInfo.duration = waktu;
    } else if (pilihan == 4) {
        cout << "Masukkan jumlah play yang baru" << endl;
        int jumlah;
        cin >> jumlah;
        newInfo.times_played = jumlah;
    }

    P->info.song_name = newInfo.song_name;
    P->info.artist_name = newInfo.artist_name;
    P->info.duration = newInfo.duration;
    P->info.times_played = newInfo.times_played;
}

void removeSongFromPlaylist(playlistAddress &P, songAddress song){
    //Menghapus lagu dari playlist user menggunakan elemen relasi MLL, kemudian mengurangi jumlah lagu dalam playlistnya dengan 1
    relasiMLLAddress R = P->first_song;
    while (R != nullptr && R->song_pointer != song){
        R = R->next;
    }
    if (R != nullptr){
        //Jika merupakan elemen pertama
        if (P->first_song == R){
            P->first_song = R->next;
            R->next = nullptr;
            if (P->first_song != nullptr){
                P->first_song->prev = nullptr;
            }
        } else{
            relasiMLLAddress Q = R->prev;
            Q->next = R->next;
            if (R->next != nullptr){
                R->next->prev = Q;
            }
            R->next = nullptr;
            R->prev = nullptr;
        }
        P->info.playlist_size -= 1;
    }
}


void addUser(Users &U, userAddress P){
    //Menambahkan userElement yang ditunjuk oleh userAddress P ke dalam List Users
    if (U.first == nullptr){
        U.first = P;
        U.last = P;
    } else{
        U.last->next = P;
        U.last = P;
    }
}

void addPlaylist(userAddress &P, playlistAddress Q){
    //Menambahkan playlistElement yang ditunjuk oleh playlistAddress Q ke dalam List Playlist yang dimiliki User Q
    //melalui pointer first_playlist
    if (P->first_playlist == nullptr){
        P->first_playlist = Q;
    } else{
        playlistAddress temp = P->first_playlist;
        while (temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = Q;
    }
}

void addSongToPlaylist(playlistAddress &P, songAddress song){
    //Menambahkan lagu ke playlist user menggunakan elemen relasi MLL, kemudian menambahkan jumlah lagu dalam playlistnya dengan 1
    relasiMLLAddress R = allocateRelasi(song);
    if (P->first_song == nullptr){
        P->first_song = R;
    } else{
        relasiMLLAddress temp = P->first_song;
        while (temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = R;
        R->prev = temp;
    }
    P->info.playlist_size += 1;
}

void deletePlaylist(userAddress &P, playlistAddress Q) {
    //Menghapus playlistElement yang ditunjuk oleh playlistAddress Q dari List Playlist milik User yang ditunjuk oleh userAddress P
    if (P->first_playlist == Q){
        P->first_playlist = Q->next;
        Q->next = nullptr;
    } else{
        playlistAddress temp = P->first_playlist;
        while (temp->next != Q){
            temp = temp->next;
        }
        temp->next = Q->next;
        Q->next = nullptr;
    }
}

userAddress findUser(Users U, string user_name){
    //Menelusuri List Users untuk mencari userElement dengan user_name yang sama
    //Mengembalikan userAddress dari userElement tersebut, mengembalikan nullptr jika tidak ditemukan
    userAddress P = U.first;
    while (P != nullptr){
        if (P->info.user_name == user_name){
            return P;
        }
        P = P->next;
    }
    return nullptr;
}

void centerText(const string text, int width) {
    //Mencetak teks yang di tengah-tengah dengan lebar tertentu
    int padding = width - text.length();
    int padLeft = padding / 2;
    int padRight = padding - padLeft;
    cout << setfill(' ') << setw(padLeft + text.length()) << right << text;
    cout << setw(padRight) << "" << left;
}

void displaySongInfo(songAddress P, int number) {
    //Menampilkan informasi dari suatu lagu dalam format yang rapi
    int maxTitleLength = 38;
    int maxArtistLength = 20;
    cout << "│ [" << number << "] ";
    cout << setfill(' ') << setw(maxTitleLength) << left << P->info.song_name << " │\n";
    cout << setfill(' ') << setw(maxArtistLength) << left << P->info.artist_name << " ";
    cout << setfill('0') << setw(2) << left << P->info.duration.menit << ":";
    cout << setfill('0') << setw(2) << left << P->info.duration.detik << "  │" << endl;
    cout << "│ · · · · · · · · · · · · · · · · · · · · ·  │" << endl;
}

int getLibrarySize(Library L) {
    //Menghitung jumlah lagu yang ada di dalam Library
    int size = 0;
    songAddress P = L.first;
    while (P != nullptr) {
        size++;
        P = P->next;
    }
    return size;
}

int getPlaylistSize(playlistAddress P) {
    //Menghitung jumlah lagu yang ada di dalam Playlist
    int size = 0;
    relasiMLLAddress R = P->first_song;
    while (R != nullptr) {
        size++;
        R = R->next;
    }
    return size;
}

void displayLibrary(Library L, int page, int n) {
    //Menampilkan seluruh lagu yang ada di dalam Library
    page = page - 1; //Mengubah halaman agar sesuai dengan index (dimulai dari 0)
    int start = page * n + 1;
    int end = page * n + n;
    int size = getLibrarySize(L);
    int totalPages = (size + n - 1) / n;
    songAddress p = L.first;
    int i;

    for (i=1; i<start; i++) {
        p = p->next;
    }

    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ [H]ome │            [F]ind Song            │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    for (i=start; i<=end; i++) {
        displaySongInfo(p, i%n);
        p = p->next;
    }
    cout << "│                                            │" << endl;
    cout << "│ [P]rev     Showing page " << page << " of " << totalPages << "     [N]ext  │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
}

void displaySongsInPlaylist(playlistAddress P, int page, int n) {
    //Menampilkan seluruh lagu yang ada di dalam Playlist
    page = page - 1; //Mengubah halaman agar sesuai dengan index (dimulai dari 0)
    int start = page * n + 1;
    int end = page * n + n;
    relasiMLLAddress R = P->first_song;
    int i;
        for (i=1; i<start; i++) {
        R = R->next;
    }

    for (i=start; i<=end; i++) {
        displaySongInfo(R->song_pointer, i%n);
        R = R->next;
    }
}

void homePage() {
    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│                  Home Page                 │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    cout << "│  [1]Library  |  [2]Artists  | [3]Playlists │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
}

void playFromPlaylist(playlistAddress P, int page, int n, int song_number, relasiMLLAddress &current, bool &isPlaying) {
    //Memainkan lagu yang ada di dalam Playlist
    //Menggunakan relasiMLLAddress R untuk menunjuk lagu yang sedang dimainkan dan boolean isPlaying untuk menandai status pemutaran lagu
    int song_position = (page - 1) * n + song_number;
    current = P->first_song;
    int i;
    for (i=1; i<song_position; i++) {
        current = current->next;
    }
    isPlaying = true;
}

relasiMLLAddress moveToSimilarSongs(userAddress U, songAddress current_song) {
    string artistName = current_song->info.artist_name;
    playlistAddress artistPlaylist = U->artists.first;
    while (artistPlaylist != U->first_playlist && artistPlaylist->info.playlist_name != artistName) {
        artistPlaylist = artistPlaylist->next;
    }
    if (artistPlaylist == U->first_playlist || artistPlaylist->info.playlist_size <= 1) {
        cout << "Tidak ditemukan lagu lain dari artist yang sama." << endl;
        return U->first_playlist->first_song; // Tidak ditemukan playlist artist atau hanya ada 1 lagu dari artist tersebut
    } else {
        relasiMLLAddress R = artistPlaylist->first_song;
        while (R != nullptr) {
            if (R->song_pointer == current_song) {
                return R;
            }
            R = R->next;
        }
        cout << "Terdapat error saat memutar lagu tersebut." << endl;
        return artistPlaylist->first_song; // Kembalikan lagu pertama dari playlist artist sebagai default, seharusnya tidak akan terjadi
    }
    
}

void playFromLibrary(Library L, int page, int n, userAddress U, int song_number, relasiMLLAddress &current, bool &isPlaying) {
    //Memainkan lagu yang ada di dalam Library
    int song_position = (page - 1) * n + song_number;
    songAddress P = L.first;
    int i;
    for (i=1; i<song_position; i++) {
        P = P->next;
    }
    isPlaying = true;
    current = moveToSimilarSongs(U, P);
}

void stopSong(bool &isPlaying) {
    //Menghentikan pemutaran lagu dengan mengubah status isPlaying menjadi false
    isPlaying = false;
}

void nextSong(relasiMLLAddress &current, bool &isPlaying) {
    //Memindahkan ke lagu berikutnya dalam playlist
    if (current->next != nullptr) {
        current = current->next;
    } else {
        cout << "Sudah di lagu terakhir dalam playlist." << endl;
        stopSong(isPlaying);
    }
}


void prevSong(relasiMLLAddress &current, bool &isPlaying) {
    //Memindahkan ke lagu sebelumnya dalam playlist
    if (current->prev != nullptr) {
        current = current->prev;
    } else {
        cout << "Sudah di lagu pertama dalam playlist." << endl;
    }
}

void nowPlays(bool isPlaying, int width) {
    if (isPlaying) {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│                 Now Playing                │" << endl;
        cout << "│";
        centerText("Home Page", width-2);
        cout << "│" << endl;
        cout << "├────────────────────────────────────────────┤" << endl;
        cout << "│    [P]rev    |    [S]top    |    [N]ext    │" << endl;
        cout << "└────────────────────────────────────────────┘" << endl;
    }
}