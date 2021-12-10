#include <ctime>
#include <time.h>
#include <limits>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>

#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>

#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

bsoncxx::document::value jsonGenerator(pthread_t threadID, int iteration, std::string insertDate)
{
    bsoncxx::document::value json = make_document(
        kvp("ThreadID", (long)threadID),
        kvp("Iteration", iteration),
        kvp("Inserted", insertDate));
    return json;
}

void clear()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string getTime()
{
    auto time = std::time(nullptr);
    auto localTime = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%H-%M-%S-%d-%m-%Y");
    return oss.str();
}

void threadFunction()
{
    clock_t begin = clock();
    mongocxx::client conn{mongocxx::uri{}};
    auto db = conn["test"];

    for (int i = 0; i < 1000; i++)
    {
        db["process"].insert_one(std::move(jsonGenerator(pthread_self(), i, getTime())));
    }

    double *time_spent = (double *)malloc(sizeof(double));

    clock_t end = clock();

    *time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    pthread_exit((void *)time_spent);
}

int welcomeScreen(int choice)
{
    clear();
    if ((choice < 1) || (choice > 5))
    {
        std::cout << "Welcome to version 1.0 of MarcosRigal's MongoDB Benchmark" << std::endl;
        std::cout << "---------------------------------------------------------" << std::endl;
        while ((choice < 1) || (choice > 5))
        {
            std::cout << "Choose the Benchmark intensity: " << std::endl;
            std::cout << "  - Choose 1 for low intensity." << std::endl;
            std::cout << "  - Choose 2 for medium intensity." << std::endl;
            std::cout << "  - Choose 3 for high intensity." << std::endl;
            std::cout << "  - Choose 4 for extreme intensity." << std::endl;
            std::cout << "  - Choose 5 for custom intensity." << std::endl;
            std::cout << " Your choice: ";
            while (!(std::cin >> choice))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input.  Try again: ";
            }
            clear();
            if ((choice < 1) || (choice > 5))
            {
                std::cout << "Error, choice invalid" << std::endl;
            }
        }
    }
    return choice;
}

int setIntensity(int choice, int numberOfThreads)
{
    if ((choice != 5) || (numberOfThreads == 0))
    {
        if (choice == 1)
        {
            numberOfThreads = 100;
        }
        else if (choice == 2)
        {
            numberOfThreads = 250;
        }
        else if (choice == 3)
        {
            numberOfThreads = 500;
        }
        else if (choice == 4)
        {
            numberOfThreads = 1000;
        }
        else if (choice == 5)
        {
            clear();
            while (numberOfThreads < 1)
            {
                std::cout << "Here you can choice the number of threads that will connect to your database." << std::endl;
                std::cout << "Number Of Threads = ";
                while (!(std::cin >> numberOfThreads))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input.  Try again: ";
                }
                clear();
                if (numberOfThreads < 1)
                {
                    std::cout << "Error, the number of threads, shouldbe at least 1" << std::endl;
                }
            }
        }
    }
    return numberOfThreads;
}

struct threadInfo
{
    pthread_t threadId;
    double time_spent;
};

void generateReport(std::vector<threadInfo> threadsInfo)
{

    FILE *file;
    std::string fileName = "../reports/report-" + getTime() + ".txt";
    std::cout << "Report generated at: " << fileName << std::endl;
    file = fopen(fileName.c_str(), "w");
    char input[512];
    for (int i = 0; i < (int)threadsInfo.size(); i++)
    {
        snprintf(input, 512, "%ld,%f\n", threadsInfo[i].threadId, threadsInfo[i].time_spent);
        fputs(input, file);
    }

    fclose(file);

    file = fopen("reportPath.txt", "w");
    snprintf(input, 512, "%s\n", fileName.c_str());
    fputs(input, file);

    fclose(file);
}

int main(int argc, char const *argv[])
{
    clear();

    int choice = 0;
    int numberOfThreads = 0;

    if (argc == 2)
    {
        choice = atoi(argv[1]);
    }

    else if (argc == 3)
    {
        choice = atoi(argv[1]);
        numberOfThreads = atoi(argv[2]);
    }

    choice = welcomeScreen(choice);
    numberOfThreads = setIntensity(choice, numberOfThreads);

    mongocxx::instance inst{};

    pthread_t threadsId[numberOfThreads];
    std::vector<threadInfo> threadsInfo;

    double *time_spent;

    for (int i = 0; i < numberOfThreads; i++)
    {
        if (pthread_create(&(threadsId[i]), NULL, (void *(*)(void *))threadFunction, NULL))
        {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < numberOfThreads; i++)
    {
        if (pthread_join(threadsId[i], (void **)&time_spent))
        {
            fprintf(stderr, "Error joining thread\n");
            exit(EXIT_FAILURE);
        }

        threadInfo threadInfo = {threadsId[i], *time_spent};
        threadsInfo.push_back(threadInfo);
    }

    std::cout << "Benchmark successfully completed!!" << std::endl;
    generateReport(threadsInfo);

    exit(EXIT_SUCCESS);
}