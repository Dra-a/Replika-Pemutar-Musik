# APLIKASI PEMUTAR MUSIK
## Replika Spotify/Apple Music dengan Struktur Data Linked List

Aplikasi konsol pemutar musik yang dikembangkan menggunakan struktur data **Singly Linked List**, **Doubly Linked List**, dan **Multi-Linked List** untuk mengelola library lagu, playlist, dan user.

---

## 1. DESKRIPSI MASALAH DAN FITUR YANG DIIMPLEMENTASIKAN

### 1.1 Deskripsi Masalah

Aplikasi pemutar musik dikembangkan untuk membantu pengguna mengelola dan memutar koleksi musik. Aplikasi ini harus mendukung dua peran utama:

1. **Admin**: Mengelola data lagu dalam library.
2. **User**: Mencari, memutar musik, dan mengelola playlist pribadi

Aplikasi ini juga harus memenuhi beberapa ketentuan khusus:
- Perubahan data lagu oleh admin harus otomatis tercermin di semua playlist user (menggunakan multi-linked list)
- Fitur next/prev lagu harus berbeda antara mode library dan playlist
- Di library: memutar lagu yang mirip berdasarkan artis atau genre
- Di playlist: mengikuti urutan lagu di playlist
- Data lagu menggunakan tipe data bentukan (record/struct)

### 1.2 Fitur-Fitur yang Sudah Diimplementasikan

#### A. Peran Admin

1. **Menambah Lagu Baru ke Library** ✅
   - Input data lengkap lagu (judul, artis, durasi dalam menit dan detik)
   - Lagu otomatis ditambahkan ke library menggunakan Doubly Linked List
   - Lagu juga dapat ditambahkan ke daftar artis (Artists)

2. **Melihat Semua Lagu di Library** ✅
   - Menampilkan semua lagu dengan format yang rapi
   - Sistem pagination untuk library yang besar
   - Menampilkan informasi lengkap setiap lagu (judul, artis, durasi)
   - Navigasi halaman dengan next/prev

3. **Mengubah Data Lagu** ✅
   - Update data lagu yang sudah ada
   - Perubahan otomatis tercermin di semua playlist (karena menggunakan reference)
   - Validasi input sebelum update

4. **Menghapus Lagu dari Library** ✅
   - Hapus lagu berdasarkan nama lagu
   - Otomatis menghapus dari semua playlist user
   - Validasi konfirmasi sebelum menghapus
   - Update pointer di Doubly Linked List

#### B. Peran User

1. **Mencari dan Memutar Lagu** ✅
   - Mencari lagu dari library berdasarkan nama
   - Memutar lagu dari library
   - Memutar lagu dari playlist
   - Menampilkan informasi lagu yang sedang diputar
   - Tampilan "Now Playing" yang informatif

2. **Navigasi Lagu** ✅
   - **Next Song**: Memutar lagu selanjutnya
     - Di playlist: mengikuti urutan playlist (next item dalam relasiMLL)
     - Di library: memutar lagu yang mirip menggunakan algoritma `moveToSimilarSongs()`
   - **Prev Song**: Memutar lagu sebelumnya
     - Di playlist: mengikuti urutan playlist (previous item)
     - Di library: memutar lagu yang mirip
   - **Stop Song**: Menghentikan lagu yang sedang diputar
   - Menampilkan notifikasi untuk setiap aksi

3. **Manajemen Playlist** ✅
   - **Membuat Playlist**: Buat playlist baru dengan nama unik
   - **Menambah Lagu**: Pilih lagu dari library untuk ditambahkan ke playlist
   - **Menghapus Lagu**: Hapus lagu dari playlist
   - **Melihat Isi Playlist**: Lihat semua lagu dalam playlist dengan pagination
   - **Edit Playlist**: Edit playlist (tambah/hapus lagu, ubah nama)
   - **Menampilkan Semua Playlist**: Lihat daftar semua playlist user

4. **Fitur Tambahan** ✅
   - Homepage dengan tampilan yang menarik
   - Pagination untuk library dan playlist yang besar
   - Tampilan "Now Playing" yang selalu terlihat
   - User management (multiple user support)
   - Sistem Artists untuk mengelompokkan lagu berdasarkan artis

