#include <stdio.h>
#include <string.h>
#include <math.h>

struct sks {
    int eval1, eval2, eval3, UAS, min, max;
    float avrg;
};

typedef struct {
    long long nrp;
    char nama[100];
    struct sks nilai;
} data;

typedef int (*CompareFunc)(data a, data b);

data Mahasiswa[100];
int  banyakMhs = 0;
float rataMhs;

// Database
void saveToFile(const char *filename);
void loadFromFile(const char *filename);

// Main Menu
void inputData(int edit, long long nrpEdit);
void printDataMenu();
void printNilai();
void editDataMenu();

int main() {
    char menu;
    int exit, showMenu;

    // Load data from database
    loadFromFile("database.bin");

    printf("\n                          Project Semester 1                          ");
    printf("\n+====================================================================+");
    printf("\n|                     oleh, Renza Syamdia Naufal                     |");
    printf("\n|                             3124600019                             |");
    printf("\n+====================================================================+");

    do {
        if (showMenu == 0) {
            printf("\npilih menu: ");
            printf("\n-----------");
            printf("\n  1. Input data mahasiswa");
            printf("\n  2. Print data mahasiswa");
            printf("\n  3. Print nilai mahasiswa");
            printf("\n  4. Database manager");
            printf("\n  5. Exit (also 'x' work)");
            showMenu = 1;
        } else
            printf("\n(use '?' for menu list)");

        printf("\n=> ");
        fflush(stdin);
        scanf("%c", &menu);
        switch (menu) {
            case '1':
                inputData(0, 0);
                break;
            case '2':
                printDataMenu();
                break;
            case '3':
                printNilai();
                break;
            case '4':
                editDataMenu();
                break;
            case '5':
            case 'x':
            case 'X':
                printf("\nExiting Program...");
                exit = 1;
                break;
            case '?':
                showMenu = 0;
                break;
            default:
                printf("\nError! : menu invalid\n");
        }
    } while (exit != 1);

    return 0;
}

void saveToFile(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }
    fwrite(&banyakMhs, sizeof(int), 1, file);
    fwrite(Mahasiswa, sizeof(data), banyakMhs, file);
    fclose(file);
}

void loadFromFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }
    fread(&banyakMhs, sizeof(int), 1, file);
    fread(Mahasiswa, sizeof(data), banyakMhs, file);
    fclose(file);
}

void getNilaiRataMhs() {
    rataMhs = 0;
    float temp = 0;
    for (int i = 0; i < banyakMhs; i++) {
        temp += Mahasiswa[i].nilai.avrg;
    }
    rataMhs = temp/banyakMhs;
}

int isNRPValid(long long nrp) {
    if (nrp < 1000000000LL || nrp > 9999999999LL) {
        printf("\n> NRP tidak valid!\n");
        return 0;
    }

    for (int i = 0; i < banyakMhs; i++) {
        if (Mahasiswa[i].nrp == nrp) {
            printf("\n> NRP sudah digunakan oleh mahasiswa lain!\n");
            return 0;
        }
    }

    return 1;
}

int getMax(int a, int b) {
    return (a > b) ? a : b;
}

int getMin(int a, int b) {
    return (a < b) ? a : b;
}

