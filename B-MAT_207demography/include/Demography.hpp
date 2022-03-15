/*
** EPITECH PROJECT, 2021
** EpitechTools
** File description:
** demography
*/

#ifndef DEMOGRAPHY_H_
#define DEMOGRAPHY_H_

/* INCLUDE */
#include <vector>
#include <string>

/* FUNCTIONS */
int Start(int ac, char **av);

/* STATIC CONST */
static const int SUCCESS = 0;
static const int FAIL = 1;
static const int ERROR = 84;

typedef struct info_s {
    std::string country_name;
    std::string id;
    std::vector<double> values;
} info_t;

class Demography
{
    public:
        Demography(int ac, char **av);
        ~Demography() = default;

        bool CheckReadable() noexcept;
        void ParseFile();
        void GetYearInfo();
        void InitStruct(int ac, char **av) noexcept;

        void DisplayCountry() noexcept;
        void FirstFit() noexcept;
        void SecondFit() noexcept;
        void Correlation() noexcept;

        void ComputeFirst(double &a, double &b);
        void ComputeSecond(double &a, double &b);
        double ComputeRMS(double &a, double &b, bool first) noexcept;
        double ComputeCorrelation();
    private:
        std::string _fileName;
        std::vector<info_t> _countryInfos;
        std::vector<double> _yearInfo;
};

#endif /* !DEMOGRAPHY_H_ */
