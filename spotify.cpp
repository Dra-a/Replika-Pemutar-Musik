#include "spotify.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

void clearScreen() {
    // \033[2J = Clear seluruh layar
    // \033[1;1H = Pindahkan kursor ke pojok kiri atas
    cout << "\033[2J\033[1;1H";
}

void inputMessage() {
    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ Ketik karakter di dalam [ ] untuk memilih. │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
    cout << "» ";
}

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
    playlist_info pl_info;
    pl_info.playlist_name = "Favorites";
    pl_info.playlist_size = 0;
    playlistAddress PL = allocatePlaylist(pl_info);

    userAddress P = new userElement;
    P->next = nullptr;
    P->info.user_name = info.user_name;
    P->info.playlist_count = info.playlist_count;
    P->info.isAdmin = info.isAdmin;
    P->first_playlist = PL;

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
    clearScreen();
    cout << "Lagu " << P->info.song_name << " berhasil dihapus dari library!" << endl;
}

void editSongFromLibrary(Library &L, songAddress &P) {
    //Setelah memilih suatu lagu di library, admin memiliki opsi untuk mengeditnya
    song_info newInfo;
    newInfo.song_name = P->info.song_name;
    newInfo.artist_name = P->info.artist_name;
    newInfo.duration = P->info.duration;

    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ Info lagu mana yang ingin diubah?          │ " << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    cout << "│ [1] Nama Lagu                              │" << endl;
    cout << "│ [2] Nama Artist                            │" << endl;
    cout << "│ [3] Lama Durasi                            │" << endl;
    cout << "│ [B] Back                                   │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
    inputMessage();
    string pilihan;
    cin >> pilihan;
    clearScreen();
    if (pilihan == "1") {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│             Mengubah Nama Lagu             │ " << endl;
        cout << "└────────────────────────────────────────────┘" << endl;
        cout << "Masukkan nama lagu yang baru: " << endl;
        cin >> newInfo.song_name;

    } else if (pilihan == "2") {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│            Mengubah Nama Artist            │ " << endl;
        cout << "└────────────────────────────────────────────┘" << endl;
        cout << "Masukkan nama artist yang baru: " << endl;
        cin >> newInfo.artist_name;

    } else if (pilihan == "3") {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│            Mengubah Lama Durasi            │ " << endl;
        cout << "└────────────────────────────────────────────┘" << endl;
        cout << "Masukkan lama durasi yang baru:" << endl;
        cout << "Menit: ";
        cin >> newInfo.duration.menit;
        cout << "Detik: ";
        cin >> newInfo.duration.detik;
    }

    P->info.song_name = newInfo.song_name;
    P->info.artist_name = newInfo.artist_name;
    P->info.duration = newInfo.duration;

    if (pilihan != "B") {
        clearScreen();
        cout << "Informasi lagu berhasil diedit!" << endl;
    }
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
    bool isAdded = true;
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
            isAdded = false;
        }
    }
    if (isAdded) {
        cout << "Lagu berhasil ditambahkan ke playlist " << P->info.playlist_name << "!" << endl;
    } else {
        cout << "Lagu sudah ada di dalam playlist!" << endl;
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
    if (number == 0) {
        number = 5;
    }
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
    cout << "│ [B]ack │            [F]ind Song            │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    i = start;
    while (i <= end && p != nullptr) {
        displaySongInfo(p, i%(n));
        p = p->next;
        i = i + 1;
    }
    cout << "│                                            │" << endl;
    cout << "│ [<]Prev     Showing page " << page << " of " << totalPages << "    Next[>] │" << endl;
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

void displaySongsInPlaylist(playlistAddress P, int page, int n, bool showEdit) {
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
    if (showEdit) {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│ [B]ack │";
        centerText(P->info.playlist_name, 26);
        cout << "│ [E]dit │" << endl;
    } else {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│ [B]ack │";
        centerText(P->info.playlist_name, 35);
        cout << "│" << endl;
    }
    cout << "├────────────────────────────────────────────┤" << endl;
    i = start;
    while (i <= end && R != nullptr) {
        displaySongInfo(R->song_pointer, i%(n));
        R = R->next;
        i = i + 1;
    }
    cout << "│                                            │" << endl;
    cout << "│ [<]Prev     Showing page " << page << " of " << totalPages << "    Next[>] │" << endl;
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

void playFromPlaylist(relasiMLLAddress &currentSong, relasiMLLAddress selectedSong, bool &isPlaying) {
    //Memainkan lagu yang ada di dalam Playlist
    //Menggunakan relasiMLLAddress R untuk menunjuk lagu yang sedang dimainkan dan boolean isPlaying untuk menandai status pemutaran lagu
    if (selectedSong != nullptr) {
        isPlaying = true;
        currentSong = selectedSong;
    }
}

relasiMLLAddress moveToSimilarSongs(Artists A, songAddress current_song) {
    string artistName = current_song->info.artist_name;
    playlistAddress artistPlaylist = A.first;
    while (artistPlaylist != nullptr && artistPlaylist->info.playlist_name != artistName) {
        artistPlaylist = artistPlaylist->next;
    }
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

void playFromLibrary(Artists A, songAddress selectedSong, relasiMLLAddress &current, bool &isPlaying) {
    //Memainkan lagu yang ada di dalam Library
    if (selectedSong != nullptr) {
        isPlaying = true;
        current = moveToSimilarSongs(A, selectedSong);
    }
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
        cout << "Kembali ke awal playlist." << endl;
        while (current->prev != nullptr) {
            current = current->prev;
        }
    }
}


void prevSong(relasiMLLAddress &current, bool &isPlaying) {
    //Memindahkan ke lagu sebelumnya dalam playlist
    if (current->prev != nullptr) {
        current = current->prev;
    } else {
        cout << "Sudah di lagu pertama dalam playlist." << endl;
        cout << "Tetap berada di lagu yang sama." << endl;
    }
}

void nowPlays(bool isPlaying, int width, relasiMLLAddress currentSong) {
    if (isPlaying) {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│                Now Playing                 │" << endl;
        cout << "│";
        centerText(currentSong->song_pointer->info.song_name, width);
        cout << "│" << endl;
        cout << "│";
        centerText(currentSong->song_pointer->info.artist_name, width);
        cout << "│" << endl;
        cout << "├────────────────────────────────────────────┤" << endl;
        cout << "│    [P]rev    |    [S]top    |    [N]ext    │" << endl;
        cout << "└────────────────────────────────────────────┘" << endl;
    }
}

void displayPlayListInfo(playlistAddress P, int number) {
    //Menampilkan informasi dari suatu playlist dalam format yang rapi
    if (number == 0) {
        number = 5;
    }
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
    cout << "│ [B]ack │              Artists              │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    i = start;
    while (i <= end && P != nullptr) {
        displayPlayListInfo(P, i%(n));
        P = P->next;
        i = i + 1;
    }
    cout << "│                                            │" << endl;
    cout << "│ [<]Prev     Showing page " << page << " of " << totalPages << "    Next[>] │" << endl;
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
    cout << "│ [B]ack │           Your Playlists          │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    i = start;
    while (i <= end && P != nullptr) {
        displayPlayListInfo(P, i%(n));
        P = P->next;
        i = i + 1;
    }
    cout << "│                                            │" << endl;
    cout << "│ [<]Prev     Showing page " << page << " of " << totalPages << "    Next[>] │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
}

void prev_page(int &page) {
    if (page > 1) {
        page--;
    }
}

void next_page(int &page, int totalItems, int itemsPerPage) {
    int totalPages = (totalItems + itemsPerPage - 1) / itemsPerPage;
    if (page < totalPages) {
        page++;
    }
}

void adminActionHandler(string input, Library &L, Users &U, Artists &A, songAddress &selectedSong) {
    if (input == "1") {
        editSongFromLibrary(L, selectedSong);
    } else if (input == "2") {
        // Hapus dari library
        deleteSongFromLibrary(L, selectedSong);
        // Hapus dari playlist user
        userAddress currentUser = U.first;
        while (currentUser != nullptr) {
            playlistAddress currentPlaylist = currentUser->first_playlist;
            while (currentPlaylist != nullptr) {
                removeSongFromPlaylist(currentPlaylist, selectedSong);
                currentPlaylist = currentPlaylist->next;
            }
            currentUser = currentUser->next;
        }
        // Hapus dari playlist artist
        playlistAddress P = A.first;
        while (P != nullptr) {
            removeSongFromPlaylist(P, selectedSong);
            P = P->next;
        }

    } else {
        clearScreen();
    }
}

void adminMenuHandler(string input, Library &L, userAddress &currentUser, Artists &A, Users &U, int contentPerPage) {
    int page = 1;
    if (input == "L") {
        currentUser = nullptr;

    } else if (input == "1") {
        string pilihanLibrary = "";
        clearScreen();
        while (pilihanLibrary != "B") {
            displayLibrary(L, page, contentPerPage);
            cin >> pilihanLibrary;
            if (pilihanLibrary >= "1" && pilihanLibrary <= "5") {
                int song_number = stoi(pilihanLibrary);
                songAddress selectedSong = getSongFromLibrary(L, page, contentPerPage, song_number);
                clearScreen();
                cout << "┌────────────────────────────────────────────┐" << endl;
                cout << "│ Anda memilih lagu:                         │" << endl;
                cout << "│   " << setfill(' ') << setw(41) << left << selectedSong->info.song_name << "│" << endl;
                cout << "│   " << setfill(' ') << setw(41) << left << "by" + selectedSong->info.artist_name << "│" << endl;
                cout << "├────────────────────────────────────────────┤" << endl;
                cout << "│ [1] Edit Lagu                              │" << endl;
                cout << "│ [2] Hapus Lagu                             │" << endl;
                cout << "│ [B] Back                                   │" << endl;
                cout << "└────────────────────────────────────────────┘" << endl;
                inputMessage();
                string aksiAdmin;
                cin >> aksiAdmin;
                adminActionHandler(aksiAdmin, L, U, A, selectedSong);
            } else if (pilihanLibrary == "<") {
                prev_page(page);
                clearScreen();
            } else if (pilihanLibrary == ">") {
                int totalSongs = getLibrarySize(L);
                int totalPages = (totalSongs + contentPerPage - 1) / contentPerPage;
                next_page(page, totalSongs, contentPerPage);
                clearScreen();
            } else if (pilihanLibrary == "F") {
                //Fungsi mencari lagu berdasarkan nama lagu
                string songName;
                cout << "Masukkan nama lagu yang ingin dicari: ";
                cin >> songName;
                songAddress foundSong = findSong(L, songName);
                if (foundSong != nullptr) {
                    cout << "Lagu ditemukan: " << foundSong->info.song_name << " oleh " << foundSong->info.artist_name << endl;
                    cout << "Pilih aksi: [1] Edit Song  [2] Delete Song  [B]ack" << endl;
                    string aksiAdmin;
                    cin >> aksiAdmin;
                    adminActionHandler(aksiAdmin, L, U, A, foundSong);
                } else {
                    cout << "Lagu dengan nama tersebut tidak ditemukan di library." << endl;
                }
            }
        }

    } else if (input == "2") {
        song_info info;
        cout << "Masukkan nama lagu: ";
        cin >> info.song_name;
        cout << "Masukkan nama artist: ";
        cin >> info.artist_name;
        cout << "Masukkan durasi (menit): ";
        cin >> info.duration.menit;
        cout << "Masukkan durasi (detik): ";
        cin >> info.duration.detik;
        songAddress P = allocateSong(info);
        addSongToLibrary(L, P);
        AddSongToArtists(A, P);
    }
}

void userActionHandlerLibrary(string input, Library L, Artists &A, userAddress currentUser, int page, int contentPerPage, int song_number, relasiMLLAddress &currentSong, bool &isPlaying, songAddress selectedSong) {
    clearScreen();
    if (input == "1") {
        playFromLibrary(A, selectedSong, currentSong, isPlaying);
        clearScreen();
    } else if (input == "2") {
        int page = 1;
        string pilihanPlayList = "";
        while (pilihanPlayList != "B") {
            cout << "Pilih playlist untuk ditambahkan lagu:" << endl;
            displayPlaylists(currentUser, page, contentPerPage);
            cout << "┌────────────────────────────────────────────┐" << endl;
            cout << "│ [+] Tambahkan ke playlist baru             │" << endl;
            cout << "└────────────────────────────────────────────┘" << endl;
            inputMessage();
            cin >> pilihanPlayList;
            clearScreen();

            if (pilihanPlayList >= "1" && pilihanPlayList <= "5") {
                int playlist_number = stoi(pilihanPlayList);
                playlistAddress selectedPlaylist = getPlaylistFromUser(currentUser, page, contentPerPage, playlist_number);
                addSongToPlaylist(selectedPlaylist, selectedSong);

            } else if (pilihanPlayList == "<") {
                prev_page(page);
                clearScreen();

            } else if (pilihanPlayList == ">") {
                int totalPlaylists = getPlaylistCount(currentUser);
                next_page(page, totalPlaylists, contentPerPage);
                clearScreen();

            } else if (pilihanPlayList == "+") {
                playlist_info newPlaylistInfo;
                cout << "Masukkan nama playlist baru: ";
                cin >> newPlaylistInfo.playlist_name;
                newPlaylistInfo.playlist_size = 0;
                clearScreen();

                playlistAddress newPlaylist = allocatePlaylist(newPlaylistInfo);
                addPlaylist(currentUser, newPlaylist);
                addSongToPlaylist(newPlaylist, selectedSong);
                cout << "Playlist " << newPlaylistInfo.playlist_name << " berhasil dibuat dan lagu ditambahkan!" << endl;
                pilihanPlayList = "B";
            }
        }
    } else if (input == "3") {
        addSongToPlaylist(currentUser->first_playlist, selectedSong);
    }
}

void userActionHandlerPlaylist(string input, playlistAddress &selectedPlaylist, relasiMLLAddress &currentSong, relasiMLLAddress &selectedSong, bool &isPlaying) {
    int page = 1;
    clearScreen();
    if (input == "1") {
        playFromPlaylist(currentSong, selectedSong, isPlaying);
    } else if (input == "2") {
        removeSongFromPlaylist(selectedPlaylist, selectedSong->song_pointer);
    }
}

void userPickSongLibrary(string input, Library L, int page, int contentPerPage, int &song_number, songAddress &selectedSong) {
    song_number = stoi(input);
    selectedSong = getSongFromLibrary(L, page, contentPerPage, song_number);
    clearScreen();
    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ Anda memilih lagu:                         │" << endl;
    cout << "│   " << setfill(' ') << setw(41) << left << selectedSong->info.song_name << "│" << endl;
    cout << "│   " << setfill(' ') << setw(41) << left << "by" + selectedSong->info.artist_name << "│" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    cout << "│ [1] Play lagu                              │" << endl;
    cout << "│ [2] Tambahkan lagu ke playlist             │" << endl;
    cout << "│ [3] Tambahkan lagu ke favorit              │" << endl;
    cout << "│ [B] Back                                   │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
    inputMessage();
}

void userPickSongPlaylist(string input, playlistAddress selectedPlaylist, int contentPerPage, int &song_number, relasiMLLAddress &selectedSong) {
    int page = 1;
    song_number = stoi(input);
    selectedSong = getSongFromPlaylist(selectedPlaylist, page, contentPerPage, song_number);
    clearScreen();
    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ Anda memilih lagu:                         │" << endl;
    cout << "│   " << setfill(' ') << setw(41) << left << selectedSong->song_pointer->info.song_name << "│" << endl;
    cout << "│   " << setfill(' ') << setw(41) << left << "by" + selectedSong->song_pointer->info.artist_name << "│" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    cout << "│ [1] Play lagu                              │" << endl;
    cout << "│ [2] Hapus lagu dari playlist               │" << endl;
    cout << "│ [B] Back                                   │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
    inputMessage();
}

void nowPlaysHandler(string input, relasiMLLAddress &currentSong, bool &isPlaying) {
    if (input == "P") {
        clearScreen();
        prevSong(currentSong, isPlaying);
    } else if (input == "S") {
        clearScreen();
        stopSong(isPlaying);
    } else if (input == "N") {
        clearScreen();
        nextSong(currentSong, isPlaying);
    }
}

void userHomePageHandler(string input, userAddress &currentUser, relasiMLLAddress &currentSong, bool &isPlaying, Library &L, Artists &A, int contentPerPage, int box_width) {
    int page = 1;
    clearScreen();
    if (input == "L") {
        currentUser = nullptr;
        isPlaying = false;

    } else if (input == "1") {
        // Library
        string pilihanLibrary = "";
        clearScreen();
        while (pilihanLibrary != "B") {
            int song_number;
            songAddress selectedSong;
            displayLibrary(L, page, contentPerPage);
            nowPlays(isPlaying, box_width, currentSong);
            cin >> pilihanLibrary;

            if (pilihanLibrary >= "1" && pilihanLibrary <= "5") {
                userPickSongLibrary(pilihanLibrary, L, page, contentPerPage, song_number, selectedSong);
                string aksiUser;
                cin >> aksiUser;
                userActionHandlerLibrary(aksiUser, L, A, currentUser, page, contentPerPage, song_number, currentSong, isPlaying, selectedSong);
            } else if (pilihanLibrary == "<") {
                prev_page(page);
                clearScreen();
            } else if (pilihanLibrary == ">") {
                int totalSongs = getLibrarySize(L);
                next_page(page, totalSongs, contentPerPage);
                clearScreen();
            } else if (pilihanLibrary == "F") {
                //Fungsi mencari lagu berdasarkan nama lagu
                string songName;
                cout << "Masukkan nama lagu yang ingin dicari: ";
                cin >> songName;
                songAddress foundSong = findSong(L, songName);
                clearScreen();

                if (foundSong != nullptr) {
                    cout << "┌────────────────────────────────────────────┐" << endl;
                    cout << "│ Anda memilih lagu:                         │" << endl;
                    cout << "│   " << setfill(' ') << setw(41) << left << foundSong->info.song_name << "│" << endl;
                    cout << "│   " << setfill(' ') << setw(41) << left << "by" + foundSong->info.artist_name << "│" << endl;
                    cout << "├────────────────────────────────────────────┤" << endl;
                    cout << "│ [1] Play lagu                              │" << endl;
                    cout << "│ [2] Tambahkan lagu ke playlist             │" << endl;
                    cout << "│ [B] Back                                   │" << endl;
                    cout << "└────────────────────────────────────────────┘" << endl;
                    inputMessage();
                    string aksiUser;
                    cin >> aksiUser;
                    userActionHandlerLibrary(aksiUser, L, A, currentUser, page, contentPerPage, song_number, currentSong, isPlaying, foundSong);
                } else {
                    cout << "Lagu dengan nama tersebut tidak ditemukan di library." << endl;
                }
            } else {
                nowPlaysHandler(pilihanLibrary, currentSong, isPlaying);
            }
        }

    } else if (input == "2") {
        // Artists
        string pilihanArtist = "";
        while (pilihanArtist != "B") {
            displayArtist(A, page, contentPerPage);
            nowPlays(isPlaying, box_width, currentSong);
            cin >> pilihanArtist;
            clearScreen();
            if (pilihanArtist >= "1" && pilihanArtist <= "5") {
                int playlist_number = stoi(pilihanArtist);
                playlistAddress selectedPlaylist = getArtistPlaylistFromArtists(A, page, contentPerPage, playlist_number);
                int playlistPage = 1;

                string pilihanSong = "";
                while (pilihanSong != "B") {
                    int song_number;
                    relasiMLLAddress selectedSong;
                    displaySongsInPlaylist(selectedPlaylist, playlistPage, contentPerPage, false);
                    nowPlays(isPlaying, box_width, currentSong);
                    cin >> pilihanSong;

                    if (pilihanSong >= "1" && pilihanSong <= "5") {
                        int page = 1;
                        song_number = stoi(pilihanSong);
                        selectedSong = getSongFromPlaylist(selectedPlaylist, page, contentPerPage, song_number);
                        clearScreen();
                        cout << "┌────────────────────────────────────────────┐" << endl;
                        cout << "│ Anda memilih lagu:                         │" << endl;
                        cout << "│   " << setfill(' ') << setw(41) << left << selectedSong->song_pointer->info.song_name << "│" << endl;
                        cout << "│   " << setfill(' ') << setw(41) << left << "by" + selectedSong->song_pointer->info.artist_name << "│" << endl;
                        cout << "├────────────────────────────────────────────┤" << endl;
                        cout << "│ [1] Play lagu                              │" << endl;
                        cout << "│ [2] Tambahkan lagu ke playlist             │" << endl;
                        cout << "│ [3] Tambahkan lagu ke favorit              │" << endl;
                        cout << "│ [B] Back                                   │" << endl;
                        cout << "└────────────────────────────────────────────┘" << endl;
                        inputMessage();

                        string aksiUser;
                        cin >> aksiUser;
                        clearScreen();
                        if (aksiUser == "1") {
                            playFromPlaylist(currentSong, selectedSong, isPlaying);
                        } else if (aksiUser == "2") {
                            int page = 1;
                            string pilihanPlayList = "";
                            while (pilihanPlayList != "B") {
                                cout << "Pilih playlist untuk ditambahkan lagu:" << endl;
                                displayPlaylists(currentUser, page, contentPerPage);
                                cout << "┌────────────────────────────────────────────┐" << endl;
                                cout << "│ [+] Tambahkan ke playlist baru             │" << endl;
                                cout << "└────────────────────────────────────────────┘" << endl;
                                inputMessage();
                                cin >> pilihanPlayList;
                                clearScreen();

                                if (pilihanPlayList >= "1" && pilihanPlayList <= "5") {
                                    int playlist_number = stoi(pilihanPlayList);
                                    playlistAddress selectedPlaylist = getPlaylistFromUser(currentUser, page, contentPerPage, playlist_number);
                                    addSongToPlaylist(selectedPlaylist, selectedSong->song_pointer);

                                } else if (pilihanPlayList == "<") {
                                    prev_page(page);
                                    clearScreen();

                                } else if (pilihanPlayList == ">") {
                                    int totalPlaylists = getPlaylistCount(currentUser);
                                    next_page(page, totalPlaylists, contentPerPage);
                                    clearScreen();

                                } else if (pilihanPlayList == "+") {
                                    playlist_info newPlaylistInfo;
                                    cout << "Masukkan nama playlist baru: ";
                                    cin >> newPlaylistInfo.playlist_name;
                                    newPlaylistInfo.playlist_size = 0;
                                    clearScreen();

                                    playlistAddress newPlaylist = allocatePlaylist(newPlaylistInfo);
                                    addPlaylist(currentUser, newPlaylist);
                                    addSongToPlaylist(newPlaylist, selectedSong->song_pointer);
                                    cout << "Playlist " << newPlaylistInfo.playlist_name << " berhasil dibuat dan lagu ditambahkan!" << endl;
                                    pilihanPlayList = "B";
                                }
                            }
                        } else if (aksiUser == "3") {
                            addSongToPlaylist(currentUser->first_playlist, selectedSong->song_pointer);
                        }
                        
                    } else if (pilihanSong == "<") {
                        prev_page(page);
                        clearScreen();
                    } else if (pilihanSong == ">") {
                        int totalSongs = getLibrarySize(L);
                        next_page(page, totalSongs, contentPerPage);
                        clearScreen();
                    } else if (pilihanSong == "E") {
                        editPlaylist(currentUser, selectedPlaylist, contentPerPage, isPlaying);
                    } else {
                        clearScreen();
                        nowPlaysHandler(pilihanSong, currentSong, isPlaying);
                    }
                }

            } else if (pilihanArtist == "<") {
                prev_page(page);
                clearScreen();

            } else if (pilihanArtist == ">") {
                int totalArtists = getArtistsCount(A);
                next_page(page, totalArtists, contentPerPage);
                clearScreen();
            } else {
                clearScreen();
                nowPlaysHandler(pilihanArtist, currentSong, isPlaying);
            }
        }

    } else if (input == "3") {
        // Playlists
        string pilihanPlaylists = "";
        while (pilihanPlaylists != "B") {
            int page = 1;
            displayPlaylists(currentUser, page, contentPerPage);
            nowPlays(isPlaying, box_width, currentSong);
            inputMessage();
            cin >> pilihanPlaylists;
            clearScreen();
            if (pilihanPlaylists >= "1" && pilihanPlaylists <= "5") {
                int playlist_number = stoi(pilihanPlaylists);
                playlistAddress selectedPlaylist = getPlaylistFromUser(currentUser, page, contentPerPage, playlist_number);
                int playlistPage = 1;

                string pilihanPlaylist = "";
                while (pilihanPlaylist != "B") {
                    int song_number;
                    relasiMLLAddress selectedSong;
                    displaySongsInPlaylist(selectedPlaylist, playlistPage, contentPerPage, true);
                    nowPlays(isPlaying, box_width, currentSong);
                    cin >> pilihanPlaylist;

                    if (pilihanPlaylist >= "1" && pilihanPlaylist <= "5") {
                        userPickSongPlaylist(pilihanPlaylist, selectedPlaylist, contentPerPage, song_number, selectedSong);
                        string aksiUser;
                        cin >> aksiUser;
                        userActionHandlerPlaylist(aksiUser, selectedPlaylist, currentSong, selectedSong, isPlaying);
                    } else if (pilihanPlaylist == "<") {
                        prev_page(page);
                        clearScreen();
                    } else if (pilihanPlaylist == ">") {
                        int totalSongs = getLibrarySize(L);
                        next_page(page, totalSongs, contentPerPage);
                        clearScreen();
                    } else if (pilihanPlaylist == "E") { 
                        editPlaylist(currentUser, selectedPlaylist, contentPerPage, isPlaying);
                    } else {
                        clearScreen();
                        nowPlaysHandler(pilihanPlaylist, currentSong, isPlaying);
                    }
                }

            } else if (pilihanPlaylists == "<") {
                prev_page(page);
                clearScreen();

            } else if (pilihanPlaylists == ">") {
                int totalPlaylists = getPlaylistCount(currentUser);
                next_page(page, totalPlaylists, contentPerPage);
                clearScreen();
            } else {
                clearScreen();
                nowPlaysHandler(pilihanPlaylists, currentSong, isPlaying);
            }
        }
    } else {
        nowPlaysHandler(input, currentSong, isPlaying);
    }
}

void editPlaylist(userAddress &U, playlistAddress &P, int contentPerPage, bool &isPlaying) {
    clearScreen();
    playlist_info newInfo;
    newInfo.playlist_name = P->info.playlist_name;
    newInfo.playlist_size = P->info.playlist_size;

    cout << "┌────────────────────────────────────────────┐" << endl;
    cout << "│ Info playlist mana yang ingin diubah?      │ " << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    cout << "│ [1] Nama Playlist                          │" << endl;
    cout << "│ [2] Hapus Lagu dari Playlist               │" << endl;
    cout << "│ [3] Hapus Playlist                         │" << endl;
    cout << "│ [B] Back                                   │" << endl;
    cout << "└────────────────────────────────────────────┘" << endl;
    inputMessage();

    string pilihan;
    cin >> pilihan;
    clearScreen();

    if (pilihan == "1") {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│           Mengubah Nama Playlist           │ " << endl;
        cout << "└────────────────────────────────────────────┘" << endl;
        cout << "Masukkan nama playlist yang baru: " << endl;
        cin >> newInfo.playlist_name;

        P->info.playlist_name = newInfo.playlist_name;
        clearScreen();
        cout << "Nama playlist berhasil diubah!" << endl;
    }

    else if (pilihan == "2") {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│         Menghapus Lagu dari Playlist       │ " << endl;
        cout << "└────────────────────────────────────────────┘" << endl;

        // tampilkan lagu di playlist
        int page = 1;
        cout << "Pilih nomor lagu yang ingin dihapus" << endl;
        displaySongsInPlaylist(P, page, contentPerPage, false);
        inputMessage();
        string pilihSong;
        cin >> pilihSong;

        int song_number;
        relasiMLLAddress selectedSong;

        if (pilihSong >= "1" && pilihSong <= "5") {
            song_number = stoi(pilihSong);
            selectedSong = getSongFromPlaylist(P, page, contentPerPage, song_number);
        } else if (pilihSong == "<") {
            prev_page(page);
            clearScreen();
        } else if (pilihSong == ">") {
            int totalSongs = getPlaylistSize(P);
            next_page(page, totalSongs, contentPerPage);
            clearScreen();
        }
        if (pilihSong != "B") {
            removeSongFromPlaylist(P, selectedSong->song_pointer);
            clearScreen();
            cout << "Lagu berhasil dihapus dari playlist!" << endl;
        }
    }

    else if (pilihan == "3") {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│              Hapus Playlist                │ " << endl;
        cout << "└────────────────────────────────────────────┘" << endl;

        deletePlaylist(U, P);
        clearScreen();
        cout << "Playlist berhasil dihapus!" << endl;
        return;
    }
}

void sortSongLibrary(Library &L) {
    songAddress i = L.first;

    while (i != nullptr) {
        songAddress min = i;
        songAddress j = i->next;

        while (j != nullptr) {
            if (j->info.song_name < min->info.song_name) {
                min = j;
            }
            j = j->next;
        }

        if (min != i) {
            song_info temp = i->info;
            i->info = min->info;
            min->info = temp;
        }

        i = i->next;
    }
}

void sortSongPlaylist(playlistAddress &P) {
    relasiMLLAddress i = P->first_song;

    while (i != nullptr) {
        relasiMLLAddress min = i;
        relasiMLLAddress j = i->next;

        while (j != nullptr) {
            if (j->song_pointer->info.song_name <
                min->song_pointer->info.song_name) {
                min = j;
            }
            j = j->next;
        }

        if (min != i) {
            songAddress temp = i->song_pointer;
            i->song_pointer = min->song_pointer;
            min->song_pointer = temp;
        }

        i = i->next;
    }
}


