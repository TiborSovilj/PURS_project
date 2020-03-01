#include "mbed.h"
#include "Prst.h"


// konstruktor
Prst::Prst(int finger, PinName pin,int active_=0, int reading_=0) : trigger(pin){
    active = active_;
    reading = reading_;
    fingerIndex = finger;
}

/*
*   funkcija koja vraća cjelobrojnu vrijesnost. vrijednost se racuna kao
*   potencija broja 2 koja će odgovarati samo jednoj jedinici na onoj
*   težini na kojoj se nalazi u poruci
*
*   algoritam je izveden preko for petlje zbog sintaksne greške
*   koju compiler javlja u pokušaju da se iskoristi pow() naredba
*   za potenciranje iz math.h biblioteke
*   
*/
int Prst::exportState(){
    int povratno = 4;   //pocetna povratna vrijednost
    
    // ako je prst savijen racuna se povratna vrijednost
    if (active == 1){
        switch(fingerIndex){
            case 0 : povratno = 4;
                break;
            default : {
                for(int i = 0; i < fingerIndex; i++){
                    povratno = povratno*2;
                }
            } break;
        }
        return povratno;
    }
    else 
        return 0;
}


// funkcija koja sprema stanje active u 1 ako je prst savijen
void Prst::catchState(){
    reading = trigger.read();
    active = (reading != 0) ? 1:0;    
}