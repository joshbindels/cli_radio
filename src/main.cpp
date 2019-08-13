#include <unistd.h>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#define VLC_PATH "/Applications/VLC.app/Contents/MacOS/VLC -I rc -q "

using json = nlohmann::json;

class RadioStation
{
    private:
        std::string name;
        std::string url;
        FILE* fd;
    public:
        RadioStation(std::string name, std::string url)
        {
            this->name = name;
            this->url = url;
            this->fd = 0;
        }
        void Start()
        {
            std::string command = VLC_PATH + this->url;
            this->fd = popen(command.c_str(), "w");
        }
        void Stop()
        {
            std::string command = "quit\n";
            fwrite(command.c_str(), sizeof(char), sizeof(command.length()), fd);
            fflush(fd);
        }
        std::string GetUrl()
        {
            return this->url;
        }
        std::string AsString()
        {
            return this->name;
        }
};

class RadioPlayer
{
    private:
        std::string configPath;
        std::vector<RadioStation> stations;
        void loadStationsFromConfig()
        {
            std::ifstream inputFileStream(this->configPath);
            json radioStations;
            inputFileStream >> radioStations;

            json j = radioStations["stations"];
            for(json::iterator it=j.begin(); it != j.end(); it++)
            {
                stations.push_back(RadioStation((*it)["name"], (*it)["url"]));
            }
        }
        void showAvailableStations()
        {
            for(int i=0; i<this->stations.size(); i++)
            {
                std::cout << i << ": "
                << this->stations.at(i).AsString() << std::endl;
            }
        }
        void PlayStation(int index)
        {
            this->stations.at(index).Start();
        }
        void StopStation(int index)
        {
            this->stations.at(index).Stop();
        }
    public:
        RadioPlayer(std::string dataPath)
        {
            this->configPath = dataPath;
            this->loadStationsFromConfig();
        }
        void Run()
        {
            bool run = true;
            int stationChoice = 0;
            char continueChoice = 0;
            while(run)
            {
                this->showAvailableStations();
                std::cout << "Choose a station: ";
                std::cin >> stationChoice;
                this->PlayStation(stationChoice);
                usleep(1000000);
                std::cout << "Enter q to quit, r to choose another station: ";
                std::cin >> continueChoice;
                if(continueChoice == 'q')
                {
                    this->StopStation(stationChoice);
                    run = false;
                }
                else if(continueChoice == 'r')
                {
                    this->StopStation(stationChoice);
                }

            }
        }
};


int main(int ac, char** av)
{
    RadioPlayer("resources/data/radio_stations.json").Run();
    exit(0);
}
