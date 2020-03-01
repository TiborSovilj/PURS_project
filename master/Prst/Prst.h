#ifndef _PRST_H_
#define _PRST_H_

#include "mbed.h"

class Prst{
    
public:
    // konstruktor sadržava informacije o inicijalnim stanjima i 
    // pinu na kojeg je svaki Prst ''spojen''
    Prst(int,PinName pin,int,int);          
    void catchState();      // ispituje ulazni napon na pinu
    int exportState();      // vratit će potenciju broja 2, trebamo 1 na toj težini
                            // tu jedinicu prosljeđuje u global polje
    
private:
    DigitalIn trigger;      // deklaracija pin-a, poziva se u konstruktoru
    bool active;            // sprema stanje prsta
    float reading;          // pretvorba ulaznog napona
    
    /*
    *   4 - palac
    *   3 - kažiprst
    *   2 - srednji
    *   1 - prstenjak
    *   0 - mali
    */
    int fingerIndex;        
};

#endif