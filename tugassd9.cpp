#include <iostream>
#include <stack>
#include <string>

class PengolahKata {
public:
    void ketik(const std::string& teks) {
        undoStack.push(teksSekarang);
        teksSekarang += teks;
        while (!redoStack.empty()) {
            redoStack.pop();
        }
        std::cout << "Teks saat ini: " << teksSekarang << std::endl;
    }

    void undo() {
        if (undoStack.empty()) {
            std::cout << "Undo stack kosong. Tidak ada yang bisa di-undo." << std::endl;
            return;
        }
        redoStack.push(teksSekarang);
        teksSekarang = undoStack.top();
        undoStack.pop();
        std::cout << "Undo dilakukan. Teks saat ini: " << teksSekarang << std::endl;
    }

    void redo() {
        if (redoStack.empty()) {
            std::cout << "Redo stack kosong. Tidak ada yang bisa di-redo." << std::endl;
            return;
        }
        undoStack.push(teksSekarang);
        teksSekarang = redoStack.top();
        redoStack.pop();
        std::cout << "Redo dilakukan. Teks saat ini: " << teksSekarang << std::endl;
    }

    bool isEmptyUndo() const {
        return undoStack.empty();
    }

    bool isEmptyRedo() const {
        return redoStack.empty();
    }

private:
    std::string teksSekarang;
    std::stack<std::string> undoStack;
    std::stack<std::string> redoStack;
};

void tampilkanMenu() {
    std::cout << "\n=== Menu Pengolah Kata ===\n";
    std::cout << "1. Ketik\n";
    std::cout << "2. Undo\n";
    std::cout << "3. Redo\n";
    std::cout << "4. IsEmptyUndo\n";
    std::cout << "5. IsEmptyRedo\n";
    std::cout << "6. Keluar\n";
    std::cout << "Pilih perintah (1-6): ";
}

int main() {
    PengolahKata pk;
    std::string perintah, teks;

    while (true) {
        tampilkanMenu();
        std::cin >> perintah;

        if (perintah == "1") {
            std::cout << "Masukkan teks: ";
            std::cin.ignore();  // Mengabaikan karakter newline yang tersisa di buffer input
            std::getline(std::cin, teks);
            pk.ketik(teks);
        } else if (perintah == "2") {
            pk.undo();
        } else if (perintah == "3") {
            pk.redo();
        } else if (perintah == "4") {
            if (pk.isEmptyUndo()) {
                std::cout << "Undo stack kosong." << std::endl;
            } else {
                std::cout << "Undo stack tidak kosong." << std::endl;
            }
        } else if (perintah == "5") {
            if (pk.isEmptyRedo()) {
                std::cout << "Redo stack kosong." << std::endl;
            } else {
                std::cout << "Redo stack tidak kosong." << std::endl;
            }
        } else if (perintah == "6") {
            std::cout << "Keluar dari program.\n";
            break;
        } else {
            std::cout << "Perintah tidak dikenal. Silakan coba lagi." << std::endl;
        }
    }

    return 0;
}
