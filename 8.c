#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 20
#define MAX_FACULTY 20
#define MAX_COURSES 50
#define MAX_ROOMS 10
#define MAX_PREREQS 50
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define MAX_CODE_LEN 10
#define MAX_SECTION_LEN 10
#define MAX_DAY_LEN 10
#define MAX_TIME_LEN 20
#define MAX_ROOM_LEN 10
#define MAX_DEPT_LEN 50
#define MAX_PROGRAM_LEN 50

// Define type aliases
typedef char str[MAX_NAME_LEN];
typedef char id_str[MAX_ID_LEN];
typedef char code_str[MAX_CODE_LEN];
typedef char section_str[MAX_SECTION_LEN];
typedef char day_str[MAX_DAY_LEN];
typedef char time_str[MAX_TIME_LEN];
typedef char room_str[MAX_ROOM_LEN];
typedef char dept_str[MAX_DEPT_LEN];
typedef char program_str[MAX_PROGRAM_LEN];

// Structure Definitions
typedef struct {
    code_str code;
    section_str section;
    int units;
    day_str day;
    time_str time;
    room_str room;
    str faculty;
} Course;

typedef struct {
    id_str id;
    str name;
    program_str program;
    code_str courses_taken[MAX_COURSES];
    int num_courses_taken;
    Course enrolled_courses[MAX_COURSES];
    int num_enrolled;
} Student;

typedef struct {
    id_str id;
    str name;
    dept_str department;
    int deloading_units;
    Course assigned_courses[MAX_COURSES];
    int num_assigned;
} Faculty;

typedef struct {
    code_str prerequisite;
    code_str course;
} Prerequisite;

// Function Prototypes
void mainMenu(Student students[], int *num_students, Faculty faculty[], int *num_faculty, Course courses[], int *num_courses, Prerequisite prerequisites[], int *num_prereqs);
void studentMenu(Student students[], int *num_students, Course courses[], int num_courses, Prerequisite prerequisites[], int num_prereqs);
void facultyMenu(Faculty faculty[], int *num_faculty, Course courses[], int num_courses, Student students[], int num_students);
void academicAssistantMenu(Student students[], int *num_students, Faculty faculty[], int *num_faculty, Course courses[], int *num_courses, Prerequisite prerequisites[], int *num_prereqs);
void enrollCourses(Student *student, Course courses[], int num_courses, Prerequisite prerequisites[], int num_prereqs);
void printEAF(Student *student);
void addCourseToFaculty(Faculty *faculty, Course courses[], int num_courses);
void printFacultyLoad(Faculty *faculty);
void printStudentList(Faculty *faculty, Student students[], int num_students);
void enterStudentInfo(Student students[], int *num_students);
void enterFacultyInfo(Faculty faculty[], int *num_faculty);
void enterCoursesAndPrerequisites(Course courses[], int *num_courses, Prerequisite prerequisites[], int *num_prereqs);
void scheduleAndRoomAssignment(Course courses[], int num_courses);
void displayFacultyList(Faculty faculty[], int num_faculty);
int checkPrerequisite(Student *student, code_str course_code, Prerequisite prerequisites[], int num_prereqs);
void saveStudentsToFile(Student students[], int num_students);
void saveFacultyToFile(Faculty faculty[], int num_faculty);
void saveCoursesToFile(Course courses[], int num_courses);
void savePrerequisitesToFile(Prerequisite prerequisites[], int num_prereqs);
void loadStudentsFromFile(Student students[], int *num_students);
void loadFacultyFromFile(Faculty faculty[], int *num_faculty);
void loadCoursesFromFile(Course courses[], int *num_courses);
void loadPrerequisitesFromFile(Prerequisite prerequisites[], int *num_prereqs);