---

## 2. PENJELASAN MENGENAI PROCEDURE/FUNCTION YANG DIGUNAKAN ATAU DIBUAT

### 2.1 Fungsi-Fungsi Library (Admin)

#### `createLibrary(Library &L)`
- **Deskripsi**: Inisialisasi library kosong
- **Parameter**: Reference ke Library
- **Algoritma**: Set first dan last ke nullptr

#### `allocateSong(song_info info)`
- **Deskripsi**: Alokasi memori untuk node lagu baru
- **Parameter**: Data song_info (judul, artis, durasi)
- **Return**: Pointer ke songElement
- **Algoritma**: 
  - Alokasi memori dengan `new`
  - Set info dengan data yang diberikan
  - Set next dan prev ke nullptr

#### `addSongToLibrary(Library &L, songAddress P)`
- **Deskripsi**: Menambahkan lagu ke library
- **Parameter**: Reference Library dan pointer ke node
- **Algoritma**: 
  - Jika library kosong: set first = last = P
  - Jika tidak: tambah di akhir (last->next = P, P->prev = last, last = P)

#### `displayLibrary(Library L, int page, int n)`
- **Deskripsi**: Menampilkan semua lagu di library dengan pagination
- **Parameter**: Library, nomor halaman, jumlah item per halaman
- **Algoritma**: 
  - Hitung total halaman
  - Traverse dari first, skip item sebelum halaman saat ini
  - Tampilkan n item per halaman
  - Tampilkan informasi pagination

#### `findSong(Library L, string song_name)`
- **Deskripsi**: Mencari lagu berdasarkan nama
- **Return**: Pointer ke node jika ditemukan, nullptr jika tidak
- **Algoritma**: Linear search dari first ke last, bandingkan song_name

#### `editSongFromLibrary(Library &L, songAddress &P)`
- **Deskripsi**: Update data lagu
- **Parameter**: Reference Library dan pointer ke node yang akan diupdate
- **Algoritma**: 
  - Input data baru dari user
  - Update info di node P
  - Karena playlist menggunakan reference, perubahan otomatis terlihat

#### `deleteSongFromLibrary(Library &L, songAddress &P)`
- **Deskripsi**: Hapus lagu dari library
- **Parameter**: Reference Library dan pointer ke node yang akan dihapus
- **Algoritma**: 
  - Update pointer: prev->next = next, next->prev = prev
  - Update first/last jika perlu
  - Hapus dari semua playlist (traverse semua user dan playlist)
  - Deallocate memory dengan `delete`

### 2.2 Fungsi-Fungsi Playlist

#### `allocatePlaylist(playlist_info info)`
- **Deskripsi**: Alokasi memori untuk playlist baru
- **Parameter**: Data playlist_info (nama, ukuran)
- **Return**: Pointer ke playlistElement
- **Algoritma**: Alokasi memori, set info, set next ke nullptr, set first_song ke nullptr

#### `addPlaylist(userAddress &P, playlistAddress Q)`
- **Deskripsi**: Menambahkan playlist ke user
- **Parameter**: Pointer ke user dan pointer ke playlist
- **Algoritma**: 
  - Jika user belum punya playlist: set first_playlist = Q
  - Jika sudah ada: traverse ke akhir, tambah di akhir

#### `addSongToPlaylist(playlistAddress &P, songAddress song)`
- **Deskripsi**: Menambah lagu ke playlist
- **Parameter**: Pointer ke playlist dan pointer ke lagu di library
- **Algoritma**: 
  - Alokasi relasiMLL baru
  - Set song_pointer ke song (reference, bukan copy)
  - Tambah ke akhir relasiMLL (doubly linked list)
  - Increment playlist_size

#### `removeSongFromPlaylist(playlistAddress &P, songAddress song)`
- **Deskripsi**: Hapus lagu dari playlist
- **Parameter**: Pointer ke playlist dan pointer ke lagu
- **Algoritma**: 
  - Traverse relasiMLL dari first_song
  - Cari item dengan song_pointer == song
  - Update pointer prev dan next
  - Deallocate memory
  - Decrement playlist_size

