﻿
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct Subject
{
    std::string name_of_sub;
    unsigned short int score;
};

struct Student
{
    std::string name;
    Subject subs[4];
    double average_score;
};


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



int main()
{
    std::vector<Student> students;
    std::string cur_stud_name;
    std::string cur_sub_name;
    unsigned short int cur_score;
    std::ifstream in("lab6_input.txt");
    std::getline(in, cur_stud_name);
    for (int i = 0; i < 5; ++i) {
        students.push_back(Student({ cur_stud_name }));
        for (int j = 0; j < 4; ++j) {
            in >> cur_sub_name >> cur_score;
            students[i].subs[j].name_of_sub = cur_sub_name;
            students[i].subs[j].score = cur_score;
        }
        in.ignore(10, '\n');
        std::getline(in, cur_stud_name);
    }
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
    out << "Count of students with high average score is " << count_of_good_studs << std::endl;
    out << "The list of theese students:" << std::endl;
    for (Student i: students) {
        if (i.average_score >= 4.0) {
            out << i.name << " ";
            out << i.average_score << std::endl;
        }
    }
    return 0;
}

