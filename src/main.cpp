#include "task_manager.h"





int main()
{
    TaskManager manager("tasks.json"); // Initialize TaskManager with the JSON file
    manager.run(); // Start the main loop
    return 0;
}




//void display_menu(TaskManager& manager)
//{
//    manager.display_menu();
//}
//
//// Display all tasks
//void display_all_tasks(TaskManager& manager) 
//{
//    manager.display_tasks();
//}
//
//// Update task statuses
//void update_task_statuses(TaskManager& manager) 
//{
//    manager.update_task_status();
//    std::cout << "Task statuses updated successfully.\n";
//}
//
//void add_new_task(TaskManager& manager)
//{
//    manager.add_new_task();
//}
//
//// Change a specific task's status
//void change_task_status(TaskManager& manager) 
//{
//
//    manager.change_task_status();
//}
//
//// Delete task by number
//void delete_task(TaskManager& manager)
//{
//    manager.delete_task();
//}
//
//// Delete all tasks
//void delete_all_tasks(TaskManager& manager) 
//{
//    manager.delete_all_tasks();   
//}
//
//
//// Change task parameters
//void edit_task(TaskManager& manager)
//{
//    manager.edit_task();
//}
//
//bool ask_back_to_menu(const std::string& prompt)
//{
//    while (true)
//    {
//        std::string user_input;
//        std::cout << prompt << " (yes to proceed, no to go back to menu): ";
//        std::cin >> user_input;
//
//        // Convert input to lowercase to handle case-insensitive input
//        user_input = toLowerAndTrim(user_input);
//
//        if (user_input == "yes")
//        {
//            return false; // proceed with the action
//        }
//        else if (user_input == "no")
//        {
//            return true; // go back to the menu
//        }
//        else
//        {
//            std::cout << "Invalid input. Please enter 'yes' or 'no'.\n";
//            // Optionally, you can add a return statement here if you want to force the loop to continue asking for valid input.
//        }
//    }
//}
//
//
//
//
//int main()
//{
//    TaskManager manager("tasks.json"); // Initialize TaskManager with JSON database file
//        
//    bool running = true; // Main loop flag
//    
//    while (running)
//    {
//        display_menu(manager);
//
//        std::cout << "Choose an option: ";
//        int choice;
//        std::cin >> choice;
//
//        // Ïðîâåðêà îøèáêè ââîäà
//        if (std::cin.fail())
//        {
//            std::cin.clear();
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//            std::cerr << "Invalid input. Please enter a valid number.\n";
//            continue;
//        }
//        
//
//        switch (choice)
//        {
//        case 1:
//            // Display all tasks 
//            display_all_tasks(manager);
//            continue; 
//        case 2:
//            // Logic to add task details 
//            if (!ask_back_to_menu("Do you want to add a new task?")) 
//                add_new_task(manager);            
//            continue;            
//        case 3:
//            // Logic to update task statuses
//            if (!ask_back_to_menu("Do you want to update all task statuses? "))
//                update_task_statuses(manager);
//            continue;
//        case 4:
//            // Logic to change task status
//            if (!ask_back_to_menu("Do you want to change task status? "))
//                change_task_status(manager);
//            continue;
//        case 5:
//            // Edit task parameters
//            if (!ask_back_to_menu("Do you want to continue and edit task?"))
//                edit_task(manager);
//            continue;
//        case 6:
//            // Delete by number
//            if (!ask_back_to_menu("Do you want to continue and delete task from a database?"))
//                delete_task(manager);
//            continue;
//        case 7:
//            // Logic to delete all tasks
//            if (!ask_back_to_menu("Do you want to delete all tasks?"))
//                delete_all_tasks(manager);
//            continue;
//        case 8:           
//            if (!ask_back_to_menu("Do you want to exit the program?"))
//            {
//                std::cout << "Exiting Personal Task Manager. Goodbye!\n";
//                return 0;
//            }
//            continue;
//        default:
//            
//            std::cerr << "Invalid input. Please enter a number from main menu.\n";
// 
//            continue;
//            
//        };
// 
//
//       
//    }
//    return 0;
//
//}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   