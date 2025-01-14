#include "task_manager.h"


// Constructor initializes TaskManager with the given database file path, loads existing tasks and checks if any tasks need status updates.

TaskManager::TaskManager(const std::string& db_file) : db_file(db_file) 
{    
    load_tasks();          // Load tasks from file (JSON format)
    update_task_status();   // Update task statuses based on due dates
}

TaskManager::~TaskManager() 
{
    // Empty destructor, no specific cleanup required.
}


void TaskManager::clear_input() 
{

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Helper function to trim and convert a string to lowercase
std::string toLowerAndTrim(const std::string& str)
{
    // Find the first and last non-whitespace characters
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");

    // If the string is entirely whitespace, return an empty string
    if (start == std::string::npos)
    {
        return "";
    }

    // Create a trimmed substring
    std::string trimmed = str.substr(start, end - start + 1);

    // Convert to lowercase
    std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), ::tolower);

    return trimmed;
}

 
void TaskManager::run()
{
    bool running = true;

    display_greeting();
    display_tasks();

    while (running)
    {
        display_menu();

        std::string input;
        std::cout << "Choose an option: ";
        input = get_valid_input("");

        try
        {
            // Convert the input to an integer
            int choice = std::stoi(input);

            switch (choice)
            {
            case 1:
                display_tasks();
                continue;
            case 2:
                if (!ask_back_to_menu("Do you want to add a new task? "))
                    add_new_task();
                continue;
            case 3:
                if (!ask_back_to_menu("Do you want to update all task statuses? "))
                    update_task_status();
                continue;
            case 4:
                if (!ask_back_to_menu("Do you want to change task status? "))
                    change_task_status();
                continue;
            case 5:
                if (!ask_back_to_menu("Do you want to edit task parameters by its number? "))
                    edit_task();
                continue;
            case 6:
                if (!ask_back_to_menu("Do you want to delete a task by its number? "))
                    delete_task();
                continue;
            case 7:
                if (!ask_back_to_menu("Do you want to delete all tasks? "))
                    delete_all_tasks();
                continue;
            case 8:
                if (!ask_back_to_menu("Do you want to exit the program? "))
                {
                    running = false;
                    std::cout << "Exiting Personal Task Manager. Goodbye!\n";
                }
                return;
            default:
                std::cerr << "Invalid input. Please enter a number from the menu.\n";
                continue;
            }
        }
        catch (const std::exception&)
        {
            std::cerr << "Invalid input. Please enter a valid number.\n";
        }
    }
}


// Display the greeting message and current date
void TaskManager::display_greeting()
{
    std::cout << "Hello, I'm your Personal Task Manager." << std::endl;
    std::cout << "Today's date: " << get_current_date() << std::endl;
    std::cout << "Welcome!\n";
}

void TaskManager::display_menu()
{

    std::cout << "\n===== Task Manager Menu =====\n";
    std::cout << "1. Display all tasks\n";
    std::cout << "2. Add a new task\n";
    std::cout << "3. Update task statuses\n";
    std::cout << "4. Change task's status\n";
    std::cout << "5. Edit task \n";
    std::cout << "6. Delete task by its number\n";
    std::cout << "7. Delete all tasks\n";
    std::cout << "8. Exit\n";
    std::cout << "=============================\n";

}


void TaskManager::display_tasks()
{
    std::cout << "Here are your tasks:\n" << std::endl;

    // Check if there are any tasks
    if (tasks.empty())
    {
        std::string user_answer;
        std::cout << "No tasks available. Would you like to add a new task? (yes/no): ";
        std::cin >> user_answer;

        // Convert input to lowercase for consistency
        user_answer = toLowerAndTrim(user_answer);

        if (user_answer == "yes")
        {
            add_new_task();
        }
        else if (user_answer == "no")
        {
            std::cout << "Returning to the menu.\n";
        }
        else
        {
            std::cout << "Invalid input. Returning to the menu.\n";
        }
        return; // Exit the function after handling empty tasks
    }

    // Update task statuses before displaying
    update_task_status();

    // Display tasks in a formatted list
    for (auto& task : tasks)
    {
        std::cout << "Task ID: " << task.id << "\n";
        std::cout << "Title: " << task.title << "\n";
        std::cout << "Description: " << task.description << "\n";
        std::cout << "Due Date: " << task.due_date << "\n";
        std::cout << "Status: " << Task::task_status_to_string(task.status) << "\n";
        std::cout << "--------------------------" << std::endl;
    }

    // Save changes if statuses were updated
    save_tasks();
}