#### `displaySongsInPlaylist(playlistAddress P, int page, int n, bool showEdit)`
- **Deskripsi**: Tampilkan isi playlist dengan pagination
- **Parameter**: Pointer playlist, halaman, item per halaman, flag untuk edit
- **Algoritma**: 
  - Traverse relasiMLL dari first_song
  - Tampilkan dengan pagination
  - Akses data melalui song_pointer (reference ke library)

### 2.3 Fungsi-Fungsi User

#### `allocateUser(user_info info)`
- **Deskripsi**: Alokasi memori untuk user baru
- **Parameter**: Data user_info (username, playlist_count, isAdmin)
- **Return**: Pointer ke userElement

#### `addUser(Users &U, userAddress P)`
- **Deskripsi**: Menambahkan user ke sistem
- **Parameter**: Reference Users dan pointer ke user
- **Algoritma**: 
  - Jika Users kosong: set first = last = P
  - Jika tidak: tambah di akhir (last->next = P, last = P)

#### `findUser(Users U, string user_name)`
- **Deskripsi**: Mencari user berdasarkan username
- **Return**: Pointer ke user jika ditemukan
- **Algoritma**: Linear search dari first ke last

### 2.4 Fungsi-Fungsi Pemutaran Lagu

#### `playFromLibrary(Artists A, songAddress selectedSong, relasiMLLAddress &current, bool &isPlaying)`
- **Deskripsi**: Memutar lagu dari library
- **Parameter**: Artists, pointer lagu yang dipilih, reference current song, flag isPlaying
- **Algoritma**: 
  - Alokasi relasiMLL baru
  - Set song_pointer ke selectedSong
  - Set current ke relasiMLL baru
  - Set isPlaying ke true
  - Tampilkan informasi lagu

#### `playFromPlaylist(relasiMLLAddress &currentSong, relasiMLLAddress selectedSong, bool &isPlaying)`
- **Deskripsi**: Memutar lagu dari playlist
- **Parameter**: Reference current song, pointer selected song, flag isPlaying
- **Algoritma**: 
  - Set currentSong ke selectedSong
  - Set isPlaying ke true
  - Tampilkan informasi lagu

#### `stopSong(bool &isPlaying)`
- **Deskripsi**: Menghentikan lagu
- **Parameter**: Reference flag isPlaying
- **Algoritma**: Set isPlaying ke false

#### `nextSong(relasiMLLAddress &current, bool &isPlaying)`
- **Deskripsi**: Memutar lagu selanjutnya
- **Parameter**: Reference current song, flag isPlaying
- **Algoritma**: 
  - Jika current->next != nullptr: set current = current->next
  - Jika tidak ada next: cari lagu mirip dengan `moveToSimilarSongs()`
  - Tampilkan informasi lagu baru

#### `prevSong(relasiMLLAddress &current, bool &isPlaying)`
- **Deskripsi**: Memutar lagu sebelumnya
- **Parameter**: Reference current song, flag isPlaying
- **Algoritma**: 
  - Jika current->prev != nullptr: set current = current->prev
  - Jika tidak ada prev: cari lagu mirip
  - Tampilkan informasi lagu baru

#### `moveToSimilarSongs(Artists A, songAddress current_song)`
- **Deskripsi**: Mencari lagu yang mirip dengan lagu saat ini
- **Parameter**: Artists, pointer lagu saat ini
- **Return**: Pointer ke relasiMLL lagu mirip
- **Algoritma**: 
  1. Cari di Artists yang memiliki artis sama dengan current_song
  2. Jika ditemukan, return relasiMLL pertama dari artis tersebut
  3. Jika tidak ditemukan, bisa return nullptr atau lagu acak

### 2.5 Fungsi-Fungsi Helper dan Tampilan

#### `displaySongInfo(songAddress P, int number)`
- **Deskripsi**: Menampilkan informasi lagu dengan format yang rapi
- **Parameter**: Pointer lagu, nomor urut
- **Algoritma**: Tampilkan nomor, judul, artis, durasi dengan format tabel

