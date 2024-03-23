#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

//gopay
// Struct untuk data history
struct History {
    int tanggal;
    char jenis[50];
    int jumlah;
    char deskripsi[50];
    char metode_pembayaran[50];
    char bank[50];
    char deskripsiBank[50];
};

// Struct untuk daftar transfer
struct DaftarTransfer {
    char namaTransfer[50];
    char jenisTransfer[50];
    char nomorRekTransfer[50];
};

// Struct untuk array stack
struct Stack {
    struct History history[100];
    int top;
};

//goride
//stack of destinasi
struct gorideDestinasi {
    char nama[50][50];
    char alamat[50][300];
    int max;
    int front;
    int lokasix[50];
    int lokasiy[50];
    char status[50];
};
struct DestinasiPilihan {
    char chosenNama[50];
    char chosenAlamat[300];
    int lokasix;
    int lokasiy;
    int isValid;
};

//jenis kendaraan
struct gorideKendaraan {
    char jenis[20];
    int waktuDisplay[2];
    int penumpang;
    int harga;
};
struct KendaraanPilihan {
    struct gorideKendaraan chosenKendaraan;
    int isValid;
};

//antraian mendapatkan driver
struct goridePerjalananQueue {
    char nomorPerjalanan[50][50];
    int max;
    int front;
    int rear;
};

//pembayaran
struct goridePembayaran {
    char jenis[30];
    char deskripsi[50];
};
struct PembayaranPilihan {
    struct goridePembayaran chosenPembayaran;
    int isValid;
};

//diskon
struct gorideDiskon{
    int jumlah;
    char kadarluarsa[9];
    char jenisKendaraan1[20];
    char jenisKendaraan2[20];
};
struct DiskonPilihan {
    struct gorideDiskon diskonPilihan;
    int isValid; 
};

//main function
void goPayMenu(struct Stack* historyStack, int* uangUser, struct DaftarTransfer* transferList, int* transferCount);
void gorideFunc(int* uangUser);

//gopayfunction
// Update uangUser
void updateSaldo(int* uangUser, int nominalTopup);

// stack
void initStack(struct Stack* stack);
int isEmpty(struct Stack* stack);
int isFull(struct Stack* stack);
void push(struct Stack* stack, struct History data);
void pop(struct Stack* stack);
void peek(struct Stack* stack);

//topup
void topUp(struct Stack* historyStack, int nominalTopup, char* deskripsi, int* uangUser);
void displayHistory(struct Stack* historyStack);

//goride function
struct DestinasiPilihan gorideDestination();
void gorideDisplayMap();
int gorideWriteMap(struct DestinasiPilihan destinasi);
int gorideUpdatemap(struct DestinasiPilihan destinasi);

struct KendaraanPilihan  pilihKendaraan();
struct PembayaranPilihan caraPembayaran(int uangUser);
void queuePemesanan();
struct DiskonPilihan diskon(char pemilihanKendaraan[20]);

//function for stack
void arrPushGorideDes(struct gorideDestinasi *goride, char nama[], char alamat[], int x, int y, char status);

//function for queue
void queuePemesanan();