void TaskManager::add_new_task()
{
    std::string title, description, due_date;
    int step = 0; // Tracks the current step in the process
    // clear_input();


    while (true)
    {
        std::string input; // Temporary input buffer for all steps
        std::string prompt;

        switch (step)
        {
        case 0: // Input task title
            prompt = "Enter task title (or 'q' to cancel): ";
            input = get_valid_input(prompt);
            if (go_back(input, "")) return; // Exit to menu if 'q' is entered
            title = input; // Assign input to title
            step++; // Proceed to the next step
            continue;

        case 1: // Input task description
            prompt = "Enter task description (or 'q' to cancel): ";
            input = get_valid_input(prompt);
            if (go_back(input, ""))
            {
                step--; // Go back to the previous step
                continue;
            }
            description = input; // Assign input to description
            step++; // Proceed to the next step
            continue;

        case 2: // Input task due date
            prompt = "Enter task due date (or 'q' to cancel): ";
            input = get_valid_input(prompt);
            if (go_back(input, ""))
            {
                step--; // Go back to the previous step
                continue;
            }

            try
            {
                // Normalize and validate the due date
                due_date = normalize_date(input);

                // Add the task with the normalized date
                add_task(title, description, due_date);
                return; // Exit the function after successful addition
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error: " << e.what() << ". Please re-enter the due date.\n";
            }
            continue;

        default:
            std::cerr << "Unexpected error occurred.\n";
            return;
        }
    }
}

// Converts a TaskStatus enum value to its corresponding string representation.
std::string Task::task_status_to_string(TaskStatus status) 

{
    switch (status) 
    {
    case TaskStatus::Pending:
        return "Pending";
    case TaskStatus::Completed:
        return "Completed";
    case TaskStatus::Expired:
        return "Expired";
    default:
        return "Unknown"; // Fallback for unexpected values
    }
}





bool TaskManager::ask_back_to_menu(const std::string& prompt)
{
    while (true)
    {
        try
        {
            std::cout << prompt << "(yes to proceed, no to go back to the menu): ";

            
            std::string user_input = get_valid_input("");
            user_input = toLowerAndTrim(user_input);

            if (user_input == "yes") return false; // Proceed
            if (user_input == "no") return true;  // Go back to menu

            std::cerr << "Invalid input. Please enter 'yes' or 'no'.\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << ". Please try again.\n";
        }
    }
}


std::string TaskManager::normalize_date(const std::string& input_date) const
{
    std::string upd_input_date = toLowerAndTrim(input_date);
    // Extract the current day, month, and year for validation
    int current_day = std::stoi(get_current_date().substr(0, 2));
    int current_month = std::stoi(get_current_date().substr(3, 2));
    int current_year = std::stoi(get_current_date().substr(6, 4));

    // Split the input date into day, month, and year
    std::istringstream date_stream(upd_input_date);
    std::string day, month, year;

    std::getline(date_stream, day, '/');
    std::getline(date_stream, month, '/');
    std::getline(date_stream, year);

    // Validate day, month, and year parts
    if (day.empty() || month.empty() || year.empty() || day.size() > 2 || month.size() > 2 || year.size() > 4)
    {
        throw std::invalid_argument("Invalid date format. Please use DD/MM/YYYY or DD/MM/YY.");
    }

    int day_int = std::stoi(day);
    int month_int = std::stoi(month);

    if (day_int < 1 || day_int > 31 || month_int < 1 || month_int > 12)
    {
        throw std::out_of_range("Invalid day or month value. Day must be between 1 and 31, and month between 1 and 12.");
    }

    // Normalize the year
    if (year.size() == 2)
    {
        year = (year >= "00" && year <= "99") ? "20" + year : throw std::out_of_range("Invalid year format.");
    }

    int year_int = std::stoi(year);

    // Validate the year
    if (year_int < current_year)
    {
        throw std::out_of_range("Invalid year. You cannot set a task for a past year.");
    }

    // Validate the month and day if the year matches the current year
    if (year_int == current_year)
    {
        if (month_int < current_month)
        {
            throw std::out_of_range("Invalid month. You cannot set a task for a past month.");
        }
        if (month_int == current_month && day_int < current_day)
        {
            throw std::out_of_range("Invalid day. You cannot set a task for a past day.");
        }
    }

    // Construct the normalized date
    std::ostringstream normalized_date;
    normalized_date << std::setw(2) << std::setfill('0') << day_int << "/"
        << std::setw(2) << std::setfill('0') << month_int << "/"
        << year;

    return normalized_date.str();
}


