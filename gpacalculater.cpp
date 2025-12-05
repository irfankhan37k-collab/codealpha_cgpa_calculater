#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>

using namespace std;

// Function to convert letter grade to numerical grade point (4.0 scale)
double getGradePoint(const string& grade) {
    // Map of common letter grades to standard 4.0 scale points
    static const map<string, double> GRADE_POINTS = {
        {"A", 4.00}, {"A-", 3.70},
        {"B+", 3.30}, {"B", 3.00}, {"B-", 2.70},
        {"C+", 2.30}, {"C", 2.00}, {"C-", 1.70},
        {"D+", 1.30}, {"D", 1.00},
        {"F", 0.00}
    };

    // Find the grade in the map
    auto it = GRADE_POINTS.find(grade);
    if (it != GRADE_POINTS.end()) {
        return it->second; // Return the corresponding point
    } else {
        // Handle invalid or unmapped grades
        cerr << "Warning: Grade '" << grade << "' is not recognized. Assuming 0.0 points." << endl;
        return 0.0;
    }
}

/**
 * Main function to run the CGPA calculation program.
 * 1. Takes input for the number of courses.
 * 2. For each course, inputs the grade and credit hours.
 * 3. Calculates total credits and total grade points.
 * 4. Computes the GPA and overall CGPA.
 * 5. Displays individual course grades and the final CGPA.
 */
void calculateCGPA() {
    // 1. Take input for the number of courses
    int numCourses;
    cout << "\n--- Semester GPA / Overall CGPA Calculator ---\n";
    cout << "Enter the number of courses taken this semester: ";
    cin >> numCourses;

    if (numCourses <= 0) {
        cout << "No courses entered. Exiting calculator.\n";
        return;
    }

    // Vectors to store course details for display
    vector<string> courseNames;
    vector<string> courseGrades;
    vector<int> courseCredits;
    vector<double> courseGradePoints; // Grade Point * Credit Hours

    // Variables for semester calculation (Step 3)
    double semesterTotalGradePoints = 0.0;
    int semesterTotalCredits = 0;

    cout << "\n--- Current Semester Details ---\n";
    // 2. For each course, input the grade and the credit hours.
    for (int i = 0; i < numCourses; ++i) {
        string name, grade;
        int credits;

        cout << "\nCourse " << i + 1 << ":\n";
        
        // Input Course Name
        cout << "  Enter Course Name (e.g., Data Structures): ";
        // Clear the buffer before reading the string input
        cin.ignore(); 
        getline(cin, name);

        // Input Credit Hours
        cout << "  Enter Credit Hours (e.g., 3): ";
        cin >> credits;

        // Input Grade
        cout << "  Enter Letter Grade (e.g., A, B+, C-): ";
        cin >> grade;

        // Calculate grade points for the course
        double points = getGradePoint(grade);
        double coursePoints = points * credits; // (grade × credit hours)

        // Store and accumulate results (Step 3)
        courseNames.push_back(name);
        courseGrades.push_back(grade);
        courseCredits.push_back(credits);
        courseGradePoints.push_back(coursePoints);

        semesterTotalGradePoints += coursePoints;
        semesterTotalCredits += credits;
    }

    // 4. Compute the GPA for the semester
    double semesterGPA = 0.0;
    if (semesterTotalCredits > 0) {
        semesterGPA = semesterTotalGradePoints / semesterTotalCredits;
    }

    // --- CGPA Calculation ---
    double prevCGPA = 0.0;
    int prevCredits = 0;
    
    cout << "\n--- Overall CGPA Details ---\n";
    cout << "Enter Previous Overall CGPA (e.g., 3.5): ";
    cin >> prevCGPA;
    cout << "Enter Previous Total Credit Hours Earned: ";
    cin >> prevCredits;

    // Calculate total points and credits for overall CGPA
    double prevTotalPoints = prevCGPA * prevCredits;
    double overallTotalPoints = prevTotalPoints + semesterTotalGradePoints;
    int overallTotalCredits = prevCredits + semesterTotalCredits;

    double overallCGPA = 0.0;
    if (overallTotalCredits > 0) {
        overallCGPA = overallTotalPoints / overallTotalCredits;
    } else {
        // If previous credits are 0, overall CGPA is just the semester GPA
        overallCGPA = semesterGPA; 
    }

    // 5. Display individual course grades and the final CGPA to the user.
    cout << fixed << setprecision(2); // Set output precision for GPA/CGPA

    cout << "\n================================================\n";
    cout << "              SEMESTER RESULTS\n";
    cout << "================================================\n";
    
    // Display individual course grades (Requirement 5)
    cout << "Course\t\tCredits\t\tGrade\t\tGrade Points (Grade * Credits)\n";
    cout << "------------------------------------------------\n";
    for (size_t i = 0; i < courseNames.size(); ++i) {
        // Use setw for alignment (needs <iomanip>)
        cout << left << setw(15) << courseNames[i].substr(0, 14) // Truncate name for clean alignment
             << right << setw(5) << courseCredits[i]
             << right << setw(15) << courseGrades[i]
             << right << setw(20) << courseGradePoints[i] << endl;
    }
    cout << "------------------------------------------------\n";

    cout << "\nSemester Total Credits: " << semesterTotalCredits << endl;
    cout << "Semester Total Grade Points: " << semesterTotalGradePoints << endl;
    
    // Display Semester GPA
    cout << "\n* Semester GPA (SGPA): " << semesterGPA << endl;
    
    // Display Overall CGPA
    cout << "\n* Overall CGPA: " << overallCGPA << endl;
    cout << "================================================\n";
}

int main() {
    // Set standard output to not tie with cerr for better performance, if needed.
    // ios_base::sync_with_stdio(false); 
    
    calculateCGPA();

    return 0;
}