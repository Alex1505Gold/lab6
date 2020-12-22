
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

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

void read_db(std::istream& in, std::vector<Student>& v)
{
    Student cur_stud;
    for (int i = 0; i < 5; ++i)
    {
        in >> cur_stud;
        v.push_back(cur_stud);
    }
}

void read_db_bin(std::istream& in, std::vector<Student>& v)
{
    std::string cur_stud_name;
    std::string cur_sub_name;
    unsigned short int cur_score;
    std::getline(in, cur_stud_name);
    for (int i = 0; i < 5; ++i) {
        v.push_back(Student({ cur_stud_name }));
        for (int j = 0; j < 4; ++j) {
            in >> cur_sub_name >> cur_score;
            v[i].subs[j].name_of_sub = cur_sub_name;
            v[i].subs[j].score = cur_score;
        }
        in.ignore(10, '\n');
        std::getline(in, cur_stud_name);
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
    for (Student i : v)
    {
        out << i.name << " ";
        out << i.average_score << std::endl;
    }
}

int main()
{
    std::vector<Student> students;
    std::ifstream in("lab6_input.txt");
    read_db(in, students);
    in.close();
    
    sort_s(students, 5);

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
    return 0;
}


