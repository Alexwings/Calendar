//
//  main.cpp
//  Calendar
//
//  Created by Xinyuan on 2022/12/22.
//

#include <iostream>
#include <string>
#include <chrono>
#include <array>
#include <utility>

using namespace std;
class Month {
public:
    friend istream& operator>>(istream& in, Month& m);
    friend ostream& operator<<(ostream& out, Month &m);
    static const std::string days_in_week[7];
    static void print(const Month &current);
private:
    void setup_weekday();
    static chrono::weekday calculate_weekday(const chrono::year_month_day &day);
    chrono::year_month data;
    array<chrono::year_month_day, 35> days;
public:
    Month(int y, int m);
    Month(): data(2022y,chrono::month(12)) {};
    std::string get_year() const ;
    std::string get_month() const;
};

const std::string Month::days_in_week[7] = {{"日"},{"一"},{"二"},{"三"},{"四"},{"五"},{"六"}};

void Month::print(const Month &current) {
    using namespace std;
    cout << "========";
    cout << current.get_year() << "年" << current.get_month() << "月";
    cout << "========" << endl;
    for (int i = 0; i < 7; i++) {
        cout << days_in_week[i] << "\t";
    }
    cout << endl;
    auto &days = current.days;
    chrono::year_month_day_last last_day {current.data / chrono::last};
    for (int i = 0; i < days.size(); i++) {
        if (!days[i].ok()) cout << " \t";
        else {
            cout << (unsigned)days[i].day() << "\t";
        }
        if ((i + 1) % 7 == 0 || days[i] == last_day) cout << endl;
    }
}

std::string Month::get_year() const {
    return std::to_string(int(this->data.year()));
}

std::string Month::get_month() const {
    return std::to_string(unsigned(this->data.month()));
}

Month::Month(int y, int m) {
    using namespace chrono;
    using namespace std::literals;
    month temp_month(m == 12 ? m : m % 12);
    year temp_year(y);
    temp_year = temp_year.ok() ? temp_year : 2021y;
    data = year_month(temp_year, temp_month);
    setup_weekday();
};

void Month::setup_weekday() {
    using namespace chrono;
    year_month_day first {data.year() / data.month() / day(1)};
    weekday first_weekday = calculate_weekday(first);
    int first_week_val = first_weekday.c_encoding();
    year_month_day_last last { data / chrono::last };
    for (int i = 0; i < days.size(); i++) {
        if ( i + 1 > (unsigned)last.day()) continue;
        if (i == first_week_val) {
            days[i] = first;
        } else if (i > first_week_val && (i - first_week_val) < 31) {
            days[i] = year_month_day(data.year(), data.month(), day( i + 1 ));
        }
    }
};

chrono::weekday Month::calculate_weekday(const std::chrono::year_month_day &day) {
    int c = ((int)day.year() / 100 );
    int y = (int)day.year() % 100;
    int m = (unsigned)day.month();
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    int d = (unsigned)day.day();
    unsigned w = y + y/4 + c/4 - 2 * c + (26 * (m+1)/10) + d - 1;
    return chrono::weekday(w % 7);
};


istream& operator>>(istream& in, Month& m) {
    int year, month;
    in >> year >> month;
    m = Month(year, month);
    return in;
};

ostream& operator<<(ostream& out, Month& m) {
    using namespace std;
    out << "========";
    out << m.get_year() << "年" << m.get_month() << "月";
    out << "========" << endl;
    for (int i = 0; i < 7; i++) {
        out << m.days_in_week[i] << "\t";
    }
    out << endl;
    auto &days = m.days;
    chrono::year_month_day_last last_day {m.data / chrono::last};
    for (int i = 0; i < days.size(); i++) {
        if (!days[i].ok()) out << " \t";
        else {
            out << (unsigned)days[i].day() << "\t";
        }
        if ((i + 1) % 7 == 0 || days[i] == last_day) cout << endl;
    }
    return out;
};

int main(int argc, const char * argv[]) {
    using namespace chrono;
    string exit = "yes";
    while (exit == "yes" || exit == "y") {
        cout << "请输入要查看的月份，格式: yyyy mm" << endl;
        Month m;
        cin >> m;
        cout << m << endl;
        cout << "继续吗？[y(es)/n(o)]";
        cin >> exit;
        cout << endl;
    }
    return 0;
}
