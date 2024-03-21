#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

//goride
//stack
struct gorideDestinasi {
    char nama[50][50];
    char alamat[50][300];
    int max;
    int front;
};
struct DestinasiPilihan {
    char chosenNama[50];
    char chosenAlamat[300];
    int isValid;
};


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

//queue
struct goridePerjalananQueue {
    char nomorPerjalanan[50][50];
    int max;
    int front;
    int rear;
};

struct goridePembayaran {
    char jenis[30];
    char deskripsi[50];
};
struct PembayaranPilihan {
    struct goridePembayaran chosenPembayaran;
    int isValid;
};

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
void gopayFunc(int* uangUser);
void gorideFunc(int* uangUser);
void gofoodFunc(int* uangUser);

//goride function
struct DestinasiPilihan gorideDestination();
void gorideDisplayMap();
struct KendaraanPilihan  pilihKendaraan();
struct PembayaranPilihan caraPembayaran(int uangUser);
void queuePemesanan();

//function stack
void arrPushGorideDes(struct gorideDestinasi *goride, char nama[], char alamat[]);
void queuePemesanan();

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

struct PembayaranPilihan caraPembayaran(int uangUser) {
    struct PembayaranPilihan pilihan = {{}, 0};
    //copying from this function
    struct goridePembayaran pembayaranData[] = {
        {"Gopay", ""},
        {"GoPay Tabungan by Jago", "Activate & earn 2.5 interest while spending "},
        {"Creadit or debit card", "Visa, Mastercard, AMEX, and JCB"},
        {"LinkAja", ""}, {"Jago Pockets", "Jago Bank account"}, 
        {"cash", ""}
    };
    sprintf(pembayaranData[0].deskripsi, "%s%d", "Balance : " , uangUser);

    //display
    int numPembayaran = sizeof(pembayaranData) / sizeof(pembayaranData[0]);
    for(int i = 0; i < numPembayaran; i++) {
        printf("%d. %s\n   %s\n", i+1, pembayaranData[i].jenis, pembayaranData[i].deskripsi);
    }

    //input
    int input;
    printf("Masukkan jenis pembayaran (number): ");
    scanf("%d", &input); getchar();
    if (input-1 >= 0 && input-1 < sizeof(pembayaranData)/sizeof(pembayaranData[0])) {
        pilihan.chosenPembayaran = pembayaranData[input-1];
        pilihan.isValid = 1;
        printf("Pembayaran: %s\n\n", pilihan.chosenPembayaran);
    } else {
        printf("Pembayaran tidak ditemukan.\n");
    }
    return pilihan;
}

struct DiskonPilihan diskon() {
    struct DiskonPilihan pilihan = {{}, 0};
    struct gorideDiskon diskon[] = {{5000, "17-03-24", "goride", "goride(comfort)"}, {10000, "17-03-24", "gocar", "gocarXL"}, {5000, "17-03-24", "goride", "goride(comfort)"}, {5000, "17-03-24", "goride", "goride(comfort)"}, {10000, "17-03-24", "gocar", "gocarXL"}, {10000, "17-03-24", "gocar", "gocarXL"}};
    int numDiskon = sizeof(diskon)/sizeof(diskon[0]);
    
    for(int i = 0; i < numDiskon; i++) {
        for(int j = i+1; j < numDiskon; j++) {
            if(diskon[i].jumlah > diskon[j].jumlah) {
                struct gorideDiskon temp = diskon[i];
                diskon[i] = diskon[j];
                diskon[j] = temp;
            }
        }
    }

    // Display
    for(int i = 0; i < numDiskon; i++) {
        printf("%d. Diskon: Rp%d valid sampai %s untuk %s, %s\n", i+1, diskon[i].jumlah, diskon[i].kadarluarsa, diskon[i].jenisKendaraan1, diskon[i].jenisKendaraan2);
    }

    //input for search
    int input;
    printf("Pilih jumlah diskon (number): ");
    scanf("%d", &input); getchar();
    if (input-1 >= 0 || input-1 < numDiskon) {  
        printf("Diskon: %d\n\n", diskon[input-1].jumlah);
        pilihan.diskonPilihan = diskon[input-1];
        pilihan.isValid = 1;
        return pilihan;
    }
    printf("Diskon not found.\n\n");  
    return pilihan;
}

int main() {
    //printing logo
    int uangUser = 187150;
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
        printf("3. Gofood\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &cek);
        printf("\n");
        if(cek == 1){
            // gopayFunc();
        }
        else if(cek == 2){
            gorideFunc(&uangUser);
        }
        else if(cek == 3){
            // gofoodFunc();
        }
        else if(cek == 0){
            break;
        }        
    }
    return 0;
}

