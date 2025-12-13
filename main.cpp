#include "spotify.h"
#include <string>
#include <iomanip>
#include <windows.h>
#include <iostream>
using namespace std;

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    Library L;
    Users U;
    Artists A;
    createLibrary(L);
    createUsers(U);
    createArtists(A);

    user_info adminInfo;
    adminInfo.user_name = "admin";
    adminInfo.playlist_count = 0;
    adminInfo.isAdmin = true;
    userAddress adminUser = allocateUser(adminInfo);
    addUser(U, adminUser);

    int box_width = 44;
    string pilihanMenu = "";
    while (pilihanMenu != "X") {
        cout << "Pilih menu: ";
        cout << "[1] Admin Login  [2] User Login  [X] Exit" << endl;
        cin >> pilihanMenu;
        string username, password;
        userAddress currentUser = nullptr;

        if (pilihanMenu == "1") {
            cout << "Masukkan username: ";
            cin >> username;
            currentUser = findUser(U, username);

            if (currentUser == nullptr) {
                cout << "Admin dengan username tersebut tidak ditemukan." << endl;
            }
        } else if (pilihanMenu == "2") {
            cin >> username;
            currentUser = findUser(U, username);

            if (currentUser == nullptr) {
                cout << "User dengan username tersebut tidak ditemukan." << endl;
                cout << "Apakah Anda ingin mendaftar sebagai user baru? [Y]es / [N]o): ";
                string pilihanDaftar;
                cin >> pilihanDaftar;
                if (pilihanDaftar == "Y") {
                    user_info newUserInfo;
                    newUserInfo.user_name = username;
                    newUserInfo.playlist_count = 0;
                    newUserInfo.isAdmin = false;
                    userAddress newUser = allocateUser(newUserInfo);
                    addUser(U, newUser);
                    cout << "Pendaftaran berhasil! Silakan login kembali." << endl;
                }
            }
        }

        while (currentUser != nullptr) {
            int page = 1;
            int contentPerPage = 5;

            if (currentUser->info.isAdmin) {
                string pilihanAdmin = "";
                while (currentUser != nullptr) {
                    cout << "Admin Menu: [1] Song List  [2] Add Song  [L]ogout" << endl;
                    cin >> pilihanAdmin;

                    if (pilihanAdmin == "L") {
                        currentUser = nullptr;
                    } else if (pilihanAdmin == "1") {
                        string pilihanLibrary = "";
                        while (pilihanLibrary != "H") {
                            displayLibrary(L, page, contentPerPage);
                            cin >> pilihanLibrary;
                            songAddress selectedSong;
                            if (pilihanLibrary >= "1" && pilihanLibrary <= "5") {
                                int song_number = stoi(pilihanLibrary);
                                selectedSong = getSongFromLibrary(L, page, contentPerPage, song_number);
                                cout << "Pilih aksi: [1] Edit Song  [2] Delete Song  [B]ack" << endl;
                                string aksiSong;
                                cin >> aksiSong;
                                if (aksiSong == "1") {
                                    editSongFromLibrary(L, selectedSong);
                                } else if (aksiSong == "2") {
                                    userAddress pU = U.first;
                                    while (pU != nullptr) {
                                        playlistAddress pPL = pU->first_playlist;
                                        while (pPL != nullptr) {
                                            relasiMLLAddress R = pPL->first_song;
                                            while (R != nullptr) {
                                                if (R->song_pointer == selectedSong) {
                                                    removeSongFromPlaylist(pPL, selectedSong);
                                                }
                                                R = R->next;
                                            }
                                            pPL = pPL->next;
                                        }
                                        pU = pU->next;
                                    }
                                    deleteSongFromLibrary(L, selectedSong);
                                }
                            } else if (pilihanLibrary == "P") {
                                if (page > 1) {
                                    page--;
                                }
                            } else if (pilihanLibrary == "N") {
                                int totalSongs = getLibrarySize(L);
                                int totalPages = (totalSongs + contentPerPage - 1) / contentPerPage;
                                if (page < totalPages) {
                                    page++;
                                }
                            } else if (pilihanLibrary == "F") {
                                string songName;
                                cout << "Masukkan nama lagu yang dicari: ";
                                cin >> songName;
                                songAddress foundSong = findSong(L, songName);
                                if (foundSong != nullptr) {
                                    cout << "Lagu ditemukan: " << foundSong->info.song_name << " oleh " << foundSong->info.artist_name << endl;
                                } else {
                                    cout << "Lagu tidak ditemukan dalam library." << endl;
                                }
                            }
                        }
                    } else if (pilihanAdmin == "2") {
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
            } else {
                relasiMLLAddress currentSong = nullptr;
                bool isPlaying = false;
                while (currentUser != nullptr) {
                    homePage(currentUser, box_width);
                    nowPlays(isPlaying, box_width, currentSong);
                    string pilihanHomePage;
                    cin >> pilihanHomePage;
                    if (pilihanHomePage == "L") {
                        currentUser = nullptr;
                    } else if (pilihanHomePage == "1") {
                        string pilihanLibrary = "";
                        while (pilihanLibrary != "H") {
                            displayLibrary(L, page, contentPerPage);
                            nowPlays(isPlaying, box_width, currentSong);
                            cin >> pilihanLibrary;
                            if (pilihanLibrary >= "1" && pilihanLibrary <= "5") {
                                int song_number = stoi(pilihanLibrary);
                                songAddress selectedSong = getSongFromLibrary(L, page, contentPerPage, song_number);
                                cout << "Pilih aksi: [1] Play Song  [2] Add to Playlist  [B]ack" << endl;
                                string aksiSong;
                                cin >> aksiSong;
                                if (aksiSong == "1") {
                                    playFromLibrary(L, page, contentPerPage, A, song_number, currentSong, isPlaying);
                                } else if (aksiSong == "2") {
                                    int pagePL = 1;
                                    string pilihanPlayList = "";
                                    while (pilihanPlayList != "H") {
                                        displayPlaylists(currentUser, pagePL, contentPerPage);
                                        cout << "Input [+] untuk menambahkan ke playlist baru" << endl;
                                        cin >> pilihanPlayList;
                                        if (pilihanPlayList >= "1" && pilihanPlayList <= "5") {
                                            int playlist_number = stoi(pilihanPlayList);
                                            playlistAddress selectedPlaylist = getPlaylistFromUser(currentUser, pagePL, contentPerPage, playlist_number);
                                            addSongToPlaylist(selectedPlaylist, selectedSong);
                                        } else if (pilihanPlayList == "P") {
                                            if (pagePL > 1) {
                                                pagePL--;
                                            }
                                        } else if (pilihanPlayList == "N") {
                                            int totalPlaylists = getPlaylistCount(currentUser);
                                            int totalPages = (totalPlaylists + contentPerPage - 1) / contentPerPage;
                                            if (pagePL < totalPages) {
                                                pagePL++;
                                            }
                                        } else if (pilihanPlayList == "+") {
                                            playlist_info newPlaylistInfo;
                                            cout << "Masukkan nama playlist baru: ";
                                            cin >> newPlaylistInfo.playlist_name;
                                            newPlaylistInfo.playlist_size = 0;
                                            playlistAddress newPlaylist = allocatePlaylist(newPlaylistInfo);
                                            addPlaylist(currentUser, newPlaylist);
                                            addSongToPlaylist(newPlaylist, selectedSong);
                                        }
                                    }
                                }
                            } else if (pilihanLibrary == "P") {
                                if (page > 1) {
                                    page--;
                                }
                            } else if (pilihanLibrary == "N") {
                                int totalSongs = getLibrarySize(L);
                                int totalPages = (totalSongs + contentPerPage - 1) / contentPerPage;
                                if (page < totalPages) {
                                    page++;
                                }
                            } else if (pilihanLibrary == "F") {
                                string songName;
                                cout << "Masukkan nama lagu yang dicari: ";
                                cin >> songName;
                                songAddress foundSong = findSong(L, songName);
                                if (foundSong != nullptr) {
                                    cout << "Lagu ditemukan: " << foundSong->info.song_name << " oleh " << foundSong->info.artist_name << endl;
                                } else {
                                    cout << "Lagu tidak ditemukan dalam library." << endl;
                                }
                            } else if (pilihanLibrary == "p") {
                                if (isPlaying) {
                                    prevSong(currentSong, isPlaying);
                                }
                            } else if (pilihanLibrary == "n") {
                                if (isPlaying) {
                                    nextSong(currentSong, isPlaying);
                                }
                            } else if (pilihanLibrary == "s") {
                                if (isPlaying) {
                                    stopSong(isPlaying);
                                }
                            }
                        }
                    } else if (pilihanHomePage == "2") {
                        int pageArtist = 1;
                        string pilihanArtist = "";
                        while (pilihanArtist != "H") {
                            displayArtist(A, pageArtist, contentPerPage);
                            cin >> pilihanArtist;
                            if (pilihanArtist >= "1" && pilihanArtist <= "5") {
                                int artist_number = stoi(pilihanArtist);
                                playlistAddress selectedPlaylist = getArtistPlaylistFromArtists(A, pageArtist, contentPerPage, artist_number);
                                string pilihanSong = "";
                                int pageSong = 1;
                                while (pilihanSong != "H") {
                                    displaySongsInPlaylist(selectedPlaylist, pageSong, contentPerPage);
                                    cin >> pilihanSong;
                                    if (pilihanSong >= "1" && pilihanSong <= "5") {
                                        int song_num = stoi(pilihanSong);
                                        playFromPlaylist(selectedPlaylist, pageSong, contentPerPage, song_num, currentSong, isPlaying);
                                    } else if (pilihanSong == "P") {
                                        if (pageSong > 1) {
                                            pageSong--;
                                        }
                                    } else if (pilihanSong == "N") {
                                        int totalPages = (selectedPlaylist->info.playlist_size + contentPerPage - 1) / contentPerPage;
                                        if (pageSong < totalPages) {
                                            pageSong++;
                                        }
                                    }
                                }
                            } else if (pilihanArtist == "P") {
                                if (pageArtist > 1) {
                                    pageArtist--;
                                }
                            } else if (pilihanArtist == "N") {
                                int totalArtists = getArtistsCount(A);
                                int totalPages = (totalArtists + contentPerPage - 1) / contentPerPage;
                                if (pageArtist < totalPages) {
                                    pageArtist++;
                                }
                            }
                        }
                    } else if (pilihanHomePage == "3") {
                        int pagePlaylist = 1;
                        string pilihanPlaylist = "";
                        while (pilihanPlaylist != "H") {
                            displayPlaylists(currentUser, pagePlaylist, contentPerPage);
                            cin >> pilihanPlaylist;
                            if (pilihanPlaylist >= "1" && pilihanPlaylist <= "5") {
                                int playlist_number = stoi(pilihanPlaylist);
                                playlistAddress selectedPlaylist = getPlaylistFromUser(currentUser, pagePlaylist, contentPerPage, playlist_number);
                                string pilihanSong = "";
                                int pageSong = 1;
                                while (pilihanSong != "H") {
                                    displaySongsInPlaylist(selectedPlaylist, pageSong, contentPerPage);
                                    cin >> pilihanSong;
                                    if (pilihanSong >= "1" && pilihanSong <= "5") {
                                        int song_num = stoi(pilihanSong);
                                        playFromPlaylist(selectedPlaylist, pageSong, contentPerPage, song_num, currentSong, isPlaying);
                                    } else if (pilihanSong == "P") {
                                        if (pageSong > 1) {
                                            pageSong--;
                                        }
                                    } else if (pilihanSong == "N") {
                                        int totalPages = (selectedPlaylist->info.playlist_size + contentPerPage - 1) / contentPerPage;
                                        if (pageSong < totalPages) {
                                            pageSong++;
                                        }
                                    }
                                }
                            } else if (pilihanPlaylist == "P") {
                                if (pagePlaylist > 1) {
                                    pagePlaylist--;
                                }
                            } else if (pilihanPlaylist == "N") {
                                int totalArtists = getArtistsCount(A);
                                int totalPages = (totalArtists + contentPerPage - 1) / contentPerPage;
                                if (pagePlaylist < totalPages) {
                                    pagePlaylist++;
                                }
                            }
                        }
                    } else if (pilihanHomePage == "p") {
                        if (isPlaying) {
                            prevSong(currentSong, isPlaying);
                        }
                    } else if (pilihanHomePage == "n") {
                        if (isPlaying) {
                            nextSong(currentSong, isPlaying);
                        }
                    } else if (pilihanHomePage == "s") {
                        if (isPlaying) {
                            stopSong(isPlaying);
                        }
                    }
                }
            }
        }
    }
    return 0;
}
