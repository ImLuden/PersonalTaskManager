#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <stdexcept>
#include <algorithm> // For std::transform
#include <ctime>
#include <sstream>
#include <fstream>
#include <regex> // For regex validation (Date format to DD/MM/YYYY)

// Helper function to trim (to avoid spaces) and convert a string to lowercase
std::string toLowerAndTrim(const std::string& str);

// Enum that defines the task status. 
// Pending - task is newly created; Completed - task was marked as done by user; Expired - due date passed without completion.
enum class TaskStatus { Pending, Completed, Expired };

// Struct representing a Task. Each Task has an ID, title, description, due date, and status.
// The to_json() and from_json() functions help convert Task objects to and from JSON for easy file storage.
struct Task 

{
    int id = 0;
    std::string title;
    std::string description;
    std::string due_date; // Date format DD/MM/YY as a simple string
    TaskStatus status = TaskStatus::Pending; // Default status is 'Pending'

    // Converts a TaskStatus enum value to its corresponding string representation.
    static std::string task_status_to_string(TaskStatus status) ;

    static TaskStatus stringToTaskStatus(const std::string& statusStr);

    // Serializes the Task to a JSON object for easy storage in a file
    nlohmann::json to_json() const;

    // Deserializes a JSON object back into a Task object
    static Task from_json(const nlohmann::json& j);
};

// Class that manages tasks. It loads tasks from a database file, stores them in a vector, and provides functions
// to add, delete, edit, and manage task statuses. The constructor initializes the database and loads existing tasks.
class TaskManager 
{
public:


    // Constructor with a database file path. This opens the file and loads any previously saved tasks.
    TaskManager(const std::string& db_file);

    // Destructor that cleans up resources (if any), primarily here for completeness.
    ~TaskManager();

    void run();

    // Greeting of the program
    void display_greeting();
    
    // Function to display the main menu options to the user
    void display_menu();

    bool ask_back_to_menu(const std::string& prompt);

    // Function to display all tasks
    void display_tasks();

    // Input of a new task's title, description, due_date
    void add_new_task();

    // Adds a new task with provided title, description, and due date, assigns an ID, and saves it to the file.
    void add_task(const std::string& title, const std::string& description, const std::string& due_date);

    // Deletes a task by its unique ID and removes it from the file.
    void delete_task(int task_id);

    // Iterates through tasks to update status based on current date and due date.
    void update_task_status();

    // Changes a task’s status (pending, completed, or expired) based on task ID.
    void change_task_status();

    // Edits task details by ID (title, description, due date), finds the task in the list, and updates its information.
    void edit_task();

    // Deletes all tasks and saves an empty state to the file.
    void delete_all_tasks();

    void delete_task();

private:
    std::vector<Task> tasks; // Stores all tasks in memory for quick access and manipulation
    std::string db_file; // Path to the JSON file storing task data   

    bool go_back(std::string& input, const std::string& prompt);

    // Loads tasks from a JSON database file into memory (called in the constructor).
    void load_tasks();

    // Generates a new unique task ID by checking the current highest ID in the list and adding one.
    int generate_task_id() const;

    // Saves all current tasks in memory to the JSON file, overwriting previous data.
    void save_tasks() const;    

    // Helper function to get the current date.
    std::string get_current_date() const;

    // Helper function get a task number in vector 
    Task& get_task_by_number(int task_number) ;

    // Validates date type (should be DD/MM/YYYY)
    std::string normalize_date(const std::string& input_date) const;

    std::string get_and_validate_due_date();

    TaskStatus get_and_update_status();

    std::string get_and_update_input(const std::string& prompt, const std::string& current_value);

    



};