//main function
void gopayFunc(int* uangUser){
}

void gorideFunc(int* uangUser){
    int cek; int dis;

    //goride array variable
    struct gorideDestinasi destinasiStruct;
    destinasiStruct.max = 50;
    destinasiStruct.front = 0;
    
    //main function 
    printf("Goride\n");
    printf("Where would you like to go?\n");

    //displayting map
    gorideDisplayMap();

    //displaying destination
    struct DestinasiPilihan destinasiPilihan = gorideDestination();
    if (!destinasiPilihan.isValid) {
        printf("Invalid destination.\n");
        return;
    }

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
    struct DiskonPilihan diskonPilihanS = diskon();
    if (!diskonPilihanS.isValid) {
        printf("Invalid discount choice. Exiting.\n");
        return;
    }
    
    printf("Destinasi: %s\n", destinasiPilihan.chosenNama);
    printf("Kendaraan: %s - %d\n", kendaraanPilihan.chosenKendaraan.jenis, kendaraanPilihan.chosenKendaraan.harga);
    printf("Pembayaran: %s\n", pembayaranPilihanS.chosenPembayaran.jenis);
    printf("Diskon: %d\n\n", diskonPilihanS.diskonPilihan.jumlah);
    *uangUser -= (diskonPilihanS.diskonPilihan.jumlah+kendaraanPilihan.chosenKendaraan.harga);
    printf("Pembayaran akhir: %d\n", *uangUser);
    return;
}

//goride function
struct DestinasiPilihan gorideDestination() {
    struct DestinasiPilihan pilihan = {{}, {}, 0};
    struct gorideDestinasi destinasi;
    destinasi.front = 0;
    destinasi.max = 50;
    int choice;
    char nama[50], alamat[100];
    FILE *fdes = fopen("C:\\Users\\User\\OneDrive\\Attachments\\Documents\\kuliah\\mata_perkuliahan\\Semester2\\DatStrucALgo\\UTS\\UTS_ASD_B_00000079266_WelliamPrasetioHoedoto\\goridetxtFiles\\destinasi.txt", "r");
    while(!feof(fdes)){
        fscanf(fdes, "%[^#]#%[^\n]\n", nama, alamat);
        arrPushGorideDes(&destinasi, nama, alamat);
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
            printf("%d. %s\n", j, destinasiTemp.nama[destinasiTemp.front-1]);
        else
            printf("%d. %s - %s\n", j, destinasiTemp.nama[destinasiTemp.front-1], destinasiTemp.alamat[destinasiTemp.front-1]);
    }

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
                strcpy(pilihan.chosenNama, destinasi.nama[choice - 1]);
                strcpy(pilihan.chosenAlamat, destinasi.alamat[choice - 1]);
                pilihan.isValid = 1;
                printf("Nama Tempat: %s\n\n", pilihan.chosenNama);
            }
            destinasi.front = destinasi.front-1;
        }
    }
    destinasi.front = 0;
    return pilihan;
}

void arrPushGorideDes(struct gorideDestinasi *goride, char nama[], char alamat[]) {
    if (goride->front < 0 || goride->front >= goride->max) {
        printf("Destination stack is full, cannot add more destinations.\n");
    } else {
        strcpy(goride->nama[goride->front], nama);
        strcpy(goride->alamat[goride->front], alamat);
        goride->front++;
    }
}

void queuePemesanan() {
    FILE *fpesan = fopen("C:\\Users\\User\\OneDrive\\Attachments\\Documents\\kuliah\\mata_perkuliahan\\Semester2\\DatStrucALgo\\UTS\\UTS_ASD_B_00000079266_WelliamPrasetioHoedoto\\goridetxtFiles\\pemesanan.txt", "r");
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

void gorideDisplayMap() {
    char logoChar;
    FILE *fmap = fopen("C:\\Users\\User\\OneDrive\\Attachments\\Documents\\kuliah\\mata_perkuliahan\\Semester2\\DatStrucALgo\\UTS\\UTS_ASD_B_00000079266_WelliamPrasetioHoedoto\\goridetxtFiles\\lokasi.txt", "r");
    while(!feof(fmap)){
        fscanf(fmap, "%c", &logoChar);;
        printf("%c", logoChar);
    }
    fclose(fmap);
    printf("\no = current location\t");
    printf("O = current destination\t");
    printf("X = other destination\n\n");
}