int main(){
    //gopay
    int uangUser =18790;
    struct Stack historyStack;
    initStack(&historyStack);
    int transferCount = 0;
    struct DaftarTransfer transferList[20];
    // Dummy data untuk di-push ke stack
    struct History data1 = { 20240320, "Pembelian", 20000, "Pembelian Makanan di Restoran A", "Go-Pay" };
    struct History data2 = { 20240319, "Pembelian", 50000, "Pembelian makanan di Dina Raos", "Go-Pay" };
    struct History data3 = { 20240318, "Pembayaran", 35000, "Pembayaran Tagihan Listrik Bulan Ini", "Go-Pay" };
    // Push data dummy ke stack history
    push(&historyStack, data1);
    push(&historyStack, data2);
    push(&historyStack, data3);

    FILE *flogo = fopen("logo.txt", "r");
    char logoChar;
    int cek;
    while(!feof(flogo)){
        fscanf(flogo, "%c", &logoChar);;
        printf("%c", logoChar);
    }
    fclose(flogo);
    
    //calling main function
    while(1){
        printf("\nWelcome to Gojek\n");
        printf("Pick a feature: \n");
        printf("1. gopay\n");
        printf("2. Goride\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &cek);
        printf("\n");
        if(cek == 1){
           goPayMenu(&historyStack, &uangUser, transferList, &transferCount);
        }
        else if(cek == 2){
            gorideFunc(&uangUser);
        }
        else if(cek == 0){
            break;
        }        
    }
    return 0;
}

void gorideFunc(int* uangUser){
    int cek; 
    int dis; 
    //main function 
    //displayting map
    printf("Goride\n");
    printf("Where would you like to go?\n");
    gorideDisplayMap();
    //displaying destination
    
    struct DestinasiPilihan destinasiPilihan = gorideDestination();
    if (!destinasiPilihan.isValid) {
        printf("Invalid destination.\n");
        return;
    }

    //displayting map
    gorideWriteMap(destinasiPilihan);
    gorideDisplayMap();

    //pilih kendaraan
    struct KendaraanPilihan kendaraanPilihan = pilihKendaraan();
    if (!kendaraanPilihan.isValid) {
        printf("Invalid vehicle.\n");
        return;
    }

    //displaying pembayaran
    struct PembayaranPilihan pembayaranPilihanS = caraPembayaran(*uangUser);
    if (!pembayaranPilihanS.isValid) {
        printf("Invalid payment.\n");
        return;
    }

    //antrian pemesanan
    queuePemesanan();  

    //pilih diskon
    struct DiskonPilihan diskonPilihanS = diskon(kendaraanPilihan.chosenKendaraan.jenis);
    if (!diskonPilihanS.isValid) {
        printf("Invalid discount choice. Exiting.\n");
        return;
    }
    gorideUpdatemap(destinasiPilihan);
    
    gorideDisplayMap();
    printf("Anda telah sampai pada destinasi\n");
    printf("Destinasi: %s\n", destinasiPilihan.chosenNama);
    printf("Kendaraan: %s - %d\n", kendaraanPilihan.chosenKendaraan.jenis, kendaraanPilihan.chosenKendaraan.harga);
    printf("Pembayaran: %s\n", pembayaranPilihanS.chosenPembayaran.jenis);
    printf("Diskon: %d\n\n", diskonPilihanS.diskonPilihan.jumlah);
    *uangUser -= (kendaraanPilihan.chosenKendaraan.harga - diskonPilihanS.diskonPilihan.jumlah);
    printf("Pembayaran akhir: %d\n", *uangUser);
    printf("Press any key to go back\n"); getchar();
    return;
}

//gopay
// Fungsi untuk menampilkan menu Go-Pay
void goPayMenu(struct Stack* historyStack, int* uangUser, struct DaftarTransfer* transferList, int* transferCount) {
    int choice;

    printf("\nMenu Go-Pay:\n");
    printf("Saldo saat ini: Rp %d,%02d\n", *uangUser, 00);
    printf("1. History\n");
    printf("2. Top Up\n");
    printf("3. Transfer ke Bank\n");
    printf("4. Kembali ke Menu Utama\n");
    printf("Pilih: ");
    scanf("%d", &choice);

    if (choice == 1) {
        // Tampilkan history
        displayHistory(historyStack);
    } else if (choice == 2) {
        // Fungsi untuk top up
        int nominalTopup;
        printf("Masukkan nominal top up: Rp ");
        scanf("%d", &nominalTopup);

        int metode;
        printf("Pilih metode pembayaran:\n");
        printf("1. Alfa-Mart\n");
        printf("2. Bank\n");
        printf("Pilih: ");
        scanf("%d", &metode);

        char deskripsi[50];
        if (metode == 1) {
            strcpy(deskripsi, "Alfa-Mart");
        } else if (metode == 2) {
            int bankChoice;
            printf("Pilih bank:\n");
            printf("1. BCA\n");
            printf("2. BRI\n");
            printf("3. Jago\n");
            printf("4. Mandiri\n");
            printf("5. BNI\n");
            printf("Pilih: ");
            scanf("%d", &bankChoice);

            if (bankChoice == 1) {
                strcpy(deskripsi, "BCA");
            } else if (bankChoice == 2) {
                strcpy(deskripsi, "BRI");
            } else if (bankChoice == 3) {
                strcpy(deskripsi, "Jago");
            } else if (bankChoice == 4) {
                strcpy(deskripsi, "Mandiri");
            } else if (bankChoice == 5) {
                strcpy(deskripsi, "BNI");
            } else {
                printf("Pilihan bank tidak valid!\n");
                return;
            }
        } else {
            printf("Pilihan metode pembayaran tidak valid!\n");
            return;
        }

        // Melakukan top up
        topUp(historyStack, nominalTopup, deskripsi, uangUser);

    } else if (choice == 3) {
        int pilchoice3;
       
        do {
            printf("\nPilihan menu untuk Transfer ke Bank: \nMenu :\n1. Daftar Transfer\n2. Tambah daftar transfer\n3. Transfer Bank langsung\n4. Kembali ke Menu Utama\n\nMenu: ");
            scanf("%d", &pilchoice3);
            if (pilchoice3 == 1) {
                // Tampilkan daftar transfer
                printf("\nDaftar Transfer:\n");
                printf("%-10s | %-20s | %-20s | %-20s\n", "No", "Nama Transfer", "Jenis Transfer", "Nomor Rekening");
                printf("--------------------------------------------------\n");
                for (int i = 0; i < *transferCount; i++) {
                    printf("%-10d | %-20s | %-20s | %-20s\n", i + 1, transferList[i].namaTransfer, transferList[i].jenisTransfer, transferList[i].nomorRekTransfer);
                }
            }

            if (pilchoice3 == 2) {
                if (*transferCount < 20) {
                    printf("\nTambah Daftar Transfer\n");
                    printf("Masukkan nama transfer: "); getchar();
                    scanf("%[^\n]", transferList[*transferCount].namaTransfer);
                    printf("Pilih jenis transfer\n");
                    printf("1. BCA\n");
                    printf("2. BRI\n");
                    printf("3. Jago\n");
                    printf("4. Mandiri\n");
                    printf("5. BNI\n");
                    printf("Pilih: ");
                    int jenis;
                    scanf("%d", &jenis);
                    switch (jenis) {
                        case 1:
                            strcpy(transferList[*transferCount].jenisTransfer, "BCA");
                            break;
                        case 2:
                            strcpy(transferList[*transferCount].jenisTransfer, "BRI");
                            break;
                        case 3:
                            strcpy(transferList[*transferCount].jenisTransfer, "Jago");
                            break;
                        case 4:
                            strcpy(transferList[*transferCount].jenisTransfer, "Mandiri");
                            break;
                        case 5:
                            strcpy(transferList[*transferCount].jenisTransfer, "BNI");
                            break;
                        default:
                            printf("Pilihan tidak valid!\n");
                            return;
                    }
                    printf("Masukkan nomor rekening: "); getchar();
                    scanf("%[^\n]", transferList[*transferCount].nomorRekTransfer);
                    (*transferCount)++;
                    printf("Daftar transfer berhasil ditambahkan.\n");
                } else {
                    printf("Daftar transfer sudah penuh!\n");
                }
            }
            if (pilchoice3 == 3) {
                // Fungsi untuk transfer ke bank
                printf("\nTransfer ke Bank\n");

                printf("\nDaftar Transfer\n");
                printf("%-10s | %-20s | %-20s | %-20s\n", "No ", "Nama Transfer", "Jenis Transfer", "Nomor Rekening");
                printf("--------------------------------------------------\n");
                for (int i = 0; i < *transferCount; i++) {
                    printf("%-10d | %-20s | %-20s | %-20s\n", i + 1, transferList[i].namaTransfer, transferList[i].jenisTransfer, transferList[i].nomorRekTransfer);
                }

                printf("Pilih akun untuk mentransfer (No index yang dipilih): ");
                int transferIndex;
                scanf("%d", &transferIndex);

                if (transferIndex < 1 || transferIndex > *transferCount) {
                    printf("Pilihan tidak valid!\n");
                    return;
                }

                int nominalTransfer;
                printf("Masukkan nominal transfer: Rp ");
                scanf("%d", &nominalTransfer);

                if (nominalTransfer > *uangUser) {
                    printf("\nSaldo tidak cukup!\n");
                    return;
                }

                // Konfirmasi transfer
                printf("\nKonfirmasi transfer:\n");
                printf("Anda akan mentransfer sejumlah Rp %d,%02d kepada %s pada bank %s dengan nomor rekening %s.\n", nominalTransfer, 00, transferList[transferIndex - 1].namaTransfer, transferList[transferIndex - 1].jenisTransfer, transferList[transferIndex - 1].nomorRekTransfer);
                printf("1. Ya\n");
                printf("2. Tidak\n");
                printf("Pilih: ");
                int konfirmasi;
                scanf("%d", &konfirmasi);

                if (konfirmasi == 1) {
                    // Proses transfer
                    *uangUser -= nominalTransfer;

                    // Tambahkan data transfer ke stack history
                    struct History transferData = { 20240320, "Pembayaran", nominalTransfer, "Transfer ke bank", "Go-Pay" };
                    push(historyStack, transferData);

                    printf("Transfer berhasil dilakukan.\n");
                }
                else {
                    printf("Transfer dibatalkan.\n");
                }
            }
        } while (pilchoice3 != 4);

    }
    else if (choice == 4) {
        printf("Kembali ke Menu Utama.\n");
    }
    else {
        printf("Pilihan tidak valid!\n");
    }
}


// Fungsi untuk memperbarui saldo pengguna setelah top up
void updateSaldo(int* uangUser, int nominalTopup) {
    *uangUser += nominalTopup;
}

// Fungsi untuk inisialisasi stack
void initStack(struct Stack* stack) {
    stack->top = -1;
}

// Fungsi untuk mengecek apakah stack kosong
int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

// Fungsi untuk mengecek apakah stack penuh
int isFull(struct Stack* stack) {
    return stack->top == 100 - 1;
}

// Fungsi untuk menambahkan elemen ke stack
void push(struct Stack* stack, struct History data) {
    if (isFull(stack)) {
        printf("Stack overflow!\n");
        return;
    }
    stack->history[++stack->top] = data;
}

// Fungsi untuk menghapus elemen dari stack
void pop(struct Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack underflow!\n");
        return;
    }
    stack->top--;
}

