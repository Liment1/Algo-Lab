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

struct gorideKendaraan {
    char jenis[20];
    int waktuDisplay[2];
    int penumpang;
    int harga;
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

struct gorideDiskon{
    int jumlah;
    char kadarluarsa[9];
    char jenisKendaraan1[20];
    char jenisKendaraan2[20];
};

struct gofoodStruct {

};

//main function
void gopayFunc(int* uangUser);
void gorideFunc(int* uangUser);
void gofoodFunc(int* uangUser);

//goride function
int gorideDestination(struct gorideDestinasi *destinasi);
void gorideDisplayMap();

//function stack
void arrPushGorideDes(struct gorideDestinasi *goride, char nama[], char alamat[]);
void queuePemesanan();

int caraPembayaran(int uangUser) {
    int pilihan;
    struct goridePembayaran pembayaran[] = {
        {"Gopay", ""},
        {"GoPay Tabungan by Jago", "Activate & earn 2.5 interest while spending "},
        {"Creadit or debit card", "Visa, Mastercard, AMEX, and JCB"},
        {"LinkAja", ""}, {"Jago Pockets", "Jago Bank account"}, 
        {"cash", ""}
    };
    sprintf(pembayaran[0].deskripsi, "%s%d", "Balance : " , uangUser);

    int numPembayaran = sizeof(pembayaran) / sizeof(pembayaran[0]);
    for(int i = 0; i < numPembayaran; i++) {
        printf("%d. %s - %s\n", i+1, pembayaran[i].jenis, pembayaran[i].deskripsi);
    }
    char input[30];
    printf("Masukkan jenis payment (string): ");
    scanf("%[^\n]", input); getchar();

    for (int i = 0; i < numPembayaran; i++) {
        if (strcmp(pembayaran[i].jenis, input) == 0) {  
            printf("Payment method found: %s - %s\n\n", pembayaran[i].jenis, pembayaran[i].deskripsi);
            return 0;
        }
    }
    printf("Payment method not found.\n");
    return 1;
}

int diskon() {
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
    char input[30];
    printf("Pilih jumlah diskon (int): ");
    scanf("%[^\n]", input); getchar();
    
    while(1){
        for (int i = 0; i < numDiskon; i++) {
            if (diskon[i].jumlah == atoi(input)) {  
                return diskon[i].jumlah;
            }
        }
        printf("Diskon not found.\n");
    }  
    return 1;
}

// //linked list
// struct gorideDestinasiNode {
//     char nama[50];
//     char alamat[300];
//     struct gorideDestinasiNode *next;
// };

// struct goridePerjalananQueueNode {
//     char nomorPerjalanan[50];
//     struct goridePerjalananQueueNode *next;
// };

// struct goridePerjalananQueue {
//     struct goridePerjalananQueueNode *front;
//     struct goridePerjalananQueueNode *rear;
// };

// void freeGorideDestinasiList(struct gorideDestinasiNode *head) {
//     struct gorideDestinasiNode *tmp;
//     while (head != NULL) {
//         tmp = head;
//         head = head->next;
//         free(tmp);
//     }
// }

// void freeGoridePerjalananQueue(struct goridePerjalananQueue *queue) {
//     struct goridePerjalananQueueNode *tmp;
//     while (queue->front != NULL) {
//         tmp = queue->front;
//         queue->front = queue->front->next;
//         free(tmp);
//     }
//     queue->rear = NULL; // After clearing, rear should also be NULL.
// }


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
    int cek;

    //goride array variable
    struct gorideDestinasi destinasiStruct;
    destinasiStruct.max = 50;
    destinasiStruct.front = 0;

    //main function 
    // Call the caraPembayaran function
    printf("Goride\n");
    printf("Where would you like to go?\n");
    gorideDisplayMap();
    if (gorideDestination(&destinasiStruct) == 1)
        return;
    caraPembayaran(*uangUser);
    queuePemesanan(); 
    int dis = diskon();
    *uangUser -= dis;
    return;
}

//goride function
int gorideDestination(struct gorideDestinasi *destinasi) {
    int i = 1;
    int choice = 0;
    char nama[50], alamat[100];
    FILE *fdes = fopen("C:\\Users\\User\\OneDrive\\Attachments\\Documents\\kuliah\\mata_perkuliahan\\Semester2\\DatStrucALgo\\UTS\\UTS_ASD_B_00000079266_WelliamPrasetioHoedoto\\goridetxtFiles\\destinasi.txt", "r");
    while(!feof(fdes)){
        fscanf(fdes, "%[^#]#%[^\n]\n", nama, alamat);
        arrPushGorideDes(destinasi, nama, alamat);
        i++;
    }
    fclose(fdes);

    if (destinasi->front == 0) {
        printf("Tidak ada destinasi.\n");
        destinasi->front = 0;
        return 1;   
    }


    //display dari terakhir (POP)
    struct gorideDestinasi destinasiTemp = *destinasi;
    for(int i = 0, j = 1; destinasiTemp.front-1 >= i; destinasiTemp.front--, j++) {
        if (strcmp(" ", destinasiTemp.alamat[destinasiTemp.front-1]) == 0)
            printf("%d. %s\n", j, destinasiTemp.nama[destinasiTemp.front-1]);
        else
            printf("%d. %s - %s\n", j, destinasiTemp.nama[destinasiTemp.front-1], destinasiTemp.alamat[destinasiTemp.front-1]);
    }

    printf("Pilih destinasi(0 to cancel): ");
    scanf("%d", &choice); getchar();
    printf("\n");

    if (choice < 0 || choice > destinasi->front) {
        printf("Pilihan Invalid.\n");
        destinasi->front = 0;
        return 1;
    } 
    else if (choice == 0) {
        destinasi->front = 0;
        return 2;
    } 
    else {
        //pop sampai ketemu
        int tempMax = destinasi->front;
        while(!(destinasi->front == 0)){
            if (destinasi->front == (tempMax - choice)){
                if (strcmp(" ", destinasi->alamat[destinasi->front]) == 0)
                    printf("Destinasi yang dipilih:  %s\n\n", destinasi->nama[destinasi->front]);
                else
                    printf("Destinasi yang dipilih: %s - %s\n\n", destinasi->nama[destinasi->front], destinasi->alamat[destinasi->front]);
            }
            destinasi->front = destinasi->front-1;
        }
    }
    destinasi->front = 0;
    return 0;
}

void arrPushGorideDes(struct gorideDestinasi *goride, char nama[], char alamat[]) {
    if (goride->front >= goride->max) {
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