void inputData(int edit, long long nrpEdit) {
    char addData;

    printf("\n\t\tInput Data Mahasiswa");
    printf("\n=====================================================================\n");

    do {
        if (edit != 1) {
            printf("Data Mahasiswa ke-%d", banyakMhs+1);
            
            long long nrp;
            do {
                printf("\nNRP Mahasiswa : ");
                if (scanf("%lld", &nrp) != 1) {
                    printf("> Input invalid! NRP hanya berisi angka.\n");
                    while (getchar() != '\n');
                    continue;
                }
            } while (!isNRPValid(nrp));
            Mahasiswa[banyakMhs].nrp = nrp;
        } else 
            Mahasiswa[banyakMhs].nrp = nrpEdit;

        fflush(stdin);
        printf("Nama Mahasiswa : ");
        fgets(Mahasiswa[banyakMhs].nama, 100, stdin);

        // Remove 'new line' character
        int length = strlen(Mahasiswa[banyakMhs].nama);
        if (length > 0 && Mahasiswa[banyakMhs].nama[length - 1] == '\n') 
            Mahasiswa[banyakMhs].nama[length - 1] = '\0';
        printf("Nilai Eval 1 : ");
        scanf("%d", &Mahasiswa[banyakMhs].nilai.eval1);
        printf("Nilai Eval 2 : ");
        scanf("%d", &Mahasiswa[banyakMhs].nilai.eval2);
        printf("Nilai Eval 3 : ");
        scanf("%d", &Mahasiswa[banyakMhs].nilai.eval3);
        printf("Nilai UAS   : ");
        scanf("%d", &Mahasiswa[banyakMhs].nilai.UAS);

        Mahasiswa[banyakMhs].nilai.max = getMax(
            Mahasiswa[banyakMhs].nilai.eval1,
            getMax(Mahasiswa[banyakMhs].nilai.eval2,
            getMax(Mahasiswa[banyakMhs].nilai.eval3,
            Mahasiswa[banyakMhs].nilai.UAS))
        );

        Mahasiswa[banyakMhs].nilai.min = getMin(
            Mahasiswa[banyakMhs].nilai.eval1,
            getMin(Mahasiswa[banyakMhs].nilai.eval2,
            getMin(Mahasiswa[banyakMhs].nilai.eval3,
            Mahasiswa[banyakMhs].nilai.UAS))
        );

        Mahasiswa[banyakMhs].nilai.avrg = ceil((Mahasiswa[banyakMhs].nilai.eval1 + Mahasiswa[banyakMhs].nilai.eval2 + Mahasiswa[banyakMhs].nilai.eval3 + Mahasiswa[banyakMhs].nilai.UAS)/4);
        banyakMhs++;

        // Save data to database
        saveToFile("database.bin");

        // Loop input
        if (edit != 1) {
            printf("\nTambah data? [Y/T]: ");
            scanf(" %c", &addData);
        } else addData == 'n';
    } while(addData == 'y' || addData == 'Y');
}

void swapData(data *a, data *b) {
    data temp = *a;
    *a = *b;
    *b = temp;
}

// Menu Sorting
int compareByNRP(data a, data b) {
    return a.nrp - b.nrp;
}

int compareByNama(data a, data b) {
    return strcmp(a.nama, b.nama);
}

int compareByNilaiEval1(data a, data b) {
    return b.nilai.eval1 - a.nilai.eval1;
}

int compareByNilaiEval2(data a, data b) {
    return b.nilai.eval2 - a.nilai.eval2;
}

int compareByNilaiEval3(data a, data b) {
    return b.nilai.eval3 - a.nilai.eval3;
}

int compareByNilaiUAS(data a, data b) {
    return b.nilai.UAS - a.nilai.UAS;
}

int compareByNilaiAkhir(data a, data b) {
    return b.nilai.avrg - a.nilai.avrg;
}

// Sorting Fuction
void sortMahasiswa(CompareFunc compare, char operator) {
    int isAscending = (operator == 'd') ? -1 : 1;

    for (int i = 0; i < banyakMhs - 1; i++) {
        for (int j = 0; j < banyakMhs - i - 1; j++) {
            if (isAscending * compare(Mahasiswa[j], Mahasiswa[j + 1]) > 0) {
                swapData(&Mahasiswa[j], &Mahasiswa[j + 1]);
            }
        }
    }
}

float cekGrade(float nilai, float na, char *nh) {
    if (nilai>=86) {
        strcpy(nh, "A+");
        return na = 4;
    }
    else if (nilai>=81) {
        strcpy(nh, "A-");
        return na = 3.75;
    }
    else if (nilai>=76) {
        strcpy(nh, "AB");
        return na = 3.5;
    }
    else if (nilai>=71) {
        strcpy(nh, "B+");
        return na = 3.25;
    }
    else if (nilai>=66) {
        strcpy(nh, "B");
        return na = 3;
    }
    else if (nilai>=61) {
        strcpy(nh, "BC");
        return na = 2.5;
    }
    else if (nilai>=56) {
        strcpy(nh, "C");
        return na = 2;
    }
    else if (nilai>=41) {
        strcpy(nh, "D");
        return na = 1;
    }
    else {
        strcpy(nh, "E");
        return na = 0;
    }
}