// Fungsi untuk menampilkan elemen teratas dari stack
void peek(struct Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty!\n");
        return;
    }
    struct History data = stack->history[stack->top];
    printf("%10d | %10s | %10d | %-50s | %20s\n", data.tanggal, data.jenis, data.jumlah, data.deskripsi, data.metode_pembayaran);
}

// Fungsi untuk top up
void topUp(struct Stack* historyStack, int nominalTopup, char* deskripsi, int* uangUser) {
    int konfirmasi;
    char metode[50] = "Pembayaran via ";
    printf("\nKonfirmasi top up:\n");
    printf("Nominal: Rp %d,%02d\n", nominalTopup, 00);
    printf("Metode Pembayaran: %s\n", deskripsi);
    printf("Deskripsi: Top up dengan nominal Rp %d,%02d dari %s atas nama Welliam P\n", nominalTopup, 00, deskripsi);
    printf("1. Ya\n");
    printf("2. Tidak\n");
    printf("Pilih: ");
    scanf("%d", &konfirmasi);

    if (konfirmasi == 1) {
        // Proses top up
        updateSaldo(uangUser, nominalTopup);

        strcat(metode, deskripsi);
        
        // Tambahkan data top up ke stack history
        struct History topUpData = {20240321, "Top Up", nominalTopup ," ", " "};
        strcpy(topUpData.deskripsi, metode);
        strcpy(topUpData.metode_pembayaran, deskripsi);
        push(historyStack, topUpData);

        printf("Top up berhasil dilakukan.\n");
    } else {
        printf("Top up dibatalkan.\n");
    }
}

