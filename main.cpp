#include "TempSensor.hpp"
#include "PHSensor.hpp"
#include <fstream>
#include <iomanip>
#include <vector>

int main() {

    std::vector<TempSensor> sensors;

    try {

        std::ifstream file("sensors.txt");
        if (!file.is_open()) {
            throw std::ios_base::failure(
                "Nie udalo sie otworzyc pliku sensors.txt");
        }
        file.exceptions(std::ifstream::badbit);

        std::string temp_place;
        double temp_temp; // temporary temperature xd

        while (std::getline(file, temp_place, ',') && (file >> temp_temp)) {
            sensors.push_back(TempSensor(temp_place, temp_temp));
            /*
            pobieranie doubla zostawia znak końca lini
            ktory zostalby wczytany przy pobieraniu nowego place
            igonre przeskakuje przez napotkane \n
            dzieki czemu nie bedzie pobierane w przyszlosci
            */
            file.ignore(1000, '\n');
        }
        file.close();

    } catch (const std::ios_base::failure &e) {
        std::cerr << "Blad I/O: " << e.what() << std::endl;
    }

    std::cout << "Wczytano " << sensors.size() << " sensorow." << std::endl;
    std::cout << std::left << std::setw(5) << "Lp." << " | " << std::left
              << std::setw(25) << "Nazwa sensora" << " |" << std::left
              << std::setw(12) << "Temperatura" << " |" << std::endl;

    std::cout << "------+---------------------------+-------------+"
              << std::endl;

    for (int i = 0; i < sensors.size(); i++) {
        std::cout << std::left << std::setw(5) << i + 1 << " | " << std::left
                  << std::setw(25) << sensors[i].getPlace() << " | "
                  << std::left << std::setw(11) << std::fixed
                  << std::setprecision(1) << sensors[i].getValue() << " |"
                  << std::endl;
    }

    PHSensor ph_sensor = PHSensor(); 
    ph_sensor.read_ph_data();

    return 0;
}