int TaskManager::generate_task_id() const 
{
    if (tasks.empty()) 
    {
        return 1; // Return 1 if no tasks exist.
    }
    // Find the highest existing ID and increment it.
    int max_id = 0;
    for (const auto& task : tasks) 
    {
        if (task.id > max_id) // if id of the object is greater than max_id it is max_id
        {
            max_id = task.id;
        }
    }
    return max_id + 1; // Return the next available ID.
}

std::string TaskManager::get_current_date() const
{
    auto t = std::time(nullptr); // Get the current time
    auto tm = *std::localtime(&t); // Convert to local time
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d/%m/%Y"); // Format the date
    return oss.str(); // Return formatted string
}






bool TaskManager::go_back(const std::string& input, const std::string& prompt) {
    if (!prompt.empty()) 
    {
        std::cout << prompt;
    }
    // If input = "q", go back
    return toLowerAndTrim(input) == "q";
}

// Function to get valid input from the user
std::string TaskManager::get_valid_input(const std::string& prompt) 
{
    while (true) 
    {
       
        std::cout << prompt;
        
        std::string input;
        std::getline(std::cin, input); // Get user input

        // Check for empty input
        if (input.empty()) 
        {
            std::cerr << "Error: Input cannot be empty. Please try again.\n";
            continue;
        }

        return input; // Valid input received
    }
}

using json = nlohmann::json;

// Method to convert a string to TaskStatus using unordered_map
TaskStatus Task::stringToTaskStatus(const std::string& statusStr) 
{
    
    static const std::unordered_map<std::string, TaskStatus> statusMap = 
    {
        {"pending", TaskStatus::Pending},
        {"completed", TaskStatus::Completed},
        {"expired", TaskStatus::Expired}
    };

    std::string lowerStatus = toLowerAndTrim(statusStr);

    auto it = statusMap.find(lowerStatus);
    if (it != statusMap.end()) 
    {
        return it->second; // Return the corresponding TaskStatus
    }

    throw std::invalid_argument("Invalid task status string: " + statusStr);
}


// Loads tasks from the database file into memory.
// This method reads the tasks from a JSON file and converts each task
// into a Task object, which is then added to the vector `tasks`.


void TaskManager::load_tasks()
{
    // Check if the file exists. If not, create it with empty JSON array
    std::ifstream file_check(db_file);
    if (!file_check.is_open())
    {
        std::cerr << "Database file not found. Creating a new one: " << db_file << "\n";
        std::ofstream new_file(db_file);
        if (!new_file.is_open())
        {
            std::cerr << "Error: Failed to create the database file.\n";
            return;
        }
        new_file << "[]"; // Initialize file with empty JSON array
        new_file.close();
    }
    file_check.close();

    // Open the database file for reading
    std::ifstream file(db_file);
    if (!file.is_open())
    {
        std::cerr << "Unable to open the database file: " << db_file << "\n";
        return; // Exit if the file cannot be opened
    }

    // Check if the file is empty
    if (file.peek() == std::ifstream::traits_type::eof())
    {
        std::cerr << "Database file is empty. No tasks loaded.\n";
        return; // Exit 
    }

    json tasks_json;
    try
    {
        file >> tasks_json; // Parse the JSON data from the file
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error reading the database file: " << e.what() << "\n";
        return; // Exit if there's an error in parsing
    }

    // Iterate over each JSON object (task) in the parsed JSON data
    for (const auto& task_data : tasks_json)
    {
        Task task;
        try
        {
            task.id = task_data.at("id").get<int>();
            task.title = task_data.at("title").get<std::string>();
            task.description = task_data.at("description").get<std::string>();
            task.due_date = task_data.at("due_date").get<std::string>();
            task.status = Task::stringToTaskStatus(task_data.at("status").get<std::string>());
            tasks.push_back(task); // Add task to the vector of tasks
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error loading task: " << e.what() << "\n";
        }
    }

    // std::cout << "Tasks loaded successfully. Total tasks: " << tasks.size() << "\n";
}

