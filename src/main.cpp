#include <time.h>
#include <limits>
#include <vector>
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

bsoncxx::document::value jsonGenerator(int pid, int i)
{
    bsoncxx::document::value json = make_document(
        kvp("PID", pid),
        kvp("Iteration", i));
    return json;
}

void clear()
{ //Funcion que limpia la terminal en función del sistema operativo que estemos utilizando
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//mongocxx::instance inst{};
//mongocxx::client conn{mongocxx::uri{}};
//auto db = conn["test"];
//db["process"].insert_one(std::move(jsonGenerator(getpid(), i)));

void th_rand() //Esta será la función que ejecuten las hebras.
{
    clock_t begin = clock();
    for (int i = 0; i < 100000; i++)
    {
        int j = 0;
        j++;
    }
    double *time_spent = (double *)malloc(sizeof(double)); //reservamos memoria para un puntero de tipo float
    clock_t end = clock();
    *time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    pthread_exit((void *)time_spent);
}

int welcomeScreen()
{
    int choice = 0;
    clear();
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
    return choice;
}

int setIntensity(int choice)
{
    int numberOfThreads = 0;
    if (choice == 1)
    {
        numberOfThreads = 100;
    }
    else if (choice == 2)
    {
        numberOfThreads = 1000;
    }
    else if (choice == 3)
    {
        numberOfThreads = 10000;
    }
    else if (choice == 4)
    {
        numberOfThreads = 100000;
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
    return numberOfThreads;
}

struct threadInfo
{
    pthread_t threadId;
    double time_spent;
};

void generateReport(std::vector<threadInfo> threadsInfo)
{
    for (int i = 0; i < (int)threadsInfo.size(); i++)
    {
        std::cout << "Thread: " << threadsInfo[i].threadId << " Time spent: " << threadsInfo[i].time_spent << std::endl;
    }
    //FILE *f;
    //GameManager *gameManager = GameManager::getInstance();
    //f = fopen("users.txt", "w");
    //char input[512];
    //for (int i = 0; i < (int)gameManager->getUsers().size(); i++)
    //{
    //   snprintf(input, 512, "%s,%s\n", gameManager->getUsers()[i].getUserName(), gameManager->getUsers()[i].getUserPassword());
    //   fputs(input, f);
    //}
    //
    //fclose(f);
}

int main()
{
    int choice = welcomeScreen();
    int numberOfThreads = setIntensity(choice);

    pthread_t threadsId[numberOfThreads];
    std::vector<threadInfo> threadsInfo;

    double *time_spent;

    for (int i = 0; i < numberOfThreads; i++)
    {
        if (pthread_create(&(threadsId[i]), NULL, (void *(*)(void *))th_rand, NULL))
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

    generateReport(threadsInfo);

    exit(EXIT_SUCCESS);
}