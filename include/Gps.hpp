#ifndef GPS_HPP
#define GPS_HPP

#include <TinyGPSPlus.h>

class Gps {
    public:
        Gps(int rxPin, int txPin, long baudRate);
        void    IniciaGps();
        void    SincronizaGps();
        bool    TemFix();
        float   GetLatitude();
        float   GetLongitude();
        int     GetNumeroSatelites();
        String  GetHora();
        void    ImprimirDadosGps();

    private:
        TinyGPSPlus gps;
        int rxPin, txPin;
        long baudRate;
        float latitude;
        float longitude;
        int numeroSatelites;
};

#endif