
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct Subject
{
    std::string name_of_sub;
    unsigned short int score = 0;
};

struct Student
{
    std::string name;
    Subject subs[4];
    double average_score = 0.0;
};

std::ostream& operator <<(std::ostream& out, Student& i)
{
    out << i.name << " ";
    out << i.average_score << std::endl;
    return out;
}

void sort_s(std::vector<Student>& arr, int n)
{
    Student tmp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++) {
            const char* d1 = arr[i].name.c_str();
            const char* d2 = arr[j].name.c_str();
            //д1 и д2 нужны, тк фунция не работала с ссылками на строки
            if (strcmp(d1, d2) > 0)
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
}

std::istream& operator >>(std::istream& in, Student& i)
{    
    std::string cur_stud_name;
    std::string cur_sub_name;
    std::getline(in, cur_stud_name);
    i.name = cur_stud_name;
    unsigned short int cur_score;
    for (int j = 0; j < 4; ++j) {
        in >> cur_sub_name >> cur_score;
        i.subs[j].name_of_sub = cur_sub_name;
        i.subs[j].score = cur_score;
    }
    in.ignore(10, '\n');
    return in;
}

void read_db(std::istream& in, std::vector<Student>& v, unsigned int n)
{
    Student cur_stud;
    for (int i = 0; i < n; ++i)
    {
        in >> cur_stud;
        v.push_back(cur_stud);
    }
}

void read_db_bin(std::istream& in, std::vector<Student>& v, unsigned int n)
{
    std::string cur_stud_name;
    std::string cur_sub_name;
    unsigned short int cur_score;
    std::string a;
    std::string score_str;
    score_str.resize(1);
    for (int i = 0; i < n; ++i) {
        cur_stud_name = "";
        a.resize(1);
        in.read(a.data(), 1);
        while (a != "\n")
        {
            cur_stud_name += a;
            in.read(a.data(), 1);
            a.resize(1);
        }
        v.push_back(Student({ cur_stud_name }));
        for (int j = 0; j < 4; ++j) {
            cur_sub_name = "";
            a.resize(1);
            in.read(a.data(), 1);
            while (a != " ")
            {
                cur_sub_name += a;
                in.read(a.data(), 1);
                a.resize(1);
            }
            in.read(score_str.data(), 1);
            score_str.resize(1);
            cur_score = static_cast<int>(score_str[0]) - 48;
            v[i].subs[j].name_of_sub = cur_sub_name;
            v[i].subs[j].score = cur_score;
            if (j != 3) in.read(a.data(), 1); // после записи оценки в файле идет еще один пробел
        }
        in.read(a.data(), 1); // после записи строки с предметами есть еще один символ перехода \n
    }
}



std::ostream& operator <<(std::ofstream& out, std::vector<Student>& v)
{
    out << "Data base:" << std::endl;
    for (Student i : v)
    {
        out << i.name << " ";
        out << i.average_score << std::endl;
    }
    return out;
}


void print_db_bin(std::vector<Student>& v, std::ofstream& out)
{
    std::string db = "Data base:\n";
    out.write(db.c_str(), db.length());
    for (Student i : v)
    {
        //out << i.name << " ";
        //out << i.average_score << std::endl;
        out.write(i.name.c_str(), i.name.length());
        std::string space = " ";
        std::string enter = "\n";
        out.write(space.c_str(), 1);
        std::stringstream s;
        s << i.average_score;
        out.write(s.str().c_str(), s.str().length());
        out.write(enter.c_str(), 1);
    }
}

int main()
{
    unsigned int count_of_studs;
    std::cout << "How much studs do you have\n";
    std::cin >> count_of_studs;
    std::vector<Student> students;
    std::ifstream in("lab6_input_bin.bin", std::ios::binary);
    read_db_bin(in, students, count_of_studs);
    in.close();
    
    sort_s(students, count_of_studs);

    double sum_of_scores = 0.0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            sum_of_scores += students[i].subs[j].score * 1.0;
        }
        students[i].average_score = sum_of_scores / 4.0;
        sum_of_scores = 0.0;
    }

    int count_of_good_studs = 0;
    for (Student i : students) {
        if (i.average_score >= 4.0) count_of_good_studs++;
    }

    std::ofstream out("lab6_result.txt");
    std::cout << "Count of students with high average score is " << count_of_good_studs << std::endl;
    std::cout << "The list of theese students:" << std::endl;
    for (Student i : students) {
        if (i.average_score >= 4.0) {
            std::cout << i;
        }
    }
    out << students;
    out.close();
    std::ofstream out_bin("lab6_res_bin.bin", std::ios::binary);
    print_db_bin(students, out_bin);
    out_bin.close();

    /*std::ofstream in_bin("lab6_input_bin.bin", std::ios::binary);
    for (Student i : students)
    {
        std::string space = " ";
        std::string enter = "\n";
        in_bin.write(i.name.c_str(), i.name.length());
        in_bin.write(enter.c_str(), 1);
        for (int j = 0; j < 4; ++j)
        {
            in_bin.write(i.subs[j].name_of_sub.c_str(), i.subs[j].name_of_sub.length());
            in_bin.write(space.c_str(), 1);
            std::string score_str = " ";
            score_str[0] = static_cast<char>(i.subs[j].score + 48);
            in_bin.write(score_str.c_str(), 1);
            if (j != 3) in_bin.write(space.c_str(), 1);
        }
        in_bin.write(enter.c_str(), 1);
    }*/
    return 0;
}