// Saves all current tasks from the tasks vector to the JSON file.
// Converts each task to JSON format and writes the full list to the file.
void TaskManager::save_tasks() const 
{
    nlohmann::json j = nlohmann::json::array(); // JSON array to hold all tasks

    // Serialize each task in the vector to JSON format and add it to the array
    for (const auto& task : tasks) {
        j.push_back(task.to_json());
    }

    // Open the file and write all tasks in JSON format
    std::ofstream file(db_file);
    file << j.dump(4); // Write JSON with 4 spaces indentation
}

// Adds a new task to the list, assigns a unique ID, sets title, description, due date,
// and saves the task list to file.
void TaskManager::add_task(const std::string& title, const std::string& description, const std::string& due_date) 
{
    Task task{ generate_task_id(), title, description, due_date }; // Create new task
    tasks.push_back(task); // Add task to tasks vector
    save_tasks(); // Save updated list to file
    std::cout << "Task successfully added.\n";
}


void TaskManager::delete_task(int task_id) 
{
    // Find the task with the matching ID
    auto it = std::find_if(tasks.begin(), tasks.end(), [task_id](const Task& task) 
        {
        return task.id == task_id;
        });

    // If the task is not found, show an error message
    if (it == tasks.end()) 
    {
        std::cerr << "Error: Task with ID " << task_id << " not found.\n";
        return;
    }

    // Remove the task from the vector
    tasks.erase(it);

    // Save the updated list to the JSON file
    save_tasks();

    // Notify the user about successful deletion
    std::cout << "Task with ID " << task_id << " has been successfully deleted.\n";
}



nlohmann::json Task::to_json() const
{
    return 
    {
        {"id", id},
        {"title", title},
        {"description", description},
        {"due_date", due_date},
        {"status", task_status_to_string(status)} 
    };
}

// Converts JSON data back to Task object with all necessary fields.
// This static method is used to load tasks from JSON file format.
Task Task::from_json(const nlohmann::json& j) 
{
    Task task;

    task.id = j.at("id").get<int>();
    task.title = j.at("title").get<std::string>();
    task.description = j.at("description").get<std::string>();
    task.due_date = j.at("due_date").get<std::string>();

    // Convert the "status" field based on its type
    if (j.contains("status")) {
        if (j.at("status").is_string()) {
            task.status = stringToTaskStatus(j.at("status").get<std::string>());
        }
        else if (j.at("status").is_number_integer()) {
            // Handle numerical status
            task.status = static_cast<TaskStatus>(j.at("status").get<int>());
        }
        else {
            throw std::invalid_argument("Invalid type for 'status' field.");
        }
    }

    return task;
}