void printData() {
    printf("\n===================================================================================================================");
    printf("\nNo  NRP\t\tNama\t\t\t\t\t\tNilai\t\t\t\t\t   Grade");
    printf("\n    \t\tMahasiswa\t\tEval1\tEval2\tEval3\tUAS\tMIN\tMAX\tRata-rata\tNA\tNH");
    printf("\n===================================================================================================================\n");
    for (int i = 0; i < banyakMhs; i++) {
        printf("%d   ", i+1);
        printf("%-10lld\t", Mahasiswa[i].nrp);
        printf("%-20.20s\t", Mahasiswa[i].nama);
        printf("%3d\t", Mahasiswa[i].nilai.eval1);
        printf("%3d\t", Mahasiswa[i].nilai.eval2);
        printf("%3d\t", Mahasiswa[i].nilai.eval3);
        printf("%3d\t", Mahasiswa[i].nilai.UAS);
        printf("%3d\t", Mahasiswa[i].nilai.max);
        printf("%3d\t", Mahasiswa[i].nilai.min);
        printf("%7.2f\t\t", Mahasiswa[i].nilai.avrg);
        char nilaiH[3];
        float nilaiA = cekGrade(Mahasiswa[i].nilai.avrg, nilaiA, nilaiH);
        printf("%2.2f\t", nilaiA);
        printf("%2s\n", nilaiH);
    }
    printf("===================================================================================================================\n");
    printf("Nilai rata-rata seluruh mahasiswa : %.2f\n", rataMhs);
}

void printDataMenu() {
    getNilaiRataMhs();
    if (banyakMhs == 0) {
        printf("\n> Belum ada data Mahasiswa\n");
        return;
    }
    
    char input[10];
    char menuPrint, sort = '\0';
    int exit, showMenu = 0;


    do {
        printData();

        if (showMenu == 0) {
            printf("\nUbah urutan tampilan?");
            printf("\n---------------------");
            printf("\n  1. NRP");
            printf("\n  2. Nama");
            printf("\n  3. Nilai Akhir");
            printf("\n  4. Tidak (also 'x' work)");
            printf("\n(Use 'a' for asecneding and 'b' for descending. ex: \"3 a\")");
            showMenu = 1;
        } else
            printf("\n(use '?' for menu list)");

        fflush(stdin);
        printf("\nMenu 2 => ");
        fgets(input, sizeof(input), stdin);

        menuPrint = input[0];

        if (strlen(input) > 2) {
            sort = input[2];
        }
        switch(menuPrint) {
            case '1':
                printf("\nsorted by: NRP");
                sortMahasiswa(compareByNRP, sort);
                break; 
            case '2':
                printf("\nsorted by: Nama");
                sortMahasiswa(compareByNama, sort);
                break; 
            case '3':
                printf("\nsorted by: Grade");
                sortMahasiswa(compareByNilaiAkhir, sort);
                break;
            case '4':
            case 'x':
            case 'X':
                exit = 1;
                break;
            case '?':
                showMenu = 0;
                break;
            default:
                printf("\nError! : menu invalid\n");
        }
    } while(exit != 1);
}

void printNilai() {
    if (banyakMhs == 0) {
        printf("\n> Belum ada data Mahasiswa\n");
        return;
    }

    char input[10];
    char menuPrint, sort = 'a';
    int exit, showMenu = 0, nilai;

    const char *categories[] = { "Eval 1", "Eval 2", "Eval 3", "UAS", "Rata-rata" };

    do {
        if (showMenu == 0) {
            printf("\nTampilkan Nilai Mahasiswa");
            printf("\n-------------------------");
            printf("\n  1. Eval 1");
            printf("\n  2. Eval 2");
            printf("\n  3. Eval 2");
            printf("\n  4. UAS");
            printf("\n  5. Rata-rata");
            printf("\n  6. Keluar (also 'x' work)");
            printf("\n(Use 't' for highest and 'r' for lowest. ex: \"3 a\")");
            showMenu = 1;
        } else
            printf("\n(use '?' for menu list)");

        fflush(stdin);
        printf("\nMenu 3 => ");
        fgets(input, sizeof(input), stdin);

        menuPrint = input[0];

        if (strlen(input) > 2) {
            sort = (input[2] == 'r') ? 'd' : 'a';
        }
        switch(menuPrint) {
            case '1':
                sortMahasiswa(compareByNilaiEval1, sort);
                nilai = Mahasiswa[0].nilai.eval1;
                break; 
            case '2':
                sortMahasiswa(compareByNilaiEval2, sort);
                nilai = Mahasiswa[0].nilai.eval2;
                break; 
            case '3':
                sortMahasiswa(compareByNilaiEval3, sort);
                nilai = Mahasiswa[0].nilai.eval3;
                break;
            case '4':
                sortMahasiswa(compareByNilaiUAS, sort);
                nilai = Mahasiswa[0].nilai.UAS;
                break;
            case '5':
                sortMahasiswa(compareByNilaiAkhir, sort);
                nilai = Mahasiswa[0].nilai.avrg;
                break;
            case '6':
            case 'x':
            case 'X':
                exit = 1;
                break;
            case '?':
                showMenu = 0;
                break;
            default:
                printf("\nError! : menu invalid\n");
        }
        if (exit != 1)printf("\nNilai %s %s: %d, oleh %s\n",
                    categories[menuPrint - '1'],
                    (sort == 'a') ? "Tertinggi" : "Rerendah",
                    nilai,
                    Mahasiswa[0].nama
                );
    } while(exit != 1);
}

