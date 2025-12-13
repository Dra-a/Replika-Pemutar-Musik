#include "spotify.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

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

playlistAddress allocatePlaylist(playlist_info info){
    //Menerima playlist_info yang berisi berbagai informasi dari suatu playlist
    //Mengembalikan playlistAddress sebagai pointer dari playlistElement untuk playlist tersebut
    playlistAddress P = new playlistElement;
    P->next = nullptr;
    P->first_song = nullptr;
    P->info.playlist_name = info.playlist_name;
    P->info.playlist_size = info.playlist_size;

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

void addArtist(Artists &A, playlistAddress P){
    //Menambahkan playlistElement yang ditunjuk oleh P ke dalam List Artists
    if (A.first == nullptr){
        A.first = P;
    } else{
        playlistAddress temp = A.first;
        while (temp->next != nullptr){
            temp = temp->next;
        }
        temp->next = P;
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
        addArtist(A, PL);

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
        cin >> newInfo.song_name;

    } else if (pilihan == 2) {
        cout << "Masukkan nama artist yang baru" << endl;
        cin >> newInfo.artist_name;
    } else if (pilihan == 3) {
        cout << "Masukkan lama durasi yang baru" << endl;
        waktu waktu;
        cin >> waktu.menit >> waktu.detik;
        newInfo.duration = waktu;
    }

    P->info.song_name = newInfo.song_name;
    P->info.artist_name = newInfo.artist_name;
    P->info.duration = newInfo.duration;
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
        P->info.playlist_size += 1;
    } else{
        relasiMLLAddress temp = P->first_song;
        while (temp->next != nullptr && temp->song_pointer != song){
            temp = temp->next;
        }
        if (temp->next == nullptr && temp->song_pointer != song){
            temp->next = R;
            R->prev = temp;
            P->info.playlist_size += 1;
        } else {
            cout << "Lagu sudah ada di dalam playlist!" << endl;
        }
    }
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
    int maxArtistLength = 32;
    cout << "│ [" << number << "] ";
    cout << setfill(' ') << setw(maxTitleLength) << left << P->info.song_name << " │\n";
    cout << "│     ";
    cout << setfill(' ') << setw(maxArtistLength) << left << P->info.artist_name;
    cout << setfill('0') << setw(2) << right << P->info.duration.menit << ":";
    cout << setfill('0') << setw(2) << right << P->info.duration.detik << "  │" << endl;
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

int getArtistsCount(Artists A) {
    //Menghitung jumlah playlist artist yang ada di dalam Artists
    int size = 0;
    playlistAddress P = A.first;
    while (P != nullptr) {
        size++;
        P = P->next;
    }
    return size;
}

int getPlaylistCount(userAddress U) {
    //Menghitung jumlah playlist yang dimiliki oleh User
    int size = 0;
    playlistAddress P = U->first_playlist;
    while (P != nullptr) {
        size++;
        P = P->next;
    }
    return size;
}

void displayLibrary(Library L, int page, int n) {
    //Menampilkan seluruh lagu yang ada di dalam Library
    //Mengubah halaman agar sesuai dengan index (dimulai dari 0)
    int start = (page-1) * n + 1;
    int end = (page-1) * n + n;
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
    i = start;
    while (i <= end && p != nullptr) {
        displaySongInfo(p, i%(n));
        p = p->next;
        i = i + 1;
    }
    cout << "│                                            │" << endl;
    cout << "│ [P]rev     Showing page " << page << " of " << totalPages << "     [N]ext  │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
}

songAddress getSongFromLibrary(Library L, int page, int n, int song_number) {
    //Mengambil songAddress dari lagu yang ada di dalam Library berdasarkan nomor lagu pada halaman tertentu
    int song_position = (page - 1) * n + song_number;
    songAddress P = L.first;
    int i;
    for (i=1; i<song_position; i++) {
        P = P->next;
    }
    return P;
}

relasiMLLAddress getSongFromPlaylist(playlistAddress P, int page, int n, int song_number) {
    //Mengambil relasiMLLAddress dari lagu yang ada di dalam Playlist berdasarkan nomor lagu pada halaman tertentu
    int song_position = (page - 1) * n + song_number;
    relasiMLLAddress R = P->first_song;
    int i;
    for (i=1; i<song_position; i++) {
        R = R->next;
    }
    return R;
}

playlistAddress getPlaylistFromUser(userAddress U, int page, int n, int playlist_number) {
    //Mengambil playlistAddress dari playlist yang dimiliki oleh User berdasarkan nomor playlist pada halaman tertentu
    int playlist_position = (page - 1) * n + playlist_number;
    playlistAddress P = U->first_playlist;
    int i;
    for (i=1; i<playlist_position; i++) {
        P = P->next;
    }
    return P;
}

playlistAddress getArtistPlaylistFromArtists(Artists A, int page, int n, int playlist_number) {
    //Mengambil playlistAddress dari playlist artist yang ada di dalam Artists berdasarkan nomor playlist pada halaman tertentu
    int playlist_position = (page - 1) * n + playlist_number;
    playlistAddress P = A.first;
    int i;
    for (i=1; i<playlist_position; i++) {
        P = P->next;
    }
    return P;
}

void displaySongsInPlaylist(playlistAddress P, int page, int n) {
    //Menampilkan seluruh lagu yang ada di dalam Playlist
    //Mengubah halaman agar sesuai dengan index (dimulai dari 0)
    int start = (page-1) * n + 1;
    int end = (page-1) * n + n;
    int totalPages = (P->info.playlist_size + n - 1) / n;
    relasiMLLAddress R = P->first_song;
    int i;
        for (i=1; i<start; i++) {
        R = R->next;
    }

    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ [H]ome │";
    centerText(P->info.playlist_name, 35);
    cout << "│" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    i = start;
    while (i <= end && R != nullptr) {
        displaySongInfo(R->song_pointer, i%(n));
        R = R->next;
        i = i + 1;
    }
    cout << "│                                            │" << endl;
    cout << "│ [P]rev     Showing page " << page << " of " << totalPages << "     [N]ext  │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
}

void homePage(userAddress U, int width) {
    //Menampilkan halaman utama setelah user berhasil login
    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│";
    centerText("Welcome, " + U->info.user_name, width);
    cout << "│" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ [L]og out │            Home Page           │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    cout << "│  [1]Library  |  [2]Artists  | [3]Playlists │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
}

void playFromPlaylist(playlistAddress P, int page, int n, int song_number, relasiMLLAddress &current, bool &isPlaying) {
    //Memainkan lagu yang ada di dalam Playlist
    //Menggunakan relasiMLLAddress R untuk menunjuk lagu yang sedang dimainkan dan boolean isPlaying untuk menandai status pemutaran lagu
    current = getSongFromPlaylist(P, page, n, song_number);
    isPlaying = true;
}

relasiMLLAddress moveToSimilarSongs(Artists A, songAddress current_song) {
    string artistName = current_song->info.artist_name;
    playlistAddress artistPlaylist = A.first;
    while (artistPlaylist != nullptr && artistPlaylist->info.playlist_name != artistName) {
        artistPlaylist = artistPlaylist->next;
    }
    if (artistPlaylist == nullptr || artistPlaylist->info.playlist_size <= 1) {
        cout << "Tidak ditemukan lagu lain dari artist yang sama." << endl;
        return A.first->first_song; // Tidak ditemukan playlist artist atau hanya ada 1 lagu dari artist tersebut
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

void playFromLibrary(Library L, int page, int n, Artists A, int song_number, relasiMLLAddress &current, bool &isPlaying) {
    //Memainkan lagu yang ada di dalam Library
    songAddress P = getSongFromLibrary(L, page, n, song_number);
    isPlaying = true;
    current = moveToSimilarSongs(A, P);
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

void nowPlays(bool isPlaying, int width, relasiMLLAddress currentSong) {
    if (isPlaying) {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│                Now Playing                 │" << endl;
        cout << "│";
        centerText(currentSong->song_pointer->info.song_name, width);
        cout << "│" << endl;
        cout << "├────────────────────────────────────────────┤" << endl;
        cout << "│    [p]rev    |    [s]top    |    [n]ext    │" << endl;
        cout << "└────────────────────────────────────────────┘" << endl;
    }
}

void displayPlayListInfo(playlistAddress P, int number) {
    //Menampilkan informasi dari suatu playlist dalam format yang rapi
    int maxTitleLength = 38;

    cout << "│ [" << number << "] ";
    cout << setfill(' ') << setw(maxTitleLength) << left << P->info.playlist_name << " │\n";
    cout << "│     " << setfill('0') << setw(2) << right << P->info.playlist_size << " Songs";
    cout << setfill(' ') << setw(34) << right << "│" << endl;
    cout << "│ · · · · · · · · · · · · · · · · · · · · ·  │" << endl;
}

void displayArtist(Artists A, int page, int n) {
    //Menampilkan seluruh playlist artist yang ada di dalam Artists
    //Mengubah halaman agar sesuai dengan index (dimulai dari 0)
    int start = (page-1) * n + 1;
    int end = (page-1) * n + n;
    int size = getArtistsCount(A);
    int totalPages = (size + n - 1) / n;

    playlistAddress P = A.first;
    int i;
    for (i=1; i<start; i++) {
        P = P->next;
    }

    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ [H]ome │              Artists              │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    i = start;
    while (i <= end && P != nullptr) {
        displayPlayListInfo(P, i%(n));
        P = P->next;
        i = i + 1;
    }
    cout << "│                                            │" << endl;
    cout << "│ [P]rev     Showing page " << page << " of " << totalPages << "     [N]ext  │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
}

void displayPlaylists(userAddress pU, int page, int n) {
    //Menampilkan seluruh playlist yang ada di dalam Playlist milik User
    //Mengubah halaman agar sesuai dengan index (dimulai dari 0)
    int start = (page-1) * n + 1;
    int end = (page-1) * n + n;
    int size = getPlaylistCount(pU);
    int totalPages = (size + n - 1) / n;

    playlistAddress P = pU->first_playlist;
    int i;
    for (i=1; i<start; i++) {
        P = P->next;
    }

    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ [H]ome │           Your Playlists          │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    i = start;
    while (i <= end && P != nullptr) {
        displayPlayListInfo(P, i%(n));
        P = P->next;
        i = i + 1;
    }
    cout << "│                                            │" << endl;
    cout << "│ [P]rev     Showing page " << page << " of " << totalPages << "     [N]ext  │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
}