void TaskManager::update_task_status()
{
    for (auto& task : tasks)
    {
        // Parse due_date into a std::tm structure
        std::tm due_date_tm = {};
        std::istringstream ss(task.due_date);
        ss >> std::get_time(&due_date_tm, "%d/%m/%Y"); // Format: DD/MM/YYYY

        if (ss.fail())
        {
            std::cerr << "Invalid date format for task: " << task.title << std::endl;
            continue; // Skip invalid dates
        }

        // Zero-out time fields for accurate day comparison
        due_date_tm.tm_hour = 0;
        due_date_tm.tm_min = 0;
        due_date_tm.tm_sec = 0;

        // Get current date and zero-out its time fields
        std::time_t now = std::time(nullptr);
        std::tm now_tm = *std::localtime(&now);
        now_tm.tm_hour = 0;
        now_tm.tm_min = 0;
        now_tm.tm_sec = 0;

        // Convert both dates back to time_t for comparison
        std::time_t due_time = std::mktime(&due_date_tm);
        std::time_t now_time = std::mktime(&now_tm);

        // Compare the due date with the current date
        if (due_time < now_time) // If the due date is strictly in the past
        {
            if (task.status != TaskStatus::Completed && task.status != TaskStatus::Expired)
            {
                task.status = TaskStatus::Expired;
                std::cout << "Task '" << task.title << "' has expired and status updated to 'Expired'.\n";
            }
        }
        else if (due_time == now_time) // Due date is today
        {
            if (task.status != TaskStatus::Completed)
            {
                task.status = TaskStatus::Pending;
            }
        }
        else // Due date is in the future
        {
            if (task.status != TaskStatus::Completed)
            {
                task.status = TaskStatus::Pending;
            }
        }
    }

    save_tasks(); // Save updated tasks back to the file
}




void TaskManager::change_task_status()
{
    int task_id;
    std::string new_status;

    while (true)
    {
        std::cout << "Enter the ID of the task to change (or enter 0 to go back): ";

        // Read the task ID
        std::cin >> task_id;

        if (std::cin.fail())
        {
            // Handle invalid input
            std::cerr << "Invalid input. Please enter a valid numeric task ID.\n";
            clear_input();
            continue;
        }

        if (task_id == 0)
        {
            // User wants to go back
            std::cout << "Returning to the menu.\n";
            return;
        }

        // Find the task with the specified ID
        auto task_it = std::find_if(tasks.begin(), tasks.end(), [task_id](const Task& task) {
            return task.id == task_id;
            });

        if (task_it == tasks.end())
        {
            std::cerr << "Error: Task with the given ID not found. Please try again.\n";
            continue;
        }

        // Found a valid task, allow the user to update its status
        while (true)
        {
            std::cout << "Enter new status (Pending, Completed): ";
            clear_input();

            std::getline(std::cin, new_status);  

            if (toLowerAndTrim(new_status) == "q")
            {
                std::cout << "Returning back.\n";
                break;
            }

            if (new_status == "pending" || new_status == "completed")
            {
                task_it->status = Task::stringToTaskStatus(new_status);
                save_tasks(); // Save changes to the database
                std::cout << "Task status updated successfully.\n";
                return;
            }
            else if (new_status == "expired")
            {
                std::cerr << "Error: The 'Expired' status is set automatically when the task deadline is missed.\n";
            }
            else
            {
                std::cerr << "Error: Invalid status. Please enter either 'Pending' or 'Completed'.\n";
            }
        }
    }
}

Task& TaskManager::get_task_by_number(int task_number) 
{

    if (task_number < 1 || task_number > tasks.size()) 
    {
        throw std::out_of_range("Invalid task number");
    }

    return tasks[task_number - 1]; // 1-based index to 0-based index
}


std::string TaskManager::get_and_validate_due_date() 
{
    while (true) 
    {
        std::string prompt =  "Enter new due date (DD/MM/YYYY or DD/MM/YY). Enter q to go back: ";
        
        std::string new_due_date = get_valid_input(prompt);

        // Check if the user wants to cancel and go back
        if (go_back(new_due_date, "")) {
            std::cout << "Date update canceled.\n";
            return ""; // Return empty string to indicate cancellation
        }
        try 
        {
            std::string normalized_date = normalize_date(new_due_date);
            std::cout << "Date updated successfully to: " << normalized_date << "\n";
            return normalize_date(new_due_date); // Validate and normalize the date
        }
        catch (const std::invalid_argument& e) 
        {
            std::cerr << "Error: " << e.what() << " Please try again.\n";
        }
       
    }
}


