#include "mbed.h"
#include "Prst.h"
#include "MPU6050.h"

Serial pc(USBTX,USBRX); 
Serial bt(PC_12,PD_2);          // bluetooth modul
I2C gyroAcc(PB_9, PB_8);        // MPU-6050

// timer koji služi za ograničavanje svake etape pokreta na određeno vrijeme
Timer motionETA;                



// instanciranje 5 Prst klasa

Prst palac(4,PF_13,0,0);
Prst kaziprst(3,PE_9,0,0);
Prst srednji(2,PE_11,0,0);
Prst prstenjak(1,PF_14,0,0);
Prst mali(0,PE_13,0,0);

int overallSum = 0;                 // suma koja definira 8-bitni zapis, MSB uvijek 0
int pal, kaz, sred, prsten, mal;    // integer definicija svakog prsta


// funkcija koja objedinjuje sva očitanja promjene stanja prstiju
void fingersCatching(){
    palac.catchState();
    kaziprst.catchState();
    srednji.catchState();
    prstenjak.catchState();
    mali.catchState();
}

/*
*   masterReset funkcija koja prekida aktualnu iteraciju 
*   beskonačne while petlje ukoliko su u bilo kojem trnutku
*   svi prsti savijeni. definirana kao bool kako bi njena
*   povratna infomacija mogla biti iskorištena kao argument
*   while petljama svake etape pokreta
*/

bool masterReset(){
    int overall;                // lokalna varijabla
    fingersCatching();
    
    pal = palac.exportState();          
    kaz = kaziprst.exportState(); 
    sred = srednji.exportState();
    prsten = prstenjak.exportState();
    mal = mali.exportState();
        
    overall =  pal+kaz+sred+prsten+mal;
    
    // ako su svi prsti savijeni daju sumu 124
    
    if(overall == 124){
        overallSum = 0;
        pc.printf("MASTER RESET\n");
        return true;    
    }
    else return false;    
}



int main() {
    
    
    pc.printf("DOBRODOSLI!\n");
    gyroAcc.frequency(400000);
    initialState();
    bt.baud(38400);
    //bt.format(8,SerialBase::None,1);
    pc.printf("MPU setup completed\n\n");
    
    /*
    *   3 faze:
    *       1. prsti
    *       2. zakret sake - žiroskop
    *       3. primicanje sake tijelu - accelerometar
    *
    *   tijekom svake stavljen timer koji ogranicava korisnika u kojem
    *   vremenskom intervalu mora uciniti pokret na pojedinoj etapi,
    *   Timer motionETA
    */
    
    
    while(1){
        
    /* 
    *   poruka ukoliko je u prethodnom koraku detektiran masterReset 
    *   i inicijalni wait prije svakog slijeda pokreta, tj. iteracije
    *   veskonačnog while-a
    *
    */
    
    if (masterReset()){
        pc.printf("MASTER RESET SE AKTIVIRAO!!!\nMolimo pricekajte 5 sekundi.\n");
        wait(5);    
    }  
    else wait(3);
    
    
    //______________SAVIJANJE PRSTIJU____________________________
    
    motionETA.start();                      // pokretanje timera
    pc.printf ("faza prsti pocela\n");
    while (!(masterReset()) && motionETA.read()<3){
       
        fingersCatching();          //detekcija savijanja
               
        pal = palac.exportState();
        kaz = kaziprst.exportState(); 
        sred = srednji.exportState();
        prsten = prstenjak.exportState();
        mal = mali.exportState();
        
        overallSum =  pal+kaz+sred+prsten+mal;
          
    }
    pc.printf("suma prstiju je: %d\n", overallSum);
    motionETA.stop();
    if (motionETA.read()==0) pc.printf("timer je resetiran\n");
    motionETA.reset();
        
    
    
    // ___________ROTACIJA SAKE_________________
    // rotacija oko Y osi
    float gyro;
    motionETA.start();
    
    pc.printf ("faza ROTACIJA\n");
    while (!(masterReset()) && motionETA.read()<3){
        gyro = gyroReading();           // očitanje sa žiroskopa;
        
        // apsolutna rotacija mora premašiti 200 stupnjeva/sek.
        if (abs(gyro) > 200.0){ 
            overallSum = overallSum + 2;
            break;    
        }
    }
    pc.printf("suma prstiju i zakreta je: %d\n", overallSum);
    motionETA.stop();
    motionETA.reset();
    if (motionETA.read()==0) pc.printf("timer je resetiran\n");
        
        
     
    // _______PRIMICANJE SAKE TIJELU_____________
    // translacija po Y osi
    
    float acc;
    motionETA.start();
    pc.printf ("faza TRANSLACIJA\n");
    while (!(masterReset()) && motionETA.read()<3){
        acc = accelReading();           // očitanje sa akcelerometra
        // apsolutno očitanje mora biti iznat 0.7g
        if (abs(acc) > 0.7){
            overallSum = overallSum + 1;
            break;    
        }
    }
    pc.printf("suma sve tri etape je: %d\n", overallSum);
    motionETA.stop();
    motionETA.reset();
    
    if (motionETA.read()==0) pc.printf("timer je resetiran\n");
    
       
    // pretvorba u char zbog BT komunikacije
    char overallSumChar = overallSum;      
    pc.printf("integer: %d\nje pretvoren u\nchar: %x\n\n\n", overallSum, overallSumChar);
   
   
   
    //________BT KOMUNIKACIJA____________
    
    bt.putc(overallSumChar);
    
}
}