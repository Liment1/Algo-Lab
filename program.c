#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

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
void gopayFunc(int* uangUser);
void gorideFunc(int* uangUser);
void gofoodFunc(int* uangUser);

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
    return;
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