TaskStatus TaskManager::get_and_update_status(const std::string& prompt, const std::string& current_value)
{
    std::string current_status = current_value;

    while (true)
    {
        std::string new_status;
        std::cout << prompt;

        // Read and preprocess input
        new_status = get_valid_input(prompt);
        new_status = toLowerAndTrim(new_status);

        // Check if the user wants to go back
        if (go_back(new_status, ""))
        {
            std::cout << "Returning to previous step.\n";
            return Task::stringToTaskStatus(current_status);
        }

        // Explicitly handle "expired" as an invalid input
        if (new_status == "expired")
        {
            std::cerr << "Error: The 'Expired' status is set automatically when the task deadline is missed.\n";
            continue; // Ask for input again
        }

        // Attempt to convert input to TaskStatus
        try
        {
            TaskStatus updated_status = Task::stringToTaskStatus(new_status);
            std::cout << "Task status successfully updated to " << new_status << ".\n";
            return updated_status; // Return valid status
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "Error: " << e.what() << ". Please enter a valid status (Pending, Completed).\n";
        }
        catch (...)
        {
            std::cerr << "An unknown error occurred. Please try again.\n";
        }
    }
}



//std::string TaskManager::get_and_update_input(const std::string& prompt, const std::string& current_value)
//{
//    while (true)
//    {
//        std::string input;
//        std::cout << prompt; // Display the prompt to the user
//                 
//        std::getline(std::cin, input); // Take user input
//
//        if (go_back(input, "")) {
//            std::cout << "Returning back...\n";
//            return current_value; // Return the current value if the user cancels
//        }
//
//        if (input.empty()) {
//            std::cerr << "Error: Input cannot be empty. Please try again.\n";
//            continue;
//        }
//
//        std::cout << "Task has been successfully updated.\n";
//        return input; // Return valid input
//    }
//}

std::string TaskManager::get_and_update_input(const std::string& prompt, const std::string& current_value) 
{
    while (true) {
        std::string input = get_valid_input(prompt);

        if (go_back(input, "")) {
            return current_value; // Return current value if user exits
        }

        return input; // Valid input received
    }
}


//void TaskManager::edit_task() 
//{
//    
//    if (tasks.empty()) 
//    {
//        std::cout << "No tasks available to edit.\n";
//        return;
//    }
//
//    try 
//    {
//        int task_number;
//        std::cout << "Enter the number of the task you want to edit (0 to go back): ";
//        clear_input();
//        std::cin >> task_number;
//        
//        
//        if (task_number == 0) return;
//        
//        Task& task = get_task_by_number(task_number);
//
//        // Display the selected task's details
//        std::cout << "Editing Task #" << task_number << ":\n";
//        std::cout << "Title: " << task.title << "\n";
//        std::cout << "Description: " << task.description << "\n";
//        std::cout << "Due Date: " << task.due_date << "\n";
//        std::cout << "Status: " << Task::task_status_to_string(task.status) << "\n";
//
//        bool editing = true;
//
//        while (editing) 
//        {
//            std::cout << "\nWhat would you like to edit?\n";
//            std::cout << "1. Title\n";
//            std::cout << "2. Description\n";
//            std::cout << "3. Due Date\n";
//            std::cout << "4. Status\n";
//            std::cout << "5. Finish editing\n";
//            std::cout << "Choose an option: ";
//
//            int option; 
//
//            std::cin >> option;
//
//            switch (option) 
//            {
//            case 1:
//            {
//                task.title = get_and_update_input("Enter new title (q to go back): ", task.title);                
//                continue;
//            }
//            case 2:
//            {
//                task.description = get_and_update_input("Enter new description (q to go back): ", task.description);                
//                continue;
//            }
//            case 3: 
//            {
//                std::string new_due_date = get_and_validate_due_date();
//                if (new_due_date.empty()) 
//                {
//                    continue; 
//                }
//                task.due_date = new_due_date;                
//                continue;
//            }
//            case 4: {
//                task.status = get_and_update_status("Enter new status (Pending, Completed). Enter q to go back: ", Task::task_status_to_string(task.status));
//                save_tasks(); // Save changes to file                
//                continue;;
//            }
//            case 5: 
//            {
//                editing = false;
//                std::cout << "Finished editing task.\n";
//                return;
//            }
//            default:
//                std::cout << "Invalid option. Please try again.\n";
//                continue;
//            }
//        }
//    }
//    catch (const std::out_of_range& e) {
//        std::cerr << "Error: " << e.what() << " Please try again.\n";
//    }
//    catch (const std::invalid_argument& e) {
//        std::cerr << "Invalid input: " << e.what() << "\n";
//    }
//    catch (const std::exception& e) {
//        std::cerr << "An unexpected error occurred: " << e.what() << "\n";
//    }
//}

