#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    struct DaftarTransfer* next;
};

// Struct untuk kepala linked list daftar transfer
struct TransferList {
    struct DaftarTransfer* head;
};

// Struct untuk node stack
struct Node {
    struct History data;
    struct Node* next;
};

// Struct untuk kepala stack
struct Stack {
    struct Node* top;
};

//antraian mendapatkan driver
struct goridePerjalananQueue {
    char nomorPerjalanan[50][50];
    int max;
    int front;
    int rear;
};

//struct goride
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

typedef struct NodeKendaraan {
    char jenis[20];
    int waktuDisplay[2];
    int penumpang;
    int harga;
    struct NodeKendaraan* next;
} NodeKendaraan;

typedef struct NodePembayaran {
    char jenis[30];
    char deskripsi[50];
    struct NodePembayaran* next;
} NodePembayaran;

typedef struct NodeDiskon {
    int jumlah;
    char kadarluarsa[9];
    char jenisKendaraan1[20];
    char jenisKendaraan2[20];
    struct NodeDiskon* next;
} NodeDiskon;

// Linked list node for trip queue
typedef struct NodePerjalananQueue {
    char nomorPerjalanan[50];
    struct NodePerjalananQueue* next;
} NodePerjalanan;

// memperbarui saldo pengguna setelah top up
void updateSaldo(int* uangUser, int nominalTopup);

// // Daftar transfer
// struct DaftarTransfer* createTransferNode(char* nama, char* jenis, char* nomorRek);
// void addTransfer(struct TransferList* list, char* nama, char* jenis, char* nomorRek);
// void displayTransferList(struct TransferList* list);
// void freeTransferList(struct TransferList* list);

// //Stack history
// struct Node* createNode(struct History data);
// void initStack(struct Stack* stack);
// int isEmpty(struct Stack* stack);
// void push(struct Stack* stack, struct History data);
// void pop(struct Stack* stack);
// void peek(struct Stack* stack);

// // Prototipe untuk top up
// void topUp(struct Stack* historyStack, int nominalTopup, char* deskripsi, int* uangUser);

// // Prototipe untuk menampilkan semua entri dalam stack history
// void displayHistory(struct Stack* historyStack);
// void goPayMenu(struct Stack* historyStack, int* uangUser, struct TransferList* transferList);

void gorideFunc(int* uangUser);
int gorideWriteMap(struct DestinasiPilihan destinasi);
int gorideUpdatemap(struct DestinasiPilihan destinasi);
void gorideDisplayMap();

//fucntions
struct DestinasiPilihan gorideDestination();
char** pilihKendaraan();
char* caraPembayaran(int uangUser);
int diskon(char pemilihanKendaraan[20]);
void queuePemesanan();

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

