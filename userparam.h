#pragma once

#include <QString>
#include <QTextStream>
#include <QException>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDebug>
#include <QFile>
#include <QDir>


class ParamException : public QException
{
    QString msg;
public:
    ParamException (const QString& str):msg(str){};
    void raise() const override {throw *this;}
    void message() const {qDebug() << "Bad initial param!: " << msg;}
};

class UserParam
{
    QString file_mask;
    bool delete_file;
    QString save_path;
    bool if_same_name;
    bool timer;
    uint8_t timer_period;
    uint64_t bin_mask;

    bool init = false;
public:

    UserParam ():file_mask(),delete_file(false),save_path(),if_same_name(false),
        timer(false),timer_period(),bin_mask(0xFFFFFFFFFFFFFFFF){};

    UserParam (int not_used)
    {
        not_used = 0;
        this->file_mask = "*.docx";
        this->delete_file = true;
        this->save_path = "C:\\Users\\Vladislav\\Desktop\\save";
        this->if_same_name = true;
        this->timer = false;
        this->timer_period = static_cast<uint8_t>(1000);
        this->bin_mask = 0x1010101010101010;

        this->init = true;
    }
    // Использую такой конструктор для отладки,
    // т.к. слишком долго вбивать все с клавиатуры

    void WhatMask();
    void DeleteFile();
    void WhatPath();
    void SameFile();
    void WhatTimer();
    void WhatBinMask();
    void InitParam ();

    QString get_file_mask() const;
    bool get_delete_file() const;
    QString get_save_path() const;
    bool get_if_same_name() const;
    bool get_timer() const;
    uint8_t get_period() const;
    uint64_t get_bin_mask() const;
    bool get_init() const;


};

void FileModification (const UserParam& par, const QString& file_path);