void TaskManager::edit_task()
{
    if (tasks.empty())
    {
        std::cout << "No tasks available to edit.\n";
        return;
    }


    int task_number;
    std::cout << "Enter the number of the task you want to edit (0 to go back): ";

    // clear_input(); // Clear any leftover input

    std::cin >> task_number;

    // If the user enters 0, go back to the previous menu
    if (task_number == 0) return;

    // Get the task by its number
    Task& task = get_task_by_number(task_number);

    // Display the selected task's details
    std::cout << "Editing Task #" << task_number << ":\n";
    std::cout << "Title: " << task.title << "\n";
    std::cout << "Description: " << task.description << "\n";
    std::cout << "Due Date: " << task.due_date << "\n";
    std::cout << "Status: " << Task::task_status_to_string(task.status) << "\n";

    while (true)
    {
        std::cout << "\nWhat would you like to edit?\n";
        std::cout << "1. Title\n";
        std::cout << "2. Description\n";
        std::cout << "3. Due Date\n";
        std::cout << "4. Status\n";
        std::cout << "5. Finish editing\n";
        std::cout << "Choose an option: ";

        int option;
        std::cin >> option;

        // Clear input buffer before processing
        clear_input();

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input. Please enter a valid option.\n";
            continue;
        }

        switch (option)
        {
        case 1:
            task.title = get_and_update_input("Enter new title (q to go back): ", task.title);
            continue;

        case 2:
            task.description = get_and_update_input("Enter new description (q to go back): ", task.description);
            continue;

        case 3:
            task.due_date = get_and_validate_due_date();
            continue;

        case 4:
            task.status = get_and_update_status(
                "Enter new status (Pending, Completed). Enter q to go back: ",
                Task::task_status_to_string(task.status)
            );
            continue;

        case 5:
            std::cout << "Finished editing task.\n";
            return;

        default:
            std::cerr << "Invalid option. Please try again.\n";
            continue;
        }
    }
}






void TaskManager::delete_task()
{
    int task_id;

    // Infinite loop until the task is found or user decides to cancel
    while (true)
    {
        std::cout << "Enter the ID of the task to delete: ";
        std::cin >> task_id;
        

        // Check for invalid input (non-numeric or other errors)
        if (std::cin.fail())
        {
            clear_input(); // clear cin buffer
            std::cout << "Invalid input. Please enter a valid task ID.\n";
            continue; // ask for input again
        }

        auto it = std::find_if(tasks.begin(), tasks.end(), [task_id](const Task& task)
            {
                return task.id == task_id;
            });

        if (it != tasks.end())
        {
            // Task found, delete it
            tasks.erase(it);

            // Renumber the remaining tasks sequentially
            int new_id = 1;
            for (auto& task : tasks)
                task.id = new_id++;

            save_tasks();    // Save changes to file
            std::cout << "Task number " << task_id << " has been successfully deleted.\n";
            break; // Exit loop after successful deletion
        }
        else
        {
            std::cerr << "Task with ID " << task_id << " not found. Try again.\n";
            // Ask if user wants to continue or cancel
            break; // Exit loop if user chooses to cancel
          
        }
    }
}


void TaskManager::delete_all_tasks() 
{
    tasks.clear(); // Clear all tasks from the vector
    save_tasks(); // Save the updated (empty) list to the JSON file
    std::cout << "All tasks have been deleted successfully.\n";
}

