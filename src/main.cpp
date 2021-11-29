#include <chrono>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

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

int main()
{
    int n, status;       //Creamos la variable que almacena el número de hijos y la que almacena el estado de salida del hijo
    pid_t pid, childpid; //Estas variables almacenan el id de los procesos hijos.
    //Pid almacena el valor devuelto al padre tras el fork y chilpid el valor devuelto al padre por la función wait cuando termina de esperar al hijo
    printf("Introduzca el número de procesos hijo que desea generar: ");
    scanf("%d", &n);                                //Recogemos la entrada
    getchar();                                      //Para vaciar el buffer
    printf("Soy %d el padre de todos\n", getpid()); //El proceso padre imprime su id

    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    auto db = conn["test"];

    for (int i = 0; i < n; i++) //Cuando hacemos el fork la variable i es distinta en cada caso
    {                           //Se crean bucles diferentes e independientes
        pid = fork();           // Aqúi el proceso tiene su hijo. En el padre pid valdrá el id del hijo y en el hijo pid valdrá 0
        switch (pid)            //En base al valor de pid cada proceso ejecutará una función
        {
        case 0:                                                                             //El fork se ha realizado corractamente
            printf("Soy %d el hijo nº %d del proceso: %d\n", getpid(), (i + 1), getppid()); //El hijo se identifica
            for (int i = 0; i < 5; i++)
            {
                db["process"].insert_one(std::move(jsonGenerator(getpid(), i)));
            }
            exit(EXIT_SUCCESS); //El hijo muere

        case -1:                                        //Ha ocurrido un error al realizar el fork
            printf("Error al crear el proceso hijo\n"); //Se informa al usuario
            exit(EXIT_FAILURE);                         //Indica que ha ocurrido un fallo en la ejecución

        default:
            printf("Esperando a que acabe mi hijo nº %d\n", i + 1);
        }                                                                 //Como el padre no ha hecho exit continua con el for y crea otro hijo
    }                                                                     //Una vez el padre ha terminado de crear los hijos que le hemos solicitado empieza a esperarlos
    while ((childpid = waitpid(-1, &status, WUNTRACED | WCONTINUED)) > 0) //Si lo hacemos así en vez de con wait podemos saber si el proceso ha sido pausado y poniendo -1 en el primer parametro de waitpid esperamos a cualquier hijo
    {                                                                     //Este bucle se repetirá mientas haya hijos que esperar cuando no haya mas wait devolverá -1
        if (WIFEXITED(status))
        { //Entrará en el caso de que el hijo haya finaizado correctamente ya que WIFEXITED(status) devolverá true
            printf("Proceso padre %d, hijo con PID %ld finalizado, status = %d\n", getpid(), (long int)childpid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        { //Entrará en el caso de que el proceso haya finalizado debido a una señar externa ya sea de finalizar o matar
            printf("Proceso padre %d, hijo con PID %ld finalizado al recibir la señal %d\n", getpid(), (long int)childpid, WTERMSIG(status));
        } //La macro WTERMSIG nos dice que señal ha sido la que ha recibido el proceso que ha producido que acabe
    }
    if (childpid == (pid_t)-1 && errno == ECHILD)
    { //Entra cuando vuelve al while y no hay más hijos que esperar porque en ese caso chilpid valdrá -1 y erno 10 que es el valor que devuelce ECHILD cuando no hay mas procesos hijo
        printf("Proceso padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
    } //strerror devuelve una cadena de caracteres que nos permite identificar el valor de la variable errno
    else
    { //Solo entra si se ha producido un error con wait
        printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE); //Acaba el proceso padre con error
    }
    exit(EXIT_SUCCESS); //Como todo ha ido bien el proceso padre acaba exitosamente
}