#include "userparam.h"

QTextStream in(stdin);
QTextStream out(stdout);


void UserParam::WhatMask()
{
    out << "Enter the mask of your input file \n" << Qt::flush;
    out << "Mask format: full name of your input file such as \"File.bin\", \n" << Qt::flush;
    out << "or just part  of your input file name such as \".txt\" \n" << Qt::flush;

    this->file_mask  =  in.readLine();
}

void UserParam::DeleteFile()
{
    out << "Do yoy want to delete your input file, after read y/n? \n" << Qt::flush;
    char v;
    in >> v;

    switch (v)
    {
    case 'y':
        this->delete_file = true;
        break;
    case 'n':
        break;
    default:
        throw ParamException("bad delete param");
    }
}

void UserParam::WhatPath()
{
    out << "Enter the path, to save your file: \n" << Qt::flush;
    in >> this->save_path;

    QFileInfo inf(this->save_path);

    if (!inf.exists())
    {
        throw ParamException("save path does not exist");
    }

    if (inf.isFile())
    {
        throw ParamException("file can not be used as path");
    }

}

void UserParam::SameFile()
{
    out << "Do yoy want to rewrite your file in the same name: y/n? \n" << Qt::flush;
    char v;
    in >> v;

    switch (v)
    {
    case 'y':
        this->if_same_name = true;
        break;
    case 'n':
        break;
    default:
        throw ParamException("bad fine name rewrite param");
    }
}

void UserParam::WhatTimer()
{
    out << "Do yoy want to write your file by timer: y/n? \n" << Qt::flush;
    char v;
    in >> v;

    switch (v)
    {
    case 'y':
    {
        this->timer = true;
        out << "Please enter time period of writing (between 1000 ms and 5000 ms) \n" << Qt::flush;
        QString time;
        in >> time;

        bool isnumber;
        int num = time.toInt(&isnumber);

        if (!isnumber)
        {
            throw ParamException("You must enter time period in range 1000 - 5000");
        }

        if (num >= 1000 && num <= 5000)
        {
            this->timer_period = num;
        }
        else
        {
            throw ParamException("You must enter time period in range 1000 - 5000");
        }

        break;
    }
    case 'n':
        break;
    default:
        throw ParamException("bad timer param");
    }
}

void UserParam::WhatBinMask()
{
    out << "Please enter your bin mask in format 0xFFFFFFFFFFFFFFFF: \n" << Qt::flush;

    QString mask = in.readLine();

    QRegularExpression re("^0x[0-9A-Fa-f]{1,16}$");
    QRegularExpressionMatch match = re.match(mask);

    if (!match.hasMatch()) {
        throw ParamException("bad bin mask");
    }

    bool ok;
    this->bin_mask = mask.toULongLong(&ok,16);

    if (!ok)
    {
        throw ParamException("bad bin mask");
    }

}

void UserParam::InitParam ()
{
    try {
        WhatMask();
        DeleteFile();
        WhatPath();
        SameFile();
        WhatTimer();
        WhatBinMask();
    }
    catch (const ParamException exept)
    {
        exept.message();
    }
    this->init = true;
}


QString UserParam::get_file_mask() const
{
    return this->file_mask;
}

bool UserParam::get_delete_file() const
{
    return this->delete_file;
}

QString UserParam::get_save_path() const
{
    return this->save_path;
}

bool UserParam::get_if_same_name() const
{
    return this->if_same_name;
}

bool UserParam::get_timer() const
{
    return this->timer;
}

uint8_t UserParam::get_period() const
{
    return this->timer_period;
}

uint64_t UserParam::get_bin_mask() const
{
    return this->bin_mask;
}

bool UserParam::get_init() const
{
    return this->init;
}

void FileModification (const UserParam& par, const QString& file_path)
{
    if (!par.get_init())
    {
        throw ParamException("No init!");
    }

    QDir save_dir(par.get_save_path());
    QDir read_dir(file_path);

    if (!read_dir.exists())
    {
        qDebug() << "Bad read file dir!";
        return;
    }

    QStringList files = read_dir.entryList({par.get_file_mask()}, QDir::Files);

    if (files.isEmpty())
    {
        qDebug() << "There is no such files";
        return;
    }

    QFile read_file(files.first());

    if (read_file.isOpen())
    {
        qDebug() << "Close the file first";
        return;
    }

    if (!read_file.isReadable())
    {
        qDebug() << "Cannot open file for read" << read_file.errorString().toStdString();
        return;
    }

    ///////////////////////////////////////////////

    read_file.open(QIODevice::ReadOnly);
    QByteArray data = read_file.readAll();
    read_file.close();

    if (read_file.remove())
    {
        qDebug() << "Old file succesfully deleted";
    }else
    {
        qDebug() << "Cannot delete old file";
    }

    for (char &byte : data) {
        byte = byte & par.get_bin_mask(); // тут самый простой вариант
        //, на тестовом задании я не хочу сильно углубляться в
        //побитовое перемножение
    }

    QString newFileName = files.first();

    int lastDot = newFileName.lastIndexOf('.');
    if (lastDot != -1) {
        newFileName.insert(lastDot, "_mod");
    } else {
        newFileName += "_mod";
    }

    QString newFilePath = save_dir.filePath(newFileName);
    QFile outputFile(newFilePath);

    if (!outputFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cant open file for writing!" << newFilePath;
        return;
    }

    outputFile.write(data);
    outputFile.close();

    qDebug() << "File succesfully writen. Path:" << newFilePath;
};