#### `homePage(userAddress U, int width)`
- **Deskripsi**: Menampilkan homepage user
- **Parameter**: Pointer user, lebar box
- **Algoritma**: Tampilkan informasi user, jumlah playlist, menu navigasi

#### `nowPlays(bool isPlaying, int width, relasiMLLAddress currentSong)`
- **Deskripsi**: Menampilkan informasi lagu yang sedang diputar
- **Parameter**: Flag isPlaying, lebar box, pointer current song
- **Algoritma**: 
  - Jika isPlaying: tampilkan informasi lagu dari currentSong->song_pointer
  - Jika tidak: tampilkan "Tidak ada lagu yang diputar"

#### `prev_page(int &page)` dan `next_page(int &page, int totalItems, int itemsPerPage)`
- **Deskripsi**: Navigasi halaman untuk pagination
- **Algoritma**: 
  - prev_page: decrement page jika > 1
  - next_page: increment page jika belum mencapai halaman terakhir

---

## 3. PENJELASAN MENGENAI STRUKTUR DATA YANG DIPILIH

### 3.1 Doubly Linked List untuk Library

#### Struktur:
```cpp
struct Library {
    songAddress first;      // Pointer ke node pertama
    songAddress last;       // Pointer ke node terakhir
};

struct songElement {
    song_info info;         // Data lagu
    songAddress next;       // Pointer ke node berikutnya
    songAddress prev;       // Pointer ke node sebelumnya
};
```

#### Alasan Pemilihan:
1. **Navigasi dua arah**: Memudahkan implementasi next/prev lagu
2. **Efisiensi insert/delete**: Dapat insert/delete di berbagai posisi dengan jika sudah ada pointer ke node
3. **Fleksibilitas**: Cocok untuk operasi CRUD yang kompleks
4. **Akses ke tail**: Dengan pointer last, node terakhir pada library dapat diakses secara langsung tanpa perlu menelusuri list dari awal.

#### Kelebihan:
- Dapat traverse maju dan mundur
- Mudah untuk navigasi next/prev
- Efisien untuk operasi di akhir list (menggunakan tail pointer)
- Cocok untuk library yang perlu diakses dua arah

#### Kekurangan:
- Menggunakan lebih banyak memori (setiap node punya 2 pointer)
- Operasi lebih kompleks dibanding singly linked list
- Perlu update 2 pointer saat insert/delete

### 3.2 Singly Linked List untuk Playlist

#### Struktur:
```cpp
struct playlistElement {
    playlist_info info;           // Data playlist (nama, ukuran)
    playlistAddress next;         // Pointer ke playlist berikutnya
    relasiMLLAddress first_song;  // Pointer ke lagu pertama di playlist
};
```

#### Alasan Pemilihan:
1. **Hanya perlu forward traversal**: Playlist biasanya di-traverse maju saja
2. **Lebih efisien memori**: Hanya perlu 1 pointer per node
3. **Cukup untuk kebutuhan**: User biasanya melihat playlist secara berurutan

#### Kelebihan:
- Lebih efisien memori
- Cukup untuk kebutuhan playlist
- Mudah diimplementasikan
- Operasi insert/delete lebih sederhana

#### Kekurangan:
- Tidak mendukung navigasi dua arah secara langsung
- Untuk mengakses elemen sebelumnya, diperlukan penelusuran dari awal list

### 3.3 Multi-Linked List untuk Relasi Playlist-Lagu

#### Struktur:
```cpp
struct relasiMLL {
    songAddress song_pointer;      // Reference ke songElement di library
    relasiMLLAddress next;         // Pointer ke item berikutnya
    relasiMLLAddress prev;         // Pointer ke item sebelumnya
};

struct playlistElement {
    relasiMLLAddress first_song;   // Pointer ke relasiMLL pertama
};
```

#### Konsep Multi-Linked List:
Playlist tidak menyimpan **copy** data Song, tetapi menyimpan **reference (pointer)** ke `songElement` yang ada di Library.

#### Keuntungan:
1. **Konsistensi data**: 
   - Perubahan di library otomatis terlihat di semua playlist
   - Update sekali, semua reference terupdate
   