// Fungsi untuk menampilkan semua entri dalam stack history
void displayHistory(struct Stack* historyStack) {
    printf("\nHistory Go-Pay:\n");
    printf("%10s | %10s | %10s | %-50s | %20s\n", "Tanggal", "Jenis", "Jumlah", "Deskripsi", "Metode Pembayaran");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (int i = historyStack->top; i >= 0; i--) {
        struct History data = historyStack->history[i];
        printf("%10d | %10s | %10d | %-50s | %20s\n", data.tanggal, data.jenis, data.jumlah, data.deskripsi, data.metode_pembayaran);
    }
}

//goride function
struct DestinasiPilihan gorideDestination() {
    struct DestinasiPilihan pilihan = {{}, {}, 0};
    struct gorideDestinasi destinasi;
    destinasi.front = 0;
    destinasi.max = 50;
    int choice, x, y;
    char nama[50], alamat[100], status;
    FILE *fdes = fopen("destinasi.txt", "r");
    //reading
    while(!feof(fdes)){
        fscanf(fdes, "%[^#]#%[^#]#%d#%d#%c\n", nama, alamat, &x, &y, &status);
        arrPushGorideDes(&destinasi, nama, alamat, x, y, status);
    }
    fclose(fdes);

    if (destinasi.front == 0) {
        printf("Tidak ada destinasi.\n");
        destinasi.front = 0;
        return pilihan;   
    }


