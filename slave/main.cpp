#include "mbed.h"
#include <string>

Serial pc (USBTX, USBRX);
Serial bt(PB_6, PB_7);
BusOut ledice(PB_4,PB_5,PA_11,PA_8, PF_1, PF_0, PB_1, PA_12);

int suma;
char data;

void dekompozicija(int sumaDCOMP) {
    string pozicije[7] = { "palac", "kaziprst", "srednji", "prstenjak", "mali", "rotacija", "translacija" };
    int current, mod, brojac = 0;
    current = sumaDCOMP;

    while (brojac < 8) {

        mod = current % 2;
        current = current / 2;

        if (mod == 0) {
            switch (brojac) {
            case 0:
                pozicije[6] = "0";
                break;
            case 1:
                pozicije[5] = "0";
                break;
            case 2:
                pozicije[4] = "0";
                break;
            case 3:
                pozicije[3] = "0";
                break;
            case 4:
                pozicije[2] = "0";
                break;
            case 5:
                pozicije[1] = "0";
                break;
            case 6:
                pozicije[0] = "0";
                break;
            }
        }
        brojac++;
    }

    for (int i = 0; i < 7; i++){
        if (pozicije[i] != "0")
        {
            pc.printf("%s\n\t", pozicije[i]);
        }
    }

}

int main()
{
    bt.baud(9600);
    //bt.format(8,SerialBase::None,1);
    while(1) {
        if (bt.readable()) {
            data = bt.getc();
        }
        
        suma = data;
        ledice = suma;
        if (suma != 0){
            pc.printf("%d\n",data);
            pc.printf("________izvjesce__________\n\t");
            dekompozicija(suma);
            pc.printf("\n\n");
        }
        
        data = 0;
        wait(1);
    }
}