2. **Efisiensi memori**: 
   - Tidak ada duplikasi data
   - Satu lagu bisa direferensikan oleh banyak playlist tanpa copy data
   
3. **Update otomatis**: 
   - Ketika admin update lagu, semua playlist otomatis terupdate
   - Ketika admin hapus lagu, perlu hapus reference dari semua playlist

#### Implementasi:
```cpp
// Saat tambah lagu ke playlist
relasiMLLAddress newRelasi = new relasiMLL;
newRelasi->song_pointer = song;  // Reference, bukan copy
newRelasi->next = nullptr;
newRelasi->prev = nullptr;

// Akses data lagu dari playlist
Song data = currentRelasi->song_pointer->info;  // Akses melalui reference
```

#### Visualisasi Multi-Linked List:
```
Library (Doubly Linked List):
[song1] <-> [song2] <-> [song3]

Playlist 1 (Multi-Linked List):
[relasi1] -> song_pointer -> [song1]
[relasi2] -> song_pointer -> [song3]

Playlist 2 (Multi-Linked List):
[relasi3] -> song_pointer -> [song2]
[relasi4] -> song_pointer -> [song1]  // Same song, different reference
```

### 3.4 Doubly Linked List untuk RelasiMLL

#### Alasan:
- RelasiMLL menggunakan Doubly Linked List untuk memudahkan next/prev di playlist
- Dengan prev pointer, navigasi mundur menjadi O(1)
- Cocok untuk fitur next/prev song di playlist

### 3.5 Singly Linked List untuk User

#### Struktur:
```cpp
struct Users {
    userAddress first;
    userAddress last;
};

struct userElement {
    user_info info;              // Data user (username, playlist_count, isAdmin)
    userAddress next;            // Pointer ke user berikutnya
    playlistAddress first_playlist;  // Pointer ke playlist pertama user
};
```

#### Alasan Pemilihan:
- User dapat memiliki banyak playlist
- Setiap user memiliki pointer ke playlist pertama
- Playlist user dihubungkan dengan singly linked list
- Cukup untuk kebutuhan user management

### 3.6 Record/Struct untuk Data

#### Struktur Data Song:
```cpp
struct waktu {
    int menit, detik;
};

struct song_info {
    string song_name;
    string artist_name;
    waktu duration;
};
```

#### Struktur Data Playlist:
```cpp
struct playlist_info {
    string playlist_name;
    int playlist_size;
};
```

#### Struktur Data User:
```cpp
struct user_info {
    string user_name;
    int playlist_count;
    bool isAdmin;
};
```

#### Alasan:
- Sesuai requirement: data lagu menggunakan tipe data bentukan (record)
- Mudah untuk menyimpan semua informasi
- Efisien untuk operasi update (update sekali, semua reference terupdate)

---

## 4. ALGORITMA UTAMA

### 4.1 Algoritma Tambah Lagu ke Library

```
1. Input data lagu dari user (judul, artis, durasi)
2. Buat song_info dengan data yang diinput
3. Alokasi node baru dengan allocateSong()
4. Jika library kosong:
   - Set first = newNode
   - Set last = newNode
5. Jika tidak kosong:
   - Set last->next = newNode
   - Set newNode->prev = last
   - Set last = newNode
```

### 4.2 Algoritma Hapus Lagu dari Library

```
1. Cari lagu berdasarkan nama dengan findSong()
2. Jika ditemukan:
   a. Update pointer:
      - Jika ada prev: prev->next = node->next
      - Jika ada next: next->prev = node->prev
      - Jika node adalah first: first = node->next
      - Jika node adalah last: last = node->prev
   b. Hapus dari semua playlist:
      - Traverse semua user
      - Untuk setiap user, traverse semua playlist
      - Untuk setiap playlist, traverse relasiMLL
      - Hapus relasiMLL yang song_pointer == node
   c. Deallocate memory dengan delete
3. Jika tidak ditemukan: tampilkan pesan error
```

### 4.3 Algoritma Tambah Lagu ke Playlist

