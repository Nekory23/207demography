/*
** EPITECH PROJECT, 2021
** B-MAT_207demography
** File description:
** Demography
*/

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <cstring>

#include "Demography.hpp"
#include "Error.hpp"

Demography::Demography(int ac, char **av)
{
    if (ac < 2)
        throw Error(std::cerr, "too few arguments");
    _fileName = "./207demography_data.csv";
    for (int i = 1; i != ac; ++i) {
        if (strlen(av[i]) != 3)
            throw Error(std::cerr, "Invalid argument");
        for (int j = 0; av[i][j] != '\0'; ++j)
            if (av[i][j] < 'A' || av[i][j] > 'Z')
                throw Error(std::cerr, "Invalid argument");
    }
    if (CheckReadable())
        throw Error(std::cerr, "the data file is not readable or does not exist");
    InitStruct(ac, av);
    ParseFile();
    GetYearInfo();
}

bool Demography::CheckReadable() noexcept
{
    int fd = open(_fileName.c_str(), O_RDONLY);

    if (!fd)
        return true;
    close(fd);
    return false;
}

void Demography::InitStruct(int ac, char **av) noexcept
{
    info_t info;

    for (int i = 0; i != ac - 1; i++) {
        info.country_name = "";
        info.id = av[i + 1];
        _countryInfos.push_back(info);
    }
}

static std::vector<std::string> split(std::string &line, char sep)
{
    std::vector<std::string> res;
    std::stringstream strStr(line);
    std::string item;

    while (std::getline(strStr, item, sep))
        res.push_back(item);
    return res;
}

void Demography::ParseFile()
{
    int fd = open(_fileName.c_str(), O_RDONLY);
    std::vector<std::string> splitted;
    std::vector<std::string> content;
    std::string fileContent;
    struct stat st;
    long int value = 0;

    stat(_fileName.c_str(), &st);
    char file[st.st_size + 1];
    read(fd, file, st.st_size);
    fileContent = file;
    content = split(fileContent, '\n');
    for (std::size_t i = 0; i != content.size(); i++) {
        for (std::size_t j = 0; j != _countryInfos.size(); j++)
            if (content[i].find(_countryInfos[j].id) != std::string::npos) {
                splitted = split(content[i], ';');
                _countryInfos[j].country_name = splitted[0];
                for (std::size_t u = 2; u != splitted.size(); u++) {
                    if (splitted[u].empty())
                        value = 0;
                    else
                        value = std::stod(splitted[u].c_str());
                    _countryInfos[j].values.push_back(value);
                }
            }
    }
    for (std::size_t i = 0; i != _countryInfos.size(); i++)
        if (_countryInfos[i].country_name.empty()) {
            close(fd);
            throw Error(std::cerr, "one of the code you gave does not exist");
        }
    close(fd);
}

void Demography::GetYearInfo()
{
    int fd = open(_fileName.c_str(), O_RDONLY);
    std::vector<std::string> splitted;
    std::vector<std::string> content;
    std::string fileContent;
    struct stat st;
    long int value;

    stat(_fileName.c_str(), &st);
    char file[st.st_size + 1];
    read(fd, file, st.st_size);
    fileContent = file;
    content = split(fileContent, '\n');
    for (std::size_t i = 0; i != content.size(); i++) {
         if (content[i].find("Country Name") != std::string::npos) {
             splitted = split(content[i], ';');
            for (std::size_t i = 2; i != splitted.size(); i++) {
                value = std::atoi(splitted[i].c_str());
                _yearInfo.push_back(value);
            }
         }
    }
    if (_yearInfo.size() == 0) {
        close(fd);
        throw Error(std::cerr, "no informations on the years");
    }
    close(fd);
}

static bool sortAlpha(std::string first, std::string second)
{
    return first < second;
}

void Demography::DisplayCountry() noexcept
{
    std::vector<std::string> alphaOrder;

    std::cout << "Country: ";
    if (_countryInfos.size() == 1) {
        std::cout << _countryInfos[0].country_name << std::endl;
        return;
    }
    for (std::size_t i = 0; i != _countryInfos.size(); i++)
        alphaOrder.push_back(_countryInfos[i].country_name);
    std::sort(alphaOrder.begin(), alphaOrder.end(), sortAlpha);
    for (std::size_t i = 0; i != _countryInfos.size(); i++) {
        if (i + 1 == alphaOrder.size()) {
            std::cout << alphaOrder[i] << std::endl;
            break;
        }
        std::cout << alphaOrder[i] << ", ";
    }
}