void addKendaraan(NodeKendaraan** head, char* jenis, int waktuDisplay1, int waktuDisplay2, int penumpang, int harga) {
    NodeKendaraan* newNode = (NodeKendaraan*)malloc(sizeof(NodeKendaraan));
    strcpy(newNode->jenis, jenis);
    newNode->waktuDisplay[0] = waktuDisplay1;
    newNode->waktuDisplay[1] = waktuDisplay2;
    newNode->penumpang = penumpang;
    newNode->harga = harga;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        NodeKendaraan* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void addPembayaran(NodePembayaran** head, const char* jenis, const char* deskripsi) {
    NodePembayaran* newNode = (NodePembayaran*)malloc(sizeof(NodePembayaran));
    strcpy(newNode->jenis, jenis);
    strcpy(newNode->deskripsi, deskripsi);
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        NodePembayaran* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void addDiskon(NodeDiskon** head, int jumlah, const char* jenisKendaraan1, const char* jenisKendaraan2, const char*  kadarluarsa) {
    NodeDiskon* newNode = (NodeDiskon*)malloc(sizeof(NodeDiskon));
    newNode->jumlah = jumlah;
    strcpy(newNode->jenisKendaraan1, jenisKendaraan1);
    strcpy(newNode->jenisKendaraan2, jenisKendaraan2);
    strcpy(newNode->kadarluarsa, kadarluarsa);
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        NodeDiskon* last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newNode;
    }
}

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
    struct NodeDestinasi *current;
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
    char **kendaraan = pilihKendaraan();

    //displaying pembayaran
    char *bayar = caraPembayaran(*uangUser);

    //antrian pemesanan
    queuePemesanan();  

    //pilih diskon
    int JmlhDiskon = diskon(kendaraan[0]);
    gorideUpdatemap(destinasiPilihan);
    
    gorideDisplayMap();
    printf("Anda telah sampai pada destinasi\n");
    printf("Destinasi: %s\n", destinasiPilihan.chosenNama);
    printf("Kendaraan: %s - %d\n", kendaraan[0], atoi(kendaraan[1]));
    printf("Pembayaran: %s\n", bayar);
    printf("Diskon: %d\n\n", JmlhDiskon);
    *uangUser -= (atoi(kendaraan[1]) - JmlhDiskon);
    printf("Pembayaran akhir: %d\n", *uangUser);
    free(kendaraan[0]);
    free(kendaraan[1]);
    free(kendaraan);
    free(bayar);
    printf("Press any key to go back\n"); getchar();
    return;
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


//Linked List (LL)
//kendaaraan
char** pilihKendaraan() {
    NodeKendaraan* head = NULL;
    addKendaraan(&head, "GoRide", 3, 7, 1, 33500);
    addKendaraan(&head, "GoRide Comfort", 1, 4, 1, 36500);
    addKendaraan(&head, "GoCar", 3, 7, 4, 44500);
    addKendaraan(&head, "GoCar Hemat", 8,12, 4, 39500);
    addKendaraan(&head, "GoBlueBird", 3, 7, 6, 90000);
    addKendaraan(&head, "GoCar XL", 3, 7, 6, 52000);
    // Display 
    int choice = 0, index = 1;
    NodeKendaraan* current = head;
    while (current != NULL) {
        printf("%d. %s %d-%dmin %d penumpang Rp%d\n", index++, current->jenis, current->waktuDisplay[0], current->waktuDisplay[1], current->penumpang, current->harga);
        current = current->next;
    }

    // inpput
    printf("Choose a vehicle (number): ");
    scanf("%d", &choice);
    getchar(); 
    
    char** chosenVehicle = (char**)malloc(2 * sizeof(char*));
    chosenVehicle[0] = (char*)malloc(20 * sizeof(char));
    chosenVehicle[1] = (char*)malloc(15 * sizeof(char));

    current = head;
    index = 1;
    while (current != NULL) {
        if (index == choice) {
            strcpy(chosenVehicle[0], current->jenis);
            sprintf(chosenVehicle[1], "%d", current->harga);
            break;
        }
        current = current->next;
        index++;
    }

    // Free the linked list
    while (head != NULL) {
        NodeKendaraan* next = head->next;
        free(head);
        head = next;
    }

    return chosenVehicle;
}

//pembayaran
char* caraPembayaran(int uangUser) {
    NodePembayaran* head = NULL;

    addPembayaran(&head, "Gopay", " ");
    addPembayaran(&head, "Gopay Tabungan by Jago", "Activate & earn 2.5 interest while spending");
    addPembayaran(&head, "Creadit or debit card", "Visa, Mastercard, AMEX, and JCB");
    addPembayaran(&head, "LinkAja", " ");
    addPembayaran(&head, "Jago Pockets", "Jago Bank account ");
    addPembayaran(&head, "cash", " ");

    // Display payment methods and get user choice
    int choice = 0, index = 1;
    NodePembayaran* current = head;
    printf("Available balance: Rp %d\n", uangUser);
    while (current != NULL) {
        printf("%d. %s\n", index++, current->jenis);
        current = current->next;
    }

    printf("Choose a payment method (number): ");
    scanf("%d", &choice);
    getchar(); 

    // Iterate again to find the chosen method
    char *chosenMethod = malloc(sizeof(char) * 30);
    current = head;
    index = 1;
    while (current != NULL) {
        if (index == choice) {
            strcpy(chosenMethod, current->jenis);
            break;
        }
        current = current->next;
        index++;
    }

    // Free the linked list
    while (head != NULL) {
        NodePembayaran* next = head->next;
        free(head);
        head = next;
    }

    return chosenMethod; 
}

//diskon
int diskon(char pemilihanKendaraan[20]) {
    NodeDiskon* head = NULL;

    addDiskon(&head, 10000, "GoCar", "GoCar XL",  "17-03-24");
    addDiskon(&head, 10000, "GoCar", "GoCar XL",  "17-03-24");
    addDiskon(&head, 5000, "GoRide", "Goride Comfort", "17-03-24");
    addDiskon(&head, 5000, "GoRide", "Goride Comfort", "17-03-24");

    // Display discounts 
    int choice = 0, index = 1;
    NodeDiskon* current = head;
    while (current != NULL) {
        printf("%d. Rp%d off for %s %s\n", index++, current->jumlah, current->jenisKendaraan1, current->jenisKendaraan2);
        current = current->next;
    }

    //input
    printf("Choose a discount (number, 0 to skip): ");
    scanf("%d", &choice);
    getchar(); 

    int selectedDiscount = 0;
    if (choice != 0) {
        current = head;
        index = 1;
        while (current != NULL) {
            if (index == choice && (strcmp(current->jenisKendaraan1, pemilihanKendaraan) == 0 || strcmp(current->jenisKendaraan2, pemilihanKendaraan) == 0)) {
                selectedDiscount = current->jumlah;
                break;
            }
            current = current->next;
            index++;
        }
    }
    printf("\n%d\n\n", selectedDiscount);

    // Free the linked list
    while (head != NULL) {
        NodeDiskon* next = head->next;
        free(head);
        head = next;
    }

    return selectedDiscount;
}