void editData() {
    long long nrpToEdit;
    int found = 0;

    printf("\nMasukkan NRP mahasiswa yang ingin diedit: ");
    scanf("%lld", &nrpToEdit);

    for (int i = 0; i < banyakMhs; i++) {
        if (Mahasiswa[i].nrp == nrpToEdit) {
            found = 1;

            // Add new Data using provided NRP
            inputData(1, nrpToEdit);

            // Shift data to remove the element
            for (int j = i; j < banyakMhs - 1; j++) {
                Mahasiswa[j] = Mahasiswa[j + 1];
            }

            // Decrease the count of students
            banyakMhs--;
            printf("\n> Data mahasiswa dengan NRP %lld berhasil diedit.\n", nrpToEdit);

            sortMahasiswa(compareByNRP, 'a');

            // Save updated data to the file
            saveToFile("database.bin");
            break;
        }
    }

    if (!found) {
        printf("\n> Mahasiswa dengan NRP %d tidak ditemukan.\n", nrpToEdit);
    }
}

void deleteData() {
    long long nrpToDelete;
    int found = 0;

    printf("\nMasukkan NRP mahasiswa yang ingin dihapus: ");
    scanf("%lld", &nrpToDelete);

    for (int i = 0; i < banyakMhs; i++) {
        if (Mahasiswa[i].nrp == nrpToDelete) {
            found = 1;

            // Shift data to remove the element
            for (int j = i; j < banyakMhs - 1; j++) {
                Mahasiswa[j] = Mahasiswa[j + 1];
            }

            banyakMhs--; // Decrease the count of students
            printf("\n> Data mahasiswa dengan NRP %lld berhasil dihapus.\n", nrpToDelete);

            sortMahasiswa(compareByNRP, 'a');

            // Save updated data to the file
            saveToFile("database.bin");
            break;
        }
    }

    if (!found) {
        printf("\n> Mahasiswa dengan NRP %lld tidak ditemukan.\n", nrpToDelete);
    }
}

void dropDB(const char *filename) {
    char confirm;
    printf("\n=========================================");
    printf("\n!!! perubahan tidak dapat di pulihkan !!!");
    printf("\n=========================================");
    printf("\nYakin untuk menghapus database? [y/n]: ");
    scanf(" %c", &confirm);
    if (confirm == 'y' || confirm == 'Y') {
        remove(filename);
        banyakMhs = 0;
        memset(Mahasiswa, 0, sizeof(Mahasiswa));
        printf("\n> Database berhasil dihapus.\n");
    } else {
        printf("\n> Database batal dihapus.\n");
    }
}

void editDataMenu() {
    if (banyakMhs == 0) {
        printf("\n> Belum ada data Mahasiswa\n");
        return;
    }

    char input[10];
    char menuPrint, sort = '\0';
    int exit, showMenu = 0;
    do {
        if (showMenu == 0) {
            printf("\nEdit Data Mahasiswa");
            printf("\n-------------------");
            printf("\n  1. Edit data");
            printf("\n  2. Hapus data");
            printf("\n  3. Hapus database");
            printf("\n  4. Keluar (also 'x' work)");
            showMenu = 1;
        } else
            printf("\n(use '?' for menu list)");

        fflush(stdin);
        printf("\nMenu 4 => ");
        fgets(input, sizeof(input), stdin);

        menuPrint = input[0];
        
        switch(menuPrint) {
            case '1':
                editData();
                break; 
            case '2':
                deleteData();
                break; 
            case '3':
                dropDB("database.bin");
                break;
            case '4':
            case 'x':
            case 'X':
                exit = 1;
                break;
            case '?':
                showMenu = 0;
                break;
            default:
                printf("\nError! : menu invalid\n");
        }
    } while(exit != 1);
}