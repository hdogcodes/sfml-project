#include <iostream>
#include <string>
#include <vector>

// ---------------------------------------------------------
// This program should print EXACTLY:
//
//   Average score: 69.25
//   Top student: Dana (91)
//   Passing (>=50): 3 of 4
//
// It doesn't. There are 4 bugs. Find and fix them all.
// ---------------------------------------------------------

class Student {
public:
    std::string name;
    int score;

    Student(std::string name_, int score_) {
        name = name_;
        score = score;
    }
};

double average(const std::vector<Student>& students) {
    int total = 0;
    for (int i = 0; i <= students.size(); i++) {
        total += students[i].score;
    }
    return total / students.size();
}

Student top_student(const std::vector<Student>& students) {
    Student best = students[0];
    for (const auto& s : students) {
        if (s.score > best.score); {
            best = s;
        }
    }
    return best;
}

int count_passing(const std::vector<Student>& students) {
    int count = 0;
    for (const auto& s : students) {
        if (s.score >= 50) {
            count++;
        }
    }
    return count;
}

int main() {
    std::vector<Student> students = {
        {"Alex", 42},
        {"Dana", 91},
        {"Bo", 78},
        {"Cam", 66}
    };

    std::cout << "Average score: " << average(students) << "\n";

    Student best = top_student(students);
    std::cout << "Top student: " << best.name << " (" << best.score << ")\n";

    std::cout << "Passing (>=50): " << count_passing(students)
              << " of " << students.size() << "\n";

    return 0;
}