    //display dari terakhir (POP)
    struct gorideDestinasi destinasiTemp = destinasi;
    for(int i = 0, j = 1; destinasiTemp.front-1 >= i; destinasiTemp.front--, j++) {
        if (strcmp(" ", destinasiTemp.alamat[destinasiTemp.front-1]) == 0)
            printf("%d. %s lokasi:(%d,%d)\n", j, destinasiTemp.nama[destinasiTemp.front-1], destinasiTemp.lokasix[destinasiTemp.front-1],destinasiTemp.lokasiy[destinasiTemp.front-1]);
        else
            printf("%d. %s - %s lokasi:(%d,%d)\n", j, destinasiTemp.nama[destinasiTemp.front-1], destinasiTemp.alamat[destinasiTemp.front-1], destinasiTemp.lokasix[destinasiTemp.front-1], destinasiTemp.lokasiy[destinasiTemp.front-1]);
    }
    printf("...\n");
    //input
        printf("Pilih destinasi(0 to cancel): ");
        scanf("%d", &choice); getchar();
        printf("\n");

        if (choice < 0 || choice > destinasi.front) {
            printf("Pilihan Invalid.\n");
            destinasi.front = 0;
            return pilihan;
        } 
        else if (choice == 0) {
            destinasi.front = 0;
            return pilihan;
        } 
        else {
            //pop sampai ketemu
            int tempMax = destinasi.front;
            while(!(destinasi.front < 0)){
                if (destinasi.front == (tempMax - choice)){
                    strcpy(pilihan.chosenNama, destinasi.nama[destinasi.front]);
                    strcpy(pilihan.chosenAlamat, destinasi.alamat[destinasi.front]);
                    pilihan.lokasix = destinasi.lokasix[destinasi.front];
                    pilihan.lokasiy = destinasi.lokasiy[destinasi.front];
                    pilihan.isValid = 1;
                    printf("Nama Tempat: %s\n\n", pilihan.chosenNama);
                }
                destinasi.front = destinasi.front-1;
            }
        }
    destinasi.front = 0;
    return pilihan;
}

//display map
void gorideDisplayMap() {
    char logoChar;
    FILE *fmap = fopen("lokasi.txt", "r");
    while(!feof(fmap)){
        fscanf(fmap, "%c", &logoChar);;
        printf("%c", logoChar);
    }
    fclose(fmap);
    printf("\no = current location\t");
    printf("O = current destination\t");
    printf("X = other destination\n\n");
}

//kendaaraan
struct KendaraanPilihan pilihKendaraan() {
    struct KendaraanPilihan pilihan = {{}, 0}; 
    //copying data
    struct gorideKendaraan kendaraanData[] = {{"GoRide", {3,7}, 1, 33500}, {"GoRide Comfort", {1, 4}, 1, 36500}, {"GoCar", {3, 7}, 4, 44500}, {"GoCar Hemat", {8,12}, 4, 39500}, {"GoBlueBird", {3, 7}, 6, 90000}, {"GoCar XL", {3, 7}, 6, 52000}};
    int numKendaraan = sizeof(kendaraanData) / sizeof(kendaraanData[0]);;

    //display
    for(int i = 0; i < numKendaraan; i++) {
        printf("%d. %s %d-%dmenit\n   %d penumpang Rp%d\n", i+1, kendaraanData[i].jenis, kendaraanData[i].waktuDisplay[0], kendaraanData[i].waktuDisplay[1],  kendaraanData[i].penumpang, kendaraanData[i].harga);
    }

