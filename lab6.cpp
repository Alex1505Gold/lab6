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
    while (!in.eof())
    {
        in >> cur_stud;
        v.push_back(cur_stud);
    }
}

void read_db_bin(std::istream& in, std::vector<Student>& v)
{
    size_t a;
    std::string tmp;
    Student cur_stud;
    while (!in.eof())
    {
        in.read(reinterpret_cast<char*>(&a), sizeof(a));
        tmp.resize(a);
        in.read(tmp.data(), a);
        cur_stud.name = tmp;
        for (int j = 0; j < 4; j++)
        {
            in.read(reinterpret_cast<char*>(&a), sizeof(a));
            tmp.resize(a);
            in.read(tmp.data(), a);
            cur_stud.subs[j].name_of_sub = tmp;
            in.read(reinterpret_cast<char*>(&a), sizeof(a));
            tmp.resize(a);
            in.read(tmp.data(), a);
            cur_stud.subs[j].score = static_cast<unsigned short int>(tmp[0] - 48);
        }
        v.push_back(cur_stud);
    }
    v.pop_back();
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
    size_t a;
    for (Student i : v)
    {
        a = i.name.size();
        out.write(reinterpret_cast<const char*>(&a), sizeof(a));
        out.write(i.name.c_str(), i.name.size());
        for (int j = 0; j < 4; ++j)
        {
            a = i.subs[j].name_of_sub.size();
            out.write(reinterpret_cast<const char*>(&a), sizeof(a));
            out.write(i.subs[j].name_of_sub.c_str(), i.subs[j].name_of_sub.size());
            std::stringstream s;
            s << i.subs[j].score;
            a = s.str().size();
            out.write(reinterpret_cast<const char*>(&a), sizeof(a));
            out.write(s.str().c_str(), s.str().size());
        }
    }
}

int main()
{
    std::vector<Student> students;
    std::ifstream in("lab6_in_bin.bin", std::ios::binary);
    read_db_bin(in, students);
    in.close();
    sort_s(students, students.size());
    
    double sum_of_scores = 0.0;
    for (int i = 0; i < students.size(); ++i) {
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
    
    std::cout << "Count of students with high average score is " << count_of_good_studs << std::endl;
    std::cout << "The list of theese students:" << std::endl;
    for (Student i : students) {
        if (i.average_score >= 4.0) {
            std::cout << i;
        }
    }

    std::ofstream out("lab6_result.txt");
    out << students;
    out.close();
    std::ofstream out_bin("lab6_res_bin.bin", std::ios::binary);
    print_db_bin(students, out_bin);
    out_bin.close();
    return 0;
}


