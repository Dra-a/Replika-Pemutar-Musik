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

    int contentPerPage = 5;
    int box_width = 44;

    string pilihanMenu = "";
    while (pilihanMenu != "X") {
        cout << "┌────────────────────────────────────────────┐" << endl;
        cout << "│               Selamat Datang               │" << endl;
        cout << "│         Login untuk mulai bermusik         │" << endl;
        cout << "├────────────────────────────────────────────┤" << endl;
        cout << "│   [1]Admin   │    [2]User   │    [X]Exit   │" << endl;
        cout << "└────────────────────────────────────────────┘" << endl;
        inputMessage();
        cin >> pilihanMenu;
        string username;
        userAddress currentUser = nullptr;

        if (pilihanMenu == "1") {
            clearScreen();
            cout << "Masukkan username admin: ";
            cin >> username;
            currentUser = findUser(U, username);

            if (currentUser == nullptr) {
                clearScreen();
                cout << "Admin dengan username tersebut tidak ditemukan." << endl;
                cout << endl;
            }
        } else if (pilihanMenu == "2") {
            clearScreen();
            cout << "Masukkan username: ";
            cin >> username;
            currentUser = findUser(U, username);

            if (currentUser == nullptr) {
                clearScreen();
                cout << "User dengan username tersebut tidak ditemukan." << endl;
                cout << "Apakah Anda ingin mendaftar sebagai user baru?" << endl;
                cout << "┌──────────────────────────┐" << endl;
                cout << "│    [Y]es    │    [N]o    │" << endl;
                cout << "└──────────────────────────┘" << endl;
                inputMessage();
                string pilihanDaftar;
                cin >> pilihanDaftar;
                if (pilihanDaftar == "Y") {
                    user_info newUserInfo;
                    newUserInfo.user_name = username;
                    newUserInfo.playlist_count = 0;
                    newUserInfo.isAdmin = false;
                    userAddress newUser = allocateUser(newUserInfo);
                    addUser(U, newUser);
                    clearScreen();
                    cout << "Pendaftaran berhasil! Silakan login kembali." << endl;
                } else {
                    clearScreen();
                }
            }
        }

        while (currentUser != nullptr) {
            if (currentUser->info.isAdmin) {
                string pilihanAdmin = "";
                while (currentUser != nullptr) {
                    clearScreen();
                    cout << "Login sebagai Admin berhasil!" << endl;
                    cout << "┌────────────────────────────────────────────┐" << endl;
                    cout << "│               Welcome, Admin               │" << endl;
                    cout << "├────────────────────────────────────────────┤" << endl;
                    cout << "│ [1] Lihat Library                          │" << endl;
                    cout << "│ [2] Tambah Lagu ke Library                 │" << endl;
                    cout << "│ [L] Logout                                 │" << endl;
                    cout << "└────────────────────────────────────────────┘" << endl;
                    inputMessage();
                    cin >> pilihanAdmin;
                    adminMenuHandler(pilihanAdmin, L, currentUser, A, U, contentPerPage);
                }
            } else {
                relasiMLLAddress currentSong = nullptr;
                bool isPlaying = false;
                string pilihanHomePage;
                while (currentUser != nullptr) {
                    clearScreen();
                    homePage(currentUser, box_width);
                    nowPlays(isPlaying, box_width, currentSong);
                    inputMessage();
                    cin >> pilihanHomePage;
                    userHomePageHandler(pilihanHomePage, currentUser, currentSong, isPlaying, L, A, contentPerPage, box_width);
                }
            }
            clearScreen();
            cout << "Anda telah logout." << endl;
        }
    }
    return 0;
}