    //input
    int input;
    printf("Masukkan jenis kendaraan (number): ");
    scanf("%d", &input); getchar();
    if (input-1 >= 0 && input-1 < sizeof(kendaraanData)/sizeof(kendaraanData[0])) {
        pilihan.chosenKendaraan = kendaraanData[input-1];
        pilihan.isValid = 1;
        printf("Kendaraan: %s\n\n", pilihan.chosenKendaraan);
    } else {
        printf("Kendaraan tidak ditemukan.\n");
    }
    return pilihan;
}   
void arrPushGorideDes(struct gorideDestinasi *goride, char nama[], char alamat[], int x, int y, char status) {
    if (goride->front < 0 || goride->front >= goride->max) {
        printf("Destination stack is full, cannot add more destinations.\n");
    } else {
        if (status == 'c'){
            return;
        }
        strcpy(goride->nama[goride->front], nama);
        strcpy(goride->alamat[goride->front], alamat);
        goride->lokasix[goride->front] = x;
        goride->lokasiy[goride->front] = y;
        goride->status[goride->front] = status;
        goride->front++;
    }
}

//writemap
int gorideWriteMap(struct DestinasiPilihan destinasi){
    int x = destinasi.lokasix;
    int y = destinasi.lokasiy;
    FILE *fmap = fopen("lokasi.txt", "r");
    char map[21][31];
    int i = 0;

    //read map 
    while (!feof(fmap)) {
        fgets(map[i], sizeof(map[i]), fmap);
        i++;
    }
    fclose(fmap); 

    int lines = i; 

    // Update the map
    for (i = 0; i < lines; ++i) {
        for (int j = 0; map[i][j] != '\0'; ++j) {
            if (i == y && j == x) {
                map[i][j] = 'O'; 
            }
        }
    }

    //writing
    fmap = fopen("lokasi.txt", "w");

    for (i = 0; i < lines; ++i) {
        fprintf(fmap, "%s", map[i]);
    }
    fclose(fmap); 

    return 0; 
}

//pembayaran
struct PembayaranPilihan caraPembayaran(int uangUser) {
    int choice;
    struct PembayaranPilihan pilihan = {{}, 0};
    //copying from this function
    struct goridePembayaran pembayaranData[15];
    while(1){
        int count = 0;
        FILE *fpemb = fopen("pembayarangoride.txt", "r");
        while(!feof(fpemb)){
            fscanf(fpemb, "%[^#]#%[^\n]\n", pembayaranData[count].jenis, pembayaranData[count].deskripsi);
            count++;
        }
        fclose(fpemb);
        sprintf(pembayaranData[0].deskripsi, "%s%d", "Balance : " , uangUser);

        //pilihan
        printf("1. Pilih metode pembayaran\n");
        printf("2. Tambah metode pembayaran\n");
        printf("Pilih menu: ");
        scanf("%d", &choice); getchar();

        if(choice == 1){
            //input
            //display
            for(int i = 0; i < count; i++) {
                if(strcmp(pembayaranData[i].deskripsi, " ") != 0)
                    printf("%d. %s\n   %s\n", i+1, pembayaranData[i].jenis, pembayaranData[i].deskripsi);
                else
                    printf("%d. %s\n", i+1, pembayaranData[i].jenis);
            }
            int input;
            printf("Masukkan jenis pembayaran (number): ");
            scanf("%d", &input); getchar();
            if (input-1 >= 0 && input-1 < sizeof(pembayaranData)/sizeof(pembayaranData[0])) {
                pilihan.chosenPembayaran = pembayaranData[input-1];
                pilihan.isValid = 1;
                printf("Pembayaran: %s\n\n", pilihan.chosenPembayaran);
                return pilihan;
            } else {
                printf("Pembayaran tidak ditemukan.\n");
            }
        }
        else if(choice == 2){
            //append to txt
            printf("Masukkan Jenis Bank/Metode Pembayaran\n");
            scanf(" %[^\n]", pembayaranData[count].jenis); getchar();
            strcpy(pembayaranData[count].deskripsi," ");

            FILE *fpembadd = fopen("pembayarangoride.txt", "a");
            fprintf(fpembadd, "%s#%s\n", pembayaranData[count].jenis,pembayaranData[count].deskripsi);
            fclose(fpembadd);
            count++;
            printf("Metode pembayaran berhasil ditambahkan\n\n");
        }
    }   
    return pilihan;
}