void Demography::FirstFit() noexcept
{
    double a = 0;
    double b = 0;

    std::cout << "Fit1" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    ComputeFirst(a, b);
    std::cout << "    Y = " << a / 1000000;
    if (b < 0)
        std::cout << " X - " << std::abs(b / 1000000) << std::endl;
    else
        std::cout << " X + " << b / 1000000 << std::endl;
    std::cout << "    Root-mean-square deviation: " << ComputeRMS(a, b, true) / 1000000 << std::endl;
    std::cout << "    Population in 2050: " << (a * 2050 + b) / 1000000 << std::endl;
}

void Demography::SecondFit() noexcept
{
    double a = 0;
    double b = 0;

    std::cout << "Fit2" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    ComputeSecond(a, b);
    std::cout << "    X = " << a * 1000000;
    if (b < 0)
        std::cout << " Y - " << std::abs(b) << std::endl;
    else
        std::cout << " Y + " << b << std::endl;
    std::cout << "    Root-mean-square deviation: " << ComputeRMS(a, b, false) / 1000000 << std::endl;
    std::cout << "    Population in 2050: " << ((2050 - b) / a) / 1000000 << std::endl;
}

void Demography::Correlation() noexcept
{
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Correlation: " << ComputeCorrelation() << std::endl;;
}

void Demography::ComputeFirst(double &a, double &b)
{
    double div = 0;
    double sx = 0;
    double sy = 0;
    double sxx = 0;
    double sxy = 0;
    std::vector<double> values;

    for (std::size_t i = 0; i != _countryInfos.size(); i++, sx = 0, sy = 0, sxx = 0, sxy = 0) {
        values = _countryInfos[i].values;
        for (std::size_t j = 0; j != values.size(); j++) {
            sx += _yearInfo[j];
            sy += values[j];
            sxx += std::pow(_yearInfo[j], 2);
            sxy += _yearInfo[j] * values[j];
        }
        div = _yearInfo.size() * sxx - std::pow(sx, 2);
        if (div == 0)
            throw Error(std::cerr, "division by 0");
        a += (_yearInfo.size() * sxy - sx * sy) / div;
        b += (sy * sxx - sx * sxy) / div;
    }
}

void Demography::ComputeSecond(double &a, double &b)
{
    double div = 0;
    double sx = 0;
    double sy = 0;
    double sxx = 0;
    double sxy = 0;
    std::vector<double> values = _countryInfos[0].values;

    for (std::size_t i = 1; i != _countryInfos.size(); i++) {
        std::vector<double> temp = _countryInfos[i].values;
        for (std::size_t j = 0; j != temp.size(); j++)
            values[j] += temp[j];
    }
    for (std::size_t i = 0; i != _yearInfo.size(); i++) {
        sx += values[i];
        sy += _yearInfo[i];
        sxx += std::pow(values[i], 2);
        sxy += _yearInfo[i] * values[i];
    }
    div = _yearInfo.size() * sxx - std::pow(sx, 2);
    if (div == 0)
        throw Error(std::cerr, "division by 0");
    a = (_yearInfo.size() * sxy - sx * sy) / div;
    b = (sy * sxx - sx * sxy) / div;
}

double Demography::ComputeRMS(double &a, double &b, bool first) noexcept
{
    std::vector<double> values = _countryInfos[0].values;
    double diff = 0;
    double res = 0;

    for (std::size_t i = 1; i != _countryInfos.size(); i++) {
        std::vector<double> temp = _countryInfos[i].values;
        for (std::size_t j = 0; j != temp.size(); j++)
            values[j] += temp[j];
    }
    for (std::size_t i = 0; i != values.size(); i++) {
        if (first)
            diff += std::pow(values[i] - (a * _yearInfo[i] + b), 2);
        else
            diff += std::pow(values[i] - ((-b + _yearInfo[i]) / a), 2);
    }
    res = std::sqrt(diff / _yearInfo.size());
    return res;
}

double Demography::ComputeCorrelation()
{
    std::vector<double> values = _countryInfos[0].values;
    double diff = 0;
    double res = 0;
    double a1 = 0;
    double b1 = 0;
    double a2 = 0;
    double b2 = 0;
    double rms1 = 0;
    double rms2 = 0;

    ComputeFirst(a1, b1);
    ComputeSecond(a2, b2);
    rms1 = ComputeRMS(a1, b1, true);
    rms2 = ComputeRMS(a2, b2, false);
    for (std::size_t i = 1; i != _countryInfos.size(); i++) {
        std::vector<double> temp = _countryInfos[i].values;
        for (std::size_t j = 0; j != temp.size(); j++)
            values[j] += temp[j];
    }
    for (std::size_t i = 0; i != _yearInfo.size(); i++)
        diff += (values[i] - (a1 * _yearInfo[i] + b1)) * (values[i] - ((-b2 + _yearInfo[i]) / a2));
    diff /= _yearInfo.size();
    if (rms1 == 0 || rms2 == 0)
        throw Error(std::cerr, "division by 0");
    res = diff / (rms1 * rms2);
    return res;
}