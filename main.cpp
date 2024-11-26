#include <QCoreApplication>

#include "userparam.h"

int main(int argc, char *argv[])
{
        QCoreApplication app(argc, argv);

      //  UserParam MyPar; // Тестил, ввод работает
      //  MyPar.InitParam();

        UserParam MyPar(5);
        QString ReadPath = "C:\\Users\\Vladislav\\Desktop\\read";


        try
        {
            FileModification(MyPar, ReadPath);
        }
        catch (const ParamException exept)
        {
            exept.message();
        }

        return app.exec();
}