```
1. Pilih lagu dari library (songAddress song)
2. Alokasi relasiMLL baru
3. Set relasiMLL->song_pointer = song (reference, bukan copy)
4. Jika playlist kosong:
   - Set first_song = newRelasi
   - Set last_song = newRelasi (jika ada)
5. Jika tidak kosong:
   - Tambah di akhir relasiMLL
   - Update pointer next dan prev
6. Increment playlist_size
```

### 4.4 Algoritma Next Song

```
1. Cek apakah ada lagu yang sedang diputar
2. Jika di playlist mode (current->next != nullptr):
   - Set current = current->next
   - Putar lagu dari current->song_pointer
3. Jika di library mode atau tidak ada next:
   - Cari lagu mirip dengan moveToSimilarSongs()
   - Jika ditemukan: putar lagu mirip
   - Jika tidak: tampilkan pesan atau fallback
4. Tampilkan informasi lagu baru
```

### 4.5 Algoritma Cari Lagu Mirip (moveToSimilarSongs)

```
1. Ambil artis dari current_song
2. Traverse Artists (daftar playlist per artis)
3. Cari playlist yang nama artisnya sama dengan current_song->artist_name
4. Jika ditemukan:
   - Ambil relasiMLL pertama dari playlist artis tersebut
   - Return relasiMLL tersebut
5. Jika tidak ditemukan:
   - Return nullptr atau lagu acak
```

### 4.6 Algoritma Pagination

```
1. Hitung total halaman: totalPages = ceil(totalItems / itemsPerPage)
2. Hitung index mulai: startIndex = (page - 1) * itemsPerPage
3. Traverse list:
   - Skip startIndex item pertama
   - Tampilkan itemsPerPage item
4. Tampilkan informasi: "Halaman X dari Y"
5. Tampilkan navigasi: [P]rev, [N]ext
```

---

## 5. CARA MENJALANKAN PROGRAM

### Kompilasi

```bash
g++ -o TubesSTD.exe main.cpp spotify.cpp -std=c++11
```

### Menjalankan

```bash
# Windows
TubesSTD.exe

# Linux/Mac
./TubesSTD
```

### Login

1. **Admin**: Pilih `[1]`, masukkan username `admin`
2. **User**: Pilih `[2]`, masukkan username (atau buat baru)

---

## 6. CONTOH PENGGUNAAN

### Skenario 1: Admin Menambah Lagu

1. Login sebagai admin (username: `admin`)
2. Pilih menu `[2]` (Tambah Lagu)
3. Input data:
   - Judul: `Bohemian_Rhapsody`
   - Artis: `Queen`
   - Durasi: `5` menit `55` detik
4. Lagu ditambahkan ke library

### Skenario 2: User Membuat Playlist dan Memutar Lagu

1. Login sebagai user (buat user baru jika belum ada)
2. Pilih menu `[P]` (Playlist)
3. Pilih `[1]` (Buat Playlist)
4. Input nama playlist: `Favorites`
5. Pilih `[2]` (Tambah Lagu)
6. Pilih lagu dari library
7. Pilih `[P]` (Play) untuk memutar lagu
8. Gunakan `[N]` (Next) untuk lagu selanjutnya

### Skenario 3: Admin Update Lagu

1. Admin edit lagu di library
2. Perubahan otomatis terlihat di semua playlist user
3. Tidak perlu update manual di setiap playlist

---

## 7. STATISTIK IMPLEMENTASI

- **Total Fungsi**: 40+ fungsi dan prosedur
- **Struktur Data**: 
  - 3 struct untuk data (song_info, playlist_info, user_info)
  - 4 struct untuk node (songElement, playlistElement, userElement, relasiMLL)
  - 3 struct untuk list (Library, Users, Artists)
- **Fitur Admin**: 4/4 (100%)
- **Fitur User**: 7/7 (100%)
- **Struktur Data yang Digunakan**:
  - ✅ Doubly Linked List (Library, relasiMLL)
  - ✅ Singly Linked List (Playlist, User)
  - ✅ Multi-Linked List (Playlist-Library relationship)

---

## 8. REFERENSI

- **Dokumentasi C++**: [cppreference.com](https://cppreference.com)
- **Struktur Data**: Buku/Diktat Struktur Data mata kuliah
- **Linked List**: Materi perkuliahan tentang Linked List