#include "spotify.h"
#include <iostream>
using namespace std;

int main() {
    Library L;
    Users U;
    Artists A;
    createLibrary(L);
    createUsers(U);
    createArtists(A);

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
            if (currentUser->info.isAdmin) {
                // Admin functionalities can be added here
            } else {
                int page = 1;
                int contentPerPage = 5;
                homePage(currentUser, box_width);

                string pilihanHomePage;
                cin >> pilihanHomePage;
                if (pilihanHomePage == "L") {
                    currentUser = nullptr;
                } else if (pilihanHomePage == "1") {
                    displayLibrary(L, page, contentPerPage);
                } else if (pilihanHomePage == "2") {
                    displayArtist(A, page, contentPerPage);
                } else if (pilihanHomePage == "3") {
                    displayPlaylists(currentUser, page, contentPerPage);
                }
            }
        }
    }
    return 0;
}
