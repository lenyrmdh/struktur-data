#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "perpustakaan";
unsigned int port = 3307;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Berhasil terhubung ke database." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

void tambah_buku(const string& judul, const string& penulis, const string& penerbit, int tahun) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO buku (judul, penulis, penerbit, tahun) VALUES ('" << judul << "', '" << penulis << "', '" << penerbit << "', " << tahun << ")";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Gagal menambah buku: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

void tampilkan_buku() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM buku")) {
            cerr << "Gagal menampilkan buku: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "Gagal mengambil hasil: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Judul: " << row[1] << ", Penulis: " << row[2] << ", Penerbit: " << row[3] << ", Tahun: " << row[4] << ", Status: " << (row[5][0] == '0' ? "Tersedia" : "Dipinjam") << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void perbarui_buku(int buku_id, const string& judul, const string& penulis, const string& penerbit, int tahun) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE buku SET judul = '" << judul << "', penulis = '" << penulis << "', penerbit = '" << penerbit << "', tahun = " << tahun << " WHERE id = " << buku_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Gagal memperbarui buku: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil diperbarui." << endl;
        }
        mysql_close(conn);
    }
}

void hapus_buku(int buku_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM buku WHERE id = " << buku_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Gagal menghapus buku: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

void pinjam_buku(int buku_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE buku SET is_borrowed = 1 WHERE id = " << buku_id << " AND is_borrowed = 0";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Gagal meminjam buku: " << mysql_error(conn) << endl;
        } else {
            if (mysql_affected_rows(conn) > 0) {
                cout << "Buku berhasil dipinjam." << endl;
            } else {
                cout << "Buku sudah dipinjam atau tidak tersedia." << endl;
            }
        }
        mysql_close(conn);
    }
}

void menu_admin() {
    int pilihan;
    while (true) {
        cout << "\nMenu Admin:\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Semua Buku\n";
        cout << "3. Perbarui Buku\n";
        cout << "4. Hapus Buku\n";
        cout << "5. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            string judul, penulis, penerbit;
            int tahun;
            cout << "Masukkan judul: ";
            cin.ignore();
            getline(cin, judul);
            cout << "Masukkan penulis: ";
            getline(cin, penulis);
            cout << "Masukkan penerbit: ";
            getline(cin, penerbit);
            cout << "Masukkan tahun: ";
            cin >> tahun;
            tambah_buku(judul, penulis, penerbit, tahun);
        } else if (pilihan == 2) {
            tampilkan_buku();
        } else if (pilihan == 3) {
            int buku_id;
            string judul, penulis, penerbit;
            int tahun;
            cout << "Masukkan ID buku yang akan diperbarui: ";
            cin >> buku_id;
            cin.ignore();
            cout << "Masukkan judul baru: ";
            getline(cin, judul);
            cout << "Masukkan penulis baru: ";
            getline(cin, penulis);
            cout << "Masukkan penerbit baru: ";
            getline(cin, penerbit);
            cout << "Masukkan tahun baru: ";
            cin >> tahun;
            perbarui_buku(buku_id, judul, penulis, penerbit, tahun);
        } else if (pilihan == 4) {
            int buku_id;
            cout << "Masukkan ID buku yang akan dihapus: ";
            cin >> buku_id;
            hapus_buku(buku_id);
        } else if (pilihan == 5) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

void menu_user() {
    int pilihan;
    while (true) {
        cout << "\nMenu User:\n";
        cout << "1. Tampilkan Semua Buku\n";
        cout << "2. Pinjam Buku\n";
        cout << "3. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            tampilkan_buku();
        } else if (pilihan == 2) {
            int buku_id;
            cout << "Masukkan ID buku yang akan dipinjam: ";
            cin >> buku_id;
            pinjam_buku(buku_id);
        } else if (pilihan == 3) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

int main() {
    int peran;
    cout << "Masukkan peran (1 untuk Admin, 2 untuk User): ";
    cin >> peran;

    if (peran == 1) {
        menu_admin();
    } else if (peran == 2) {
        menu_user();
    } else {
        cout << "Peran tidak valid. Keluar." << endl;
    }

    return 0;
}