// Main Function
int main() {
    Student students[MAX_STUDENTS];
    Faculty faculty[MAX_FACULTY];
    Course courses[MAX_COURSES];
    Prerequisite prerequisites[MAX_PREREQS];
    int num_students = 0, num_faculty = 0, num_courses = 0, num_prereqs = 0;

    loadStudentsFromFile(students, &num_students);
    loadFacultyFromFile(faculty, &num_faculty);
    loadCoursesFromFile(courses, &num_courses);
    loadPrerequisitesFromFile(prerequisites, &num_prereqs);

    mainMenu(students, &num_students, faculty, &num_faculty, courses, &num_courses, prerequisites, &num_prereqs);

    saveStudentsToFile(students, num_students);
    saveFacultyToFile(faculty, num_faculty);
    saveCoursesToFile(courses, num_courses);
    savePrerequisitesToFile(prerequisites, num_prereqs);

    return 0;
}

// Rest of the code remains the same...


// Main Menu
void mainMenu(Student students[], int *num_students, Faculty faculty[], int *num_faculty, Course courses[], int *num_courses, Prerequisite prerequisites[], int *num_prereqs) {
    int choice;
    do {
        printf("\nMain Menu\n");
        printf("1] Student\n");
        printf("2] Faculty\n");
        printf("3] Academic Assistant\n");
        printf("4] Exit\n");
        printf("Answer: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: studentMenu(students, num_students, courses, *num_courses, prerequisites, *num_prereqs); break;
            case 2: facultyMenu(faculty, num_faculty, courses, *num_courses, students, *num_students); break;
            case 3: academicAssistantMenu(students, num_students, faculty, num_faculty, courses, num_courses, prerequisites, num_prereqs); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);
}

//Student Menu
void studentMenu(Student students[], int *num_students, Course courses[], int num_courses, Prerequisite prerequisites[], int num_prereqs) {
    char id[MAX_ID_LEN];
    printf("Enter Student ID: ");
    scanf("%s", id);

    Student *student = NULL;
    int i;
	int found = 0;

	while (i < *num_students && !found){
		if (strcmp(students[i].id, id) == 0){
			student = &students[i];
			found = 1;
		}
		i++;
	}

    if (student == NULL) {
        printf("Student not found.\n");
    } 
	else {
		int choice;		
		int exitMenu = 0;  // Flag to control loop
	
       while (!exitMenu) {
        printf("\nStudent Menu\n");
        printf("1] Enroll Courses\n");
        printf("2] Print EAF\n");
        printf("3] Exit to Main Menu\n");
        printf("Answer: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                enrollCourses(student, courses, num_courses, prerequisites, num_prereqs);
                break;
            case 2:
                printEAF(student);
                break;
            case 3:
                printf("Returning to Main Menu...\n");
                exitMenu = 1;  // Set flag to exit loop
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
}

void enrollCourses(Student *student, Course courses[], int num_courses, Prerequisite prerequisites[], int num_prereqs) {
    printf("\nAvailable Courses:\n");
    printf("CODE\tSECTION\tUNITS\tDAY\tTIME\tROOM\tFACULTY\n");
    
    int i;
    for (i = 0; i < num_courses; i++) {
        printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\n",
               courses[i].code, courses[i].section, courses[i].units,
               courses[i].day, courses[i].time, courses[i].room, courses[i].faculty);
    }

    char code[MAX_CODE_LEN], section[MAX_SECTION_LEN];
    int exitEnrollment = 0;  // Loop control variable

    printf("\nEnter course code and section (e.g., CCPROG2 S12A). Type EXIT to stop.\n");

    while (!exitEnrollment) {
        printf("Enter: ");
        scanf("%s", code);

        if (strcmp(code, "EXIT") == 0) {
            exitEnrollment = 1;  // Exit loop naturally
        } else {
            scanf("%s", section);
            int found = 0;
            i = 0;

            // Search for the course without using break
            while (i < num_courses && !found) {
                if (strcmp(courses[i].code, code) == 0 && strcmp(courses[i].section, section) == 0) {
                    found = 1;
                }
                if (!found) {
                    i++;
                }
            }

            if (found) {
                if (checkPrerequisite(student, code, prerequisites, num_prereqs)) {
                    if (student->num_enrolled < MAX_COURSES) {  // Ensure enrollment limit is not exceeded
                        student->enrolled_courses[student->num_enrolled++] = courses[i];
                        printf("Enrolled in %s %s.\n", code, section);
                    } else {
                        printf("Enrollment limit reached! Cannot enroll in more courses.\n");
                    }
                } else {
                    printf("Cannot enroll in %s. Prerequisite not met.\n", code);
                }
            } else {
                printf("Course not found.\n");
            }
        }
    }
}

// Print EAF
void printEAF(Student *student) {
    printf("\nEnrollment Assessment Form (EAF)\n");
    printf("Name: %s\n", student->name);
    printf("ID: %s\n", student->id);
    printf("Program: %s\n", student->program);
    printf("Courses Enrolled:\n");
    printf("CODE\tSECTION\tUNITS\tDAY\tTIME\tROOM\tFACULTY\n");
    int i;
    for (i = 0; i < student->num_enrolled; i++) {
        printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\n",
               student->enrolled_courses[i].code, student->enrolled_courses[i].section,
               student->enrolled_courses[i].units, student->enrolled_courses[i].day,
               student->enrolled_courses[i].time, student->enrolled_courses[i].room,
               student->enrolled_courses[i].faculty);
    }
}


void facultyMenu(Faculty faculty[], int *num_faculty, Course courses[], int num_courses, Student students[], int num_students) {
    char id[MAX_ID_LEN];
    printf("Enter Faculty ID: ");
    scanf("%s", id);

    Faculty *faculty_member = NULL;
    int i;
	int found = 0;

    // Search for faculty member
    for (i = 0; i < *num_faculty && !found; i++) {
        if (strcmp(faculty[i].id, id) == 0) {
            faculty_member = &faculty[i];
			found = 1;
        }
    }

    if (faculty_member == NULL) {
        printf("Faculty not found.\n");
        return;
    }

    int choice;
    do {
        printf("\nFaculty Menu\n");
        printf("1] Enter Deloading Units\n");
        printf("2] Select Course Load\n");
        printf("3] Print Course Load\n");
        printf("4] Print Student List\n");
        printf("5] Exit to Main Menu\n");
        printf("Answer: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: 
                printf("Enter Deloading Units: ");
                scanf("%d", &faculty_member->deloading_units);
                break;
            case 2: 
                addCourseToFaculty(faculty_member, courses, num_courses);
                break;
            case 3: 
                printFacultyLoad(faculty_member);
                break;
            case 4: 
                printStudentList(faculty_member, students, num_students);
                break;
            case 5: 
                printf("Returning to Main Menu...\n");
                break;
            default: 
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
}

void addCourseToFaculty(Faculty *faculty_member, Course courses[], int num_courses) {
    printf("\nAvailable Courses:\n");
    printf("CODE\tSECTION\tUNITS\tDAY\tTIME\tROOM\n");
    
    int i;
    for (i = 0; i < num_courses; i++) {
        printf("%s\t%s\t%d\t%s\t%s\t%s\n",
               courses[i].code, courses[i].section, courses[i].units,
               courses[i].day, courses[i].time, courses[i].room);
    }

    char code[MAX_CODE_LEN], section[MAX_SECTION_LEN];
    int exitAssignment = 0;  // Loop control variable

    printf("\nEnter course code and section (e.g., CCPROG2 S12A). Type EXIT to stop.\n");

    while (!exitAssignment) {
        printf("Enter: ");
        scanf("%s", code);

        if (strcmp(code, "EXIT") == 0) {
            exitAssignment = 1;  // Exit loop naturally
            continue;  // Skip the rest of the loop iteration
        }

        scanf("%s", section);
        int found = 0;

        for (i = 0; i < num_courses; i++) {
            if (strcmp(courses[i].code, code) == 0 && strcmp(courses[i].section, section) == 0) {
                faculty_member->assigned_courses[faculty_member->num_assigned++] = courses[i];
                printf("Assigned to %s %s.\n", code, section);
                found = 1;
            }
        }

        if (!found) {
            printf("Course not found.\n");
        }
    }
}


// Print Faculty Load
void printFacultyLoad(Faculty *faculty_member) {
    printf("\nFaculty Load for %s\n", faculty_member->name);
    printf("Deloading Units: %d\n", faculty_member->deloading_units);
    printf("Assigned Courses:\n");
    printf("CODE\tSECTION\tUNITS\tDAY\tTIME\tROOM\n");
    int i;
    for (i = 0; i < faculty_member->num_assigned; i++) {
        printf("%s\t%s\t%d\t%s\t%s\t%s\n",
               faculty_member->assigned_courses[i].code, faculty_member->assigned_courses[i].section,
               faculty_member->assigned_courses[i].units, faculty_member->assigned_courses[i].day,
               faculty_member->assigned_courses[i].time, faculty_member->assigned_courses[i].room);
    }
}

// Print Student List
void printStudentList(Faculty *faculty_member, Student students[], int num_students) {
    printf("\nStudent List for %s\n", faculty_member->name);
    int i, j, k;
    for (i = 0; i < faculty_member->num_assigned; i++) {
        printf("Course: %s %s\n", faculty_member->assigned_courses[i].code, faculty_member->assigned_courses[i].section);
        printf("Students:\n");
        for (j = 0; j < num_students; j++) {
            for (k = 0; k < students[j].num_enrolled; k++) {
                if (strcmp(students[j].enrolled_courses[k].code, faculty_member->assigned_courses[i].code) == 0 &&
                    strcmp(students[j].enrolled_courses[k].section, faculty_member->assigned_courses[i].section) == 0) {
                    printf("%s\t%s\n", students[j].id, students[j].name);
                }
            }
        }
    }
}

// Academic Assistant Menu
void academicAssistantMenu(Student students[], int *num_students, Faculty faculty[], int *num_faculty, Course courses[], int *num_courses, Prerequisite prerequisites[], int *num_prereqs) {
    int choice;
    do {
        printf("\nAcademic Assistant Menu\n");
        printf("1] Enter Student Information\n");
        printf("2] Enter Faculty Information\n");
        printf("3] Enter Courses and Prerequisites\n");
        printf("4] Schedule and Room Assignment\n");
        printf("5] Display Faculty List\n");
        printf("6] Exit to Main Menu\n");
        printf("Answer: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: enterStudentInfo(students, num_students); break;
            case 2: enterFacultyInfo(faculty, num_faculty); break;
            case 3: enterCoursesAndPrerequisites(courses, num_courses, prerequisites, num_prereqs); break;
            case 4: scheduleAndRoomAssignment(courses, *num_courses); break;
            case 5: displayFacultyList(faculty, *num_faculty); break;
            case 6: printf("Returning to Main Menu...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);
}

void enterStudentInfo(Student students[], int *num_students) {
    if (*num_students >= MAX_STUDENTS) {
        printf("Maximum number of students reached.\n");
        return;
    }

    Student new_student;
    printf("Enter Student ID: ");
    scanf("%s", new_student.id);
	getchar();

    printf("Enter Name: ");
    fgets(new_student.name, sizeof(new_student.name), stdin);
    new_student.name[strcspn(new_student.name, "\n")] = 0; // Remove newline

    printf("Enter Program: ");
    fgets(new_student.program, sizeof(new_student.program), stdin);
    new_student.program[strcspn(new_student.program, "\n")] = 0; // Remove newline

    new_student.num_courses_taken = 0;
    new_student.num_enrolled = 0;

    printf("Enter Courses Taken (Type EXIT to stop):\n");
    char course[MAX_CODE_LEN];
    int exitInput = 0;  // Loop control variable

    while (!exitInput) {
        printf("Enter: ");
        scanf("%s", course);

        if (strcmp(course, "EXIT") == 0) {
            exitInput = 1;  // Set flag to exit loop
            continue;  // Skip remaining loop execution
        }

        strcpy(new_student.courses_taken[new_student.num_courses_taken++], course);
    }

    students[(*num_students)++] = new_student;
    printf("Student added successfully.\n");
}

void enterFacultyInfo(Faculty faculty[], int *num_faculty) {
    if (*num_faculty >= MAX_FACULTY) {
        printf("Maximum number of faculty reached.\n");
        return;
    }

    Faculty new_faculty;
    printf("Enter Faculty ID: ");
    scanf("%s", new_faculty.id);

    getchar(); // Consume the leftover newline from the previous input

    int validInput = 0; // Loop control variable

    while (!validInput) {
        printf("Enter Name: ");
        fgets(new_faculty.name, sizeof(new_faculty.name), stdin);
        new_faculty.name[strcspn(new_faculty.name, "\n")] = 0; // Remove newline
        validInput = 1; // Set flag to exit loop
    }

    validInput = 0; // Reset for next input
    while (!validInput) {
        printf("Enter Department: ");
        fgets(new_faculty.department, sizeof(new_faculty.department), stdin);
        new_faculty.department[strcspn(new_faculty.department, "\n")] = 0; // Remove newline
        validInput = 1;
    }

    validInput = 0;
    while (!validInput) {
        printf("Enter Deloading Units: ");
        if (scanf("%d", &new_faculty.deloading_units) == 1) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
        }
    }

    new_faculty.num_assigned = 0;

    faculty[(*num_faculty)++] = new_faculty;
    printf("Faculty added successfully.\n");
}

void enterCoursesAndPrerequisites(Course courses[], int *num_courses, Prerequisite prerequisites[], int *num_prereqs) {
    if (*num_courses >= MAX_COURSES) {
        printf("Maximum number of courses reached.\n");
        return;
    }

    Course new_course;
    printf("Enter Course Code: ");
    scanf("%s", new_course.code);
    printf("Enter Section: ");
    scanf("%s", new_course.section);
    printf("Enter Units: ");
    scanf("%d", &new_course.units);
    printf("Enter Day: ");
    scanf("%s", new_course.day);

    getchar(); // Consume leftover newline before fgets

    int validInput = 0;
    while (!validInput) {
        printf("Enter Time: ");
        fgets(new_course.time, sizeof(new_course.time), stdin);
        new_course.time[strcspn(new_course.time, "\n")] = 0; // Remove newline
        validInput = 1;
    }

    validInput = 0;
    while (!validInput) {
        printf("Enter Room: ");
        scanf("%s", new_course.room);
        validInput = 1;
    }

    getchar(); // Consume leftover newline before next fgets

    validInput = 0;
    while (!validInput) {
        printf("Enter Faculty: ");
        fgets(new_course.faculty, sizeof(new_course.faculty), stdin);
        new_course.faculty[strcspn(new_course.faculty, "\n")] = 0; // Remove newline
        validInput = 1;
    }

    courses[(*num_courses)++] = new_course;
    printf("Course added successfully.\n");

    if (*num_prereqs >= MAX_PREREQS) {
        printf("Maximum number of prerequisites reached.\n");
        return;
    }

    Prerequisite new_prereq;
    validInput = 0;
    while (!validInput) {
        printf("Enter Prerequisite Course Code: ");
        scanf("%s", new_prereq.prerequisite);
        validInput = 1;
    }

    validInput = 0;
    while (!validInput) {
        printf("Enter Course Code: ");
        scanf("%s", new_prereq.course);
        validInput = 1;
    }

    prerequisites[(*num_prereqs)++] = new_prereq;
    printf("Prerequisite added successfully.\n");
}

// Schedule and Room Assignment
void scheduleAndRoomAssignment(Course courses[], int num_courses) {
    printf("Room and Schedule Assignment:\n");
    int i;
    for (i = 0; i < num_courses; i++) {
        printf("Course: %s %s\n", courses[i].code, courses[i].section);
        printf("Assigned Room: %s\n", courses[i].room);
        printf("Assigned Schedule: %s %s\n", courses[i].day, courses[i].time);
    }
}

// Display Faculty List
void displayFacultyList(Faculty faculty[], int num_faculty) {
    printf("\nFaculty List:\n");
    int i;
    for (i = 0; i < num_faculty; i++) {
        printf("ID: %s\tName: %s\tDepartment: %s\tDeloading Units: %d\n",
               faculty[i].id, faculty[i].name, faculty[i].department, faculty[i].deloading_units);
    }
}

int checkPrerequisite(Student *student, char *course_code, Prerequisite prerequisites[], int num_prereqs) {
    int i, j;
    int allMet = 1; // Assume prerequisites are met

    for (i = 0; i < num_prereqs; i++) {
        if (strcmp(prerequisites[i].course, course_code) == 0) {
            int found = 0;

            for (j = 0; j < student->num_courses_taken && !found; j++) {
                if (strcmp(student->courses_taken[j], prerequisites[i].prerequisite) == 0) {
                    found = 1;
                }
            }

            if (!found) {
                allMet = 0; // At least one prerequisite is missing
            }
        }
    }

    return allMet;
}

void saveStudentsToFile(Student students[], int num_students) {
    FILE *file = fopen("students.txt", "w");

    int i, j;
    if (file != NULL) {
        for (i = 0; i < num_students; i++) {
            fprintf(file, "%s %s\n", students[i].name, students[i].id);
            
            for (j = 0; j < students[i].num_courses_taken; j++) {
                fprintf(file, "%s\n", students[i].courses_taken[j]);
            }

            fprintf(file, "EAF\n");

            for (j = 0; j < students[i].num_enrolled; j++) {
                fprintf(file, "%s %s %d %s %s %s %s\n",
                        students[i].enrolled_courses[j].code, students[i].enrolled_courses[j].section,
                        students[i].enrolled_courses[j].units, students[i].enrolled_courses[j].day,
                        students[i].enrolled_courses[j].time, students[i].enrolled_courses[j].room,
                        students[i].enrolled_courses[j].faculty);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    } else {
        printf("Error opening file.\n");
    }
}

void saveFacultyToFile(Faculty faculty[], int num_faculty) {
    FILE *file = fopen("faculty.txt", "w");

    int i, j;
    if (file != NULL) {
        for (i = 0; i < num_faculty; i++) {
            fprintf(file, "%s %s %s %d\n", faculty[i].name, faculty[i].id, faculty[i].department, faculty[i].deloading_units);
            
            for (j = 0; j < faculty[i].num_assigned; j++) {
                fprintf(file, "%s %s\n", faculty[i].assigned_courses[j].code, faculty[i].assigned_courses[j].section);
            }

            fprintf(file, "\n");
        }
        fclose(file);
    } else {
        printf("Error opening file.\n");
    }
}

void saveCoursesToFile(Course courses[], int num_courses) {
    FILE *file = fopen("courses.txt", "w");

    int i;
    if (file != NULL) {
        for (i = 0; i < num_courses; i++) {
            fprintf(file, "%s %s %d %s %s %s %s\n",
                    courses[i].code, courses[i].section, courses[i].units,
                    courses[i].day, courses[i].time, courses[i].room, courses[i].faculty);
        }
        fclose(file);
    } else {
        printf("Error opening file.\n");
    }
}

void savePrerequisitesToFile(Prerequisite prerequisites[], int num_prereqs) {
    FILE *file = fopen("prerequisites.txt", "w");

    int i;
    if (file != NULL) {
        for (i = 0; i < num_prereqs; i++) {
            fprintf(file, "%s %s\n", prerequisites[i].prerequisite, prerequisites[i].course);
        }
        fclose(file);
    } else {
        printf("Error opening file.\n");
    }
}

void loadStudentsFromFile(Student students[], int *num_students) {
    FILE *file = fopen("students.txt", "r");

    if (file != NULL) {
        *num_students = 0;
        while (fscanf(file, "%s %s", students[*num_students].name, students[*num_students].id) != EOF) {
            students[*num_students].num_courses_taken = 0;
            students[*num_students].num_enrolled = 0;

            char buffer[MAX_CODE_LEN];
            while (fscanf(file, "%s", buffer) != EOF && strcmp(buffer, "EAF") != 0) {
                strcpy(students[*num_students].courses_taken[students[*num_students].num_courses_taken++], buffer);
            }

            while (fscanf(file, "%s %s %d %s %s %s %s",
                          students[*num_students].enrolled_courses[students[*num_students].num_enrolled].code,
                          students[*num_students].enrolled_courses[students[*num_students].num_enrolled].section,
                          &students[*num_students].enrolled_courses[students[*num_students].num_enrolled].units,
                          students[*num_students].enrolled_courses[students[*num_students].num_enrolled].day,
                          students[*num_students].enrolled_courses[students[*num_students].num_enrolled].time,
                          students[*num_students].enrolled_courses[students[*num_students].num_enrolled].room,
                          students[*num_students].enrolled_courses[students[*num_students].num_enrolled].faculty) != EOF) {
                students[*num_students].num_enrolled++;
            }

            (*num_students)++;
        }
        fclose(file);
    } else {
        printf("No student data found.\n");
    }
}

void loadFacultyFromFile(Faculty faculty[], int *num_faculty) {
    FILE *file = fopen("faculty.txt", "r");

    if (file != NULL) {
        *num_faculty = 0;

        while (fscanf(file, "%s %s %s %d", 
                      faculty[*num_faculty].name, 
                      faculty[*num_faculty].id, 
                      faculty[*num_faculty].department, 
                      &faculty[*num_faculty].deloading_units) != EOF) {

            faculty[*num_faculty].num_assigned = 0;

            while (fscanf(file, "%s %s", 
                          faculty[*num_faculty].assigned_courses[faculty[*num_faculty].num_assigned].code, 
                          faculty[*num_faculty].assigned_courses[faculty[*num_faculty].num_assigned].section) != EOF) {
                faculty[*num_faculty].num_assigned++;
            }

            (*num_faculty)++;
        }

        fclose(file);
    } else {
        printf("No faculty data found.\n");
    }
}

void loadCoursesFromFile(Course courses[], int *num_courses) {
    FILE *file = fopen("courses.txt", "r");

    if (file != NULL) {
        *num_courses = 0;

        while (fscanf(file, "%s %s %d %s %s %s %s",
                      courses[*num_courses].code, 
                      courses[*num_courses].section, 
                      &courses[*num_courses].units,
                      courses[*num_courses].day, 
                      courses[*num_courses].time, 
                      courses[*num_courses].room, 
                      courses[*num_courses].faculty) != EOF) {
            (*num_courses)++;
        }

        fclose(file);
    } else {
        printf("No course data found.\n");
    }
}

void loadPrerequisitesFromFile(Prerequisite prerequisites[], int *num_prereqs) {
    FILE *file = fopen("prerequisites.txt", "r");

    if (file != NULL) {
        *num_prereqs = 0;

        while (fscanf(file, "%s %s", 
                      prerequisites[*num_prereqs].prerequisite, 
                      prerequisites[*num_prereqs].course) != EOF) {
            (*num_prereqs)++;
        }

        fclose(file);
    } else {
        printf("No prerequisite data found.\n");
    }
}

