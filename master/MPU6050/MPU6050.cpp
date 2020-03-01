#include "mbed.h"
#include "MPU6050.h"

#define ADDRESS 0x68        //adresa I2C komunikacije
#define ZERO_ARRAY 0x00     //defultne postavke za pojedine registre

extern I2C gyroAcc ;        //(PA_10, PA_9);


// short vrijednosti u koje se sprema char raw data iz registara
short accelX, accelY, accelZ, gyroX, gyroY, gyroZ;      

//  preračunata float vrijednost u ovisnosti o preciznosti očitanja
//  koje smo odabrali prilagodbama određenih registara
float gForceX, gForceY, gForceZ, rotX, rotY, rotZ;


// inicijalizacija MPU6050 jedinice, postavljanje defaultnih postavki
void initialState(){
    char reg,id;
    reg = 0x75;
    
    // vraća defaultnu adresu I2C komunikacije, 0x68, samo za slucaj kad nemamo 
    // drugi gyro spojen na AD0 pin
    gyroAcc.write((ADDRESS) << 1, &reg, 1, true);
    gyroAcc.read((ADDRESS) << 1, &id, 1, false);
    wait(2);
       
    // power managment, potrebno isključiti SLEEP mode
    char power_register[2] = {0x6B, ZERO_ARRAY};
    gyroAcc.write((ADDRESS) << 1, power_register, 2, false);
    
    // inicijalizacija žiroskopa i akcelerometra
    char gyro_init[2] = {0x1B, ZERO_ARRAY};   // +/- 250 stupnjeva/sec
    char accel_init[2] = {0x1C, ZERO_ARRAY};  //  +/- 2g
    gyroAcc.write((ADDRESS) << 1, gyro_init, 2, false);
    gyroAcc.write((ADDRESS) << 1, accel_init, 2, false);
}

// očitanje iz registra za akcelerometar
float accelReading(){
    char data_accel[6];
    data_accel[0]= 0x3B;
    
    gyroAcc.write((ADDRESS) << 1, &data_accel[0], 1, true);
    gyroAcc.read((ADDRESS) << 1, data_accel, 6, false);
    
    accelX = data_accel[0] << 8 | data_accel[1];
    accelY = data_accel[2] << 8 | data_accel[3];
    accelZ = data_accel[4] << 8 | data_accel[5];
    
    gForceX = accelX / 16384.0;
    gForceY = accelY / 16384.0;
    gForceZ = (accelZ / 16384.0)-1;
    
    // vraćanje isključivo relevantne vrijednosti u main
    return gForceY;     
}

//očitanje iz registra za žiroskop
float gyroReading(){
    char data_gyro[6];
    data_gyro[0]= 0x43;
    
    gyroAcc.write((ADDRESS) << 1, &data_gyro[0], 1, true);
    gyroAcc.read((ADDRESS) << 1, data_gyro, 6, false);
    
    gyroX = data_gyro[0] << 8 | data_gyro[1];
    gyroY = data_gyro[2] << 8 | data_gyro[3];
    gyroZ = data_gyro[4] << 8 | data_gyro[5];
    
    rotX = gyroX / 131.0;
    rotY = gyroY / 131.0;
    rotZ = (gyroZ / 131.0)+1;   
    
    // vraćanje isključivo relevantne vrijednosti u main
    return rotY;
}
/*
*   funkcija koja ispisuje na terminal sva očitanja koja smo prethodno
*   izračunali
*
*   void printData(){
*       pc.printf("force X: %f\tforce Y: %f\tforce Z: %f\trotation X: %f\t\trotation Y: %f\t\trotation Z: %f\r", 
*           gForceX, gForceY, gForceZ, rotX, rotY, rotZ);     
*
*   }
*
*/