//queue
void queuePemesanan() {
    FILE *fpesan = fopen("pemesanan.txt", "r");
    struct goridePerjalananQueue queue;
    queue.max = 50;
    queue.front = 0;
    queue.rear = 0;
    char pemesanan[30];
    char nomorAntrian[] = "RB-136456-5908191";
    printf("Nomor antrian = %s\n", nomorAntrian);
    if(queue.rear < queue.max){
        while(!feof(fpesan)){
            fscanf(fpesan, "%s\n", pemesanan);
            //enqueue
            strcpy(queue.nomorPerjalanan[queue.rear], pemesanan);
            queue.rear++;
        }
        fclose(fpesan);
        //nomor pemesanan goride
        strcpy(queue.nomorPerjalanan[queue.rear], nomorAntrian);
    }
    else {
        printf("Queue full\n");
        return;
    }

    // Dequeue + seek
    for(queue.front; queue.front <= queue.rear; queue.front++) {
        printf("Antrian Perjalanan: %s\n", queue.nomorPerjalanan[queue.front]);
        sleep(1);
    }
    printf("\n");
    queue.front = 0;
    queue.rear = 0;
    return;
}

//diskon
struct DiskonPilihan diskon(char pemilihanKendaraan[20]) {
    struct DiskonPilihan pilihan = {{}, 0};
    struct gorideDiskon diskon[] = {{5000, "17-03-24", "GoRide", "GoRide Comfort"}, {10000, "17-03-24", "GoCar", "GoCar XL"}, {5000, "17-03-24", "GoRide", "GoRide Comfort"}, {5000, "17-03-24", "GoRide", "GoRide Comfort"}, {10000, "17-03-24", "GoCar", "GoCar XL"}, {10000, "17-03-24", "GoCar", "GoCar XL"}};
    int numDiskon = sizeof(diskon)/sizeof(diskon[0]);
    
    //sorting (bubble)
    for(int i = 0; i < numDiskon; i++) {
        for(int j = i+1; j < numDiskon; j++) {
            if(diskon[i].jumlah < diskon[j].jumlah) {
                struct gorideDiskon temp = diskon[i];
                diskon[i] = diskon[j];
                diskon[j] = temp;
            }
        }
    }
    int input;
    while(1){
        // Display
        for(int i = 0; i < numDiskon; i++) {
            printf("%d. Diskon: Rp%d valid sampai %s untuk %s, %s\n", i+1, diskon[i].jumlah, diskon[i].kadarluarsa, diskon[i].jenisKendaraan1, diskon[i].jenisKendaraan2);
        }
        printf("0.Skip\n");

        //input for search
        printf("Pilih jumlah diskon (number): ");
        scanf("%d", &input); getchar();
        if (input > 0 && input <= numDiskon) {  
            if(strcmp(pemilihanKendaraan, diskon[input-1].jenisKendaraan1) == 0 || strcmp(pemilihanKendaraan, diskon[input-1].jenisKendaraan2) == 0){
                printf("Diskon: %d\n\n", diskon[input-1].jumlah);
                pilihan.diskonPilihan = diskon[input-1];
                pilihan.isValid = 1;
                return pilihan;
            }
            else {
                printf("Diskon tidak tersedia untuk kendaraan tersebut, Mohon coba lagi\n");
            }
        }
        else if (input-1 == -1) {
            printf("Tidak memilih diskon\n");
            pilihan.diskonPilihan.jumlah = 0;
            strcpy(pilihan.diskonPilihan.jenisKendaraan1,"");
            strcpy(pilihan.diskonPilihan.jenisKendaraan2,"");
            strcpy(pilihan.diskonPilihan.kadarluarsa, "");
            pilihan.isValid = 1;
            return pilihan;
        }

        else {
            printf("Diskon not found.\n\n");  
        }
    }
    return pilihan;
}

//update map
int gorideUpdatemap(struct DestinasiPilihan destinasi){
    int x = destinasi.lokasix;
    int y = destinasi.lokasiy;
    FILE *fmap = fopen("lokasi.txt", "r");
    char map[21][31];
    int i = 0;

    //read map 
    while (!feof(fmap)) {
        fgets(map[i], sizeof(map[i]), fmap);
        i++;
    }
    fclose(fmap); 

    int lines = i; 

    // Update the map
    for (i = 0; i < lines; ++i) {
        for (int j = 0; map[i][j] != '\0'; ++j) {
            if (map[i][j] == 'O')
                map[i][j] = 'o';
            else if (map[i][j] == 'o')
                map[i][j] = 'X'; 
        }
    }

    //writing
    fmap = fopen("lokasi.txt", "w");

    for (i = 0; i < lines; ++i) {
        fprintf(fmap, "%s", map[i]);
    }
    fclose(fmap); 

    return 0; 
}
